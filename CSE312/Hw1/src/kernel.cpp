
#include <common/types.h>
#include <drivers/ata.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gdt.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <lib/utility.h>
#include <memorymanagement.h>
#include <multitasking.h>
#include <syscalls.h>

#include <drivers/amd_am79c973.h>

// #define GRAPHICSMODE

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

class PrintfKeyboardEventHandler : public KeyboardEventHandler {
  public:
    void OnKeyDown(char c) {
        char *foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler {
    int8_t x, y;

  public:
    MouseToConsole() {
        uint16_t *VideoMemory = (uint16_t *) 0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80 * y + x] =
            (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }

    virtual void OnMouseMove(int xoffset, int yoffset) {
        static uint16_t *VideoMemory = (uint16_t *) 0xb8000;
        VideoMemory[80 * y + x] =
            (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);

        x += xoffset;
        if (x >= 80)
            x = 79;
        if (x < 0)
            x = 0;
        y += yoffset;
        if (y >= 25)
            y = 24;
        if (y < 0)
            y = 0;

        VideoMemory[80 * y + x] =
            (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }
};

void anotherFunction() {
    printf("child1\n");
    for (int i = 0; i < 100000000; i++);
    printf("bitti1\n");
    exit();
}

void collatz(int32_t n) {
    void *args[] = {&n};
    printFormatted("%d: ", args);

    while (n != 1) {
        args[0] = &n;
        printFormatted("%d, ", args);

        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
    }

    args[0] = &n;
    printFormatted("%d\n", args);
}

int32_t long_running_program(int32_t n) {
    int32_t result = 0;

    for (int32_t i = 0; i < n; i++) {
        for (int32_t j = 0; j < n; j++) {
            result += i * j;
        }
    }

    return result;
}

void another_function() {
    printf("execve function\n");
    for (int i = 0; i < 100000000; i++);
    printf("execve function finished\n");
    exit();
}

void syscall_test() {
    pid_t pid1;
    pid_t pid2;

    fork(&pid1);

    if (pid1 == 0) {
        printf("child1\n");
        execve(another_function);
    }

    fork(&pid2);

    if (pid2 == 0) {
        printf("child2\n");
        for (int i = 0; i < 100000000; i++);
        printf("child2 finished\n");
        exit();
    }

    printf("waiting for children\n");
    waitpid(-1);
    printf("all children finished\n");
    exit();
}

void initA() {
    for (int32_t i = 0; i < 6; i++) {
        pid_t pid;

        fork(&pid);

        if (pid == 0) {
            if (i % 2 == 0) {
                collatz(7);
                exit();
            } else {
                int32_t result = long_running_program(1000);
                void *args[] = {&result};
                printFormatted("long running result: %d\n", args);
                exit();
            }
        }
    }

    waitpid(-1);
    printf("all children finished\n");
    exit();
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
    for (constructor *i = &start_ctors; i != &end_ctors; i++) (*i)();
}

extern "C" void kernelMain(const void *multiboot_structure, uint32_t /*multiboot_magic*/) {
    printf("Hello World! --- http://www.AlgorithMan.de\n");

    GlobalDescriptorTable gdt;

    uint32_t *memupper = (uint32_t *) (((size_t) multiboot_structure) + 8);
    size_t heap = 10 * 1024 * 1024;
    MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    // printf("heap: 0x");
    // printfHex((heap >> 24) & 0xFF);
    // printfHex((heap >> 16) & 0xFF);
    // printfHex((heap >> 8) & 0xFF);
    // printfHex((heap) & 0xFF);

    // void *allocated = memoryManager.malloc(1024);
    // printf("\nallocated: 0x");
    // printfHex(((size_t) allocated >> 24) & 0xFF);
    // printfHex(((size_t) allocated >> 16) & 0xFF);
    // printfHex(((size_t) allocated >> 8) & 0xFF);
    // printfHex(((size_t) allocated) & 0xFF);
    // printf("\n");

    TaskManager taskManager(&gdt, &memoryManager);
    Task initTask(&gdt, initA);
    // Task test(&gdt, syscall_test);
    taskManager.AddTask(&initTask);
    // taskManager.AddTask(&test);

    InterruptManager interrupts(0x20, &gdt, &taskManager);
    SyscallHandler syscalls(&interrupts, 0x80);

    /* printf("Initializing Hardware, Stage 1\n");

#ifdef GRAPHICSMODE
    Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
#endif

    DriverManager drvManager;

#ifdef GRAPHICSMODE
    KeyboardDriver keyboard(&interrupts, &desktop);
#else
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
#endif
    drvManager.AddDriver(&keyboard);

#ifdef GRAPHICSMODE
    MouseDriver mouse(&interrupts, &desktop);
#else
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
#endif
    drvManager.AddDriver(&mouse);

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);

#ifdef GRAPHICSMODE
    VideoGraphicsArray vga;
#endif

    printf("Initializing Hardware, Stage 2\n");
    drvManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");

#ifdef GRAPHICSMODE
    vga.SetMode(320, 200, 8);
    Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
    desktop.AddChild(&win1);
    Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
    desktop.AddChild(&win2);
#endif */

    /*
    printf("\nS-ATA primary master: ");
    AdvancedTechnologyAttachment ata0m(true, 0x1F0);
    ata0m.Identify();

    printf("\nS-ATA primary slave: ");
    AdvancedTechnologyAttachment ata0s(false, 0x1F0);
    ata0s.Identify();
    ata0s.Write28(0, (uint8_t*)"http://www.AlgorithMan.de", 25);
    ata0s.Flush();
    ata0s.Read28(0, 25);

    printf("\nS-ATA secondary master: ");
    AdvancedTechnologyAttachment ata1m(true, 0x170);
    ata1m.Identify();

    printf("\nS-ATA secondary slave: ");
    AdvancedTechnologyAttachment ata1s(false, 0x170);
    ata1s.Identify();
    // third: 0x1E8
    // fourth: 0x168
    */

    // amd_am79c973 *eth0 = (amd_am79c973 *) (drvManager.drivers[2]);
    // eth0->Send((uint8_t *) "Hello Network", 13);

    interrupts.Activate();

    while (1) {
#ifdef GRAPHICSMODE
        desktop.Draw(&vga);
#endif
    }
}

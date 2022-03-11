#ifndef GPP_INTERPRETER
#define GPP_INTERPRETER
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct int_vals{
    struct int_vals* next;
    int val; 
}int_list;

typedef struct ids{
    struct ids* next;
    char id[16];
    int val;
}id_list;

id_list* ids = NULL;

int_list* createIntList(int initVal) {
    int_list* list = (int_list*)malloc(sizeof(int_list));
    list->next = NULL;
    list->val = initVal;
    return list;
}

id_list* createIdList(int initVal, char* initId) {
    id_list* list = (id_list*)malloc(sizeof(id_list));
    strcpy(list->id, initId);
    list->val = initVal;
    list->next = NULL;
    return list;
}

int contains(char* id) {
    int counter = 0;
    id_list* current = ids;

    while(current != NULL) {
        if(strcmp(current->id, id) == 0) {
            return counter;
        }
        counter++;
        current = current->next;
    }

    return -1;
}

void addInt(int_list* list, int val) {
    int_list* current = list;

    while(current->next != NULL) {
        current = current->next;
    }

    current->next = (int_list*)malloc(sizeof(int_list));
    current->next->next = NULL;
    current->next->val = val;
}

void addVar(char* id, int val) {
    if(ids == NULL) {
        ids = createIdList(val, id);
    }
    else {
        id_list* current = ids;

        while(current->next != NULL) {
            current = current->next;
        }

        current->next = (id_list*)malloc(sizeof(id_list));
        current->next->next = NULL;
        strcpy(current->next->id, id);
        current->next->val = val;
    }
}

void updateVar(char* id, int newVal) {
    int counter = contains(id);
    id_list* current = ids;

    if(current == NULL) {
        ids = createIdList(newVal, id);
    }
    else {
        if(counter != -1) {
            for(int i = 0; i < counter; i++) {
                current = current->next;
            }
            current->val = newVal;
        }
        else {
            while(current->next != NULL) {
                current = current->next;
            }
            current->next = (id_list*)malloc(sizeof(id_list));
            current->next->next = NULL;
            strcpy(current->next->id, id);
            current->next->val = newVal;
        }
    }
}

int getIdVal(char* id) {
    int counter = contains(id);
    id_list* current = ids;

    if(counter == -1) {
        printf("IDENTIFIER NOT FOUND\n");
        exit(-1);
    }
    
    for(int i = 0; i < counter; i++) {
        current = current->next;
    }

    return current->val;
}
#endif
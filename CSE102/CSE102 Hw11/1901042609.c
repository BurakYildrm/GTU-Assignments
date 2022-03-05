#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
typedef struct list{
    int content;
    struct list *next;
}linked_list;
struct wrong_entries{
    int n1, n2;
};
int* read_array(char *filename, int *array, int *size);
void read_linkedlist(char *filename, linked_list *list, int *size);
float* stats_array(int *array, int size);
float* stats_linkedlist(linked_list *list, int size);
struct wrong_entries* compare_entries(int *array, linked_list* list, struct wrong_entries *deteched_entries);
void randomly_change_entries(int* array, int size);
int main(){
    int *array_of_num = (int*)malloc(sizeof(int)), size_of_array = 0, size_of_list = 0, number, choice, *array_for_part2 = (int*)malloc(sizeof(int)), i;
    linked_list *list_of_num = (linked_list*)malloc(sizeof(linked_list)), *list_for_part2 = (linked_list*)malloc(sizeof(linked_list)), *iter;
    struct wrong_entries *deteched_entries = (struct wrong_entries*)malloc(sizeof(struct wrong_entries));
    clock_t time_read_array, time_read_ll, time_stats_array, time_stats_ll, average_read_array = 0, average_read_ll = 0;
    float *stats_of_array, *stats_of_list;
    printf("What part do you want to execute: ");
    scanf("%d", &choice);
    if(choice == 1){
        for(i = 0; i < 4; i++){
            if(i%2 == 0){
                time_read_array = clock();
                array_of_num = read_array("list.txt", array_of_num, &size_of_array);
                time_read_array = clock() - time_read_array;
                average_read_array += time_read_array;
                time_read_ll = clock();
                read_linkedlist("list.txt", list_of_num, &size_of_list);
                time_read_ll = clock() - time_read_ll;
                average_read_ll += time_read_ll;
            }
            else{
                time_read_ll = clock();
                read_linkedlist("list.txt", list_of_num, &size_of_list);
                time_read_ll = clock() - time_read_ll;
                average_read_ll += time_read_ll;
                time_read_array = clock();
                array_of_num = read_array("list.txt", array_of_num, &size_of_array);
                time_read_array = clock() - time_read_array;
                average_read_array += time_read_array;
            }
        }
        time_stats_array = clock();
        stats_of_array = stats_array(array_of_num, size_of_array - 1);
        time_stats_array = clock() - time_stats_array;
        time_stats_ll = clock();
        stats_of_list = stats_linkedlist(list_of_num, size_of_list - 1);
        time_stats_ll = clock() - time_stats_ll;
        printf("---read_array function stats---\n");
        printf("Allocated memory(in bytes): %d\n", size_of_array*4);
        printf("Max\t\tMin\t\tMean\t\tStd\n");
        for(i = 0; i < 4; i++){
            printf("%-16f", stats_of_array[i]);
        }
        printf("\n------------------------------------------------------------\n\n");
        printf("---read_linkedlist function stats---\n");
        printf("Allocated memory(in bytes): %d\n", size_of_list*8);
        printf("Max\t\tMin\t\tMean\t\tStd\n");
        for(i = 0; i < 4; i++){
            printf("%-16f", stats_of_list[i]);
        }
        printf("\n------------------------------------------------------------\n\n");
        printf("Time taken by read_array function is %lf seconds\n", ((double)average_read_array)/4/CLOCKS_PER_SEC);
        printf("Time taken by read_linkedlist function is %lf seconds\n", ((double)average_read_ll)/4/CLOCKS_PER_SEC);
        printf("Time taken by stats_array function is %lf seconds\n", ((double)time_stats_array)/CLOCKS_PER_SEC);
        printf("Time taken by stats_linkedlist function is %lf seconds\n", ((double)time_stats_ll)/CLOCKS_PER_SEC);
        free(list_of_num);
        free(array_of_num);
    }
    if(choice == 2){
        printf("Enter a character to stop entering integers\n");
        i = 0;
        iter = list_for_part2;
        while(scanf("%d", &number) == 1){
            array_for_part2 = realloc(array_for_part2, (i + 1)*sizeof(int));
            array_for_part2[i] = number;
            iter->content = number;
            iter->next = (linked_list*)malloc(sizeof(linked_list));
            iter->next->next = NULL;
            iter = iter->next;
            i++;
        }
        srand(time(NULL));
        array_for_part2 = realloc(array_for_part2, (i + 1)*sizeof(int));
        array_for_part2[i] = -1;
        randomly_change_entries(array_for_part2, i);
        deteched_entries = compare_entries(array_for_part2, list_for_part2, deteched_entries);
        printf("\nElements of array\n");
        i = 0;
        while(array_for_part2[i] != -1){
            printf("%d ", array_for_part2[i]);
            i++;
        }
        printf("\n\nElements of linked list\n");
        iter = list_for_part2;
        while(iter->next != NULL){
            printf("%d ", iter->content);
            iter = iter->next;
        }
        printf("\n\n");
        i = 0;
        while(deteched_entries[i].n1 != 697868){
            printf("Wrong entries no%i: %d %d\n", i + 1, deteched_entries[i].n1, deteched_entries[i].n2);
            i++;
        }
    }
    return 0;
}
int* read_array(char *filename, int *array, int *size){
    FILE *fp = fopen(filename, "r");
    int i = 0, number;
    char comma;
    while(fscanf(fp, "%d%c", &number, &comma) == 2){
        array = realloc(array, (i + 1)*sizeof(int));
        array[i] = number;
        i++;
    }
    array = realloc(array, (i + 1)*sizeof(int));
    array[i] = -1;
    *size = i + 1;
    fclose(fp);
    return array;
}
void read_linkedlist(char *filename, linked_list *list, int *size){
    FILE *fp = fopen(filename, "r");
    linked_list *iter = list;
    int number, counter = 0;
    char comma;
    while(fscanf(fp, "%d%c", &number, &comma) == 2){
        iter->content = number;
        iter->next = (linked_list*)malloc(sizeof(linked_list));
        iter->next->next = NULL;
        iter = iter->next;
        counter++;
    }
    *size = counter + 1; //For the extra node allocated at the end of the file.
    fclose(fp);
}
float* stats_array(int *array, int size){
    double std = 0, mean = 0;
    int min = array[0], max = array[0], i = 0;
    static float results[4];
    while(array[i] != -1){
        if(array[i] > max){
            max = array[i];
        }
        i++;
    }
    i = 0;
    while(array[i] != -1){
        if(array[i] < min){
            min = array[i];
        }
        i++;
    }
    i = 0;
    while(array[i] != -1){
        mean += array[i];
        i++;
    }
    mean /= size;
    i = 0;
    while(array[i] != -1){
        std += (array[i] - mean)*(array[i] - mean);
        i++;
    }
    std = sqrt(std/size);
    results[0] = max;
    results[1] = min;
    results[2] = mean;
    results[3] = std;
    return results;
}
float* stats_linkedlist(linked_list *list, int size){
    double std = 0, mean = 0;
    int min = list->content, max = list->content;
    static float results[4];
    linked_list *iter = list;
    while(iter->next->next != NULL){
        if(iter->next->content > max){
            max = iter->next->content;
        }
        iter = iter->next;
    }
    iter = list;
    while(iter->next->next != NULL){
        if(iter->next->content < min){
            min = iter->next->content;
        }
        iter = iter->next;
    }
    iter = list;
    while(iter->next != NULL){
        mean += iter->content;
        iter = iter->next;
    }
    mean /= size;
    iter = list;
    while(iter->next != NULL){
        std += (iter->content - mean)*(iter->content - mean);
        iter = iter->next;
    }
    std = sqrt(std/size);
    results[0] = max;
    results[1] = min;
    results[2] = mean;
    results[3] = std;
    return results;
}
struct wrong_entries* compare_entries(int *array, linked_list* list, struct wrong_entries *deteched_entries){
    linked_list* iter = list;
    int i = 0, j = 0;
    while(array[i] != -1){
        if(array[i] != iter->content){  //If entries does not match function assigns them to the struct n1 and n2
            deteched_entries = (struct wrong_entries*)realloc(deteched_entries, (j + 1)*sizeof(struct wrong_entries));
            deteched_entries[j].n1 = array[i];
            deteched_entries[j].n2 = iter->content;
            j++;
        }
        i++;
        iter = iter->next;
    }
    deteched_entries = (struct wrong_entries*)realloc(deteched_entries, (j + 1)*sizeof(struct wrong_entries));
    deteched_entries[j].n1 = 697868;    //ASCII equivalent of END;
    return deteched_entries;
}
void randomly_change_entries(int* array, int size){
    int random_index, random_number, change_number;     //change_number is the integer that decides how many entries is going to be changed.
    change_number = (rand() % (size - 1)) + 1;
    for(int i = 0; i < change_number; i++){
        random_index = rand() % size;   //random_index determines a random index between 0 and size
        random_number = rand() % 10000;
        array[random_index] = random_number;
    }
}
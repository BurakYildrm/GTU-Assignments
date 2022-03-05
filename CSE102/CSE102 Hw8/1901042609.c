#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define N 8
void menu();
void menu_for_hofstadter();
void generate_hofstadters_sequence (int *arr, int n);
int find_max (int arr[], int index, int max_value);
int sum_array (int arr[]);
int find_length(int arr[]);
double std_array (int arr[], double *mean, int n, int index);
void install_pipes (int visited[N][N], int x, int y, int orientation, int counter);
int proper_orientations(int x, int y, int array[N][N]);
void set_orientation(int *x, int *y, int orientation);
char* remove_duplicates (char* str);
int main(){
    menu();
    return 0;
}
void menu(){
    int array[N][N], direction, choice = 0;
    char message[100];
    char *pointer;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(i == 0 & j == 0){
                array[i][j] = 1;
            }
            else{
                array[i][j] = 0;
            }
        }
    }
    while(choice != 4){
        printf("Welcome to Homework 8, please choose one of the parts to continue\n");
        printf("------------------------\n\n");
        printf("1) Execute Part 1\n");
        printf("2) Execute Part 2\n");
        printf("3) Execute Part 3\n");
        printf("4) Exit\n");
        scanf(" %d", &choice);
        switch(choice){
            case 1:
                menu_for_hofstadter();
                break;
            case 2:
                printf("\n");
                install_pipes(array, 0, 0, 1, 102);
                break;
            case 3:
                printf("Please enter the string you want to fix:\n");
                scanf(" %[^\n]", message);
                pointer = remove_duplicates(message);
                printf("\nFixed version of the string is:\n");
                printf("%s\n\n", pointer);
                break;
            case 4:
                printf("GOOD BYE\n");
                break;
            default:
                printf("UNSUPPORTED OPERATION!\n\n");
        }
    }
}
void menu_for_hofstadter(){
    int choice = 0, n, numbers[500], i;
    double mean;
    while(choice != 5){
        printf("Please make your choice:\n");
        printf("-----------------------\n");
        printf("1) Fill Array\n");
        printf("2) Find Biggest Number\n");
        printf("3) Calculate Sum\n");
        printf("4) Calculate Standard Deviation\n");
        printf("5) Exit\n");
        scanf(" %d", &choice);
        switch(choice){
            case 1:
                printf("Please enter a number: ");
                scanf("%d", &n);
                printf("\n");
                memset(numbers, 0, 500*sizeof(int));
                generate_hofstadters_sequence(numbers, n);
                break;
            case 2:
                printf("\nBiggest number in the array is %d\n\n", find_max(numbers, n, 0));
                break;
            case 3:
                printf("\nSum of the elements in the array is %d\n\n", sum_array(numbers));
                break;
            case 4:
                mean = 0;
                printf("\nStandard deviation of this array is %lf\n\n", std_array(numbers, &mean, n, n));
                break;
            case 5:
                printf("SEE YOU IN THE MENU\n\n");
                break;
            default:
                printf("UNSUPPORTED OPERATION!\n\n");
        }
    }
}
void generate_hofstadters_sequence (int *arr, int n){
    for(int i = 0; i < n; i++){
        if(i >= 500){
            break;
        }
        if(i < 2){
            arr[i] = 1;
        }
        else{
            arr[i] = arr[i - arr[i - 1]] + arr[i - arr[i - 2]];
        }
    }
}
int find_max (int arr[], int index, int max_value){
    if(index >= 2){
       max_value = find_max(arr, index - 1, max_value);
    }
    if(index < 2){
        if(arr[index] > arr[index - 1]){
            return arr[index];
        }
        else{
            return arr[index - 1];
        }
    }
    else{
        if(arr[index] > max_value){
            return arr[index];
        }
        else{
            return max_value;
        }
    }
}
int sum_array (int arr[]){
    int length = find_length(arr), sum = 0;
    if(length == 1 || arr[0] == 0){
        return arr[0];
    }
    else{
        sum += arr[0] + sum_array(&arr[1]);
        return sum;
    }
}
int find_length(int arr[]){
    int i = 0, count = 0;
    while(arr[i] != 0){
        count++;
        i++;
        if(i == 500){
            break;
        }
    }
    return count;
}
double std_array (int arr[], double *mean, int n, int index){
    double std;
    int temp;
    int sum = 0;
    if(index == n){
        temp = n;
        index++;
    }
    if(*mean == 0){
        if(n == 1){
            return arr[0];
        }
        else{
            sum += arr[0] + std_array(&arr[1], mean, (n - 1), index);
            if(find_length(arr) != temp){
                return sum;
            }
            else{
                *mean = (double) sum/temp;
            }
        }
    }
    if(n == 1){
        return pow((arr[0] - *mean), 2);
    }
    if(n == temp){
        std = pow((arr[0] - *mean), 2) + std_array(&arr[1], mean, n - 1, index + 1);
        return sqrt(std/find_length(arr));

    }
    else{
        std = pow((arr[0] - *mean), 2) + std_array(&arr[1], mean, n - 1, index + 1);
        return std;
    }
}
void install_pipes (int visited[N][N], int x, int y, int orientation, int counter){
    int temp_x = x, temp_y = y, orientations[8], propers, j, check = 0, element = 0;
    memset(orientations, 0, 8*sizeof(int));
    static int temp_o = 0;
    srand(clock());
    propers = proper_orientations(x, y, visited);
    if(propers == 0){
        visited[y][x] = 0;
    }
    else{
        j = 0;
        while(propers != 0){
            orientations[j] = propers%10;
            propers /= 10;
            j++;
        }
        orientation = orientations[(rand() % find_length(orientations))];
        if(orientation == temp_o){
            element++;
            if(element >= find_length(orientations)){
                temp_o = 0;
                check++;
            }
            else{
                while(orientation == temp_o){
                    orientation = orientations[rand() % find_length(orientations)];
                }
                temp_o = 0;
            }
        }
        if(check == 1){
            visited[x][y] = 0;
        }
        else{
            set_orientation(&temp_x, &temp_y, orientation);
            visited[temp_y][temp_x] = counter%100;
            temp_o = 0;
            install_pipes(visited, temp_x, temp_y, orientation, counter + 1);
            if(visited[temp_y][temp_x] == 0){
                temp_o = orientation;
                install_pipes(visited, x, y, 1, counter);
            }
        }
    }
}
int proper_orientations(int x, int y, int array[N][N]){
    int tempx = x, tempy = y, sum = 0;
    for(int i = 1; i < 9; i++){
        set_orientation(&tempx, &tempy, i);
        if(array[tempy][tempx] == 0 && tempx < 8 && tempx >= 0 && tempy < 8 && tempy >= 0){
            sum = sum*10 + i;
        }
        tempx = x;
        tempy = y;
    }
    return sum;
}
void set_orientation(int *x, int *y, int orientation){
    switch(orientation){
        case 1:
            *x += 1;
            *y += 2;
            break;
        case 2:
            *x -= 2;
            *y += 1;
            break;
        case 3:
            *x -= 1;
            *y -= 2;
            break;
        case 4:
            *x += 2;
            *y -= 1;
            break;
        case 5:
            *x += 2;
            *y += 1;
            break;
        case 6:
            *x -= 1;
            *y += 2;
            break;
        case 7:
            *x -= 2;
            *y -= 1;
            break;
        case 8:
            *x += 1;
            *y -= 2;
            break; 
    }
}
char* remove_duplicates (char* str){
    char *ptr, *temp;
    if(strlen(str) == 1){
        ptr = str;
        return ptr;
    }
    else{
        temp = remove_duplicates(&str[1]);
        //printf("%s\n", temp);
        sprintf(&str[1], "%s", temp);
        if(str[0] == str[1]){
            for(int j = 0; j < strlen(str); j++){
                str[j] = str[j + 1];
            }
        }
        ptr = str;
        return ptr;
    }
}
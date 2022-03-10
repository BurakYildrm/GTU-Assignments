#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
void menu();
void read_news(char buffer[500], char file_path[10], int is_Only_Title);
void append_file(char* file_path, char c);
void read_magic_numbers(char buffer_magic[10], char buffer_news[500]);
int array_length(char array[]);
double f_func(int x);
double g_func(double f(int x), int a);
int main(){
    menu();
    return 0;
}
void menu(){
    FILE *txt = fopen("readed_news_id.txt", "a+");
    char *readed_news = "readed_news_id.txt";
    int again = 1, count = 0, decrypt;
    char c = '0', choice, rnews, readed, news;
    char buffer[500];
    char magic[10];
    memset(magic, '\0', 10*sizeof(char));//If I don't in the first round numbers are going crazy.
    printf("**********Daily Press**************\n\n");
    printf("Today's new are listed for you:\n\n");
    printf("Title of 1. news:");
    read_news(buffer,"news/1.txt",0);
    printf("Title of 2. news:");
    read_news(buffer,"news/2.txt",0);
    printf("Title of 3. news:");
    read_news(buffer,"news/3.txt",0);
    printf("Title of 4. news:");
    read_news(buffer,"news/4.txt",0);
    printf("\n");
    /*In a or a+ mode fopen sends the pointer to the end of the file.If the user wants to select b first pointer must be pointing the beginning of the file so in order to do that I used the rewind function.*/
    rewind(txt);
    while(c != 'n'){
        printf("What do you want to do?\n");
        printf("a.Read a new\n");
        printf("b.List the readed news\n");
        printf("c.Get the decrypted information from the news\n");
        scanf(" %c", &choice);
        if(choice == 'a'){
            printf("Which news do you want to read?: ");
            scanf(" %c", &news);
            /*If the user selected b in the previous round in order to scan the whole file I used the rewind function.*/
            rewind(txt);
            while(fscanf(txt, "%c", &readed) != EOF){
                if(readed == news){
                    if(count == 0){
                        printf("This new is readed. Do you want to read it again? Yes(1) / No(0):\n");
                        scanf(" %d", &again);
                        if(again != 0 && again != 1){
                            while(again != 1 && again != 0){
                                printf("Invalid operation, please enter 1 or 0\n");
                                scanf(" %d", &again);//In case if the user enters a integer beside 1 and 0.
                            }
                        }
                        if(again == 0){
                            printf("Do you want to continue? Yes(y)/No(n):\n");
                            scanf(" %c", &c);
                            if(c != 'y' && c != 'n'){
                                while(c != 'y' && c != 'n'){
                                    printf("Invalid operation, please enter y or n\n");
                                    scanf(" %c", &c);//In case if the user enters a character beside y and n.
                                } 
                            }
                            if(c == 'n'){
                                printf("Good Bye!\n");
                            }
                        }
                        count++;
                    }
                }
            }
            count = 0;
            if(again == 1){
                switch(news){
                    case '1':
                        read_news(buffer,"news/1.txt",1);
                        printf("\n");
                        append_file(readed_news, '1');
                        break;
                    case '2':
                        read_news(buffer,"news/2.txt",1);
                        printf("\n");
                        append_file(readed_news, '2');
                        break;
                    case '3':
                        read_news(buffer,"news/3.txt",1);
                        printf("\n");
                        append_file(readed_news, '3');
                        break;
                    case '4':
                        read_news(buffer,"news/4.txt",1);
                        printf("\n");
                        append_file(readed_news, '4');
                        break;
                }
                printf("Do you want to continue? Yes(y)/No(n):\n");
                scanf(" %c", &c);
                if(c != 'y' && c != 'n'){
                    while(c != 'y' && c != 'n'){
                        printf("Invalid operation, please enter y or n\n");
                        scanf(" %c", &c);//In case if the user enters a character beside y and n.
                    } 
                }
                if(c == 'n'){
                    printf("Good Bye!\n");
                }
            }
        }
        else if(choice == 'b'){
            printf("\n");
            printf("Readed news are listed below:\n");
            /*If the user selected a before b in order to scan the whole file I used the rewind function.*/
            rewind(txt);
            while(!feof(txt)){
                fscanf(txt, "%c", &rnews);
                if(rnews == '\n'){
                    printf("\n");
                }
                else{
                    printf("%c. new is readed", rnews);
                }
            }
            printf("Do you want to continue? Yes(y)/No(n):\n");
            scanf(" %c", &c);
            if(c != 'y' && c != 'n'){
                while(c != 'y' && c != 'n'){
                    printf("Invalid operation, please enter y or n\n");
                    scanf(" %c", &c);//In case if the user enters a character beside y and n.
                } 
            }
            if(c == 'n'){
                printf("Good Bye!\n");
            }
        }
        else if(choice == 'c'){
            printf("Which news would you like to decrypt?: ");
            scanf("%d", &decrypt);
            switch(decrypt){
                case 1:
                    magic[0] = 1;
                    read_magic_numbers(magic, buffer);
                    break;
                case 2:
                    magic[0] = 2;
                    read_magic_numbers(magic, buffer);
                    break;
                case 3:
                    magic[0] = 3;
                    read_magic_numbers(magic, buffer);
                    break;
                case 4:
                    magic[0] = 4;
                    read_magic_numbers(magic, buffer);
                    break;
            }
            printf("\nDo you want to continue? Yes(y)/No(n):\n");
            scanf(" %c", &c);
            if(c != 'y' && c != 'n'){
                while(c != 'y' && c != 'n'){
                    printf("Invalid operation, please enter y or n\n");
                    scanf(" %c", &c);//In case if the user enters a character beside y and n.
                } 
            }
            if(c == 'n'){
                printf("Good Bye!\n");
            }
        }
        else{
            printf("Invalid Operation!\n\n");
        }   
    }
    fclose(txt);
}
void read_news(char buffer[500], char file_path[10], int is_Only_Title){
    FILE *fp;
    fp = fopen(file_path, "r");
    if(is_Only_Title == 0){
       fscanf(fp, "%[^\n]", buffer);
       printf("%s\n", buffer);
    }
    if(is_Only_Title == 1){
        while(fgets(buffer, 500, fp) != NULL){
            printf("%s", buffer);
        }
    }
    fclose(fp);
}
void append_file(char* file_path, char c){
    FILE *fpointer;
    fpointer = fopen(file_path, "a+");
    int counter = 0;
    char x;
    rewind(fpointer);
    while(fscanf(fpointer, "%c", &x) != EOF){    
        if(x == c){
            counter++;
        }
    }
    if(counter == 0){
        fprintf(fpointer, "%c\n", c);
    }
    fclose(fpointer);
}
void read_magic_numbers(char buffer_magic[10], char buffer_news[500]){
    char check = buffer_magic[0];//To store the information about which news is sent to be read. 
    int i, j = 0, x;
    double sum = 0, temp;
    FILE *magic_number;
    switch(check){
        case 1:
            magic_number = fopen("news/1.txt", "r");
            break;
        case 2:
            magic_number = fopen("news/2.txt", "r");
            break;
        case 3:
            magic_number = fopen("news/3.txt", "r");
            break;
        case 4:
            magic_number = fopen("news/4.txt", "r");
            break;
    }
    while(fgets(buffer_news, 500, magic_number) != NULL){
        printf("%s", buffer_news);
        i = 0;
        while(buffer_news[i] != '\0'){
            if(buffer_news[i] == '#'){
                buffer_magic[j] = buffer_news[i+1];
                j++;
            }
            i++;
        }
    }
    i = 0;
    while(buffer_magic[i] != '\0'){
        x = buffer_magic[i] - '0';
        temp = g_func(f_func, x);
        sum += temp;
        i++;
    }
    //If I don't do this the last magic number of the this new is counted as one of the next news' magic numbers.  
    memset(buffer_magic, '\0', 10*sizeof(char));
    switch(check){
        case 1:
            printf("\nnumber of tests performed = %.2lf", sum);
            break;
        case 2:
            printf("\nnumber of sick people = %.2lf", sum);
            break;
        case 3:
            printf("\nnumber of deaths = %.2lf", sum);
            break;
        case 4:
            printf("\nexpected number of sick people = %.2lf", sum);
            break;
    }
    fclose(magic_number);
}
double f_func(int x){
    return(pow(x, 3) - pow(x, 2) + 2);
}
double g_func(double f(int x), int a){
    double result;
    result = f(a);
    result = pow(result, 2);
    return result;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
void menu();
void old_encrypted_messages(char symbol, FILE *file);
void encrypt_messages(char* message);
void track_machine();
void refresh_position(int *X, int *Y, double *D, double *R);
int find_length(int number);
void map(int x, int y);
void print_column(int y, int flag);
void print_raw(int x, int flag);
void decrypt_and_print(char* file_path);
void deep_decrypt_and_print(char* file_path);
char decrypt_numbers(int number);
int main(){
    menu();
    return 0;
}
void menu(){
    int i = 0;
    char *file = "encrypted_p1.img";
    char *f = "encrypted_p2.img";
    char *about_to_be_encrypted; 
    char c;
    FILE *decrypted;
    FILE *old_encrypted;
    while(i != 5){
        printf("1-) Decrypt and print encrypted_p1.img\n");
        printf("2-) Decrypt and print encrypted_p2.img\n");
        printf("3-) Switch on the track machine\n");
        printf("4-) Encrypt the message\n");
        printf("5-) Switch off\n");
        scanf("%d", &i);
        switch(i){
            case 1:
                decrypt_and_print(file);
                break;
            case 2:
                deep_decrypt_and_print(f);
                break;
            case 3:
                track_machine();
                break;
            case 4:
                decrypted = fopen("decrypted_p4.img", "r");
                /*old_encrypted.img file is the encrypted version of the decrypted_p4.img.I reverse encrypted the file according to the decryption method I used in part 1*/
                old_encrypted = fopen("old_encrypted.txt", "w");
                while(!feof(decrypted)){
                    if(feof(decrypted)){
                        break;
                    }
                    else{
                        fscanf(decrypted, "%c", &c);
                        old_encrypted_messages(c, old_encrypted);
                    }
                }
                fclose(decrypted);
                fclose(old_encrypted);
                about_to_be_encrypted = "old_encrypted.txt";
                /*First I converted the symbols into number and then I sent the file which has the numbers to the function to be encrypted according to the formula which is given in part 4*/
                encrypt_messages(about_to_be_encrypted);
                printf("Your statement is encrypted\n");
                break;
            case 5:
                exit(0);
                break;
        }
    }
}
void decrypt_and_print(char* file_path){
    int num;
    char c;
    FILE *fp;
    fp = fopen(file_path, "r");
    while(!feof(fp)){
        fscanf(fp, "%c", &c);
        if(c == '\n'){ 
            printf("\n");
        }
        num = c - '0';
        printf("%c", decrypt_numbers(num));
    }
    fclose(fp);
}
char decrypt_numbers (int number){
    switch(number){
        case 0:
            return(' ');
        case 1:
            return('-');
        case 2:
            return('_');
        case 3:
            return('|');
        case 4:
            return('/');
        case 5:
            return('\\');
        case 6:
            return('O');
    }
    return 0;
}
void deep_decrypt_and_print(char* file_path){
    int num1, num2, num3, i = 0, result, count = 0;
    char a, b, c;
    FILE *fpointer1;
    FILE *fpointer2;
    FILE *fpointer3;
    fpointer1 = fopen(file_path, "r");
    fpointer2 = fopen(file_path, "r");
    fpointer3 = fopen(file_path, "r");
    while(!feof(fpointer1)){
        if(i == 0){
             for(int j=0;j<2;j++){
                fscanf(fpointer3, "%c", &c);    //I scanned the file twice before fpointer1 in order to fpointer3 to scan 2 characters ahead of fpointer1.
            }
            fscanf(fpointer2, "%c", &b);    //I scanned the file once before fpointer1 in order to fpointer3 to scan 1 character ahead of fpointer1.
            i++;
        }
        fscanf(fpointer1, "%c", &a);
        if(a != '\n'){
            num1 = a - '0';
        }
        //I wrote all these conditions in order to add last 2 0s at the end of each line
        if(count == 2){
            num2 = 0;
        }
        else{
            fscanf(fpointer2, "%c", &b);
            if(c == '\n' || c == EOF){
                num2 = 0;
                count++;
            }
            else{
                num2 = b - '0';
            }
        }
        if(count == 1 || count == 2){
            num3 = 0;
        }
        else{
            fscanf(fpointer3, "%c", &c);
            if(c == '\n' || c == EOF){
                num3 = 0;
                count++;
            }
            else{
                num3 = c - '0';
            }
        }
        //I wrote this condition to do the scanning prosedure before fpointer1 at the beginning of each line. 
        if(a == '\n' || a == EOF){
            printf("\n");
            i = 0;
            count = 0;
        }
        else{
            result = (num1 + num2 + num3)%7;
            printf("%c", decrypt_numbers(result));
        }
    }
    printf("\n");
    fclose(fpointer1);
    fclose(fpointer2);
    fclose(fpointer3);
}
void track_machine(){
    int x, y;
    double distance, displacement;
    char c;
    srand(time(NULL));
    /*I started at a random point because I can't show x = 0 and y = 0 on a 11x11 coordinate system according to the homework pdf.*/
    x = (rand() %11) + 1;
    y = (rand() %11) + 1;
    distance = sqrt(pow(abs(x - 6), 2) + pow(abs(y - 6), 2));
    displacement = sqrt(pow(x, 2) + pow(y, 2));
    map(x, y);
    printf("\nEnemies X position: %d, Y position: %d, Displacement: %.2lf, Distance to our camp: %.2lf ", x, y, displacement, distance);
    printf("Command waiting...: ");
    scanf(" %c", &c);
    //I did this in order to send the coordinates of the previous enemy position to the refresh function.
    if(x < 10 && y < 10){
        displacement = 10*y + x;
    }
    if(x >= 10 && y < 10){
        displacement = 10*(100*y + x);
    }
    if(x < 10 && y >= 10){
        displacement = 10*(10*y + x) + 1;
    }
    if(x >= 10 && y >= 10){
        displacement = 10*(100*y + x);
    } 
    while(c != 'E'){
        if(c != 'R'){
            while(c != 'R'){
                scanf("%c", &c);
            }
        }
        while(c == 'R'){
            refresh_position(&x, &y, &distance, &displacement);
            printf("\nEnemies X position: %d, Y position: %d, Displacement: %.2lf, Distance to our camp: %.2lf ", x, y, displacement, distance);
            printf("Command waiting...: ");
            scanf(" %c", &c);
            if(x < 10 && y < 10){
                displacement = 10*y + x;
            }
            if(x >= 10 && y < 10){
                displacement = 10*(100*y + x);
            }
            if(x < 10 && y >= 10){
                displacement = 10*(10*y + x) + 1;
            }
            if(x >= 10 && y >= 10){
                displacement = 10*(100*y + x);
            } 
        }
    } 
}
int find_length(int number){
    int i = 0;
    while(number != 0){
        number /= 10;
        i++;
    }
    return(i);
}
void refresh_position(int *X, int *Y, double *D, double *R){
    int x, y, tempx, tempy, tempr;
    x = (rand() %11) + 1;
    y = (rand() %11) + 1;
    /*I sent the coordinates of previous enemy position in different types in order to decide whether x and y are greater or equal to 10 or not so in order to get the x and y I divided and modded the *R.*/
    tempr = floor(*R);  
    if(find_length(tempr) == 2){
        tempx = tempr%10;
        tempy = tempr/10;
    }
    else if(find_length(tempr) == 4){
        if(tempr%10 == 0){
            tempr /= 10;
            tempx = tempr%100;
            tempy = tempr/100;
        }
        else if(tempr%10 == 1){
            tempr /= 10;
            tempx = tempr%10;
            tempy = tempr/10;
        }
    }
    else if(find_length(tempr) == 5){
        tempr /= 10;
        tempx = tempr%1000;
        tempy = tempr/100;
    }
    map(x, y);
    *R = sqrt(pow(abs(x - tempx), 2) + pow(abs(y - tempy), 2));
    *D = sqrt(pow(abs(x - 6), 2) + pow(abs(y - 6), 2));
    *X = x;
    *Y = y;
}
void print_column(int y, int flag){
    int i;
    /*I used a flag in order to decide if the function prints dots until it reaches the enemy's x position or if it prints dots from enemy's x position to the end of the line.*/
    if(flag == 0){  
        for(i=1;i<y;i++){
            printf(".\t");
        }
    }
    else{
        for(i=0;i<y;i++){
            if(i == (y - 1)){
                printf(".\n");
            }
            else{
                printf(".\t");
            }
        }
    }
}
void print_raw(int x, int flag){
    int i;
    /*I used a flag to decide if the function prints raws until it reaches the enemy's y position or if it prints raws from enemy's y position to the 11th raw.*/
    if(flag == 0){
        for(i=1;i<x;i++){
            print_column(11, 1);
        }
    }
    else{
        for(i=0;i<x;i++){
            print_column(11, 1);
        }
    }
}
void map(int x, int y){
    if(x < 6){
        print_raw(x, 0);
        print_column(y, 0);
        printf("E\t");
        print_column((11 - y), 1);
        if(y == 11){
            printf("\n");
        }
        print_raw((6 - x), 0);
        print_column(6, 0);
        printf("O\t");
        print_column(5, 1);
        print_raw(5, 1);
    }
    if(x == 6){
        print_raw(6, 0);
        if(y < 6){
            print_column(y, 0);
            printf("E\t");
            print_column((6 - y), 0);
            printf("O\t");
            print_column(5, 1);
        }
        if(y == 6){
            print_column(6, 0);
            printf("Œ\t");  //Œ stands for O and E, means that enemy's and our positions are the same.
            print_column(5, 1);
        }
        if(y > 6){
            print_column(6, 0);
            printf("O\t");
            print_column((y - 6), 0);
            printf("E\t");
            print_column((11 - y), 1);
            if(y == 11){
                printf("\n");
            }
        }
        print_raw(5, 1);
    }
    if(x > 6){
        print_raw(6, 0);
        print_column(6, 0);
        printf("O\t");
        print_column(5, 1);
        print_raw((x - 6), 0);
        print_column(y, 0);
        printf("E\t");
        print_column((11 - y), 1);
        if(y == 11){
            printf("\n");
        }
        print_raw((11 - x), 1);
    }
    
}
void old_encrypted_messages(char symbol, FILE *file){
    /*This encryption is made according to the decryption in part 1*/
    switch(symbol){
        case ' ':
            fprintf(file, "%c", '0');
            break;
        case '-':
            fprintf(file, "%c", '1');
            break;
        case '_':
            fprintf(file, "%c", '2');
            break;
        case '|':
            fprintf(file, "%c", '3');
            break;
        case '/':
            fprintf(file, "%c", '4');
            break;
        case '\\':
            fprintf(file, "%c", '5');
            break;
        case 'O':
            fprintf(file, "%c", '6');
            break;
        case '\n':
            fprintf(file, "\n");
            break;
    }
}
void encrypt_messages(char* message){
    char a, b, c;
    int num1, num2, num3, count = 0, result, counter = 0;
    FILE *fp1;
    FILE *fp2;
    FILE *fp3;
    FILE *new_encrypted;
    new_encrypted = fopen("encrypted_p4.img", "w");
    fp1 = fopen(message, "r");
    fp2 = fopen(message, "r");
    fp3 = fopen(message, "r");
    while(!feof(fp1)){
        fscanf(fp1, "%c", &a);
        if(a != '\n'){
            num1 = a - '0';
            counter = 0;
        }
        if(a == '\n'){
            counter++;
        }
        /*I used count in order to make fp1 to scan 2 characters ahead of fp3 and 1 character ahead of fp2.*/
        if(count == 0){
            num2 = 0;
            num3 = 0;
            count++;
        }
        else if(count == 1){
            fscanf(fp2, "%c", &b);
            num2 = b - '0';
            num3 = 0;
            count++;
        }
        else if(count == 2){
            fscanf(fp2, "%c", &b);
            num2 = b - '0';
            fscanf(fp3, "%c", &c);
            num3 = c - '0';
        }
        if(a == '\n' || a == EOF){
            /*I used counter to if fp1 reachs the end of the line first time function will equalize the all 3 pointers and if fp1 reads another \n fp2 and fp3 will scan once to ensure that all of the 3 pointer are at the same location of the file until fp1 reads a character except \n.*/
            if(counter == 1){
                for(int i=0;i<2;i++){
                    fscanf(fp3, "%c", &c);
                }
                fscanf(fp2, "%c", &b);
                fprintf(new_encrypted, "\n");
                count = 0;
            }
            else{
                fscanf(fp3, "%c", &c);
                fscanf(fp2, "%c", &b);
                fprintf(new_encrypted, "\n");
                count = 0;
            }
        }
        else{
            result = (num1 + num2 + num3)%7;
            fprintf(new_encrypted, "%d", result);
        }
    }
    fclose(new_encrypted);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}
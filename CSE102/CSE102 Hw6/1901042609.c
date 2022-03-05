#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
int find_word(char array[20][20], char guess[], int row, int column, int length);
void upper_case(char array[20][20], int aspect, int length, int row, int column);
void selected_words(char array[150], char guess[15], int flag);
int main(){
    int direction, j, random_word, i, column, length, counter = 0, row, check = 0, tempx, tempy, guess_column, true_guesses = 0, length_of_guess, store = 0, sidebar = 0;
    char words[15], puzzle[20][20], random_char, guess[15], guess_row;
    char printf_word[150];
    //I used memset because sometimes when I declared the array computer fills it with random characters.
    memset(printf_word, '\0', 150*sizeof(char));
    memset(guess, '\0', 15*sizeof(char));
    for(i = 0; i < 20; i++){ //I put dots at every slot of the array in order to make sure that there is nothing else in the way of the selected word but dot.
        for(j = 0; j < 20; j++){
            puzzle[i][j] = '.';
        }
    }
    FILE *fp = fopen("words.txt", "r");
    srand(time(NULL));
    srandom(time(NULL));
    while(counter != 10){
        column = rand() % 20;
        row = rand() % 20;
        direction = rand() % 8;
        random_word = (rand() % 100) + 1;
        tempx = column;
        tempy = row;
        for(i = 0; i < random_word; i++){
            if(i == (random_word - 1)){
                memset(words, '\0', 15*sizeof(char));
                fgets(words, 15, fp);
            }
            else{
                fgets(words, 15, fp);
            }  
        }
        rewind(fp);
        if(random_word == 100){
            length = strlen(words);//When the word is the last one in the file fgets scans only the word so I used strlen without subtracting anything.
        }
        else{
            length = strlen(words) - 2; /*I subtracted 2 from the strlen function because fgets was scanning two extra \n character so I subtracted them to make sure that the code works as expected.*/
        }
        /*I used the first ifs after the direction if to check if the word goes beyond the puzzle array's boundries and if it does check is incremented by 1 and all of the assignment of direction and word selection is repeated until a proper position is given.*/
        /*I stored all the words to the printf_word array to use later on printing them at each turn and if the user enters the true guess making the guess upper case.*/
        if(direction == 0){
            if(length + column <= 20){
                for(i = 0; i < length; i++){
                    if(puzzle[tempy][tempx] != '.'){
                        check++;
                    }
                    tempx++;
                }
                if(check == 0){
                    for(i = 0; i < length; i++){
                        puzzle[row][column] = words[i];
                        column++;
                    }
                    for(i = 0; i < length; i++){
                        printf_word[store] = words[i];
                        store++;
                    }
                    printf_word[store] = '\n';
                    store++;
                }
            }
            else{
                check++;
            }
        }
        else if(direction == 1){
            if(length + column <= 20 && length + row <= 20){
                for(i = 0; i < length; i++){
                    if(puzzle[tempy][tempx] != '.'){
                        check++;
                    }
                    tempx++;
                    tempy++;
                }
                if(check == 0){
                    for(i = 0; i < length; i++){
                        puzzle[row][column] = words[i];
                        column++;
                        row++;
                    }
                    for(i = 0; i < length; i++){
                        printf_word[store] = words[i];
                        store++;
                    }
                    printf_word[store] = '\n';
                    store++;
                }
            }
            else{
                check++;
            }
        }
        else if(direction == 2){
            if(length + row <= 20){
                for(i = 0; i < length; i++){
                    if(puzzle[tempy][tempx] != '.'){
                        check++;
                    }
                    tempy++; 
                }
                if(check == 0){
                    for(i = 0; i < length; i++){
                        puzzle[row][column] = words[i];
                        row++;
                    }
                    for(i = 0; i < length; i++){
                        printf_word[store] = words[i];
                        store++;
                    }
                    printf_word[store] = '\n';
                    store++;
                }
            }
            else{
                check++;
            }
        }
        else if(direction == 3){
            if(length + row <= 20 && column - length >= 0){
                for(i = 0; i < length; i++){
                    if(puzzle[tempy][tempx] != '.'){
                        check++;
                    }
                    tempy++;
                    tempx--;
                }
                if(check == 0){
                    for(i = 0; i < length; i++){
                        puzzle[row][column] = words[i];
                        row++;
                        column--;
                    }
                    for(i = 0; i < length; i++){
                        printf_word[store] = words[i];
                        store++;
                    }
                    printf_word[store] = '\n';
                    store++;
                }
            }
            else{
                check++;
            }
        }
        else if(direction == 4){
            if(column - length >= 0){
                for(i = 0; i < length; i++){
                    if(puzzle[tempy][tempx] != '.'){
                        check++;
                    }
                    tempx--;
                }
                if(check == 0){
                    for(i = 0; i < length; i++){
                        puzzle[row][column] = words[i];
                        column--;
                    }
                    for(i = 0; i < length; i++){
                        printf_word[store] = words[i];
                        store++;
                    }
                    printf_word[store] = '\n';
                    store++;
                }
            }
            else{
                check++;
            }
        }
        else if(direction == 5){
            if(row - length >= 0 && column - length >= 0){
                for(i = 0; i < length; i++){
                    if(puzzle[tempy][tempx] != '.'){
                        check++;
                    }
                    tempy--;
                    tempx--;
                }
                if(check == 0){
                    for(i = 0; i < length; i++){
                        puzzle[row][column] = words[i];
                        column--;
                        row--;
                    }
                    for(i = 0; i < length; i++){
                        printf_word[store] = words[i];
                        store++;
                    }
                    printf_word[store] = '\n';
                    store++;
                }
            }
            else{
                check++;
            }
        }
        else if(direction == 6){
            if(row - length >= 0){
                for(i = 0; i < length; i++){
                    if(puzzle[tempy][tempx] != '.'){
                        check++;
                    }
                    tempy--;
                }
                if(check == 0){
                    for(i = 0; i < length; i++){
                        puzzle[row][column] = words[i];
                        row--;
                    }
                    for(i = 0; i < length; i++){
                        printf_word[store] = words[i];
                        store++;
                    }
                    printf_word[store] = '\n';
                    store++;
                }
            }
            else{
                check++;
            }
        }
        else if(direction == 7){
            if(row - length >= 0 && column + length <= 20){
                for(i = 0; i < length; i++){
                    if(puzzle[tempy][tempx] != '.'){
                        check++;
                    }
                    tempy--;
                    tempx++;
                }
                if(check == 0){
                    for(i = 0; i < length; i++){
                        puzzle[row][column] = words[i];
                        column++;
                        row--;
                    }
                    for(i = 0; i < length; i++){
                        printf_word[store] = words[i];
                        store++;
                    }
                    printf_word[store] = '\n';
                    store++;
                }
            }
            else{
                check++;
            }
        }
        if(check == 0){
            counter++;
        }
        check = 0;
    }
    //This function places random characters to the slots that have dot character in it.
    for(i = 0; i < 20; i++){
        for(j = 0; j < 20; j++){
            random_char = "abcdefghijklmnopqrstuvwxyz"[random() % 26];
            if(puzzle[i][j] == '.'){
                puzzle[i][j] = random_char;
            }
        }
    }
    printf("Selected words:\n");
    selected_words(printf_word, guess, 0);
    for(i = 0; i < 20; i++){
        if(i >= 0 && i < 10){
            printf("%d  ", i);
        }
        else{
            printf("%d ", i);
            if(i == 19){
                printf("\n");
            }
        }
    }
    for(i = 0; i < 59; i++){
        printf("-");
        if(i == 58){
            printf("\n");
        }
    }
    for(i = 0; i < 20; i++){
        for(j = 0; j < 20; j++){
            printf("%c  ", puzzle[i][j]);
            if(j == 19){
                printf(" |%c", 'A' + sidebar);
                sidebar++;
            }
        }
        printf("\n\n");
    }
    while(true_guesses != 10){
        sidebar = 0;
        printf("\n\nPlease enter the coordinate and the word:\n");
        scanf(" %c%d %s", &guess_row, &guess_column, guess);
        //To exit the game the user must enter a character and a number then enter exit.
        if(strcmp(guess, "exit") == 0){
            exit(0);
        }
        length_of_guess = strlen(guess);
        if(find_word(puzzle, guess, (int)guess_row, guess_column, length_of_guess) != 0){
            upper_case(puzzle, find_word(puzzle, guess, (int)guess_row, guess_column, length_of_guess), length_of_guess, (int)guess_row, guess_column);
            printf("\nSelected words:\n");
            selected_words(printf_word, guess, 1);
            true_guesses++;
        }
        else{
            printf("\nSelected words:\n");
            selected_words(printf_word, guess, 0);
        }
        for(i = 0; i < 20; i++){
            if(i >= 0 && i < 10){
                printf("%d  ", i);
            }
            else{
                printf("%d ", i);
                if(i == 19){
                    printf("\n");
                }
            }
        }
        for(i = 0; i < 59; i++){
            printf("-");
            if(i == 58){
                printf("\n");
            }
        }
        for(i = 0; i < 20; i++){
            for(j = 0; j < 20; j++){
                printf("%c  ", puzzle[i][j]);
                if(j == 19){
                    printf(" |%c", 'A' + sidebar);
                    sidebar++;
                }
            }
            printf("\n\n");
        }
    }
    printf("WELL DONE!");
    return 0;
}
int find_word(char array[20][20], char guess[], int row, int column, int length){
    int i = 0;
    row = row - 97;//I substracted 97 from the row because the row is the ascii representation of a word from a to z and a's ascii representation is 97.
    /*If the guess matches with the puzzle the function returns integers from 10 to 17.The last digit indicates the direction which is used for finding the word*/ 
    if(array[row][column] == guess[i]){
        if(array[row][column + 1] == guess[i + 1]){
            for(i = 2; i < length; i++){
                if(array[row][column + 2] != guess[i]){
                    return 0;
                }
                column++;
            }
            return 10;
        }
        if(array[row + 1][column + 1] == guess[i + 1]){
            for(i = 2; i < length; i++){
                if(array[row + 2][column + 2] != guess[i]){
                    return 0;
                }
                row++;
                column++;
            }
            return 11;
        }
        if(array[row + 1][column] == guess[i + 1]){
            for(i = 2; i < length; i++){
                if(array[row + 2][column] != guess[i]){
                    return 0;
                }
                row++;
            }
            return 12;
        }
        if(array[row + 1][column - 1] == guess[i + 1]){
            for(i = 2; i < length; i++){
                if(array[row + 2][column - 2] != guess[i]){
                    return 0;
                }
                row++;
                column--;
            }
            return 13;
        }
        if(array[row][column - 1] == guess[i + 1]){
            for(i = 2; i < length; i++){
                if(array[row][column - 2] != guess[i]){
                    return 0;
                }
                column--;
            }
            return 14;
        }
        if(array[row - 1][column - 1] == guess[i + 1]){
            for(i = 2; i < length; i++){
                if(array[row - 2][column - 2] != guess[i]){
                    return 0;
                }
                row--;
                column--;
            }
            return 15;
        }
        if(array[row - 1][column] == guess[i + 1]){
            for(i = 2; i < length; i++){
                if(array[row - 2][column] != guess[i]){
                    return 0;
                }
                row--;
            }
            return 16;
        }
        if(array[row - 1][column + 1] == guess[i + 1]){
            for(i = 2; i < length; i++){
                if(array[row - 2][column + 2] != guess[i]){
                    return 0;
                }
                row--;
                column++;
            }
            return 17;
        }
    }
    if(array[row][column] == guess[length - 1]){
        if(array[row][column + 1] == guess[length - 2]){
            for(i = (length - 3); i >= 0; i--){
                if(array[row][column + 2] != guess[i]){
                    return 0;
                }
                column++;
            }
            return 10;
        }
        if(array[row + 1][column + 1] == guess[length - 2]){
            for(i = (length - 3); i >= 0; i--){
                if(array[row + 2][column + 2] != guess[i]){
                    return 0;
                }
                row++;
                column++;
            }
            return 11;
        }
        if(array[row + 1][column] == guess[length - 2]){
            for(i = (length - 3); i >= 0; i--){
                if(array[row + 2][column] != guess[i]){
                    return 0;
                }
                row++;
            }
            return 12;
        }
        if(array[row + 1][column - 1] == guess[length - 2]){
            for(i = (length - 3); i >= 0; i--){
                if(array[row + 2][column - 2] != guess[i]){
                    return 0;
                }
                row++;
                column--;
            }
            return 13;
        }
        if(array[row][column - 1] == guess[length - 2]){
            for(i = (length - 3); i >= 0; i--){
                if(array[row][column - 2] != guess[i]){
                    return 0;
                }
                column--;
            }
            return 14;
        }
        if(array[row - 1][column - 1] == guess[length - 2]){
            for(i = (length - 3); i >= 0; i--){
                if(array[row - 2][column - 2] != guess[i]){
                    return 0;
                }
                row--;
                column--;
            }
            return 15;
        }
        if(array[row - 1][column] == guess[length - 2]){
            for(i = (length - 3); i >= 0; i--){
                if(array[row - 2][column] != guess[i]){
                    return 0;
                }
                row--;
            }
            return 16;
        }
        if(array[row - 1][column + 1] == guess[length - 2]){
            for(i = (length - 3); i >= 0; i--){
                if(array[row - 2][column + 2] != guess[i]){
                    return 0;
                }
                row--;
                column++;
            }
            return 17;
        }
    }
    else{
        return 0;
    }
    return 0;
}
void upper_case(char array[20][20], int aspect, int length, int row, int column){
    int direction = aspect%10, i;
    row = row - 97;//I substracted 97 from the row because the row is the ascii representation of a word from a to z and a's ascii representation is 97.
    if(direction == 0){
        for(i = 0; i < length; i++){
            array[row][column] -= 32;
            column++;
        }
    }
    if(direction == 1){
        for(i = 0; i < length; i++){
            array[row][column] -= 32;
            column++;
            row++;
        }
    }
    if(direction == 2){
        for(i = 0; i < length; i++){
            array[row][column] -= 32;
            row++;
        }
    }
    if(direction == 3){
        for(i = 0; i < length; i++){
            array[row][column] -= 32;
            column--;
            row++;
        }
    }
    if(direction == 4){
        for(i = 0; i < length; i++){
            array[row][column] -= 32;
            column--;
        }
    }
    if(direction == 5){
        for(i = 0; i < length; i++){
            array[row][column] -= 32;
            column--;
            row--;
        }
    }
    if(direction == 6){
        for(i = 0; i < length; i++){
            array[row][column] -= 32;
            row--;
        }
    }
    if(direction == 7){
        for(i = 0; i < length; i++){
            array[row][column] -= 32;
            column++;
            row--;
        }
    }
}
void selected_words(char array[150], char guess[15], int flag){
    int i = 0, length1, length2, length3, length4, length5, length6, length7, length8, length9, length10, total_length;
    char word1[15], word2[15], word3[15], word4[15], word5[15], word6[15], word7[15], word8[15], word9[15], word10[15];
    if(flag == 1){
        //I put \n between words at the main function so with the help of this I used %[^\n] to get the each word from array and put them in different arrays
        sscanf(array, "%[^\n] %[^\n] %[^\n] %[^\n] %[^\n] %[^\n] %[^\n] %[^\n] %[^\n] %[^\n]", word1, word2, word3, word4, word5, word6, word7, word8, word9, word10);
        length1 = strlen(word1);
        length2 = strlen(word2);
        length3 = strlen(word3);
        length4 = strlen(word4);
        length5 = strlen(word5);
        length6 = strlen(word6);
        length7 = strlen(word7);
        length8 = strlen(word8);
        length9 = strlen(word9);
        length10 = strlen(word10);
        /*I used strcmp to determine which word is equal to guess and according to that result I added the lengths of the words before it and the number of \n between them to determine which slot of the array to start upper casing.*/
        if(strcmp(word1, guess) == 0){
            total_length = 0;
        }
        if(strcmp(word2, guess) == 0){
            total_length = length1 + 1;
        }
        if(strcmp(word3, guess) == 0){
            total_length = length1 + length2 + 2;
        }
        if(strcmp(word4, guess) == 0){
            total_length = length1 + length2 + length3 + 3;
        }
        if(strcmp(word5, guess) == 0){
            total_length = length1 + length2 + length3 + length4 + 4;
        }
        if(strcmp(word6, guess) == 0){
            total_length = length1 + length2 + length3 + length4 + length5 + 5;
        }
        if(strcmp(word7, guess) == 0){
            total_length = length1 + length2 + length3 + length4 + length5 + length6 + 6;
        }
        if(strcmp(word8, guess) == 0){
            total_length = length1 + length2 + length3 + length4 + length5 + length6 + length7 + 7;
        }
        if(strcmp(word9, guess) == 0){
            total_length = length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + 8;
        }
        if(strcmp(word10, guess) == 0){
            total_length = length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + 9;
        }
        while(array[total_length] != '\n'){
            array[total_length] -=32;
            total_length++;
        }
        //After the upper casing operation I decremented the flag by 1 to print the array.
        flag--;
    }
    if(flag == 0){
        while(array[i] != '\0'){
            printf("%c", array[i]);
            i++;
        }
        printf("\n");
    }
}
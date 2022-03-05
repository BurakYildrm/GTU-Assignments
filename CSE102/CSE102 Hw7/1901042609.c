#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//I used flags because number of genres and number of platforms are different.
int check_platform_genre(char array[][20], char data[], int flag);
int index_number(char array[][20], char data[], int flag);
int main(){
    char genre[13], platform[5], str[200], list_of_genres[12][20], list_of_platforms[10][20], names[2000][100], select_game[100];
    char *pointer;
    float na, eu, global, score, information[2000][7], average = 0;
    int year, i = 0, j = 0, index, store_platform = 0, store_genre = 0, check = 0, gfrequence[12][1], pfrequence[10][1], select_year, until_or_since, check_year = 0, choice = 0;
    FILE *fp = fopen("Video_Games.txt", "r");
    //I used memset because sometimes computer can assign random characters to the array.
    memset(select_game, '\0', 100*sizeof(char));
    for(i = 0; i < 12; i++){
        gfrequence[i][0] = 0;
    }
    for(i = 0; i < 10; i++){
        pfrequence[i][0] = 0;
    }
    fscanf(fp, "%[^\n]", str);//To skip the first line because it has no data about any game
    fseek(fp, 1, SEEK_CUR);
    for(index = 0; index < 2000; index++){
        //I used strtok to get rid of the commas
        fscanf(fp, "%[^\n]", str);
        pointer = strtok(str, ",");
        sscanf(pointer, "%s", names[index]);
        pointer = strtok(NULL, ",");
        sscanf(pointer, "%s", genre);
        pointer = strtok(NULL, ",");
        sscanf(pointer, "%s", platform);
        pointer = strtok(NULL, ",");
        sscanf(pointer, "%d", &year);
        pointer = strtok(NULL, ",");
        sscanf(pointer, "%f", &na);
        pointer = strtok(NULL, ",");
        sscanf(pointer, "%f", &eu);
        pointer = strtok(NULL, ",");
        sscanf(pointer, "%f", &global);
        //As a safety belt for not available
        if(sscanf(pointer, "%f", &global) != 1){
            check++;
        }
        pointer = strtok(NULL, ",");
        sscanf(pointer, "%f", &score);
        if(strcmp(genre, "simulation") == 0){
            gfrequence[0][0] += 1;
        }
        if(strcmp(genre, "shooter") == 0){
            gfrequence[1][0] += 1;
        }
        if(strcmp(genre, "fighting") == 0){
            gfrequence[2][0] += 1;
        }
        if(strcmp(genre, "action") == 0){
            gfrequence[3][0] += 1;
        }
        if(strcmp(genre, "role_playing") == 0){
            gfrequence[4][0] += 1;
        }
        if(strcmp(genre, "racing") == 0){
            gfrequence[5][0] += 1;
        }
        if(strcmp(genre, "misc") == 0){
            gfrequence[6][0] += 1;
        }
        if(strcmp(genre, "puzzle") == 0){
            gfrequence[7][0] += 1;
        }
        if(strcmp(genre, "platform") == 0){
            gfrequence[8][0] += 1;
        }
        if(strcmp(genre, "strategy") == 0){
            gfrequence[9][0] += 1;
        }
        if(strcmp(genre, "sports") == 0){
            gfrequence[10][0] += 1;
        }
        if(strcmp(genre, "adventure") == 0){
            gfrequence[11][0] += 1;
        }
        if(strcmp(platform, "pc") == 0){
            pfrequence[0][0] += 1;
        }
        if(strcmp(platform, "ps") == 0){
            pfrequence[1][0] += 1;
        }
        if(strcmp(platform, "dc") == 0){
            pfrequence[2][0] += 1;
        }
        if(strcmp(platform, "ps2") == 0){
            pfrequence[3][0] += 1;
        }
        if(strcmp(platform, "x") == 0){
            pfrequence[4][0] += 1;
        }
        if(strcmp(platform, "gba") == 0){
            pfrequence[5][0] += 1;
        }
        if(strcmp(platform, "gc") == 0){
            pfrequence[6][0] += 1;
        }
        if(strcmp(platform, "ds") == 0){
            pfrequence[7][0] += 1;
        }
        if(strcmp(platform, "psp") == 0){
            pfrequence[8][0] += 1;
        }
        if(strcmp(platform, "x360") == 0){
            pfrequence[9][0] += 1;
        }
        if(check_platform_genre(list_of_genres, genre, 0) == 1){
            strcpy(list_of_genres[store_genre], genre);
            store_genre++;
        }
        if(check_platform_genre(list_of_platforms, platform, 1) == 1){
            strcpy(list_of_platforms[store_platform], platform);
            store_platform++;
        }
        information[index][0] = index_number(list_of_genres, genre, 0);
        information[index][1] = index_number(list_of_platforms, platform, 1);
        information[index][2] = year;
        information[index][3] = na;
        information[index][4] = eu;
        if(check == 0){
            information[index][5] = global;
        }
        else{
            //I used -1 as a not available value because I scanned the video games file and there was no -1 as any kind of info
            information[index][5] = -1;
            check = 0;
        }
        information[index][6] = score;
        fseek(fp, 1, SEEK_CUR);
    }
    while(choice != 8){
        printf("0: List of the Genres\n");
        printf("1: List of the Platforms\n");
        printf("2: List of the Games Through the Years\n");
        printf("3. All Information of a Single Game\n");
        printf("4: Average of the User Scores\n");
        printf("5: Geographical Information of a Single Game\n");
        printf("6: Frequence of the Genres\n");
        printf("7: Frequence of the Platforms\n");
        printf("8: Exit\n\n");
        printf("Please select an operaion: ");
        scanf(" %d", &choice);
        printf("\n");
        switch(choice){
            case 0:
                for(i = 0; i < 12; i++){
                    printf("%s\n", list_of_genres[i]);
                }
                printf("\n");
                break;
            case 1:
                for(i = 0; i < 10; i++){
                    printf("%s\n", list_of_platforms[i]);
                }
                printf("\n");
                break;
            case 2:
                check_year = 0;
                while(check_year == 0){
                    printf("Enter a year:\n");
                    scanf(" %d", &select_year);
                    printf("Until (0) or Since (1) %d: ", select_year);
                    scanf("%d", &until_or_since);
                    printf("\n");
                    if(until_or_since == 0){
                        check_year = 0;
                        for(index = 0; index < 2000; index++){
                            if(information[index][2] < select_year){
                                printf("%s\n", names[index]);
                                check_year++;
                            }
                        }
                        if(check_year == 0){
                            printf("TRY AGAIN\n");
                        }
                        printf("\n");
                    }
                    if(until_or_since == 1){
                        check_year = 0;
                        for(index = 0; index < 2000; index++){
                            if(information[index][2] > select_year){
                                printf("%s\n", names[index]);
                                check_year++;
                            }
                        }
                        if(check_year == 0){
                            printf("TRY AGAIN\n");
                        }
                        printf("\n");
                    }
                }
                break;
            case 3:
                //To check if there is any index that satisfies the condition.
                i = -1;
                while(i == -1){
                    printf("Please enter the name of the game:\n");
                    scanf("%s", select_game);
                    for(index = 0; index < 2000; index++){
                        if(strcmp(select_game, names[index]) == 0){
                            i++;
                            //To get the index of the first game that satisfies the condition.
                            break;
                        }
                    }
                    if(i == -1){
                        printf("TRY AGAIN\n\n");
                    }
                }
                printf("Name:\t\t%s\n", select_game);
                printf("Genre:\t\t%s\n", list_of_genres[(int)information[index][0]]);
                printf("Platform:\t%s\n", list_of_platforms[(int)information[index][1]]);
                printf("Year:\t\t%.0f\n", information[index][2]);
                printf("Sales In NA:\t%f\n", information[index][3]);
                printf("Sales in EU:\t%f\n", information[index][4]);
                if(information[index][5] == -1){
                    printf("Total Sales:\tNot Available\n");
                }
                else{
                    printf("Total Sales:\t%f\n", information[index][5]);
                }
                printf("User Score:\t%f\n\n", information[index][6]);
                break;
            case 4:
                for(index = 0; index < 2000; index++){
                    average += information[index][6];
                }
                average /= 2000;
                printf("Average: %f\n\n", average);
                break;
            case 5:
                //To check if there is any index that satisfies the condition.
                i = -1;
                while(i == -1){
                    printf("Please enter the name of the game:\n");
                    scanf("%s", select_game);
                    for(index = 0; index < 2000; index++){
                        if(strcmp(select_game, names[index]) == 0){
                            i++;
                            //To get the index of the first game that satisfies the condition.
                            break;
                        }
                    }
                    if(i == -1){
                        printf("TRY AGAIN\n\n");
                    }
                }
                if(information[index][3] > information[index][4]){
                    printf("This game was more popular in North America.\n\n");
                }
                else if(information[index][3] < information[index][4]){
                    printf("This game was more popular in Europe.\n\n");
                }
                else{
                    printf("This game was equally popular in North America and Europe.\n\n");
                }
                break;
            case 6:
                j = 0;
                for(i = 0; i < 12; i++){
                    printf("%s", list_of_genres[i]);
                    if(strlen(list_of_genres[i]) < 8){
                        printf("\t\t%d\n", gfrequence[j][0]);
                        j++;
                    }
                    else{
                        printf("\t%d\n", gfrequence[j][0]);
                        j++;
                    }
                }
                printf("\n");
                break;
            case 7:
                j = 0;
                for(i = 0; i < 10; i++){
                    printf("%s\t", list_of_platforms[i]);
                    printf("%d\n", pfrequence[j][0]);
                    j++;
                }
                printf("\n");
                break;
            case 8:
                printf("GOOD BYE\n");
                break;
            default:
                printf("TRY AGAIN\n\n");
        }
    }
    return 0;
}
int check_platform_genre(char array[][20], char data[], int flag){
    int i;
    if(array[0][0] == '\0'){
        return 1;
    }
    else{
        if(flag == 0){
            for(i = 0; i < 12; i++){
                if(strcmp(array[i], data) == 0){
                    return 0;
                }
            }
        }
        else{
            for(i = 0; i < 10; i++){
                if(strcmp(array[i], data) == 0){
                    return 0;
                }
            }
        }
        return 1;
    }
}
int index_number(char array[][20], char data[], int flag){
    int i, count = -1;
    if(flag == 0){
        for(i = 0; i < 12; i++){
            count++;
            if(strcmp(array[i], data) == 0){
                return count;
            }
        }
    }
    else{
        for(i = 0; i < 10; i++){
            count++;
            if(strcmp(array[i], data) == 0){
                return count;
            }
        }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
struct Hospital {
    char* name;
    char citiesServed[4];
};
struct card {
    const char *face; 
    const char *suit;
};
int numPathsHome(int street, int avenue);
int canOfferCovidCoverage(char _cities[6], char _locations[4][4], int _numHospitals, struct Hospital results[4]);
int check_combinations(struct Hospital array[4], char cities[6]);
int check_locations(char location[][4]);
int main(){
    FILE *fp = fopen("cards.txt", "w+");
    int choice, street, avenue, random_num, order[52], counter = 0, check = 0, num_hospital, result;
    char faces[13][10] = {"Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
    char suits[4][10] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    char temp_card[30], cities[6] = {'A', 'B', 'C', 'D', 'E', 'F'}, locations[4][4] = {{'A', 'B', 'C'}, {'A', 'C', 'D'}, {'B', 'F'}, {'C', 'E', 'F'}};
    char card_face[10], card_of[5], card_suit[10];
    const char *pointer1, *pointer2;
    struct card combinations[4][13];
    struct Hospital hospitals[4];
    srand(time(NULL));
    for(int i = 0; i < 52; i++){
        order[i] = 0;   /*First I assigned 0 to all slots of order array because in the reading part of part 3 I randomly selected numbers from 1 to 52 so any new number wouldn't overlap with the ones in the array.*/
    }
    printf("Which part do you want to execute?\n");
    scanf(" %d", &choice);
    switch(choice){
        case 1:
            printf("Enter the street number: ");
            scanf("%d", &street);
            printf("street: %d\n", street);
            printf("Enter the avenue number: ");
            scanf("%d", &avenue);
            printf("avenue: %d\n", avenue);
            printf("Number of optimal paths to take back home: %d\n", numPathsHome(street - 1, avenue - 1));/*I decremented the street and avenue values by 1 beacuse the ending point is (1,1), not (0,0)*/
            break;
        case 2:
            printf("Enter the maximum number of hospitals that can be constructed: ");
            scanf("%d", &num_hospital);
            for(int i = 0; i < 4; i++){
                memset(hospitals[i].citiesServed, '\0', 4*sizeof(char));//To prevent computer to assign random characters to the array
            }
            if(canOfferCovidCoverage(cities, locations, num_hospital, hospitals) == 0){
                printf("No, some cities are not covered.\n");
            }
            else{
                printf("Yes, can offer health care to all!\n");
                for(int i = 0; i < 4; i++){
                    if(hospitals[i].citiesServed[0] != '\0'){
                        printf("Hospital - %d: %s\n", i + 1, hospitals[i].citiesServed);
                    }
                }
            }
            break;
        case 3:
            for(int i = 0; i < 4; i++){
                pointer1 = &*suits[i]; //I used pointers because other way it wouldn't hold the data;
                for(int j = 0; j < 13; j++){
                    pointer2 = &*faces[j];
                    combinations[i][j].face = &*pointer2;
                    combinations[i][j].suit = &*pointer1;
                }
            }
            /*First I took all suits and faces one by one and placed them in to combinations and took the data from combinations one by one and printed them into cards.txt. Then I randomly selected a number from 1 to 52 and scanned the line which is indicated by random number and printed it into console. I assigned the random number to the order array to be sure that the same number wouldn't be selected*/
            for(int i = 0; i < 4; i++){
                for(int j = 0; j< 13; j++){
                    fprintf(fp, "%s of %s\n", combinations[i][j].face, combinations[i][j].suit);
                }
            }
            fclose(fp);
            fp = fopen("cards.txt", "r");
            while(counter != 52){
                random_num = (rand() % 52) + 1;
                for(int i = 0; i < 52; i++){
                    if(order[i] == random_num){
                        check++;
                    }
                }
                if(check == 0){
                    order[counter] = random_num;
                    counter++;
                    for(int i = 0; i < random_num; i++){
                        fgets(temp_card, 30, fp);
                    }
                    temp_card[strlen(temp_card) - 1] = '\0';//To remove the new line character
                    /*To allign the ofs I scanned the strings one by one from temp and printed them into the console in a format that ofs are alligned*/
                    sscanf(temp_card, "%s %s %s", card_face, card_of, card_suit);
                    if(counter%2 == 0){
                        printf("%7s %s %s\n", card_face, card_of, card_suit);
                    }
                    else{
                        printf("%7s %s %s\t", card_face, card_of, card_suit);
                    }
                    rewind(fp);
                }
                check = 0;
            }
            fclose(fp);
            break;
    }
    return 0;
}
int numPathsHome(int street, int avenue){
    int numerator = 1, denominator_street = 1, denominator_avenue = 1;
    static int check_numerator = 0, check_street = 0, check_avenue = 0, temp_sum = 0, temp_street, temp_avenue;
    /*I used 3 static check variables to make sure that function calculates the factorials one by one and used 3 temp variables to make sure that when factorial process is done function doesn't returns the result to the main function and uses it onwards*/
    if(check_numerator == 0){
        if(temp_sum == 0){
            temp_avenue = avenue;
            temp_street = street;
            temp_sum = street + avenue;
        }
        if(street + avenue == 1){
            return 1;
        }
        if(street >= 0){
            numerator = (street + avenue)*numPathsHome(street - 1, avenue);
            if(street + avenue != temp_sum){
                return numerator;
            }
            else{
                check_numerator++;//To advance the next stage
            }
        }
        else{
            numerator = (street + avenue)*numPathsHome(street, avenue - 1);
            return numerator;
        }
    }
    if(check_street == 0){
        if(temp_street == 1){
            denominator_street = 1;
        }
        else{
            if(street == 1){
                return 1;
            }
            else{
                denominator_street = street*numPathsHome(street - 1, avenue);
                if(street != temp_street){
                    return denominator_street;
                }
                else{
                    check_street++;//To advance the next stage
                }
            }
        }
    }
    if(check_avenue == 0){
        if(temp_avenue == 1){
            denominator_avenue = 1;
        }
        else{
            if(avenue == 1){
                return 1;
            }
            else{
                denominator_avenue = avenue*numPathsHome(street, avenue- 1);
                if(avenue != temp_avenue){
                    return denominator_avenue;
                }
                else{
                    check_avenue++;//To advance the next stage
                }
            }
        }
    }
    return numerator/denominator_avenue/denominator_street;//(street + avenue)!/street!/avenue! gives the number of possible shortest ways back home
}
int canOfferCovidCoverage(char _cities[6], char _locations[4][4], int _numHospitals, struct Hospital results[4]){
    char temp[15];
    int check, i, j;
    if(_numHospitals == 0){
        if(check_combinations(results, _cities) == 1){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        strcpy(results[_numHospitals - 1].citiesServed, _locations[0]);
        check = canOfferCovidCoverage(_cities, &_locations[1], _numHospitals - 1, results);
        if(check == 1){
            return 1;
        }
        else{
            if(check_locations(_locations) == 2 || check_locations(_locations) == 3){
                /*Since the location[0] has min 2 max 3 cities if result is not more than 2 or 3 thah means this is the last location*/
                return 0;
            }
            else{
                check = canOfferCovidCoverage(_cities, &_locations[1], _numHospitals, results);
                if(check == 1){
                    return 1;
                }
                else{
                    return 0;
                }
            }
        }
    }
}
int check_combinations(struct Hospital array[4], char cities[6]){
    char temp[15];
    int counter = 0, lenght = 0;
    strcpy(temp, array[0].citiesServed);
    for(int i = 0; i < 3; i++){
        lenght += strlen(array[i].citiesServed);
        sprintf(&temp[lenght], "%s", array[i + 1].citiesServed);
    }
    for(int i = 0; i < 6; i++){//Checks if the all cities that are covered by the hospitals in the array match with the cities that are in cities array
        for(int j = 0; j < strlen(temp); j++){
            if(cities[i] == temp[j]){//If it matches counter is incremented
                counter++;
                break;
            }
        }
    }
    if(counter == 6){//Since there is 6 cities if the counter is 6 then it means that this line up of hospitals is covering the all cities
        return 1;
    }
    else{//Else returns 0 indicating that not all the cities are covered
        return 0;
    }
}
int check_locations(char location[][4]){
    int count = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){//Checks if the location[0] is the last location
            if(location[i][j] == 'A' || location[i][j] == 'B' || location[i][j] == 'C' || location[i][j] == 'D' || location[i][j] == 'E' || location[i][j] == 'F'){
                count++;
            }
        }
    }
    return count;
}
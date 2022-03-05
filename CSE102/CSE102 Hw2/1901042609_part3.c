#include<stdio.h>
int main(){
    int currency, choice;
    float amount, euro, dollar;
    euro = 6.69;
    dollar = 6.14;
    printf("***** Welcome to ABC Exchange Office *****\n");
    printf("Enter your amount:\n");
    scanf("%f", &amount);
    printf("Please select your currency\n");
    printf("1. Turkish Liras\n");
    printf("2. Euro\n");
    printf("3. Dollar\n");
    scanf("%d", &currency);
    switch(currency){
        case 1:
            printf("You have %f Turkish Liras\n", amount);
            break;
        case 2:
            printf("You have %f Euro\n", amount);
            break;
        case 3:
            printf("You have %f Dollars\n", amount);
            break;
        default:
            printf("Your selection is invalid!\n");
    }
    printf("Choose which currency you want to convert\n");
    scanf("%d", &choice);
    if(currency == 1){
        switch(choice){
            case 1:
                printf("You have %f Turkish Liras", amount);
                break;
            case 2:
                amount = amount/euro;
                printf("You have %f Euro", amount);
                break;
            case 3:
                amount = amount/dollar;
                printf("You have %f Dollars", amount);
                break;
            default:
                printf("Your selection is invalid!\n");
        }
    }
    if(currency == 2){
        switch(choice){
            case 1:
                amount = amount*euro;
                printf("You have %f Turkish Liras", amount);
                break;
            case 2:
                printf("You have %f Euro", amount);
                break;
            case 3:
                amount = amount*(euro/dollar);
                printf("You have %f Dollars", amount);
                break;
            default:
                printf("Your selection is invalid!\n");
        }
    }
    if(currency == 3){
        switch(choice){
            case 1:
                amount = amount*dollar;
                printf("You have %f Turkish Liras", amount);
                break;
            case 2:
                amount = amount/(euro/dollar);
                printf("You have %f Euro", amount);
                break;
            case 3:
                printf("You have %f Dollars", amount);
                break;
            default:
                printf("Your selection is invalid!\n");
        }
    }
    return 0;
}
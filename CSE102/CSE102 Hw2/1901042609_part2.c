#include<stdio.h>
#include<math.h>
int number_length(int number);
int find_digit(int number, int index);
int main(){
    int number, x, y, index;
    printf("Enter a number (maximum 6 digits): ");
    scanf("%d", &number);
    /*I used x and y to make code more readible*/
    x = number_length(number);
    printf("Your number has %d digits\n",x);
    printf("When your number is written 100 times next to each other, which digit of this number would you like to see?: ");
    scanf("%d", &index);
    y = find_digit(number, index);
    printf("%dth digit of the big number sequence: %d", index, y);
}
int number_length(int number){
    number = number/10;
    if(number == 0){
        return(1);
    }
    number = number/10;
    if(number == 0){
        return(2);
    }
    number = number/10;
    if(number == 0){
        return(3);
    }
    number = number/10;
    if(number == 0){
        return(4);
    }
    number = number/10;
    if(number == 0){
        return(5);
    }
    number = number/10;
    if(number == 0){
        return(6);
    }
    return 0;
}
int find_digit(int number, int index){
    int a, b, temp;
    a = number_length(number);
    b = index%a;
    if(b == 0){
        temp = number;
        number = number/pow(10, 1);
        number = number*10;
        return(temp - number);
    }
    if(b == 1){
        temp = number/pow(10, (a - 1));
        return(temp);
    }
    if(b == 2){
        temp = number/pow(10, (a - 2));
        number = number/pow(10, (a - 1));
        number = number*10;
        return(temp - number);
    }
    if(b == 3){
        temp = number/pow(10, (a - 3));
        number = number/pow(10, (a - 2));
        number = number*10;
        return(temp - number);
    }
    if(b == 4){
        temp = number/pow(10, (a - 4));
        number = number/pow(10, (a - 3));
        number = number*10;
        return(temp - number);
    }
    if(b == 5){
        temp = number/pow(10, (a - 5));
        number = number/pow(10, (a - 4));
        number = number*10;
        return(temp - number);
    }
    return 0;
}
#include <stdio.h>
int find_length(int n);
int binary(int n);
int andop(int a, int b);
int power(int base, int exponent);
int main(){
    int num1, num2, x, y;
    printf("First integer: ");
    scanf("%d", &num1);
    printf("Second integer: ");
    scanf("%d", &num2);
    x = find_length(num1) - find_length(num2);
    y = binary(num1) + binary(num2);
    while(x != 0 || y != 2){
        if(x != 0){
            printf("Integers should have the same length, please enter 2 new integers.\n");
        }
        else{
            printf("Integers should be binary, please enter 2 new integers.\n");
        }
        printf("First integer: ");
        scanf("%d", &num1);
        printf("Second integer: ");
        scanf("%d", &num2);
        x = find_length(num1) - find_length(num2);
        y = binary(num1) + binary(num2);
    }
    printf("%d AND %d = %d\n", num1, num2, andop(num1, num2));
    return 0;
}
int find_length(int n){
    int counter = 0;
    while(n != 0){
        n /= 10;
        counter++;
    }
    return counter;
}
int binary(int n){
    int j;
    while(n > 0){
        j = n%10;
        if(j != 0 && j != 1){
            return 0;
        }
        n /= 10;
    }
    return 1;
}
int power(int base, int exponent){
    int j = 1, temp = base;
    while(j != exponent){
        base *= temp;
        j++;
    }
    return base;
}
int andop(int a, int b){
    int x, y, i = 0, sum = 0, temp, pow;
    /*Program takes the last digits of the number1 and number2 and compares them.If they are both 1 temp is set to 1, in other cases temp is set to 0.Int i is incremented by 1 at the end of the each cycle and sent to power function if the temp is 1.I used addition to align the 1s and 0s side by side.Function does this procedure until the both number1 and number2 is 0.*/
    while(a != 0){
        x = a%10;
        a /= 10;
        y = b%10;
        b /= 10;
        if(x == 1 && y == 1){
            temp = 1;
        }
        else{
            temp = 0;
        }
        if(i == 0){
            if(temp == 1){
                sum = sum + temp;
            }
        }
        else if(temp == 1){
            pow = power(10,i);
            sum = sum + temp*pow;
        }
        i++;
    }
    return sum; 
}
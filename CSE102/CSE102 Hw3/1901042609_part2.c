#include <stdio.h>
#include <math.h>
int isprime(int a); 
int main(){
    int N, i, temp;
    printf("Please enter an integer: ");
    scanf("%d", &N);
    //I used for loop in the main function to print the prime numbers from main function instead of the isprime function.
    for(i=2;i<=N;i++){
        temp = isprime(i);
        if(temp == 1){
            printf("%d is a prime number\n", i);
        }
        else{
            printf("%d is not a prime number, it is dividible by %d\n", i, temp);
        }
    }
    return 0;
}
int isprime(int a){
    int x = sqrt(a), y;
    for(y=2;y<=x;y++){
        if(a%y == 0){
            return y;
        }
    }
    return 1;
}
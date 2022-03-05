#include <stdio.h>
int sum(int n, int flag);
int mult(int n, int flag);
int main(){
    int N, operation, choice;
    printf("Enter an integer: ");
    scanf("%d", &N);
    printf("Please enter '0' for sum, '1' for multiplication\n");
    scanf("%d", &operation);
    switch(operation){
        case 0:
            printf("Please enter '0' to work on even numbers, '1' to work on odd numbers\n");
            scanf("%d", &choice);
            printf(" = %d\n", sum(N, choice));
            break;
        case 1:
            printf("Please enter '0' to work on even numbers, '1' to work on odd numbers\n");
            scanf("%d", &choice);
            printf(" = %d\n", mult(N, choice));
            break;
        default:
            printf("Unsupported operation.\n");
    }
    return 0;
}
//I returned the total sum because int typed functions should return something.
int sum(int n, int flag){
    int i, sum = 0;
    if(flag == 0){
        for(i=1;i<=n;i++){
            if(i == n || i == (n - 1)){
                if(i%2 == 0){
                    sum += i;
                    printf("%d", i);
                    return sum;
                }
            }
            else if(i%2 == 0){
                printf("%d + ", i);
                sum += i;
            }
        }
    }
    else if(flag == 1){
        for(i=1;i<=n;i++){
            if(i == n || i == (n - 1)){
                if(i%2 != 0){
                    sum += i;
                    printf("%d", i);
                    return sum;
                }
            }
             else if(i%2 != 0){
                printf("%d + ", i);
                sum += i;
            }
        }
    }
    else{
        printf("Invalid value for odd/even selection\n");
    }
    return 0;
}
int mult(int n, int flag){
    int i , multiple = 1; 
    if(flag == 0){
        for(i=1;i<=n;i++){
            if(i == n || i == (n - 1)){
                if(i%2 == 0){
                    multiple *= i;
                    printf("%d", i);
                    return multiple;
                }
            }
            else if(i%2 == 0){
                printf("%d * ", i);
                multiple *= i;
            }
        }
    }
    else if(flag == 1){
        for(i=1;i<=n;i++){
            if(i == n || i == (n - 1)){
                if(i%2 != 0){
                    multiple *= i;
                    printf("%d", i);
                    return multiple;
                }
            }
            else if(i%2 != 0){
                printf("%d * ", i);
                multiple *= i;
            }
        }
    }
    else{
        printf("Invalid value for odd/even selection\n");
    }
    return 0;
}
#include <stdio.h>

float Add(float num1, float num2)
{
    float Add=num1+num2;
    printf("Result is %f", Add);
    return(Add);
}

float Substract(float num1, float num2)
{
    float Substract=num1-num2;
    printf("Result is %f", Substract);
    return(Substract);
}

float Multiply(float num1, float num2)
{
    float Multiply=num1*num2;
    printf("Result is %f", Multiply);
    return(Multiply);
}

float Divide(float num1, float num2)
{
    float Divide=num1/num2;
    printf("Result is %f", Divide);
    return(Divide);
}

float Power(float num1, float num2)
{
    float i,Power=1;
    for(i=0;i<num2;i++)
    {
        Power*=num1;
    }
    printf("Result is %f", Power);
    return(Power);
}

float Average()
{
    char c;
    float counter=-1.0;
    float sum=0.0,num;
    while((c=getchar())!= 'q' && c!=EOF)
    {
        printf("Number: ");
        scanf("%f", &num);
        sum=sum+num;
        num=0;
        counter++;
    }
    printf("Result is %f", sum/counter);
    return(sum/counter);
}

float Maximum()
{
    char a;
    float max=-999999999999999999;
    float num;
    while((a=getchar())!= 's' && a!=EOF)
    {
        printf("Number: ");
        scanf("%f", &num);
        if(num>max)
        {
            max=num;
        }
    }
    printf("Maximum number is %f", max);
    return(max);
}
    


int main()
{
    printf("WELCOME TO GTU CALCULATOR MACHINE\n");
    printf("STUDENT NAME: BURAK YILDIRIM\n");
    printf("MENU:\n");
    printf("(1) ADD TWO NUMBERS\n");
    printf("(2) SUBSTRACT TWO NUMBERS\n");
    printf("(3) MULTIPLY TWO NUMBERS\n");
    printf("(4) DIVIDE TWO NUMBERS\n");
    printf("(5) TAKE THE NTH POWER OF A NUMBER\n");
    printf("(6) FIND AVERAGE OF NUMBERS INPUTTED\n");
    printf("(7) FIND THE MAXIMUM OF NUMBERS INPUTTED\n");
    printf("(0) EXIT\n");
    printf("PLEASE SELECT\n");
    
    float num1, num2, num3;
    
    while(num3 != 0)
    {
        printf("\nPlease choose an operation: \n");
        scanf("%f", &num3);
    
        if(num3==1)
        {
            printf("Please enter num1: \n");
            scanf("%f", &num1);
            printf("Please enter num2: \n");
            scanf("%f", &num2);
            Add(num1, num2);
        }
    
        if(num3==2)
        {
            printf("Please enter num1: \n");
            scanf("%f", &num1);
            printf("Please enter num2: \n");
            scanf("%f", &num2);
            Substract(num1, num2);
        }
    
        if(num3==3)
        {
            printf("Please enter num1: \n");
            scanf("%f", &num1);
            printf("Please enter num2: \n");
            scanf("%f", &num2);
            Multiply(num1, num2);
        }
    
        if(num3==4)
        {
            printf("Please enter num1: \n");
            scanf("%f", &num1);
            printf("Please enter num2: \n");
            scanf("%f", &num2);
            Divide(num1, num2);
        }
    
        if(num3==5)
        {
            printf("Please enter base number: \n");
            scanf("%f", &num1);
            printf("Please power number: \n");
            scanf("%f", &num2);
            Power(num1, num2);
        }
        
        if(num3==6)
        {
            Average();
        }
        
        if(num3==7)
        {
            Maximum();
        }
    }
    return 0;
}


    





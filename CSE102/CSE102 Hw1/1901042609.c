#include <stdio.h>
#include <math.h>
double root_for_newton_method(int num1, int num2, int num3, int num4);
void find_root();
void find_newtonian_root();
int find_multiple_closest(int a, int b);
int main(){       /*I used a bit many variables for not to mix variables from different functions.*/
    find_root();
    find_newtonian_root();
    int a,b,c;                                  
    printf("Please enter the first integer: ");
    scanf("%d", &a);
    printf("Please enter the second integer: ");
    scanf("%d", &b);
    /*I assigned an extra variable c to compare with first integer and according to the result output will change.*/
    c = find_multiple_closest(a, b);
    if(c == a){
        printf("Closest number to %d that is multiple of %d is itself.\n",a,b);
    }
    if(c != a){
        printf("Closest number to %d that is multiple of %d is %d.\n",a,b,c);
    }
    return 0;
}
/*I wrote another root function for newton method because there are different roots 
but newtons method converges only to the root that is closest to the initial number so I had to determine that root*/
double root_for_newton_method(int num1, int num2, int num3, int num4){   
    int delta;
    delta = (num2*num2)-(4*num1*num3);
    double x1, x2;
    x1 = (-num2 + sqrt(delta))/2/num1;
    x2 = (-num2 - sqrt(delta))/2/num1;
    /*I took the absolute values of the differences between roots and initial to decide which root that newton method will converge*/  
    if(fabs(x1 - num4) > fabs(x2 - num4)){      
        return x2;
    }
    if(fabs(x1 - num4) < fabs(x2 - num4)){
        return x1;
    }
    return 0;
}
void find_root(){
    int int1, int2, int3;                                    
    printf("Please enter the first coefficent: ");
    scanf("%d", &int1);
    printf("Please enter the second coefficent: ");
    scanf("%d", &int2);
    printf("Please enter the third coefficent: ");
    scanf("%d", &int3);
    /*I assigned delta as a type double variable because square root function takes double typed values and returns it as a double typed value*/ 
    double delta,x1,x2; 
    delta = (int2*int2)-(4*int1*int3);
    if(delta < 0){
        printf("Your equation %dx² %+dx %+d does not have any real roots.\n", int1, int2, int3);
    }
    if(delta == 0){
        x1 = (-int2)/2/int1;
        printf("Your equation %dx² %+dx %+d has a double root {%.2lf}.\n", int1, int2, int3, x1);
    }
    if(delta > 0){
        x1 = (-int2 + sqrt(delta))/2/int1;
        x2 = (-int2 - sqrt(delta))/2/int1;
        printf("Your equation %dx² %+dx %+d has real roots {%.2lf,%.2lf}.\n", int1, int2, int3, x1, x2);  
    }
}
void find_newtonian_root(){
    int num1, num2, num3, num4;
    printf("Please enter the first coefficent: ");
    scanf("%d", &num1);
    printf("Please enter the second coefficent: ");
    scanf("%d", &num2);
    printf("Please enter the third coefficent: ");
    scanf("%d", &num3);
    printf("Please enter the initial: ");
    scanf("%d", &num4);
    double x,y,f,df,difference;
    int i;
    printf("Your equation is %dx² %+dx %+d and iterations are\n",num1,num2,num3);
    printf("Step    x       f(x)   Difference\n");
    double x0 = num4;
    i = 1;
    y = root_for_newton_method(num1,num2,num3,num4);
    f = num1*x0*x0 + num2*x0 + num3;        //function itself with the initial value.
    df = num1*2*x0 + num2;                  //derivative of the function.
    x = x0 - (f/df);                        //x value that obtained by newton's method.
    difference = fabs(y - x);                //the difference between approximate root and real root.
    f = num1*x*x + num2*x + num3;           //function itself with the x value by obtained from newton's method.
    if(f < 10){                                         /*for all of the f(x)s to have the same widht.*/
        printf("x%d   %.4lf  %.5lf    %.4lf\n",i,x,f,difference);
    }
    if(f >= 10){
        printf("x%d   %.4lf  %.4lf    %.4lf\n",i,x,f,difference);
    }
    x0 = x;
    i++;
    f = num1*x0*x0 + num2*x0 + num3;        
    df = num1*2*x0 + num2;                 
    x = x0 - (f/df);                        
    difference = fabs(y - x);                
    f = num1*x*x + num2*x + num3;           
    if(f < 10){                                         
        printf("x%d   %.4lf  %.5lf    %.4lf\n",i,x,f,difference);
    }
    if(f >= 10){
        printf("x%d   %.4lf  %.4lf    %.4lf\n",i,x,f,difference);
    }
    x0 = x;
    i++;
    f = num1*x0*x0 + num2*x0 + num3;        
    df = num1*2*x0 + num2;                 
    x = x0 - (f/df);                        
    difference = fabs(y - x);                
    f = num1*x*x + num2*x + num3;           
    if(f < 10){                                         
        printf("x%d   %.4lf  %.5lf    %.4lf\n",i,x,f,difference);
    }
    if(f >= 10){
        printf("x%d   %.4lf  %.4lf    %.4lf\n",i,x,f,difference);
    }
    x0 = x;
    i++;
    f = num1*x0*x0 + num2*x0 + num3;        
    df = num1*2*x0 + num2;                 
    x = x0 - (f/df);                        
    difference = fabs(y - x);                
    f = num1*x*x + num2*x + num3;           
    if(f < 10){                                         
        printf("x%d   %.4lf  %.5lf    %.4lf\n",i,x,f,difference);
    }
    if(f >= 10){
        printf("x%d   %.4lf  %.4lf    %.4lf\n",i,x,f,difference);
    }
    x0 = x;
    i++;
    f = num1*x0*x0 + num2*x0 + num3;        
    df = num1*2*x0 + num2;                 
    x = x0 - (f/df);                        
    difference = fabs(y - x);                
    f = num1*x*x + num2*x + num3;           
    if(f < 10){                                         
        printf("x%d   %.4lf  %.5lf    %.4lf\n",i,x,f,difference);
    }
    if(f >= 10){
        printf("x%d   %.4lf  %.4lf    %.4lf\n",i,x,f,difference);
    }
    x0 = x;
    i++;
}
int find_multiple_closest(int a, int b){
    if(a%b == 0){
        return(a);
    }
    if(a%b != 0){
        int temp,check;
        temp = a/b;
        check = temp + 1;
        if((check*b - a) > (a - temp*b)){   /*checks which one of the variables are closer to the first integer that inputted by user*/
            return(b*temp);
        }
        else{
            return(b*check);
        }
    }
    return 0;
}
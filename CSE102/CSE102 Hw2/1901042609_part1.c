#include<stdio.h>
#include<math.h>
int draw_triangle(int side1,int side2, int side3);
void type_triangle(int side1,int side2, int side3);
int perimeter_triangle(int side1,int side2, int side3);
double area_triangle(int side1,int side2, int side3,int perimeter);
int main(){
    int side1 ,side2, side3, x, perimeter;
    float area;
    printf("The length of the first side: ");
    scanf("%d", &side1);
    printf("The length of the second side: ");
    scanf("%d", &side2);
    printf("The length of the third side: ");
    scanf("%d", &side3);
    x = draw_triangle(side1, side2, side3);
    if(x == 1){         /*I used an extra variable x in order to make the documantation of the code better*/
        printf("According to the triangle inequality theorem, this triangle can be drawn.\n");
        type_triangle(side1, side2, side3);
        perimeter = perimeter_triangle(side1, side2, side3);
        printf("The perimeter of the triangle: %d\n",perimeter);
        area = area_triangle(side1, side2, side3, perimeter);
        printf("The area of the triangle: %f",area);
    }
    if(x != 1){
        printf("According to the triangle inequality theorem, this triangle cannot be drawn.\n");
    }
}
int draw_triangle(int side1,int side2, int side3){
    /*Variables' last 2 digits are the side numbers of the triange.For example sum12 means sum of side1 and side2.*/
    int sum12, sum13, sum23, subt12, subt13, subt23;   
    int i = 0;
    sum12 = side1 + side2;
    sum13 = side1 + side3;
    sum23 = side2 + side3;
    /*I tried fabs function for substaction but compiler gave me warning so I gave up using fabs and instead of fabs I used if function to check if the first one is greater or smaller*/
    if(side1 >= side2){
        subt12 = side1 - side2;
    }
    if(side1 < side2){
        subt12 = side2 - side1;
    }
    if(side1 >= side3){
        subt13 = side1 - side3;
    }
    if(side1 < side3){
        subt13 = side3 - side1;
    }
    if(side2 >= side3){
        subt23 = side2 - side3;
    }
    if(side2 < side3){
        subt23 = side3 - side2;
    }
    /*These 3 if functions are the triangle inequality theorem's conditions.*/
    if(side1 < sum23 && side1 > subt23){
        i++;
    }
    if(side2 < sum13 && side2 > subt13){
        i++;
    }
    if(side3 < sum12 && side3 > subt12){
        i++;
    }
    /*If all the conditions are satisfied then the function will return 0 and if not then it will return 0.*/
    if(i == 3){
        return(1);
    }
    else{
        return(0);
    }
}
void type_triangle(int side1,int side2, int side3){
    int i = 0;
    /*I used divide operator to decide whether the sides are equal or not.*/
    if(side1/side2 == 1){
        i++;
    }
    if(side1/side3 == 1){
        i++;
    }
    if(side2/side3 == 1){
        i++;
    }
    if(i == 3){
        printf("It is a equilateral triangle\n");
    }
    if(i == 1){
        printf("It is a isosceles triangle\n");
    }
    if(i == 0){
        printf("It is a scalene triangle\n");
    }
}
int perimeter_triangle(int side1,int side2, int side3){
    return(side1 + side2 + side3);
}
double area_triangle(int side1,int side2, int side3,int perimeter){
    float i, p;
    p = perimeter/2.0;
    i = sqrt(p*(p - side1)*(p - side2)*(p - side3));
    return(i);
}
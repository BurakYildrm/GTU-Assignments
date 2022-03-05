#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265
struct coordinate{
    float x;
    float y;
};
struct objects{
    char name[10];
    char definition[21][10];
    struct point{
        char name[10];
        struct coordinate coordinates;
    }points;
    struct line{
        char name[10];
        struct coordinate dots[2];
    }lines;
    struct polygon{
        char name[10];
        struct coordinate elements[40];
    }polygons;
};
struct actions{
    char operation[10];
    char components[2][10];
};
void get_data(struct objects *info, FILE *commands, int num_line);
void get_action(struct actions *info, FILE *commands, int num_line);
void execute_actions(struct objects *data, struct actions *action, int num_data, int num_action, char file_path[]);
int check_perpendicular(struct coordinate line_coordinates[2], struct coordinate point_coordinates);
int check_polygons(struct coordinate elements[40]);
int count_polygon_elements(struct coordinate elements[40]);
void find_index(struct objects *data, char target[], int num_data, int* target_index);
void line_equation(float x1, float y1, float x2, float y2, float *a, float *b, float *c);
int count_elements(char array[21][10]);
void remove_tabs(char* arr);
int main(){
    int num_data, num_action, counter = 0, i, j, k, index_num;
    struct objects *data;
    struct actions *action;
    char buffer[100], output_file[20], component[10];
    FILE *fp = fopen("commands.txt", "r");
    fscanf(fp, "%s", buffer);    
    if(strcmp(buffer, "data") == 0){
        fseek(fp, 1, SEEK_CUR);
        fscanf(fp, "%d", &num_data);
        fseek(fp, 2, SEEK_CUR);
        data = (struct objects*)calloc(num_data, sizeof(struct objects));
        get_data(data, fp, num_data);
        fseek(fp, 1, SEEK_CUR);
    }
    //First I gathered all the data to one 2d char array and then seperated them into different structures according to the type of object.
    for(i = 0; i < num_data; i++){
        if(data[i].definition[0][0] < 58){
            strcpy(data[i].points.name, data[i].name);
            sscanf(data[i].definition[0], "%f", &data[i].points.coordinates.x);
            sscanf(data[i].definition[1], "%f", &data[i].points.coordinates.y);
        }
        else{
            /*For lines I stored coordinates of end points and for polygons first I checked the first element of polygon whether it's a point or not.If it's a point I stored the coordinates of all the point and if it's not I searched the name of line in the line structure and stored the end points of that line.*/
            strcpy(component, data[i].definition[0]);
            find_index(data, component, num_data, &index_num);
            if(index_num%10 == 1){
                if(count_elements(data[i].definition) == 2){
                    strcpy(data[i].lines.name, data[i].name);
                    for(j = 0; j < 2; j++){
                        data[i].lines.dots[j].x = data[index_num/10].points.coordinates.x;
                        data[i].lines.dots[j].y = data[index_num/10].points.coordinates.y;
                        strcpy(component, data[i].definition[1]);
                        find_index(data, component, num_data, &index_num);
                    }
                }
                else{
                    strcpy(data[i].polygons.name, data[i].name);
                    for(j = 0; j < count_elements(data[i].definition); j++){
                        data[i].polygons.elements[j].x = data[index_num/10].points.coordinates.x;
                        data[i].polygons.elements[j].y = data[index_num/10].points.coordinates.y;
                        if(j != 20){
                            strcpy(component, data[i].definition[j + 1]);
                            find_index(data, component, num_data, &index_num);
                        } 
                    }
                    data[i].polygons.elements[count_elements(data[i].definition)].x = 697868; //ASCII equivalent of END
                }
            }
            else{
                strcpy(data[i].polygons.name, data[i].name);
                k = 0;
                for(j = 0; j < count_elements(data[i].definition); j++){
                    data[i].polygons.elements[k].x = data[index_num/10].lines.dots[0].x;
                    data[i].polygons.elements[k].y = data[index_num/10].lines.dots[0].y;
                    k++;
                    data[i].polygons.elements[k].x = data[index_num/10].lines.dots[1].x;
                    data[i].polygons.elements[k].y = data[index_num/10].lines.dots[1].y;
                    k++;
                    if(j != 20){
                        strcpy(component, data[i].definition[j + 1]);
                        find_index(data, component, num_data, &index_num);
                    } 
                }
                if(k != 40){
                    data[i].polygons.elements[2*count_elements(data[i].definition)].x = 697868; //ASCII equivalent of END
                }
            }
        }
    }
    fscanf(fp, "%s", buffer);
    if(strcmp(buffer, "actions") == 0){
        fseek(fp, 1, SEEK_CUR);
        fscanf(fp, "%s", output_file);
        fseek(fp, 1, SEEK_CUR);
        fscanf(fp, "%d", &num_action);
        fseek(fp, 2, SEEK_CUR);
        action = (struct actions*)calloc(num_action, sizeof(struct actions));
        get_action(action, fp, num_action);
    }
    execute_actions(data, action, num_data, num_action, output_file);
    free(data);
    free(action);
    fclose(fp);
    return 0;
}
void get_data(struct objects *info, FILE *commands, int num_line){
    int i = 0, j = 0;
    char *buffer, *pointer, data[10];
    buffer = (char*)calloc(100, sizeof(char));
    for(i = 0; i < num_line; i++){
        for(j = 0; j < 21; j++){
            memset(info[i].definition[j], '\0', 10*sizeof(char));
        }
    }
    for(int k = 0; k < num_line; k++){
        memset(buffer, '\0', 100*sizeof(char));
        fgets(buffer, 100, commands);
        //First I removed tabs to use strtok because if I use strtok(buffer, " ") without removing tabs it ignores the // and reads the comment.
        remove_tabs(buffer);
        pointer = strtok(buffer, " ");
        j = 0;
        //I used strtok to get elements one by one until it reaches // or NULL character.
        while(pointer != NULL){
            sscanf(pointer, "%s", data);
            if(strcmp(data, "//") == 0){
                break;
            }
            strcpy(info[k].definition[j], data);
            j++;
            pointer = strtok(NULL, " ");
        }
        //Since the last element of the line is the name of the object I copied the last element to the name array and erased it from the definition array.
        strcpy(info[k].name, info[k].definition[count_elements(info[k].definition) - 1]);
        memset(info[k].definition[count_elements(info[k].definition) - 1], '\0', 10*sizeof(char));
    }
    free(buffer);
}
void get_action(struct actions *info, FILE *commands, int num_line){
    int i = 0, j = 0;
    char *buffer, *pointer, action[10];
    buffer = (char*)calloc(100, sizeof(char));
    for(i = 0; i < num_line; i++){
        for(j = 0; j < 2; j++){
            memset(info[i].components[j], '\0', 10*sizeof(char));
        }
    }
    for(int k = 0; k < num_line; k++){
        memset(buffer, '\0', 100*sizeof(char));
        fgets(buffer, 100, commands);
        //First I removed tabs to use strtok because if I use strtok(buffer, " ") without removing tabs it ignores the // and reads the comment.
        remove_tabs(buffer);
        pointer = strtok(buffer, " ");
        j = 0;
        //I used strtok to get elements one by one until it reaches // or NULL character.
        while(pointer != NULL){
            sscanf(pointer, "%s", action);
            if(strcmp(action, "//") == 0){
                break;
            }
            /*Since the first element of the line is the name of action first I checked if this is the first. If it is I copied it to operation array and if it is not I copied it to components array.*/
            if(j == 0){
                strcpy(info[k].operation, action);
            }
            else{
                strcpy(info[k].components[j - 1], action);
            }
            j++;
            pointer = strtok(NULL, " ");
        }
    }
    free(buffer);
}
void execute_actions(struct objects *data, struct actions *action, int num_data, int num_action, char file_path[]){
    int i = 0, j = 0, index1, index2, temp_index1, temp_index2, check;
    float a, b, c, temp1_x, temp1_y, temp2_x, temp2_y, temp3_x, temp3_y, temp4_x, temp4_y, vektor1_x, vektor1_y, vektor2_x, vektor2_y, check_x = 0, check_y = 0, angle, circumference = 0, multiple_x1y2 = 0, multiple_y1x2 = 0;
    char component1[10], component2[10];
    FILE *output = fopen(file_path, "w");
    for(i = 0; i < num_action; i++){
        if(strcmp(action[i].operation, "Distance") == 0){
            strcpy(component1, action[i].components[0]);
            strcpy(component2, action[i].components[1]);
            find_index(data, component1, num_data, &index1);
            find_index(data, component2, num_data, &index2);
            //Checks if the components are both points
            if(index1%10 == 1 && index2%10 == 1){
                temp1_x = data[index1/10].points.coordinates.x;
                temp1_y = data[index1/10].points.coordinates.y;
                temp2_x = data[index2/10].points.coordinates.x;
                temp2_y = data[index2/10].points.coordinates.y;
                fprintf(output, "Distance(%s,%s) = %.1f\n", data[index1/10].points.name, data[index2/10].points.name, sqrt(pow(fabs(temp1_x - temp2_x), 2) + pow(fabs(temp1_y - temp2_y), 2)));
            }
            else{
                if(index1%10 == 2 && index2%10 == 1){
                    temp_index1 = index1;   //temp_index1 always holds the index of line
                    temp_index2 = index2;   //temp_index2 always holds the index of point
                }
                else{
                    temp_index1 = index2;
                    temp_index2 = index1;
                }
                temp1_x = data[temp_index1/10].lines.dots[0].x;
                temp1_y = data[temp_index1/10].lines.dots[0].y;
                temp2_x = data[temp_index1/10].lines.dots[1].x;
                temp2_y = data[temp_index1/10].lines.dots[1].y;
                line_equation(temp1_x, temp1_y, temp2_x, temp2_y, &a, &b, &c);
                temp3_x = data[temp_index2/10].points.coordinates.x;
                temp3_y = data[temp_index2/10].points.coordinates.y;
                /*According to the check_perpendicular function calculation changes. If it's 0 it means point is perpendicular to the line, if it's positive first point of the line is the closest point to the point otherwise same stuff is viable for the second point of the line.*/
                if(check_perpendicular(data[temp_index1/10].lines.dots, data[temp_index2/10].points.coordinates) == 0){
                    fprintf(output, "Distance(%s,%s) = %.1f\n", data[temp_index2/10].points.name, data[temp_index1/10].lines.name, fabs(a*temp3_x + b*temp3_y + c)/sqrt(pow(a, 2) + pow(b, 2)));
                }
                else{
                    if(check_perpendicular(data[temp_index1/10].lines.dots, data[temp_index2/10].points.coordinates) == 1){
                        fprintf(output, "Distance(%s,%s) = %.1f\n", data[temp_index2/10].points.name, data[temp_index1/10].lines.name, sqrt(pow(temp3_x - temp2_x, 2) + pow(temp3_y - temp2_y, 2)));
                    }
                    else{
                        fprintf(output, "Distance(%s,%s) = %.1f\n", data[temp_index2/10].points.name, data[temp_index1/10].lines.name, sqrt(pow(temp3_x - temp1_x, 2) + pow(temp3_y - temp1_y, 2)));
                    }
                }
            }
        }
        if(strcmp(action[i].operation, "Angle") == 0){
            strcpy(component1, action[i].components[0]);
            strcpy(component2, action[i].components[1]);
            find_index(data, component1, num_data, &index1);
            find_index(data, component2, num_data, &index2);
            temp1_x = data[index1/10].lines.dots[0].x;
            temp1_y = data[index1/10].lines.dots[0].y;
            temp2_x = data[index1/10].lines.dots[1].x;
            temp2_y = data[index1/10].lines.dots[1].y;
            temp3_x = data[index2/10].lines.dots[0].x;
            temp3_y = data[index2/10].lines.dots[0].y;
            temp4_x = data[index2/10].lines.dots[1].x;
            temp4_y = data[index2/10].lines.dots[1].y;
            check_x = 0;
            check_y = 0;
            line_equation(temp1_x, temp1_y, temp2_x, temp2_y, &a, &b, &c);
            //Checks if the first line is a horizontal line or a vertical line.
            if(a == 1 && b == 0){
                check_x++;
            }
            if(a == 0 && b == 1){
                check_y++;
            }
            line_equation(temp3_x, temp3_y, temp4_x, temp4_y, &a, &b, &c);
            //Checks if the second line is a horizontal line or a vertical line.
            if(a == 1 && b == 0){
                check_x++;
            }
            if(a == 0 && b == 1){
                check_y++;
            }
            //If they are both horizontal lines or vertical lines this means they are parallel lines.
            if(check_x == 2 || check_y == 2){
                fprintf(output, "Angle(%s,%s) = 0.0\n", data[index1/10].lines.name, data[index2/10].lines.name);
            }
            //If not function divides the result of dot product of vectors by the lenghts of lines and sends it to the arccosine function then converts the result into degree. 
            else{
                vektor1_x = temp1_x - temp2_x;
                vektor1_y = temp1_y - temp2_y;
                vektor2_x = temp3_x - temp4_x;
                vektor2_y = temp3_y - temp4_y;
                angle = acos((vektor1_x*vektor2_x + vektor1_y*vektor2_y)/sqrt(pow(fabs(temp1_x - temp2_x), 2) + pow(fabs(temp1_y - temp2_y), 2))/sqrt(pow(fabs(temp3_x - temp4_x), 2) + pow(fabs(temp3_y - temp4_y), 2)));
                angle = angle*180/PI;
                fprintf(output, "Angle(%s,%s) = %.1f\n", data[index1/10].lines.name, data[index2/10].lines.name, angle);
            }
        }
        if(strcmp(action[i].operation, "Length") == 0){
            strcpy(component1, action[i].components[0]);
            find_index(data, component1, num_data, &index1);
            //Checks whether the component is a line or a polygon.
            if(index1%10 == 2){
                //If it is a line function implements the formula of distance between two points.
                temp1_x = data[index1/10].lines.dots[0].x;
                temp1_y = data[index1/10].lines.dots[0].y;
                temp2_x = data[index1/10].lines.dots[1].x;
                temp2_y = data[index1/10].lines.dots[1].y;
                fprintf(output, "Length(%s) = %.1f\n", data[index1/10].lines.name, sqrt(pow(fabs(temp1_x - temp2_x), 2) + pow(fabs(temp1_y - temp2_y), 2)));
            }
            else{
                circumference = 0;
                strcpy(component2, data[index1/10].definition[0]);
                find_index(data, component2, num_data, &index2);
                //Checks whether the polygon is defined by points or defined by lines.
                if(index2%10 == 1){
                    //If polygon is defined by points function calculates the distance between points.
                    for(j = 0; j < count_polygon_elements(data[index1/10].polygons.elements) - 1; j++){
                        temp1_x = data[index1/10].polygons.elements[j].x;
                        temp1_y = data[index1/10].polygons.elements[j].y;
                        temp2_x = data[index1/10].polygons.elements[j + 1].x;
                        temp2_y = data[index1/10].polygons.elements[j + 1].y;
                        circumference += sqrt(pow(fabs(temp1_x - temp2_x), 2) + pow(fabs(temp1_y - temp2_y), 2));
                    }
                    temp1_x = data[index1/10].polygons.elements[j].x;
                    temp1_y = data[index1/10].polygons.elements[j].y;
                    temp2_x = data[index1/10].polygons.elements[0].x;
                    temp2_y = data[index1/10].polygons.elements[0].y;
                    circumference += sqrt(pow(fabs(temp1_x - temp2_x), 2) + pow(fabs(temp1_y - temp2_y), 2));
                    fprintf(output, "Length(%s) = %.1f\n", data[index1/10].polygons.name, circumference);
                }
                else{
                    /*If polygon is defined by lines function first checks if the polygon is complete then if it is calculates the distance between end points of lines*/
                    if(check_polygons(data[index1/10].polygons.elements) == 0){
                        fprintf(output, "Length(%s) = Can not be done because polygon is incomplete\n", data[index1/10].polygons.name);
                    }
                    else{
                        j = 0;
                        for(int k = 0; k < count_polygon_elements(data[index1/10].polygons.elements)/2; k++){
                            temp1_x = data[index1/10].polygons.elements[j].x;
                            temp1_y = data[index1/10].polygons.elements[j].y;
                            j++;
                            temp2_x = data[index1/10].polygons.elements[j].x;
                            temp2_y = data[index1/10].polygons.elements[j].y;
                            j++;
                            circumference += sqrt(pow(fabs(temp1_x - temp2_x), 2) + pow(fabs(temp1_y - temp2_y), 2));
                        }
                        fprintf(output, "Length(%s) = %.1f\n", data[index1/10].polygons.name, circumference);
                    }
                }
            }
        }
        if(strcmp(action[i].operation, "Area") == 0){
            strcpy(component1, action[i].components[0]);
            find_index(data, component1, num_data, &index1);
            strcpy(component2, data[index1/10].definition[0]);
            find_index(data, component2, num_data, &index2);
            check = 0;
            //Checks if the polygon is complete. If not function increases check by one.
            if(index2%10 == 2){
                if(check_polygons(data[index1/10].polygons.elements) == 0){
                    check++;
                    fprintf(output, "Area(%s) = Can not be done because polygon is incomplete\n", data[index1/10].polygons.name);
                }
            }
            if(check == 0){
                //If the polygon is complete function takes the 2 points of polygon, multiplies the x of first point and y of second point and multiplies the y of first point and x of second point. Does this process consecutively.
                multiple_x1y2 = 0;
                multiple_y1x2 = 0;
                for(j = 0; j < count_polygon_elements(data[index1/10].polygons.elements) - 1; j++){
                    temp1_x = data[index1/10].polygons.elements[j].x;
                    temp1_y = data[index1/10].polygons.elements[j].y;
                    temp2_x = data[index1/10].polygons.elements[j + 1].x;
                    temp2_y = data[index1/10].polygons.elements[j + 1].y;
                    multiple_x1y2 += temp1_x*temp2_y;
                    multiple_y1x2 += temp1_y*temp2_x;
                }
                //Lastly function subtracts the multiple_y1x2 from multiple_x1y2 and divides it by 2 to find the area of the polygon.
                fprintf(output, "Area(%s) = %.1f\n", data[index1/10].polygons.name, (multiple_x1y2 - multiple_y1x2)/2);
            }
        }
    }
    fclose(output);
}
int check_perpendicular(struct coordinate line_coordinates[2], struct coordinate point_coordinates){
    struct coordinate AB, BE;
    //A and B are the end points of the line and E is the point.
    //AB is from A to B and BE is from B to E.
    AB.x = line_coordinates[1].x - line_coordinates[0].x;
    AB.y = line_coordinates[1].y - line_coordinates[0].y;
    BE.x = point_coordinates.x - line_coordinates[1].x;
    BE.y = point_coordinates.y - line_coordinates[1].y;
    //Performs an dot product of AB and BE and if it is positive this means B is the closest point to E, if it is negative it means A is the closest point to E and if it is 0 then it means E is perpendicular to the line and that line is the closest distance from E to AB line.
    if(AB.x*BE.x + AB.y*BE.y > 0){
        return 1;
    }
    if(AB.x*BE.x + AB.y*BE.y < 0){
        return -1;
    }
    return 0;
}
int check_polygons(struct coordinate elements[40]){
    int i;
    float x1, y1, x2, y2;
    i = 1;
    //Checks if the all lines are connected to each other. If not returns 0 otherwise returns 1.
    for(int j = 0; j < count_polygon_elements(elements)/2 - 1; j++){
        x1 = elements[i].x;
        y1 = elements[i].y;
        x2 = elements[i + 1].x;
        y2 = elements[i + 1].y;
        if(x1 != x2 | y1 != y2){
            return 0;
        }
        i += 2;
    }
    x1 = elements[i].x;
    y1 = elements[i].y;
    x2 = elements[0].x;
    y2 = elements[0].y;
    if(x1 != x2 | y1 != y2){
        return 0;
    }
    return 1;
}
int count_polygon_elements(struct coordinate elements[40]){
    int counter = 0;
    //If it finds 697868 as a mark of end of elemets it stops and returns the counter otherwise this means array is full and returns counter as 40.
    for(int i = 0; i < 40; i++){
        if(elements[i].x == 697868){
            return counter;
        }
        counter++;
    }
    return counter;
}
void find_index(struct objects *data, char target[], int num_data, int* target_index){
    *target_index = 0;
    /*Funciton takes searchs the target entry first in points structure then lines structure then polygons structure. If it finds in one of these it stops searching.*/
    //I added 1, 2 or 3 at the end of index number to specify in which structure target was found. 1 indicates point, 2 indicates line, 3 indicates polygon.
    for(int i = 0; i < num_data; i++){
        if(strcmp(data[i].points.name, target) == 0){
            *target_index = i*10 + 1; 
        }
    }
    if(*target_index == 0){
        for(int i = 0; i < num_data; i++){
            if(strcmp(data[i].lines.name, target) == 0){
                *target_index = i*10 + 2;
            }
        }
    }
    if(*target_index == 0){
        for(int i = 0; i < num_data; i++){
            if(strcmp(data[i].polygons.name, target) == 0){
                *target_index = i*10 + 3;
            }
        }
    }
}
void line_equation(float x1, float y1, float x2, float y2, float *a, float *b, float *c){
    if(x2 - x1 == 0){   //This situation indicates that the line is a vertical x line
        *a = 1;
        *b = 0;
        *c = x1*-1;
    }
    else if(y2 - y1 == 0){  //This situation indicates that the is a horizontal y line
        *a = 0;
        *b = 1;
        *c = y1*-1;
    }
    else{
        //I took the line equation formula (y2 )/(x2 - x1) = (y - y2)/(x - x2) and adjusted it to my function
        *a = y2 ;
        *b = (x2 - x1)*-1;
        *c = (y2 - y1)*-1*x2 + ((x2 - x1)*-1*y2)*-1;
    }
}
int count_elements(char array[21][10]){
    int counter = 0, i;
    for(i = 0; i < 21; i++){
        if(array[i][0] == '\0'){
            break;
        }
        counter++;
    }
    return counter;
}
void remove_tabs(char* arr){
    char *temp;
    int i;
    temp = arr;
    i = 0;
    while(temp[i] != '\0'){
        //If it finds a tab, it replaces it with a whitespace.
        if(temp[i] == '\t'){
            arr[i] = ' ';
        }
        else{
            arr[i] = temp[i];
        }
        i++;
    }
}
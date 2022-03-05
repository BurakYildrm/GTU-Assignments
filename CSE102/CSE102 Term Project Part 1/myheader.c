#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myheader.h"
Figure * start_figure(double width, double height){
    Figure *coordinates = (Figure*)malloc(sizeof(Figure));
    coordinates->temp_width = coordinates->width = width;
    coordinates->temp_height = coordinates->height = height;
    coordinates->binarytree = (Tree*)malloc(sizeof(Tree));
    coordinates->binarytree->x = -999;
    coordinates->binarytree->left = NULL;
    coordinates->binarytree->right = NULL;
    coordinates->points = NULL;
    coordinates->next = NULL;
    return coordinates;
}
void set_thickness_resolution(Figure * fig, double thickness, double resolution){
    fig->thickness = thickness;
    fig->resolution = resolution;
}
void set_color(Figure * fig, Color c){
    fig->colour.red = c.red;
    fig->colour.green = c.green;
    fig->colour.blue = c.blue;
}
void draw_fx(Figure * fig, double f(double x), double start_x, double end_x,double step_size){
    fig->points = (Point2D*)malloc(sizeof(Point2D));
    fig->shape_type = 1;
    Point2D* temp = fig->points;
    for(double i = start_x; i <= end_x; i += step_size){
        if(sqrt(pow(step_size, 2) + pow(f(i) - f(i + step_size), 2)) >= fig->resolution){
            temp->x = i;
            temp->y = f(i);
            temp->next = (Point2D*)malloc(sizeof(Point2D));
            temp->next->next = NULL;
            temp = temp->next;
        }
    }
}
void draw_polyline(Point2D * poly_line, int n, Figure* fig){
    fig->points = (Point2D*)malloc(sizeof(Point2D));
    fig->shape_type = 2;
    Point2D* temp = poly_line, *iter = fig->points;
    for(int i = 0; i < n; i++){
        iter->x = temp->x;
        iter->y = temp->y;
        iter->next = (Point2D*)malloc(sizeof(Point2D));
        iter->next->next = NULL;
        iter = iter->next;
        temp = temp->next;
    }
}
void draw_polygon(Point2D * poly_line, Figure* fig){
    fig->points = (Point2D*)malloc(sizeof(Point2D));
    fig->shape_type = 3;
    Point2D* temp = poly_line, *iter = fig->points;
    for(int i = 0; temp != NULL; i++){
        iter->x = temp->x;
        iter->y = temp->y;
        iter->next = (Point2D*)malloc(sizeof(Point2D));
        iter->next->next = NULL;
        iter = iter->next;
        temp = temp->next;
    }
    iter->x = poly_line->x;
    iter->y = poly_line->y;
    iter->next = (Point2D*)malloc(sizeof(Point2D));
    iter->next->next = NULL;
}
void draw_ellipse(Point2D * centre, Point2D * width_height, Figure* fig){
    fig->points = (Point2D*)malloc(sizeof(Point2D));
    fig->shape_type = 4;
    Point2D* temp = fig->points;
    int counter = 0;
    double start_x, end_x, centre_x, centre_y, width, height;
    centre_x = centre->x;
    centre_y = centre->y;
    width = width_height->x;
    height = width_height->y;
    start_x = centre_x - width;
    end_x = centre_x + width;
    for(double i = start_x; i <= end_x; i += 1){
        if(sqrt(pow(0.1, 2) + pow(ellipse_formula(centre_x, centre_y, width, height, i, 0) - ellipse_formula(centre_x, centre_y, width, height, i + 0.1, 0), 2)) >= fig->resolution){
            if(ellipse_formula(centre_x, centre_y, width, height, i, 0) != 787984){
                temp->x = i;
                temp->y = ellipse_formula(centre_x, centre_y, width, height, i, 0);
                temp->next = (Point2D*)malloc(sizeof(Point2D));
                temp->next->next = NULL;
                temp = temp->next;
            }
        }
    }
    for(double i = end_x; i >= start_x; i -= 1){
        if(sqrt(pow(0.1, 2) + pow(ellipse_formula(centre_x, centre_y, width, height, i, 1) - ellipse_formula(centre_x, centre_y, width, height, i - 0.1, 1), 2)) >= fig->resolution){
            if(ellipse_formula(centre_x, centre_y, width, height, i, 1) != 787984){
                temp->x = i;
                temp->y = ellipse_formula(centre_x, centre_y, width, height, i, 1);
                temp->next = (Point2D*)malloc(sizeof(Point2D));
                temp->next->next = NULL;
                temp = temp->next;
            }
        }
    }
}
double ellipse_formula(double center_x, double center_y, double width, double height, double x, int flag){
    double result, a, b, c, delta, y1, y2, temp;
    result = pow(x - center_x, 2)/pow(width, 2);
    result = pow(center_y, 2) - pow(height, 2)*(1 - result);
    a = 1;
    b = 2*center_y*-1;
    c = result;
    delta = pow(b, 2) - 4*a*c;
    if(delta >= 0){
        y1 = (b*-1 + sqrt(delta))/2/a;
        y2 = (b*-1 - sqrt(delta))/2/a;
        if(y2 > y1){
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        if(flag == 0){
            return y1;
        }
        else{
            return y2;
        }
    }
    else{
        return 787984;
    }
}
void draw_binary_tree(Tree * root, Tree* fig){
    if(root != NULL){
        fig->x = root->x;
        fig->left = (Tree*)malloc(sizeof(Tree));
        fig->left->left = NULL;
        fig->left->right = NULL;
        draw_binary_tree(root->left, fig->left);
        fig->right = (Tree*)malloc(sizeof(Tree));
        fig->right->left = NULL;
        fig->right->right = NULL;
        draw_binary_tree(root->right, fig->right);
    }
}
void append_figures(Figure * fig1, Figure * fig2){
    Figure* temp = fig1;
    Point2D* iter, *temp2 = fig2->points;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = (Figure*)malloc(sizeof(Figure));
    temp->next->next = NULL;
    temp = temp->next;
    temp->colour.red = fig2->colour.red;
    temp->colour.green = fig2->colour.green;
    temp->colour.blue = fig2->colour.blue;
    temp->height = fig2->height;
    temp->width = fig2->width;
    temp->thickness = fig2->thickness;
    temp->resolution = fig2->resolution;
    temp->points = (Point2D*)malloc(sizeof(Point2D));
    iter = temp->points;
    if(temp2 != NULL){
        while(temp2->next != NULL){
            iter->x = temp2->x;
            iter->y = temp2->y;
            iter->next = (Point2D*)malloc(sizeof(Point2D));
            iter->next->next = NULL;
            iter = iter->next;
            temp2 = temp2->next;
        }
    }
    temp->temp_height = fig2->temp_height;
    temp->temp_width = fig2->temp_width;
    temp->resize = fig2->resize;
    temp->shape_type = fig2->shape_type;
    temp->binarytree = (Tree*)malloc(sizeof(Tree));
    temp->binarytree->left = NULL;
    temp->binarytree->right = NULL;
    append_tree(temp->binarytree, fig2->binarytree);
}
void append_tree(Tree* tree1, Tree* tree2){
    if(tree2->x == -999){
        tree1->x = -999;
    }
    else{
        if(tree2->left != NULL || tree2->right != NULL){
            tree1->x = tree2->x;
            tree1->left = (Tree*)malloc(sizeof(Tree));
            tree1->left->left = NULL;
            tree1->left->right = NULL;
            tree1->right = (Tree*)malloc(sizeof(Tree));
            tree1->right->left = NULL;
            tree1->right->right = NULL;
            append_tree(tree1->left, tree2->left);
            append_tree(tree1->right, tree2->right);
        }
    }
}
void scale_figure(double scale_x, double scale_y, Figure* fig){
    Point2D* temp = fig->points;
    double start_x, end_x;
    if(temp != NULL){
        while(temp->next != NULL){
            temp->x *= scale_x;
            temp->y *= scale_y;
            temp = temp->next;
        }
    }
}
void resize_figure(Point2D *start_roi, Point2D *end_roi, Figure* fig){
    fig->height = end_roi->y - start_roi->y;
    fig->width = end_roi->x - start_roi->x;
    fig->resize = 1;
}
int biggest_canvas(Figure* fig){
    Figure* temp = fig;
    double *arr = (double*)malloc(sizeof(double)), max;
    int i, max_index = 0;
    for(i = 0; temp != NULL; i++){
        arr = realloc(arr, (i + 1)*sizeof(double));
        arr[i] = temp->temp_height*temp->temp_width;
        temp = temp->next;
    }
    max = arr[0];
    for(int j = 0; j < i; j++){
        if(arr[j] > max){
            max = arr[j];
            max_index = j;
        }
    }
    return max_index;
}
void export_eps(Figure * fig, char * file_name){
    struct coordinates points[20];
    int counter = 0, i = 0, fig_index;
    Figure *temp = fig, *size = fig;
    Point2D* iter;
    FILE *fp = fopen(file_name, "w");
    while(temp != NULL){
        iter = temp->points;
        if(counter == 0){
            fig_index = biggest_canvas(fig);
            for(int j = 0; j < fig_index; j++){
                size = size->next;
            }
            fprintf(fp, "%%!PS\nnewpath\n0 0 moveto\ngsave\n%lf 0 lineto\n%lf %lf lineto\n0 %lf lineto\n0 0 lineto\n", size->width, size->width, size->height, size->height);
            fprintf(fp, "gsave\nstroke\ngrestore\nclip\n");
            if(size->resize == 1){
                fprintf(fp, "%lf %lf translate\n0 0 moveto\n", size->temp_width/2, size->temp_height/2);
            }
            else{
                fprintf(fp, "%lf %lf translate\n0 0 moveto\n", size->width/2, size->height/2);
            }
            counter++;
        }
        fprintf(fp, "%lf setlinewidth\n", temp->thickness);
        fprintf(fp, "%lf %lf %lf setrgbcolor\n", temp->colour.red, temp->colour.green, temp->colour.blue);
        if(temp->binarytree->x != -999){
            print_tree(temp->binarytree, fp, points, 0, 0, size->temp_height/2);
            fprintf(fp, "stroke\n");
            fprintf(fp, "0 %lf translate\n", (size->temp_height/2 - 30)*-1);
        }
        else{
            while(iter->next != NULL){
                if(i == 0){
                    fprintf(fp, "%lf %lf moveto\n", iter->x, iter->y);
                    i++;
                }
                else{
                    fprintf(fp, "%lf %lf lineto\n", iter->x, iter->y);
                }
                iter = iter->next;
            }
            if(temp->shape_type == 4){
                fprintf(fp, "closepath\nstroke\n");
            }
            else{
                fprintf(fp, "stroke\n");
            }
            if(temp->next != NULL){
                fprintf(fp, "0 0 moveto\n");
            }
            i = 0;
        }
        temp = temp->next;
    }
    fclose(fp);
}
void print_tree(Tree* fig, FILE* fp, struct coordinates points[], int flag, int temp, double height){
    double val = 3.14159265/180.0;
    if(fig->left != NULL || fig->right != NULL){
        if(temp == 0){
            fprintf(fp, "0 %lf translate\n0 20 moveto\n", height - 30);
            points[temp].x = 0.0;
            points[temp].y = 0.0;
            for(double i = 0; i < 360; i += 0.1){
                fprintf(fp, "%lf %lf lineto\n", sin(i*val)*20, cos(i*val)*20);
            }
            if(digitnum(fig->x) == 1){
                fprintf(fp, "%lf %lf moveto\n/Times-Roman findfont\n15 scalefont\nsetfont\n(%d) show\n", points[temp].x - 4, points[temp].y - 4, fig->x);
            }
            if(digitnum(fig->x) == 2){
                fprintf(fp, "%lf %lf moveto\n/Times-Roman findfont\n15 scalefont\nsetfont\n(%d) show\n", points[temp].x - 7, points[temp].y - 4, fig->x);
            }
            if(digitnum(fig->x) == 3){
                fprintf(fp, "%lf %lf moveto\n/Times-Roman findfont\n15 scalefont\nsetfont\n(%d) show\n", points[temp].x - 11, points[temp].y - 4, fig->x);
            }
            fprintf(fp, "%lf %lf moveto\n", sin(val*210)*20, cos(val*210)*20);
            print_tree(fig->left, fp, points, 0, temp + 1, height);
            fprintf(fp, "%lf %lf moveto\n", sin(val*150)*20, cos(val*150)*20);
            print_tree(fig->right, fp, points, 1, temp + 1, height);
        }
        else{
            if(temp == 1){
                if(flag == 0){
                    points[temp].x = points[temp - 1].x - 70;
                    points[temp].y = points[temp - 1].y - 60;
                }
                else{
                    points[temp].x = points[temp - 1].x + 70;
                    points[temp].y = points[temp - 1].y - 60;
                }
            }
            else{
                if(flag == 0){
                    points[temp].x = points[temp - 1].x - 40;
                    points[temp].y = points[temp - 1].y - 60;
                }
                else{
                    points[temp].x = points[temp - 1].x + 40;
                    points[temp].y = points[temp - 1].y - 60;
                }
            }
            fprintf(fp, "%lf %lf lineto\n", points[temp].x, points[temp].y + 20);
            fprintf(fp, "%lf %lf moveto\n", points[temp].x - 20, points[temp].y);
            for(double i = points[temp].x - 20; i <= points[temp].x + 20; i += 1){
                if(circle_equation(points[temp].x, points[temp].y, 20, i, 0) != 787984){
                    fprintf(fp, "%lf %lf lineto\n", i, circle_equation(points[temp].x, points[temp].y, 20, i, 0));
                }
            }
            for(double i = points[temp].x + 20; i >= points[temp].x - 20; i -= 1){
                if(circle_equation(points[temp].x, points[temp].y, 20, i, 1) != 787984){
                    fprintf(fp, "%lf %lf lineto\n", i, circle_equation(points[temp].x, points[temp].y, 20, i, 1));
                }
            }
            fprintf(fp, "closepath\n");
            if(digitnum(fig->x) == 1){
                fprintf(fp, "%lf %lf moveto\n/Times-Roman findfont\n15 scalefont\nsetfont\n(%d) show\n", points[temp].x - 4, points[temp].y - 4, fig->x);
            }
            if(digitnum(fig->x) == 2){
                fprintf(fp, "%lf %lf moveto\n/Times-Roman findfont\n15 scalefont\nsetfont\n(%d) show\n", points[temp].x - 7, points[temp].y - 4, fig->x);
            }
            if(digitnum(fig->x) == 3){
                fprintf(fp, "%lf %lf moveto\n/Times-Roman findfont\n15 scalefont\nsetfont\n(%d) show\n", points[temp].x - 11, points[temp].y - 4, fig->x);
            }
            fprintf(fp, "%lf %lf moveto\n", points[temp].x - sin(val*30)*20, points[temp].y - cos(val*30)*20);
            print_tree(fig->left, fp, points, 0, temp + 1, height);               
            fprintf(fp, "%lf %lf moveto\n", points[temp].x + sin(val*30)*20, points[temp].y - cos(val*30)*20);
            print_tree(fig->right, fp, points, 1, temp + 1, height);
        }
    }
}
double circle_equation(double center_x, double center_y, double r, double x, int flag){
    double a, b, c, delta, y1, y2, temp;
    a = 1;
    b = 2*center_y*-1;
    c = pow(center_y, 2) + pow(x - center_x, 2) - pow(r, 2);
    delta = pow(b, 2) - 4*a*c;
    if(delta >= 0){
        y1 = (b*-1 + sqrt(delta))/2/a;
        y2 = (b*-1 - sqrt(delta))/2/a;
        if(y2 > y1){
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        if(flag == 0){
            return y1;
        }
        else{
            return y2;
        }
    }
    else{
        return 787984;
    }
}
int digitnum(int x){
    int counter = 0;
    if(x == 0){
        return 1;
    }
    else{
        while(x != 0){
            x /= 10;
            counter++;
        }
        return counter;
    }
}
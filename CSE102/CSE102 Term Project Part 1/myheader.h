#ifndef myheader
#define myheader
struct coordinates{
    double x, y;
};
typedef struct binary{
    int x;
    struct binary *left;
    struct binary *right;
}Tree;
typedef struct rgb{
    double red;
    double green;
    double blue;
}Color;
typedef struct point{
    double x, y;
    struct point *next;
}Point2D;
typedef struct canvas{
    int shape_type;
    int resize;
    double temp_width;
    double temp_height;
    double width;
    double height;
    double thickness;
    double resolution;
    Color colour;
    Point2D* points;
    Tree *binarytree;
    struct canvas* next;
}Figure;
Figure * start_figure(double width, double height);
void set_thickness_resolution(Figure * fig, double thickness, double resolution);
void set_color(Figure * fig, Color c);
void draw_fx(Figure * fig, double f(double x), double start_x, double end_x,double step_size);
void draw_polyline(Point2D * poly_line, int n, Figure* fig);
void draw_polygon(Point2D * poly_line, Figure* fig);
void draw_ellipse(Point2D * centre, Point2D * width_height, Figure* fig);
void draw_binary_tree(Tree * root, Tree* fig);
void scale_figure(double scale_x, double scale_y, Figure* fig);
void resize_figure(Point2D *start_roi, Point2D *end_roi, Figure* fig);
void append_figures(Figure * fig1, Figure * fig2);
void append_tree(Tree* tree1, Tree* tree2);
void export_eps(Figure * fig, char * file_name);
double ellipse_formula(double center_x, double center_y, double width, double height, double x, int flag);
double func(double x);
int biggest_canvas(Figure* fig);
int digitnum(int x);
void print_tree(Tree* fig, FILE* fp, struct coordinates points[], int flag, int temp, double height);
double circle_equation(double center_x, double center_y, double r, double x, int flag);
#endif
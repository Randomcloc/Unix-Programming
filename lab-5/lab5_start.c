/* -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Point
{
    double x;
    double y;
} Point;

// == Modify Below here for step 1 == //
double rectangle_area(struct Point a, struct Point b)
{
    // a and b are (x,y) cordinates of
    // opposite corners of a rectangle.
    // Return the area of the rectangle.
    double x_diff, y_diff;
    
    x_diff = fabs(a.x - b.x);
    y_diff = fabs(a.y - b.y);

    return x_diff * y_diff;
}

double compute_cost(double area, double cost)
{
    return cost * area;
}

// == Modify above here for step 1 == //

int main(int argc, char *argv[])
{

    if (argc != 6)
    {
        fprintf(stderr, "Error, incorrect arguments\n"
                        "Usage: %s a.x a.y b.x b.y cost\n",
                argv[0]);
        return 1;
    }

    struct Point P1;
    struct Point P2;
    P1.x = atof(argv[1]);
    P1.y = atof(argv[2]);
    P2.x = atof(argv[3]);
    P2.y = atof(argv[4]);

    double cost = atof(argv[5]);
    double area = rectangle_area(P1, P2);

    printf("Area:%2.2lf Cost: %2.2lf\n", area, compute_cost(area, cost));
    return 0;
}
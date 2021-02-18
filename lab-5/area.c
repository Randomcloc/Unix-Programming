/* -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 */

#include "rectangle.h"

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

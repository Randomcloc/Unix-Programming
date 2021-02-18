/*
 * File: rectangle.h
 * Project: lab-5
 * Created Date: Thursday, 18th February 2021 6:01:44 pm
 * -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * -----
 * Modified By: Abhijeet Suryawanshi
 * Date Modified: Thursday, 18th February 2021 6:14 pm
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Point
{
    double x;
    double y;
} Point;

double rectangle_area(struct Point a, struct Point b);
double compute_cost(double area, double cost);


#endif
/*
 * customMath.c
 *
 *  Created on: Nov 22, 2020
 *      Author: amag0
 */

#include <math.h>
#define MATH_PI 3.14159265358979323846

double arctan(double __y, double __x){
    if (__x > 0)
        return atan(__y / __x);

    if ((__x < 0) && (__y >= 0))
        return (atan(__y/__x) + MATH_PI);

    if ((__x < 0) && (__y < 0))
            return (atan(__y/__x) - MATH_PI);

    if ((__x == 0) && (__y > 0))
        return (MATH_PI / 2);

    if ((__x == 0) && (__y < 0))
        return (-1 * MATH_PI / 2);

    // Could be undefined
    if ((__x == 0) && (__y == 0))
        return -1;

    return 0;
}


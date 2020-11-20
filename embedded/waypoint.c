/* DriverLib Includes */
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <waypoint.h>

#define pi 3.14159265
#define ratio 180/pi

//struct point {
//	float x;
//	float y;
//};
//
//struct waypoint {
//	float distance;
//	float angle;	//angle is calculated from the positve side of x axis
//	char turnDir;	//turn direction, L = turn left, R = turn right
//};


float GetABS(float x){
    if(x >= 0) return x;
    if(x < 0) return -x;
}
int quadrant(struct point beg, struct point des){
    float xDiff = des.x - beg.x;
    float yDiff = des.y - beg.y;
    if(xDiff > 0 && yDiff > 0) return 1;
    if(xDiff < 0 && yDiff > 0) return 2;
    if(xDiff < 0 && yDiff < 0) return 3;
    if(xDiff > 0 && yDiff < 0) return 4;
}
double RadToDegree (double rad) {
    double value = 180 / pi;
    double degree = rad * value;
    return degree;
}



float GetDistance (struct point beg, struct point des) {
    float xDiff = des.x - beg.x;
//  printf("xDiff %f", xDiff);
    xDiff = GetABS(xDiff);
    float yDiff = des.y - beg.y;
//  printf("yDiff %f", yDiff);
    yDiff = GetABS(yDiff);

    float aSqr = xDiff * xDiff;
    float bSqr = yDiff * yDiff;

//  printf("x %f",des.x);
//  printf("y %f",des.y);
//
//
//  printf("aSqr %f", aSqr);
//  printf("bSqr %f", bSqr);

    float distance = sqrt(aSqr + bSqr);


    return distance;
}

float GetAngle (struct point beg, struct point des) {
    float distance = GetDistance (beg, des);
    int quad = quadrant(beg, des);

    if(quad == 1 || quad == 3) {
        float xDiff = des.x - beg.x;
        xDiff = GetABS(xDiff);

        double r = xDiff / distance;

        double angleRad = asin(r);

        double angleDegree = RadToDegree(angleRad);

        if(quad == 1) return angleDegree;
        else return angleDegree = angleDegree + 180;
    }
    if(quad == 2 || quad == 4) {
        float yDiff = des.y - beg.y;
        yDiff = GetABS(yDiff);

        double r = yDiff / distance;

        double angleRad = asin(r);

        double angleDegree = RadToDegree(angleRad);

        if(quad == 2) return angleDegree = angleDegree + 270;
        else return angleDegree = angleDegree + 90;
    }
}

struct waypoint setWaypoint (struct point startPoint, struct point desPoint) {
    struct waypoint temp;
    temp.distance = GetDistance(startPoint,desPoint);
    temp.angle = GetAngle(startPoint,desPoint);
//  temp. turnDir = GetTurnDirection(startPoint,desPoint);
    return temp;
}

/*--------------------------------------------------
 * Copy following codes to main to retrieve the data
 * -------------------------------------------------
    //This function return the struct waypoint which contains
     * the distance from begin location to destination location (in degree, 1 degree ~= 11km)
     * and angle for the path, this angle respects to true north (0 degree).
     *
     * startpoint is location of begining
     * desPoint is location of destination

    struct waypoint path = setWaypoint(struct point startPoint, struct point desPoint);

 */

// int main () {
//    struct point begin;
//    struct point des, des1, des2, des3;
//
//    begin.x = 0;
//    begin.y = 0;
//
//    des.x = 13.5;
//    des.y = 4.5;
//
//    des1.x = -13.5;
//    des1.y = 4.5;
//
//    des2.x = -13.5;
//    des2.y = -4.5;
//
//    des3.x = 13.5;
//    des3.y = -4.5;
//
//    printf("Distance is %f \n",GetDistance(begin, des));
//    printf("Angle is %f degree\n",GetAngle(begin,des));
////    printf("Turn direction from positive x-axis: %c\n",GetTurnDirection(begin, des));
//
//   printf("Distance is %f\n",GetDistance(begin, des1));
//   printf("Angle is %f degree\n",GetAngle(begin,des1));
////   printf("Turn direction from positive x-axis: %c\n",GetTurnDirection(begin, des1));
//
//   printf("Distance is %f\n",GetDistance(begin, des2));
//   printf("Angle is %f degree\n",GetAngle(begin,des2));
////   printf("Turn direction from positive x-axis: %c\n",GetTurnDirection(begin, des2));
//
//   printf("Distance is %f\n",GetDistance(begin, des3));
//   printf("Angle is %f degree\n",GetAngle(begin,des3));
////   printf("Turn direction from positive x-axis: %c\n",GetTurnDirection(begin, des3));
//
////	struct waypoint test;
////	test = setWaypoint(begin, des);
////
////	printf("Distance is %f \n",test.distance);
////	printf("Angle is %f degree\n",test.angle);
////    printf("Turn direction from positive x-axis: %c\n",test.turnDir);
//
//    return(0);
// }



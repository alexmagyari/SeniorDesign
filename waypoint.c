/* DriverLib Includes */
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define pi 3.14159265
#define ratio 180/pi

struct point {
	float x;
	float y;
};

struct waypoint {
	float distance;
	float angle;	//angle is calculated from the positve side of x axis
	char turnDir;	//turn direction, L = turn left, R = turn right 
};

double RadToDegree (double rad) {
	double value = 180 / pi;
	double degree = rad * value;
	return degree;
}



float GetDistance (struct point beg, struct point des) {
	float xDiff = des.x - beg.x;
	xDiff = abs(xDiff);
	float yDiff = des.y - beg.y;
	yDiff = abs(yDiff);	
	
	float aSqr = xDiff * xDiff;
	float bSqr = yDiff * yDiff;
	
	float distance = sqrt(aSqr + bSqr);
	
	
	return distance;	
}

float GetAngle (struct point beg, struct point des) {
	float distance = GetDistance (beg, des);
	
	float yDiff = des.y - beg.y;
	yDiff = abs(yDiff);
	
	double r = yDiff / distance;
	
	double angleRad = asin(r);
	
	double angleDegree = RadToDegree(angleRad);
	
	if (des.x > beg.x && des.y > beg.y) {	};
	
	if (des.x < beg.x && des.y > beg.y) angleDegree = angleDegree + 90;
	
	if (des.x < beg.x && des.y < beg.y) angleDegree = angleDegree + 90;
	
	if (des.x > beg.x && des.y < beg.y) {	};
	
	return angleDegree;	
}

char GetTurnDirection(struct point beg, struct point des) {
	char turnDir;
	
	if (des.x > beg.x && des.y > beg.y) return turnDir = 'L';
	
	if (des.x < beg.x && des.y > beg.y) return turnDir = 'L';
	
	if (des.x < beg.x && des.y < beg.y) return turnDir = 'R';
	
	if (des.x > beg.x && des.y < beg.y) return turnDir = 'R';
}

struct waypoint setWaypoint (struct point startPoint, struct point desPoint) {
	struct waypoint temp;
	temp.distance = GetDistance(startPoint,desPoint);
	temp.angle = GetAngle(startPoint,desPoint);
	temp. turnDir = GetTurnDirection(startPoint,desPoint);
	return temp;
}

 int main () {
    struct point begin;
    struct point des, des1, des2, des3;
   
    begin.x = 0;
    begin.y = 0;
   
    des.x = 13.5;
    des.y = 4.5;
   
    des1.x = -13.5;
    des1.y = 4.5;
   
    des2.x = -13.5;
    des2.y = -4.5;
   
    des3.x = 13.5;
    des3.y = -4.5;
   
    printf("Distance is %f \n",GetDistance(begin, des));
    printf("Angle is %f degree\n",GetAngle(begin,des));
    printf("Turn direction from positive x-axis: %c\n",GetTurnDirection(begin, des));
//   
//    printf("Distance is %f\n",GetDistance(begin, des1));
//    printf("Angle is %f degree\n",GetAngle(begin,des1));
//    printf("Turn direction from positive x-axis: %c\n",GetTurnDirection(begin, des1));
//   
//    printf("Distance is %f\n",GetDistance(begin, des2));
//    printf("Angle is %f degree\n",GetAngle(begin,des2));
//    printf("Turn direction from positive x-axis: %c\n",GetTurnDirection(begin, des2));
//   
//    printf("Distance is %f\n",GetDistance(begin, des3));
//    printf("Angle is %f degree\n",GetAngle(begin,des3));
//    printf("Turn direction from positive x-axis: %c\n",GetTurnDirection(begin, des3));

	struct waypoint test;
	test = setWaypoint(begin, des);
	
	printf("Distance is %f \n",test.distance);
	printf("Angle is %f degree\n",test.angle);
    printf("Turn direction from positive x-axis: %c\n",test.turnDir);
    
    return(0);
 }



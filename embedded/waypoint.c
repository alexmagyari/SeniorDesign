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



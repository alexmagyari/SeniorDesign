/*
 * waypoint.h
 *
 *  Created on: Nov. 9, 2020
 *      Author: Sang
 */

#ifndef WAYPOINT_H_
#define WAYPOINT_H_
/*
 * Structure contain the coordination of start point and destination point.
 * x is Latitude in decimal form
 * y is Longitude in decimal form
 */
struct point {
	float x;
	float y;
};
/*
 * Structure contains the information for navigation
 * distance is in degree
 */
struct waypoint {
	float distance;	//in degree
	float angle;	//angle is calculated from the positve side of x axis
	char turnDir;	//turn direction, L = turn left, R = turn right 
};

//function change angle to degree
double RadToDegree (double rad);

//Function return distance between begin point and destination point
float GetDistance (struct point beg, struct point des);

//function return angle from positive x axis
float GetAngle (struct point beg, struct point des);

//Function return direction for the angle. Left or Right of the x_axis
char GetTurnDirection(struct point beg, struct point des);

//function return the struct waypoint.
struct waypoint setWaypoint(struct point startPoint, struct point desPoint);

#endif /* WAYPOINT_H_ */

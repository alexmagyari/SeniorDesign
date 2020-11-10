/*
 * waypoint.h
 *
 *  Created on: Nov. 9, 2020
 *      Author: Sang
 */

#ifndef WAYPOINT_H_
#define WAYPOINT_H_

struct point {
	float x;
	float y;
};

struct waypoint {
	float distance;
	float angle;	//angle is calculated from the positve side of x axis
	char turnDir;	//turn direction, L = turn left, R = turn right 
};

double RadToDegree (double rad);

float GetDistance (struct point beg, struct point des);

float GetAngle (struct point beg, struct point des);

char GetTurnDirection(struct point beg, struct point des);

#endif /* WAYPOINT_H_ */

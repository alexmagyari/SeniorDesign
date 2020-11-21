/*
Author: Leslie Nix
Date Created: 27 October 2020
Date Updated: 19 November 2020
Status: Probably Fine

Flight Control and Stabilization Algorithms
*/

#ifndef ALTHOLD_H_
#define ALTHOLD_H_

#include "gyro.h"

typedef struct {
  float altitude, velocity, acceleration;
  float altitudeLPF;
} altitude_t;

void initAltHold(void);

void getAlt(gyroAngle_t *angle, imuData *imuData, altitude_t *altitude, uint32_t now, float dt);
float updateAltHold(float aux, altitude_t *altitude, float throttle, uint32_t now, float dt);
void resetAltHold(altitude_t *altitude);

#endif

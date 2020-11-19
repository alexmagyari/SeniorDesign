/*
Author: Leslie Nix
Date Created: 14 November 2020
Date Updated: 18 November 2020
Status: Probably Fine
*/


#ifndef IMU_H_
#define IMU_H_

#include "../embedded/gyro.h"

#define DEG_TO_RAD  0.017453292519943295769236907684886f
#define RAD_TO_DEG  57.295779513082320876798154814105f
#define GRAV 9.80665f

typedef struct{
  float thrust_cmd, roll_cmd, pitch_cmd, yaw_cmd;
} cntrl_cmd;

// *** Switch Board Orientation to Use NED ***
void boardOrientation(sensorRaw_t *sensorRaw);

// *** Rotates from NED to body frame ***
void rotateV(sensor_t *v, const gyroAngle_t *angle);

void convert_for_cntrl(imuData *data, gyroAngle_t *angle, sensor_t *mag, float dt);


#endif

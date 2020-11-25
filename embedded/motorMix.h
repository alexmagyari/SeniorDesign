/*
Author: Leslie Nix
Date Created: 27 October 2020
Date Updated: 27 October 2020
Status: Probably Fine
*/
/*****************************************************
System Model
Configuration: 'X'
Forward Direction: x-axis (NED coordinates)
// double-check me
m0 - Front Right // CW
m1 - Front Left  // CCW
m2 - Back Right  // CCW
m3 - Back Left   // CW
*****************************************************/
typedef struct{
  float thrust, roll, pitch, yaw;
  float m0, m1, m2, m3;
  int m0_duty, m1_duty, m2_duty, m3_duty;
} motor_mix;

#ifndef MOTOR_MIX_H_
#define MOTOR_MIX_H_

#define MOTOR_MAP_OFFSET 1000.0f
#define MAX_MOTOR_OUT 1900
#define MIN_MOTOR_OUT 1000
void compute_motor_commands(motor_mix *m_cmd, float thrust_cmd, float yaw_cmd, float pitch_cmd, float roll_cmd);
#endif

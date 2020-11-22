/*
Author: Leslie Nix
Date Created: 29 October 2020
Date Updated: 3 November 2020
Status: Not working; needs output, #includes, and whatnot

Motor Mixing Algorithm
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


#include "../embedded/motor.h"
#include "IMU.h"
#define MAX_MOTOR_OUT 1900
#define MIN_MOTOR_OUT 1000
void compute_motor_commands(motor_mix m_cmd, float thrust_cmd, float yaw_cmd, float pitch_cmd, float roll_cmd)
{
  m_cmd.m0 = thrust_cmd + yaw_cmd + pitch_cmd + roll_cmd;
  m_cmd.m1 = thrust_cmd - yaw_cmd + pitch_cmd - roll_cmd;
  m_cmd.m2 = thrust_cmd - yaw_cmd - pitch_cmd + roll_cmd;
  m_cmd.m3 = thrust_cmd + yaw_cmd - pitch_cmd - roll_cmd;

  m_cmd.m0_duty = m_cmd.m0 * (MAX_MOTOR_OUT - MIN_MOTOR_OUT);
  m_cmd.m1_duty = m_cmd.m1 * (MAX_MOTOR_OUT - MIN_MOTOR_OUT);
  m_cmd.m2_duty = m_cmd.m2 * (MAX_MOTOR_OUT - MIN_MOTOR_OUT);
  m_cmd.m3_duty = m_cmd.m3 * (MAX_MOTOR_OUT - MIN_MOTOR_OUT);
}

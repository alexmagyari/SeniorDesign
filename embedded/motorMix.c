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
#include <math.h>
float mapf(float input, float in_min, float in_max, float out_min, float out_max);

int mapDuty(float input, float in_min, float in_max, float out_min, float out_max);

#include "./motor.h"
#include "IMU.h"
#include "motorMix.h"
#include "PID.h"
//#include <uartManager.h>
void compute_motor_commands(motor_mix *m_cmd, float thrust_cmd, float yaw_cmd, float pitch_cmd, float roll_cmd)
{
  // calculate individual motor commands for x-config quadcopter
  m_cmd->m0 = thrust_cmd + yaw_cmd + pitch_cmd - roll_cmd;
  m_cmd->m1 = thrust_cmd - yaw_cmd - pitch_cmd + roll_cmd;
  m_cmd->m2 = thrust_cmd - yaw_cmd - pitch_cmd - roll_cmd;
  m_cmd->m3 = thrust_cmd + yaw_cmd + pitch_cmd + roll_cmd;

  // map/constrain to PWM min/max
  // all PID have min=-100, max=100 so in_min -400 and in_max is 400
  float m0_mapped = mapf(m_cmd->m0, -1 * MOTOR_MAP_OFFSET, MOTOR_MAP_OFFSET, MIN_MAIN_MOTOR_PWM, MAX_MAIN_MOTOR_PWM);
  float m1_mapped = mapf(m_cmd->m1, -1 * MOTOR_MAP_OFFSET, MOTOR_MAP_OFFSET, MIN_MAIN_MOTOR_PWM, MAX_MAIN_MOTOR_PWM);
  float m2_mapped = mapf(m_cmd->m2, -1 * MOTOR_MAP_OFFSET, MOTOR_MAP_OFFSET, MIN_MAIN_MOTOR_PWM, MAX_MAIN_MOTOR_PWM);
  float m3_mapped = mapf(m_cmd->m3, -1 * MOTOR_MAP_OFFSET, MOTOR_MAP_OFFSET, MIN_MAIN_MOTOR_PWM, MAX_MAIN_MOTOR_PWM);


  // map those commands to a duty cycle?
  m_cmd->m0_duty = mapDuty(m0_mapped, MIN_MAIN_MOTOR_PWM, MAX_MAIN_MOTOR_PWM, 0.0f, 85.0f);
  m_cmd->m1_duty = mapDuty(m1_mapped, MIN_MAIN_MOTOR_PWM, MAX_MAIN_MOTOR_PWM, 0.0f, 85.0f);
  m_cmd->m2_duty = mapDuty(m2_mapped, MIN_MAIN_MOTOR_PWM, MAX_MAIN_MOTOR_PWM, 0.0f, 85.0f);
  m_cmd->m3_duty = mapDuty(m3_mapped, MIN_MAIN_MOTOR_PWM, MAX_MAIN_MOTOR_PWM, 0.0f, 85.0f);



/*
  m_cmd.m0_duty = ((m_cmd.m0 / 400) * (MOTOR_PWM_PERIOD - MIN_MAIN_MOTOR_PWM) * 100 + MIN_MAIN_MOTOR_PWM * 100) / MOTOR_PWM_PERIOD;
  m_cmd.m1_duty = ((m_cmd.m1 / 400) * (MOTOR_PWM_PERIOD - MIN_MAIN_MOTOR_PWM) * 100 + MIN_MAIN_MOTOR_PWM * 100) / MOTOR_PWM_PERIOD;
  m_cmd.m2_duty = ((m_cmd.m2 / 400) * (MOTOR_PWM_PERIOD - MIN_MAIN_MOTOR_PWM) * 100 + MIN_MAIN_MOTOR_PWM * 100) / MOTOR_PWM_PERIOD;
  m_cmd.m3_duty = ((m_cmd.m3 / 400) * (MOTOR_PWM_PERIOD - MIN_MAIN_MOTOR_PWM) * 100 + MIN_MAIN_MOTOR_PWM * 100) / MOTOR_PWM_PERIOD;
  */

/*
  UART2PCString("M0 Duty: ");
  UART2PCFloat (m_cmd.m0_duty);
  UART2PCNewLine();
  UART2PCString("M1 Duty: ");
  UART2PCFloat(m_cmd.m1_duty);
  UART2PCNewLine();
  UART2PCString("M2 Duty: ");
  UART2PCFloat(m_cmd.m2_duty);
  UART2PCNewLine();
  UART2PCString("M3 Duty: ");
  UART2PCFloat(m_cmd.m3_duty);
  UART2PCNewLine();
  */

/*
  if (m_cmd.m0_duty > 100) m_cmd.m0_duty = 100;
  if (m_cmd.m1_duty > 100) m_cmd.m1_duty = 100;
  if (m_cmd.m2_duty > 100) m_cmd.m2_duty = 100;
  if (m_cmd.m3_duty > 100) m_cmd.m3_duty = 100;
  if (m_cmd.m0_duty < MIN_MAIN_MOTOR_PWM) m_cmd.m0_duty = MIN_MAIN_MOTOR_PWM;
  if (m_cmd.m1_duty < MIN_MAIN_MOTOR_PWM) m_cmd.m1_duty = MIN_MAIN_MOTOR_PWM;
  if (m_cmd.m2_duty < MIN_MAIN_MOTOR_PWM) m_cmd.m2_duty = MIN_MAIN_MOTOR_PWM;
  if (m_cmd.m3_duty < MIN_MAIN_MOTOR_PWM) m_cmd.m3_duty = MIN_MAIN_MOTOR_PWM;
  */

}

float mapf(float input, float in_min, float in_max, float out_min, float out_max)
{
  float val = (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return constrain(val, out_min, out_max);
}

int mapDuty(float input, float in_min, float in_max, float out_min, float out_max)
{
  float val = (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return round(constrain(val, out_min, out_max));
}

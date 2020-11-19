/*
Author: Leslie Nix
Date Created: 27 October 2020
Date Updated: 27 October 2020
Status: Probably Fine
*/

#ifndef PID_H_
#define PID_H_

#include "lpFilter.h"

// integral clamping
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

typedef struct {
  float kp;
  float ki;
  float kd;
  float lastErr, iTerm, intLim;
  float max, min; // not currently using these in PID.c
  float fc;  //cutoff frequency in Hz
  low_pass_t lowPass;
} pid_data;

void pid_init(void);
float pid_compute(pid_data *pidType, float setpoint, float input, float dt);

// might not need these?
void pid_resetRPY(void);
void pid_resetAlt(void);
void pid_resetXY(void);

extern pid_data pidRoll, pidPitch, pidYaw, pidAlt, pidX, pidY;

#endif

/*
Author: Leslie Nix
Date Created: 27 October 2020
Date Updated: 27 October 2020
Status: Probably Fine; add lpFilter
*/

#include "PID.h"

pid_data pidRoll, pidPitch, pidYaw, pidAlt, pidX, pidY;

void initPID(void){
  pidRoll.kp = 0.250f;
  pidRoll.ki = 0.950f;
  pidRoll.kd = 0.011f;
  pidRoll.lastErr = 0.0f;
  pidRoll.iTerm = 0.0f;
  pidRoll.intLim = 5.85f;
  pidRoll.max = 200.0f;
  pidRoll.min = -200.0f;
  pidRoll.fc = 150.0f;
  pidRoll.lowPass.prevOutput = 0.0f;

  pidPitch.kp = 0.250f;
  pidPitch.ki = 0.950f;
  pidPitch.kd = 0.011f;
  pidPitch.lastErr = 0.0f;
  pidPitch.iTerm = 0.0f;
  pidPitch.intLim = 5.85f;
  pidPitch.max = 200.0f;
  pidPitch.min = -200.0f;
  pidPitch.fc = 150.0f;
  pidPitch.lowPass.prevOutput = 0.0f;

  pidYaw.kp = 0.680f;
  pidYaw.ki = 0.500f;
  pidYaw.kd = 0.0001f;
  pidYaw.lastErr = 0.0f;
  pidYaw.iTerm = 0.0f;
  pidYaw.intLim = 10.0f;
  pidYaw.max = 100.0f;
  pidYaw.min = -100.0f;
  pidYaw.fc = 150.0f;
  pidPitch.lowPass.prevOutput = 0.0f;

  pidAlt.kp = 1.0f;
  pidAlt.ki = 0.0f;
  pidAlt.kd = 0.0f;
  pidAlt.lastErr = 0.0f;
  pidAlt.iTerm = 0.0f;
  pidAlt.intLim = 5.0f;
  pidAlt.max = 100.0f;
  pidAlt.min = -100.0f;
  pidAlt.fc = 150.0f;
  pidAlt.lowPass.prevOutput = 0.0f;

  pidX.kp = 1.0f;
  pidX.ki = 0.0f;
  pidX.kd = 0.0f;
  pidX.lastErr = 0.0f;
  pidX.iTerm = 0.0f;
  pidX.intLim = 5.0f;
  pidX.max = 100.0f;
  pidX.min = -100.0f;
  pidX.fc = 150.0f;
  pidX.lowPass.prevOutput = 0.0f;

  pidY.kp = 1.0f;
  pidY.ki = 0.0f;
  pidY.kd = 0.1f;
  pidY.lastErr = 0.0f;
  pidY.iTerm = 0.0f;
  pidY.intLim = 5.0f;
  pidY.max = 100.0f;
  pidY.min = -100.0f;
  pidY.fc = 150.0f;
  pidY.lowPass.prevOutput = 0.0f;
}

float pid_compute(float setpoint, float current, float dt, pid_data *pid)
{
  float err, pTerm, deltaErr, deriv, dTerm, output;

  // calc error
  err = setpoint - current;

  // calc P-Term
  pTerm = pid->kp*err;

  // calc I-Term
  // trapezoidal integration
  pid->iTerm += pid->ki*(err + pid->lastErr)/2.0f*dt;
  // clamping for anti-windup
  pid->iTerm = constrain(pid->iTerm, -pid->intLim, pid->intLim);

  // calc D-Term
  deltaErr = (err - pid->lastErr) / dt;
  pid->lastErr = err;

  // exponential smoothing to reduce noise impact
  pid->lowPass.Fc = pid->fc;  // set lowPass frequency
  deriv = applyLowPass(&pid->lowPass, deltaErr, dt);
  pid->lowPass.prevOutput = deriv;
  dTerm = pid->kd * deriv;


  output = (pTerm + pid->iTerm + dTerm);

/*
  if(pid->max < output)
  {
    output = pid->max;
    pid-> iErr=0.0f;
  }
  else if(pid->min > output)
  {
    output = pid->min;
    pid->iErr = 0.0f;
  }
  */

  return output;
}

void pid_resetRPY(void)
{
  pidRoll.iTerm = pidRoll.lastErr = pidRoll.lowPass.prevOutput = 0.0f;
  pidPitch.iTerm = pidPitch.lastErr = pidPitch.lowPass.prevOutput = 0.0f;
  pidYaw.iTerm = pidYaw.lastErr = pidYaw.lowPass.prevOutput = 0.0f;
}

void pid_resetAlt(void)
{
  pidAlt.iTerm = pidAlt.lastErr = pidAlt.lowPass.prevOutput = 0.0f;
}

void pid_resetXY(void)
{
  pidX.iTerm = pidX.lastErr = pidX.lowPass.prevOutput = 0.0f;
  pidY.iTerm = pidY.lastErr = pidY.lowPass.prevOutput = 0.0f;
}

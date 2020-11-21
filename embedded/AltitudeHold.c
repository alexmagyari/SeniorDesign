/*
Author: Leslie Nix
Date Created: 27 October 2020
Date Updated: 19 November 2020
Status: Probably Fine
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "AltitudeHold.h"
#include "IMU.h"
#include "lpFilter.h"
#include "gyro.h"
#include "PID.h"
#include "motor.h"
#include "altimeter.h"

#define MIN_MOTOR_OUT 1000
#define MAX_MOTOR_OUT 1900

static baro_t altimeter;
static float altitudeSetpoint;
static bool altHoldActive;

void initAltHold(void)
{
  altimeter_init();
}

void getAltitude(gyroAngle_t *angle, imuData *imuData, altitude_t *altitude, float dt)
{
  // estimate alt using barometer, lp filter data
  static low_pass_t baro_lp = {.Fc = 8.38f}; // cutoff frequency in Hz
  float lastBaroAltitude = baro_lp.prevOutput;
  float baroAltitude = applyLowPass(&baro_lp, altimeter.absoluteAlt - altimeter.groundAlt, dt);

  // estimate baro velocity
  float baroVelocity = (baroAltitude - lastBaroAltitude) / dt;

  // Rotate body frame to inertial frame
  gyroAngle_t rotAngle = {
    .axis = {
      .roll = -angle->axis.roll * DEG_TO_RAD,
      .pitch = -angle->axis.pitch * DEG_TO_RAD,
      .yaw = -angle->axis.yaw * DEG_TO_RAD,
    }
  };

  sensor_t accInertialFrame = imuData->accelBody;
  rotateV(&accInertialFrame, &rotAngle);

  // estimate altitude and velocity using acceleration
  float accelZ = -(accInertialFrame.axis.Z + ACCEL_SCALE_FACTOR);

  // convert to m/s^2
  altitude->acceleration = accelZ / ACCEL_SCALE_FACTOR * GRAV;

  float accDt = altitude->acceleration * dt;
  // estimate velocity using acceleration
  float accVelocity = altitude->velocity + accDt;
  // estimate altitude using acceleration
  float accAltitude = altitude->altitude + altitude->velocity * dt + 0.5f * accDt * dt;

  // estimate altitude and velocity using complimentary filter on baro and accel estimates
  static const float velocity_cf = 0.985f;
  altitude->velocity = velocity_cf * accVelocity + (1.0f - velocity_cf) * baroVelocity;

  static const float altitude_cf = 0.965f;
  altitude->altitude = altitude_cf * accAltitude + (1.0f - altitude_cf) * baroAltitude;

  // lp filter altitude estimate
  static low_pass_t altitude_lp = {.Fc = .80}; //cutoff frequency in Hz
  altitude->altitudeLPF = applyLowPass(&altitude_lp, altitude->altitude, dt);

}

float updateAltitudeHold(float aux, altitude_t *altitude, float throttle, float dt)
{
  // add 5% to minimum so motors do not fully shut off
  static const float MIN_MOTOR_OFFSET = (MAX_MOTOR_OUT - MIN_MOTOR_OUT) * 0.05f;

  // throttle when altitude hold was activated
  static float altHoldInitThrottle = 30.0f;

  if (aux >= 50){
    if(!altHoldActive){
      altHoldActive = true;
      altHoldInitThrottle = throttle;
      pid_resetAlt();
    }
  }

    float altHoldOut = pid_compute(&pidAlt, altitudeSetpoint, altitude->altitudeLPF, dt);

    throttle = constrain(altHoldInitThrottle + altHoldOut, MIN_MOTOR_OUT + MIN_MOTOR_OFFSET, MAX_MOTOR_OUT);

    return throttle;
}

void resetAltitudeHold(altitude_t *altitude)
{
  altHoldActive = false;
  altitudeSetpoint = altitude->altitudeLPF;
}

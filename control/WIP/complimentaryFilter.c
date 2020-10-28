/*
Author: Leslie Nix
Date Created: 27 October 2020
Date Updated: 27 October 2020
Status: Psuedocode/Notes; does not compile

Complimentary Filter for combining gyro/altimeter measurements to reduce jitter from altimeter and drift error from gyro
*/

/*
Notes and nonsense; not actual code yet
*/

// convert gyro/accel readings to engineering units from ADC
// gyro [G] <- 300 mV/g
// accel [rad] <- 2mV per degree/sec, Vref=3.3V
// N <- output from ADC
accel_out = (N-512)/1024*(double)10.78;  //replace 512 with measured
gyro_out = (N-512)/1024*(double)28.783;  //zero-g bias

//filter
#include <math.h>

#define ACCEL_SENSITIVITY 8192.0
#define GYRO_SENSITIVITY 65.536 //these numbers are garbage
#define MY_PI 3.14159265359

#define DT 0.01 //time-step

void ComplementaryFilter(short accData[3], short gyrData[3], float *pitch, float *roll)
{
   float pitchAcc, rollAcc;

   // Integrate the gyroscope data -> int(angularSpeed) = angle
   *pitch += ((float)gyrData[0] / GYRO_SENSITIVITY) * DT; // Angle around the X-axis
   *roll -= ((float)gyrData[1] / GYRO_SENSITIVITY) * DT;    // Angle around the Y-axis

   // Compensate for drift with accelerometer data if !bullshit
   // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
   int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
   if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
   {
 // Turning around the X axis results in a vector on the Y-axis
       pitchAcc = atan2f((float)accData[1], (float)accData[2]) * 180 / MY_PI;
       *pitch = *pitch * 0.98 + pitchAcc * 0.02;

 // Turning around the Y axis results in a vector on the X-axis
       rollAcc = atan2f((float)accData[0], (float)accData[2]) * 180 / MY_PI;
       *roll = *roll * 0.98 + rollAcc * 0.02;
   }
}

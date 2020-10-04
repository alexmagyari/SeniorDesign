/*
 * gyro.h
 *
 *  Created on: Oct 3, 2020
 *      Author: amag0
 */

#ifndef GYRO_H_
#define GYRO_H_

// The address of the gyro is 110100X, where X is ADC0, allowing
// for two gyros to be connected. pin 0 is grounded for this connection
#define GYROSCOPE 0x68


// *** User control registers ***
#define GYRO_USER_CTRL 0x03

// Digital motion processor enable
#define GYRO_DMP_EN    0x80
// Digital motion processor reset
#define GYRO_DMP_RST   0x08


// *** Power modes ***
#define GYRO_PWR_MGMT_1 0x06

// Enable the onboard PLL
#define GYRO_CLOCK_PLL 0x05


// *** Power register 2
#define GYRO_PWR_MGMT_2 0x07

// Disable the accelerometer
#define GYRO_DISABLE_ACCEL 0x38


// *** Gyro Data Registers ***
#define GYRO_X_LSB     0x34
#define GYRO_X_MSB     0x33
#define GYRO_Y_LSB     0x36
#define GYRO_Y_MSB     0x35
#define GYRO_Z_LSB     0x38
#define GYRO_Z_MSB     0x37

// *** Initialize the gyroscope
void gyro_init();

// *** Return the 32 bit X value from the gyro
int16_t gyroX();

// *** Return the 32 bit Y value from the gyro
int16_t gyroY();

// *** Return the 32 bit Z value from the gyro
int16_t gyroZ();


#endif /* GYRO_H_ */

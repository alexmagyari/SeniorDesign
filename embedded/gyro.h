/*
 * gyro.h
 *
 *  Created on: Oct 3, 2020
 *      Author: amag0
 *  Status: power mode?
 *            mode8 = 9DOF w opt DMP
 *          add sensor calibration methods?
 *          add check if IMU ready to be read?
 *          convert fns to use struct?
 *          add temp methods?
 *          figure out wth the DMP does
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
#define GYRO_DISABLE_ACCEL 0x38  //nooooo


// *** Gyro Data Registers ***
#define GYRO_X_LSB     0x34
#define GYRO_X_MSB     0x33
#define GYRO_Y_LSB     0x36
#define GYRO_Y_MSB     0x35
#define GYRO_Z_LSB     0x38
#define GYRO_Z_MSB     0x37

// *** Accel Data Ragisters ***
#define ACCEL_X_LSB    0x2E
#define ACCEL_X_MSB    0x2D
#define ACCEL_Y_LSB    0x30
#define ACCEL_Y_MSB    0x2F
#define ACCEL_Z_LSB    0x32
#define ACCEL_Z_MSB    0x31

// *** Temp Data Registers ***
#define TEMP_LSB       0x3A
#define TEMP_MSB       0x39

/*
This is where I get hella confused bc the mag data and scale/sensitivity configurations are in different user banks.
I'm assuming we call the change in gyro.c fns
*/

// *** Change User Bank ***
#define REG_BANK_SEL   0x7F

// Select user bank
#define USER_BANK_0    0x00
#define USER_BANK_2    0x10

// *** Gyro Config ***
// USR2
#define GYRO_CONFIG    0x01

// Set Gyro Full Scale to +- 2000 dps
// USR2
#define GYRO_FS_SEL    0x06

// *** Accel Config ***
// USR2
#define ACCEL_CONFIG   0x14

// Set Accel Full Scale to +- 8g
#define ACCEL_FS_SEL   0x04

// *** Mag Data Registers ***
// Not in a user bank? maybe in 0?
#define MAG_X_LSB      0x11
#define MAG_X_MSB      0x12
#define MAG_Y_LSB      0x13
#define MAG_Y_MSB      0x14
#define MAG_Z_LSB      0x15
#define MAG_Z_MSB      0x16

// *** Mag Power Mode? ***
#define MAG_CNTL_2     0x31

// Set power mode to something?
// #define SNGL_MODE     0x01
// #define CONT_MODE_1   0x02
// #define CONT_MODE_2   0x04
// #define CONT_MODE_3   0x06
// #define CONT_MODE_4   0x10

// *** Type Definitions ***
typedef struct{
  float gx, gy, gz, ax, ay, az, mx, my, mz;
} imuData;

// *** Initialize the gyroscope
void imu_init();

// *** Return the 16 bit X rate from the gyro
// Gyro angular rate = gyroOut/GyroSensitivity
float gyroX();

// *** Return the 16 bit Y rate from the gyro
float gyroY();

// *** Return the 16 bit Z rate from the gyro
float gyroZ();

// *** Return 16 bit X,Y,Z readings from the accel
// acceleration = accelOut/AccelSensitivity
float accelX();
float accelY();
float accelZ();

// *** Return 16 bit X,Y,Z readings from mag
float magX();
float magY();
float magZ();

#endif /* GYRO_H_ */

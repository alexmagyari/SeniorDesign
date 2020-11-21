/*
 * gyro.h
 *
 *  Created on: Oct 3, 2020
 *      Author: amag0
 *  Status: power mode?
 *            mode8 = 9DOF w opt DMP
 *          **add sensor calibration methods?
 *          add check if IMU ready to be read?
 *          add temp methods?
 */

#ifndef GYRO_H_
#define GYRO_H_

#include <stdint.h>

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

// *** Scale Factors ***
#define GYRO_SCALE_FACTOR   16.4f
#define ACCEL_SCALE_FACTOR  4096.0f
#define MAG_SCALE_FACTOR   0.15f

// *** Type Definitions ***
// raw (but scaled) sensor data for accel, gyro
typedef union {
  struct{
    int16_t X, Y, Z;
  } __attribute__((packed)) axis;
  int16_t data[3];
} sensorRaw_t;

// 'cleaned' sensor data
typedef union {
  struct{
    float X, Y, Z;
  } __attribute__((packed)) axis;
  int16_t data[3];
} sensor_t;

// converted gyro angles
typedef union {
  struct {
    float roll, pitch, yaw;
  } __attribute__((packed)) axis;
  float data[3];
} gyroAngle_t;

// magnetometer data
typedef union {
  struct {
    float X, Y, Z;
  } __attribute__((packed)) axis;
  float data[3];
} mag_t;

// big struct to hold all of the above data types
typedef struct{
  sensorRaw_t accel, gyro;  //raw readings
  sensor_t accelBody;  //magnitude of accel in body frame
  gyroAngle_t gyroRate; //gyro readings in dps
  sensor_t mag;  //mag data in microTeslas
} imuData;

// configuration struct to hold zero-vales
extern imuData cfg;

// get measures at rest to use as baseline
void getZeroMeas(imuData *cfg);

// *** Return 16 bit X,Y,Z readings from gyro, accel, and mag ***
void get_gyro_data(imuData *data);
void get_accel_data(imuData *data);
void get_mag_data(imuData *data);
void imu_init(void);


#endif /* GYRO_H_ */

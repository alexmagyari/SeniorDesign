/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>

#include <i2cManager.h>
#include <uartManager.h>
#include <gyro.h>

// not entirely sure if this is needed bc it might be set in init
#define GYRO_SCALE_FACTOR   16.4f
#define ACCEL_SCALE_FACTOR  4096.0f
#define GYRO_SCALE_FACTOR   0.15f

void imu_init(){
    // ensure in USR0; maybe better as a check with I2CRead?
    // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);

    uint8_t prefs = GYRO_DMP_EN | GYRO_DMP_RST;
    I2CWrite(GYROSCOPE, GYRO_USER_CTRL, prefs);
    I2CWrite(GYROSCOPE, GYRO_PWR_MGMT_1, GYRO_CLOCK_PLL);
    // I2CWrite(GYROSCOPE, GYRO_PWR_MGMT_2, GYRO_DISABLE_ACCEL);

    // change to USR2
    I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_2);
    // gyro config
    I2CWrite(GYROSCOPE, GYRO_CONFIG, GYRO_FS_SEL);
    // accel config
    I2CWrite(GYROSCOPE, ACCEL_CONFIG, ACCEL_FS_SEL);
}

void get_gyro_data(imuData *data)
{
  // ensure in USR0; maybe better as a check with I2CRead?
  // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
  int16_t x = I2CRead(GYROSCOPE, GYRO_X_MSB);
  x = x << 8;
  x |= I2CRead(GYROSCOPE, GYRO_X_LSB);
  float total = x;
  data->gx = total / GYRO_SCALE_FACTOR;

  int16_t y = I2CRead(GYROSCOPE, GYRO_Y_MSB);
  y = y << 8;
  y |= I2CRead(GYROSCOPE, GYRO_Y_LSB);
  float total = y;
  data->gy = total / GYRO_SCALE_FACTOR;

  int16_t z = I2CRead(GYROSCOPE, GYRO_Z_MSB);
  z = z << 8;
  z |= I2CRead(GYROSCOPE, GYRO_Z_LSB);
  float total = z;
  data->gz = total / GYRO_SCALE_FACTOR;
}

void get_accel_data(imuData *data)
{
  // ensure in USR0; maybe better as a check with I2CRead?
  // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
  int16_t x = I2CRead(GYROSCOPE, ACCEL_X_MSB);
  x = x << 8;
  x |= I2CRead(GYROSCOPE, ACCEL_X_LSB);
  float total = x;
  data->ax = total / ACCEL_SCALE_FACTOR;

  int16_t y = I2CRead(GYROSCOPE, ACCEL_Y_MSB);
  y = y << 8;
  y |= I2CRead(GYROSCOPE, ACCEL_Y_LSB);
  float total = y;
  data->ay = total / ACCEL_SCALE_FACTOR;

  int16_t z = I2CRead(GYROSCOPE, ACCEL_Z_MSB);
  z = z << 8;
  z |= I2CRead(GYROSCOPE, ACCEL_Z_LSB);
  float total = z;
  data->az = total / ACCEL_SCALE_FACTOR;
}

void get_mag_data(imuData *data)
{
  // ensure in USR0; maybe better as a check with I2CRead?
  // also only somewhat sure mag registers are in USR0
  // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
  int16_t x = I2CRead(GYROSCOPE, MAG_X_MSB);
  x = x << 8;
  x |= I2CRead(GYROSCOPE, MAG_X_LSB);
  float total = x;
  data->mx = total / MAG_SCALE_FACTOR;

  int16_t y = I2CRead(GYROSCOPE, MAG_Y_MSB);
  y = y << 8;
  y |= I2CRead(GYROSCOPE, MAG_Y_LSB);
  float total = y;
  data->my = total / MAG_SCALE_FACTOR;

  int16_t z = I2CRead(GYROSCOPE, MAG_Z_MSB);
  z = z << 8;
  z |= I2CRead(GYROSCOPE, MAG_Z_LSB);
  float total = z;
  data->mz = total / MAG_SCALE_FACTOR;
}

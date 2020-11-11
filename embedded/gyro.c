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

float gyroX(){
    // ensure in USR0; maybe better as a check with I2CRead?
    // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
    int16_t x = I2CRead(GYROSCOPE, GYRO_X_MSB);
    x = x << 8;
    x |= I2CRead(GYROSCOPE, GYRO_X_LSB);
    float total = x;
    return total / GYRO_SCALE_FACTOR;
}

float gyroY(){
    // ensure in USR0; maybe better as a check with I2CRead?
    // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
    int16_t y = I2CRead(GYROSCOPE, GYRO_Y_MSB);
    y = y << 8;
    y |= I2CRead(GYROSCOPE, GYRO_Y_LSB);
    float total = y;
    return total / GYRO_SCALE_FACTOR;
}

float gyroZ(){
    // ensure in USR0; maybe better as a check with I2CRead?
    // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
    int16_t z = I2CRead(GYROSCOPE, GYRO_Z_MSB);
    z = z << 8;
    z |= I2CRead(GYROSCOPE, GYRO_Z_LSB);
    float total = z;
    return total / GYRO_SCALE_FACTOR;
}

float accelX()
{
  // ensure in USR0; maybe better as a check with I2CRead?
  // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
  int16_t x = I2CRead(GYROSCOPE, ACCEL_X_MSB);
  x = x << 8;
  x |= I2CRead(GYROSCOPE, ACCEL_X_LSB);
  float total = x;
  return total / ACCEL_SCALE_FACTOR;
}

float accelY()
{
  // ensure in USR0; maybe better as a check with I2CRead?
  // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
  int16_t y = I2CRead(GYROSCOPE, ACCEL_Y_MSB);
  y = y << 8;
  y |= I2CRead(GYROSCOPE, ACCEL_Y_LSB);
  float total = y;
  return total / ACCEL_SCALE_FACTOR;
}

float accelZ()
{
  // ensure in USR0; maybe better as a check with I2CRead?
  // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
  int16_t z = I2CRead(GYROSCOPE, ACCEL_Z_MSB);
  z = z << 8;
  z |= I2CRead(GYROSCOPE, ACCEL_Z_LSB);
  float total = z;
  return total / ACCEL_SCALE_FACTOR;
}

float magX()
{
  // ensure in USR0; maybe better as a check with I2CRead?
  // also only somewhat sure mag registers are in USR0
  // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
  int16_t x = I2CRead(GYROSCOPE, MAG_X_MSB);
  x = x << 8;
  x |= I2CRead(GYROSCOPE, MAG_X_LSB);
  float total = x;
  return total / MAG_SCALE_FACTOR;
}

float magY()
{
  // ensure in USR0; maybe better as a check with I2CRead?
  // also only somewhat sure mag registers are in USR0
  // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
  int16_t y = I2CRead(GYROSCOPE, MAG_Y_MSB);
  y = y << 8;
  y |= I2CRead(GYROSCOPE, MAG_Y_LSB);
  float total = y;
  return total / MAG_SCALE_FACTOR;
}

float magZ()
{
  // ensure in USR0; maybe better as a check with I2CRead?
  // also only somewhat sure mag registers are in USR0
  // I2CWrite(GYROSCOPE, REG_BANK_SEL, REG_BANK_0);
  int16_t z = I2CRead(GYROSCOPE, MAG_Z_MSB);
  z = z << 8;
  z |= I2CRead(GYROSCOPE, MAG_Z_LSB);
  float total = z;
  return total / MAG_SCALE_FACTOR;
}

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
    int16_t x = I2CRead(GYROSCOPE, GYRO_X_MSB);
    x = x << 8;
    x |= I2CRead(GYROSCOPE, GYRO_X_LSB);
    float total = x;
    return total / 250;
}

float gyroY(){
    int16_t y = I2CRead(GYROSCOPE, GYRO_Y_MSB);
    y = y << 8;
    y |= I2CRead(GYROSCOPE, GYRO_Y_LSB);
    float total = y;
    return total / 250;
}

float gyroZ(){
    int16_t z = I2CRead(GYROSCOPE, GYRO_Z_MSB);
    z = z << 8;
    z |= I2CRead(GYROSCOPE, GYRO_Z_LSB);
    float total = z;
    return total / 250;
}

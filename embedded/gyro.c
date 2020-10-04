/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>

#include <i2cManager.h>
#include <uartManager.h>
#include <gyro.h>

void gyro_init(){
    uint8_t prefs = GYRO_DMP_EN | GYRO_DMP_RST;
    I2CWrite(GYROSCOPE, GYRO_USER_CTRL, prefs);
    I2CWrite(GYROSCOPE, GYRO_PWR_MGMT_1, GYRO_CLOCK_PLL);
    // I2CWrite(GYROSCOPE, GYRO_PWR_MGMT_2, GYRO_DISABLE_ACCEL);
}

int16_t gyroX(){
    int16_t x = I2CRead(GYROSCOPE, GYRO_X_MSB);
    x = x << 8;
    x |= I2CRead(GYROSCOPE, GYRO_X_LSB);
    return x;
}

int16_t gyroY(){
    int16_t y = I2CRead(GYROSCOPE, GYRO_Y_MSB);
    y = y << 8;
    y |= I2CRead(GYROSCOPE, GYRO_Y_LSB);
    return y;
}

int16_t gyroZ(){
    int16_t z = I2CRead(GYROSCOPE, GYRO_Z_MSB);
    z = z << 8;
    z |= I2CRead(GYROSCOPE, GYRO_Z_LSB);
    return z;
}

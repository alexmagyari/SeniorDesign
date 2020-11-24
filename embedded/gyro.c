/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>

#include <i2cManager.h>
#include <uartManager.h>
#include <IMU.h>
#include "gyro.h"

imuData _CFG;
void imu_init()
{
    // ensure in USR0; maybe better as a check with I2CRead?
    // I2CWrite(GYROSCOPE, REG_BANK_SEL, USER_BANK_0);

    uint8_t prefs = GYRO_DMP_EN | GYRO_DMP_RST;
    I2CWrite(GYROSCOPE, GYRO_USER_CTRL, prefs);
    I2CWrite(GYROSCOPE, GYRO_PWR_MGMT_1, GYRO_CLOCK_PLL);
    // I2CWrite(GYROSCOPE, GYRO_PWR_MGMT_2, GYRO_DISABLE_ACCEL);

    // change to USR2
//    I2CWrite(GYROSCOPE, MAG_CNTL_2, CONT_MODE_1);
    // gyro config
//    I2CWrite(GYROSCOPE, GYRO_CONFIG, GYRO_FS_SEL);
    // accel config
//    I2CWrite(GYROSCOPE, ACCEL_CONFIG, ACCEL_FS_SEL);
}

void get_gyro_data(imuData *data)
{
    // ensure in USR0; maybe better as a check with I2CRead?
    int16_t x = I2CRead(GYROSCOPE, GYRO_X_MSB);
    x = x << 8;
    x |= I2CRead(GYROSCOPE, GYRO_X_LSB);
    float total = x / GYRO_SCALE_FACTOR;
    UART2PCString("Gyro X Shifted: ");
       UART2PCFloat(total);
       UART2PCNewLine();

    int16_t y = I2CRead(GYROSCOPE, GYRO_Y_MSB);
    y = y << 8;
    y |= I2CRead(GYROSCOPE, GYRO_Y_LSB);
    float totalY = y / GYRO_SCALE_FACTOR;
    data->gyro.axis.Y = totalY;
    UART2PCString("Gyro Y: ");
    UART2PCFloat(totalY);
    UART2PCNewLine();

    int16_t z = I2CRead(GYROSCOPE, GYRO_Z_MSB);
    z = z << 8;
    z |= I2CRead(GYROSCOPE, GYRO_Z_LSB);
    float totalZ = z / GYRO_SCALE_FACTOR;
    data->gyro.axis.Z = totalZ;

    UART2PCString("Gyro Z: ");
    UART2PCFloat(totalZ);
    UART2PCNewLine();

}

void get_accel_data(imuData *data)
{
    // ensure in USR0; maybe better as a check with I2CRead?
  I2CWrite(GYROSCOPE, REG_BANK_SEL, USER_BANK_0);
    int16_t x = I2CRead(GYROSCOPE, ACCEL_X_MSB);
    x = x << 8;
    x |= I2CRead(GYROSCOPE, ACCEL_X_LSB);
    float totalX = x;
    data->accel.axis.X = totalX / ACCEL_SCALE_FACTOR;
    UART2PCString("Accel X: ");
    UART2PCFloat(totalX / ACCEL_SCALE_FACTOR);
    UART2PCNewLine();
    int16_t y = I2CRead(GYROSCOPE, ACCEL_Y_MSB);
    y = y << 8;
    y |= I2CRead(GYROSCOPE, ACCEL_Y_LSB);
    float totalY = y;
    data->accel.axis.Y = totalY / ACCEL_SCALE_FACTOR;
    UART2PCString("Accel Y: ");
    UART2PCFloat(totalY / ACCEL_SCALE_FACTOR);
    UART2PCNewLine();
    int16_t z = I2CRead(GYROSCOPE, ACCEL_Z_MSB);
    z = z << 8;
    z |= I2CRead(GYROSCOPE, ACCEL_Z_LSB);
    float totalZ = z;
    data->accel.axis.Z = totalZ / ACCEL_SCALE_FACTOR;
    UART2PCString("Accel Z: ");
    UART2PCFloat(totalZ / ACCEL_SCALE_FACTOR);
    UART2PCNewLine();
}

void get_mag_data(imuData *data)
{
    I2CWrite(GYROSCOPE, MAG_CNTL_2, CONT_MODE_1);
    int16_t x = I2CRead(GYROSCOPE, MAG_X_LSB);
    x = x << 8;
    x |= I2CRead(GYROSCOPE, MAG_X_MSB);
    float totalX = x;
    data->mag.axis.X = totalX / MAG_SCALE_FACTOR;

//    UART2PCString("Mag X: ");
//    UART2PCFloat(totalX / MAG_SCALE_FACTOR);
//    UART2PCNewLine();
    int16_t y = I2CRead(GYROSCOPE, MAG_Y_LSB);
    y = y << 8;
    y |= I2CRead(GYROSCOPE, MAG_Y_MSB);
    float totalY = y;
    data->mag.axis.Y = totalY / MAG_SCALE_FACTOR;
//    UART2PCString("Mag Y: ");
//    UART2PCFloat(totalY / MAG_SCALE_FACTOR);
//    UART2PCNewLine();
    int16_t z = I2CRead(GYROSCOPE, MAG_Z_LSB);
    z = z << 8;
    z |= I2CRead(GYROSCOPE, MAG_Z_MSB);
    float totalZ = z;
    data->mag.axis.Z = totalZ / MAG_SCALE_FACTOR;
//    UART2PCString("Mag Z: ");
//    UART2PCFloat(totalZ / MAG_SCALE_FACTOR);
//    UART2PCNewLine();
}

void getZeroMeas(imuData *cfg)
{
    boardOrientation(&(cfg->accel));
    boardOrientation(&(cfg->gyro));
    get_accel_data(cfg);
    get_gyro_data(cfg);
    get_mag_data(cfg);
}

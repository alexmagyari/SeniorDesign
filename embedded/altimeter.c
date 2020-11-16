/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>

#include <i2cManager.h>
#include <uartManager.h>
#include <altimeter.h>

float startAltitude;
void altimeter_init()
{

    // Set device to active mode
    uint8_t prefs = ALTIMETER_CTRL1_SBYB | ALTIMETER_ALTIMETER_MODE
            | ALTIMETER_OVERSAMPLE_1 | ALTIMETER_OVERSAMPLE_2
            | ALTIMETER_OVERSAMPLE_0;
    I2CWrite(ALTIMETER, ALTIMETER_CTRL1, prefs);
    startAltitude = altitude();
}

float changeInAltitude()
{
    return startAltitude - altitude();
}

float altitude()
{
    // Altitude is stored as MSB and CSB as twos compliment
    // and LSB as fraction of meter
    float altitude = 0;
    int16_t bigData = 0x0000;
    uint8_t littleData = I2CRead(ALTIMETER, ALTIMETER_PRESSURE_LSB);
    bigData = I2CRead(ALTIMETER, ALTIMETER_PRESSURE_MSB);
    bigData = bigData << 8;
    bigData |= (I2CRead(ALTIMETER, ALTIMETER_PRESSURE_CSB));

    // Convert to float
    altitude += (littleData >> 4);
    altitude /= 16;
    altitude += bigData;
    return altitude;
}

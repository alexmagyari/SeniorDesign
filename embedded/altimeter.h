/*
 * altimeter.h
 *
 *  Created on: Oct 3, 2020
 *      Author: amag0
 */

#ifndef ALTIMETER_H_
#define ALTIMETER_H_


#define ALTIMETER           0x60
#define ALTIMETER_READ      0xC1
#define ALTIMETER_WRITE     0xC0

// Altimeter control registers
// Refer to datasheet for max polling times with oversampling
#define ALTIMETER_CTRL1 0x26

#define ALTIMETER_CTRL1_SBYB  0x01
#define ALTIMETER_CTRL1_RESET 0x02
#define ALTIMETER_OVERSAMPLE_0 0x08
#define ALTIMETER_OVERSAMPLE_1 0x10
#define ALTIMETER_OVERSAMPLE_2 0x20
#define ALTIMETER_ALTIMETER_MODE 0x80

// Altimer status registers
#define ALTIMETER_STATUS 0x00
#define ALTIMETER_DATA_READY 0x06

// ID Register
#define ALTIMETER_WHO_AM_I 0x0C
// Pressure registers
#define ALTIMETER_PRESSURE_MSB 0x01
#define ALTIMETER_PRESSURE_CSB 0x02
#define ALTIMETER_PRESSURE_LSB 0x03

// Initialize altimeter
void altimeter_init(void);

// Returns the float value of the altitude
float altitude(void);

// Returns the change in altitude since program start
float changeInAltitude(void);
#endif /* ALTIMETER_H_ */

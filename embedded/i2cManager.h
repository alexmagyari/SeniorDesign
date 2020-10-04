/*
 * i2cManager.h
 *
 *  Created on: Oct 1, 2020
 *      Author: amag0
 */

#ifndef I2CMANAGER_H_
#define I2CMANAGER_H_

// Initialize the I2C Module
void I2C_init();

// Read a value from I2C, where slave is the 7 bit slave address,
// and regToRead is the register to read from. Returns register value
uint8_t I2CRead(uint8_t slave, uint8_t regToRead);

// Writes to register regToWriteTo. slave is 7 bit address.
void I2CWrite(uint8_t slave, uint8_t regToWriteTo, uint8_t dataToWrite);

#endif /* I2CMANAGER_H_ */

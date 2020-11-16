/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 *  MSP432 I2C - EUSCI_B0 I2C Master TX  bytes to MSP432 Slave - Repeated Start
 *
 *  Description: This demo connects two MSP432 's via the I2C bus. The master
 *  transmits to the slave. This is the MASTER CODE. It continuously
 *  transmits an array of data and demonstrates how to implement an I2C
 *  master transmitter sending multiple bytes followed by a repeated start,
 *  followed by a read of multiple bytes.  This is a common operation for
 *  reading register values from I2C slave devices such as sensors. The
 *  transaction for the I2C that is written looks as follows:
 *
 *  _________________________________________________________
 *  |  Start   |      |  Start   |                   |       |
 *  | 0x48Addr | 0x04 | 0x48Addr |  <10 Byte Read>   | Stop  |
 *  |    W     |      |    R     |                   |       |
 *  |__________|______|__________|___________________|_______|
 *
 *  ACLK = n/a, MCLK = HSMCLK = SMCLK = BRCLK = default DCO = ~3.0MHz
 *
 *                                /|\  /|\
 *                MSP432P401      10k  10k      MSP432P401
 *                   slave         |    |         master
 *             -----------------   |    |   -----------------
 *            |     P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA     |
 *            |                 |  |       |                 |
 *            |                 |  |       |                 |
 *            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
 *            |                 |          |                 |
 *
  *****************************************************************************/
/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <i2cManager.h>
#include <uartManager.h>

/* Variables */
//const uint8_t TXData[] = {0x04};
static volatile uint32_t xferIndex;

/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // Autostop
};

void I2C_init(){
    /* Select Port 1 for I2C - Set Pin 6, 7 to input Primary Module Function,
     *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
     */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);

    MAP_I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);

}

void I2CWrite(uint8_t slave, uint8_t regToWriteTo, uint8_t dataToWrite){

    /* Making sure the last transaction has been completely sent out */
    while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE));

    // Enable Module in write mode
    MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, slave);
    MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
    MAP_I2C_enableModule(EUSCI_B0_BASE);

    while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE));
    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, regToWriteTo);
    MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, dataToWrite);
    MAP_I2C_masterSendMultiByteStop(EUSCI_B0_BASE);

}

uint8_t I2CRead(uint8_t slave, uint8_t regToRead){
    uint8_t RXData;
    /* Making sure the last transaction has been completely sent out */
    while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE))
        ;
    // Enable Module in write mode
    MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, slave);
    MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_MODE);
    MAP_I2C_enableModule(EUSCI_B0_BASE);

    /* Send out EEPROM Mock Read Cmd (2 databytes) */
    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, regToRead);  // Start + 1Byte


    /* Now we need to initiate the read */
            /* Wait until 2nd Byte has been output to shift register */
            while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));

            // Send the restart condition, read one byte, send the stop condition right away
            EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_TR);
            EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
            while(MAP_I2C_masterIsStartSent(EUSCI_B0_BASE));
            EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
            while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0));
            RXData = EUSCI_B0->RXBUF;
            return RXData;
}



/*******************************************************************************
 * eUSCIB0 ISR. The repeated start and transmit/receive operations happen
 * within this ISR.
 *******************************************************************************/
//void EUSCIB0_IRQHandler(void)
//{
//    uint_fast16_t status;
//
//    status = MAP_I2C_getEnabledInterruptStatus(EUSCI_B0_BASE);
//
//    /* Receives bytes into the receive buffer. If we have received all bytes,
//     * send a STOP condition */
//    if (status & EUSCI_B_I2C_RECEIVE_INTERRUPT0)
//    {
//        // One-byte Read
//        UART2PCChar( MAP_I2C_masterReceiveSingle(EUSCI_B0_BASE));
//        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
//    }
//}
//

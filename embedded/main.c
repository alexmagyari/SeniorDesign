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
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: Senior Design II Team 10
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Board includes */
#include <ti/devices/msp432p4xx/inc/msp.h>

/* Project Relevant Includes */
#include <motor.h>
#include <altimeter.h>
#include <uartManager.h>
#include <i2cManager.h>
#include <gyro.h>
#include "gps.h"


struct location gps;
int main(void)
{
    /*** Watchdog timer and clock set-up ***/
    WDTCTL = WDTPW | WDTHOLD;    // Stop Watchdog timer
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR__AM_LDO_VCORE1; // Enable correct voltage for 48MHZ
//                                                                // Change line 71 in system_msp432p401r.c to 4
//////
    UART_init();
    I2C_init();
    initPWM(MOTOR_PWM_PERIOD, 0);
    altimeter_init();
    gyro_init();

    volatile int32_t c = 4800000;
    while (c != 0)
        c--;
    armMotors();
    editMainPWM(20);
    c = 10000000;
    while (c != 0)
        c--;
    disarmMotors();





  while (1){
      gps = gpsTake();

      UART2PCString("Latitude: ");
      UART2PCString(gps.latitude);
      UART2PCNewLine();
      UART2PCString("Longitude: ");
      UART2PCString(gps.longitude);
      UART2PCNewLine();
      UART2PCString("Longitude direction: ");
      UART2PCChar(gps.lonDirection);
      UART2PCNewLine();
      UART2PCString("Latitude direction: ");
      UART2PCChar(gps.latDirection);
      UART2PCNewLine();
      UART2PCString("Latitude in decimal: ");
      UART2PCFloat(gps.LatDecimal);
      UART2PCNewLine();
      UART2PCString("Longitude in decimal: ");
      UART2PCFloat(gps.LonDecimal);
      UART2PCNewLine();
      UART2PCNewLine();



      float x = gyroX();
      float y = gyroY();
      float z = gyroZ();
      char gyroOutput[] = "**** Gyro values: ****\0";
      char gyroXc[] = "X: \0";
      char gyroYc[] = "Y: \0";
      char gyroZc[] = "Z: \0";
      UART2PCString(gyroOutput);
      UART2PCNewLine();
      UART2PCString(gyroXc);
      UART2PCFloat(x);
      UART2PCNewLine();
      UART2PCString(gyroYc);
      UART2PCFloat(y);
      UART2PCNewLine();
      UART2PCString(gyroZc);
      UART2PCFloat(z);
      UART2PCNewLine();
      UART2PCNewLine();
      UART2PCNewLine();

      char altitudeOutput[] = "*** Altitude Values***\0";
              UART2PCString(altitudeOutput);
              UART2PCNewLine();
              char ao2[] = "Current Altitude: \0";
              UART2PCString(ao2);
              UART2PCFloat(altitude());
              UART2PCNewLine();
              char output[] = "Delta Altitude: \0";
              UART2PCString(output);
              UART2PCFloat(changeInAltitude());
              UART2PCNewLine();
              UART2PCNewLine();
  }
//
//    int c = 0;
//    float x, y, z = 0;
////    while (1)
//    {

//while(1){
//        while (c < 1000000)
//            c++;
//
//        c = 0;
////        // Altitude out
//        char altitudeOutput[] = "*** Altitude Values***\0";
//        UART2PCString(altitudeOutput);
//        UART2PCNewLine();
//        char ao2[] = "Current Altitude: \0";
//        UART2PCString(ao2);
//        UART2PCFloat(altitude());
//        UART2PCNewLine();
//        char output[] = "Delta Altitude: \0";
//        UART2PCString(output);
//        UART2PCFloat(changeInAltitude());
////        UART2PCNewLine();
////        UART2PCNewLine();
//
//       //  Gyro Out
//        x = gyroX();
//        y = gyroY();
//        z = gyroZ();
//        char gyroOutput[] = "**** Gyro values: ****\0";
//        char gyroXc[] = "X: \0";
//        char gyroYc[] = "Y: \0";
//        char gyroZc[] = "Z: \0";
//        UART2PCString(gyroOutput);
//        UART2PCNewLine();
//        UART2PCString(gyroXc);
//        UART2PCFloat(x);
//        UART2PCNewLine();
//        UART2PCString(gyroYc);
//        UART2PCFloat(y);
//        UART2PCNewLine();
//        UART2PCString(gyroZc);
//        UART2PCFloat(z);
//        UART2PCNewLine();
//        UART2PCNewLine();
//        UART2PCNewLine();
//    }


    return 0;
}

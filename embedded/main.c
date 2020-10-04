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

/* Board includes */
#include <ti/devices/msp432p4xx/inc/msp.h>

/* Project Relevant Includes */
#include <motor.h>
#include <altimeter.h>
#include <uartManager.h>
#include <i2cManager.h>
#include <gyro.h>
int main(void)
{
    /*** Watchdog timer and clock set-up ***/
    WDTCTL = WDTPW | WDTHOLD;    // Stop Watchdog timer
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR__AM_LDO_VCORE1; // Enable correct voltage for 48MHZ
                                                                // Change line 71 in system_msp432p401r.c to 4
////
    UART_init();
    I2C_init();
    altimeter_init();
    gyro_init();


    int c = 0;
    float x, y, z = 0;
    while (1)
    {
        while (c < 1000000)
            c++;

        c = 0;

        // Altitude out
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

        // Gyro Out
        x = gyroX();
        y = gyroY();
        z = gyroZ();
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
    }




    // MOTOR SWEEP STUFF
    int allMotorPWM = MIN_MAIN_MOTOR_PWM;
    int motor1PWM = 0;
    int motor2PWM = -1 * MAX_MOTOR_OFFSET;
    int motor3PWM = MAX_MOTOR_OFFSET;
    int allMotorIncrease = 1;
    int singleMotorIncrease1 = 1;
    int singleMotorIncrease2 = 1;
    int singleMotorIncrease3 = 0;
    initPWM(MOTOR_PWM_PERIOD, allMotorPWM);

  int count = 0;


//  while (1){
//      if (count < 10000)
//      {
//          count ++;
//      }
//      else
//      {
//          if (allMotorIncrease == 1)
//          {
//              if (allMotorPWM < MAX_MAIN_MOTOR_PWM)
//              {
//                  allMotorPWM ++;
//              }
//              else
//              {
//                  allMotorIncrease = 0;
//              }
//          }
//          else
//          {
//              if (allMotorPWM > MIN_MAIN_MOTOR_PWM)
//              {
//                  allMotorPWM = allMotorPWM - 1;
//              }
//              else
//              {
//                  allMotorIncrease = 1;
//              }
//          }
//
//
//          if (singleMotorIncrease1 == 1)
//          {
//              if (motor1PWM < MAX_MOTOR_OFFSET)
//              {
//                  motor1PWM = (allMotorIncrease == 1) ? motor1PWM + 1 : motor1PWM + 3;
//              }
//              else
//              {
//                  singleMotorIncrease1 = 0;
//              }
//          }
//          else
//          {
//              if (motor1PWM > -1 * MAX_MOTOR_OFFSET)
//              {
//                  motor1PWM = (allMotorIncrease == 1) ? motor1PWM - 3 : motor1PWM - 1;
//              }
//              else
//              {
//                  singleMotorIncrease1 = 1;
//              }
//          }
//
//          if (singleMotorIncrease2 == 1)
//                    {
//                        if (motor2PWM < MAX_MOTOR_OFFSET)
//                        {
//                            motor2PWM = (allMotorIncrease == 1) ? motor2PWM + 1 : motor2PWM + 3;
//                        }
//                        else
//                        {
//                            singleMotorIncrease2 = 0;
//                        }
//                    }
//                    else
//                    {
//                        if (motor2PWM > -1 * MAX_MOTOR_OFFSET)
//                        {
//                            motor2PWM = (allMotorIncrease == 1) ? motor2PWM - 3 : motor2PWM - 1;
//                        }
//                        else
//                        {
//                            singleMotorIncrease2 = 1;
//                        }
//                    }
//
//
//          if (singleMotorIncrease3 == 1)
//                    {
//                        if (motor3PWM < MAX_MOTOR_OFFSET)
//                        {
//                            motor3PWM = (allMotorIncrease == 1) ? motor3PWM + 1 : motor3PWM + 3;
//                        }
//                        else
//                        {
//                            singleMotorIncrease3 = 0;
//                        }
//                    }
//                    else
//                    {
//                        if (motor3PWM > -1 * MAX_MOTOR_OFFSET)
//                        {
//                            motor3PWM = (allMotorIncrease == 1) ? motor3PWM - 3 : motor3PWM - 1;
//                        }
//                        else
//                        {
//                            singleMotorIncrease3 = 1;
//                        }
//                    }
//            editMainPWM(allMotorPWM);
//            editMotorPWM(1, allMotorPWM + motor1PWM);
//            editMotorPWM(2, allMotorPWM + motor2PWM);
//            editMotorPWM(3, allMotorPWM + motor3PWM);
//            count = 0;
//      }
//  }

    return 0;
}

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
#include <gps.h>
#include <PID.h>

// Practicing my bad practices
struct location gps;

void PID_Tune(int whichPID, char * prop, float val, pid_data * PIDs);
void printPIDs(pid_data * PIDs);
void updatePIDs(pid_data * PIDs);
int main(void)
{

    /*** Watchdog timer and clock set-up ***/
    WDTCTL = WDTPW | WDTHOLD;    // Stop Watchdog timer
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR__AM_LDO_VCORE1; // Enable correct voltage for 48MHZ

    pid_init();
    pid_data PIDs[6] = { pidRoll, pidPitch, pidYaw, pidAlt, pidX, pidY };

    UART_init();

    I2C_init();
    initPWM(MOTOR_PWM_PERIOD, 0);
    altimeter_init();
    imu_init();

    volatile int32_t c = 4800000;
    while (c != 0)
        c--;
    armMotors();
    editMainPWM(20);
//    c = 10000000;
//    while (c != 0)
//        c--;
//    disarmMotors();

    // PID Tuner

    while (1)
    {

        printPIDs(PIDs);
        while (!UART_HAS_MAIL)
            ;
        UART_getMail();
        UART2PCNewLine();
        updatePIDs(PIDs);
    }

    while (1)
    {

//        gps = gpsTake();
//
//        UART2PCString("Latitude: \0");
//        UART2PCString(strcat(gps.latitude, "\0"));
//        UART2PCNewLine();
//        UART2PCString("Longitude: \0");
//        UART2PCString(strcat(gps.longitude, "\0"));
//        UART2PCNewLine();
//        UART2PCString("Longitude direction: \0");
//        UART2PCChar((char)*gps.lonDirection);
//        UART2PCNewLine();
//        UART2PCString("Latitude direction: \0");
//        UART2PCChar((char)*gps.latDirection);
//        UART2PCNewLine();
//        UART2PCString("Latitude in decimal: \0");
//        UART2PCFloat(gps.LatDecimal);
//        UART2PCNewLine();
//        UART2PCString(strcat("Longitude in decimal: ", "\0"));
//        UART2PCFloat(gps.LonDecimal);
//        UART2PCNewLine();
//        UART2PCNewLine();
//
//        float x = gyroX();
//        float y = gyroY();
//        float z = gyroZ();
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
//
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
//        UART2PCNewLine();
//        UART2PCNewLine();

    }

    return 0;
}

void updatePIDs(pid_data * PIDs)
{
    char * token;
    char * PIDName;
    char * PIDProp;
    float PIDVal;
    int c = 0;
    token = strtok(UART_MAIL, "->");
    PIDName = token;
    c++;
    while (token != NULL)
    {
        token = strtok(NULL, "->");
        if (c == 1)
        {

            PIDProp = token;
            c++;
        }
        else if (c == 2)
        {
            c++;
            char * newBuf = strcat(token, '\0');
            PIDVal = atof(newBuf);
            if (strcmp(PIDName, "t") == 0)
            {
                PID_Tune(6, PIDProp, PIDVal, PIDs);
            }
            else if (strcmp(PIDName, "roll") == 0)
            {
                PID_Tune(0, PIDProp, PIDVal, PIDs);
            }
            else if (strcmp(PIDName, "pitch") == 0)
            {
                PID_Tune(1, PIDProp, PIDVal, PIDs);
            }
            else if (strcmp(PIDName, "yaw") == 0)
            {
                PID_Tune(2, PIDProp, PIDVal, PIDs);
            }
            else if (strcmp(PIDName, "alt") == 0)
            {
                PID_Tune(3, PIDProp, PIDVal, PIDs);
            }
            else if (strcmp(PIDName, "x") == 0)
            {
                PID_Tune(4, PIDProp, PIDVal, PIDs);
            }
            else if (strcmp(PIDName, "y") == 0)
            {
                PID_Tune(5, PIDProp, PIDVal, PIDs);
            }
            else
            {
                UART2PCString("Incorrect format! Read the rules, dammit");
                UART2PCNewLine();
            }
        }
    }
}
void printPIDs(pid_data * PIDs)
{
    UART2PCNewLine();
    UART2PCNewLine();
    UART2PCNewLine();
    int i = 0;
    for (i = 0; i < 6; i++)
    {

        UART2PCString(PIDs[i].desc);
        UART2PCChar('\t');
        UART2PCChar('\t');
        UART2PCChar('\t');
    }
    UART2PCNewLine();
    i = 0;
    for (i = 0; i < 6; i++)
    {

        UART2PCString("kp: \0");
        UART2PCFloat(PIDs[i].kp);
        UART2PCChar('\t');
        UART2PCChar('\t');
    }
    UART2PCNewLine();
    i = 0;
    for (i = 0; i < 6; i++)
    {

        UART2PCString("ki: \0");
        UART2PCFloat(PIDs[i].ki);
        UART2PCChar('\t');
        UART2PCChar('\t');
    }
    UART2PCNewLine();
    i = 0;
    for (i = 0; i < 6; i++)
    {

        UART2PCString("kd: \0");
        UART2PCFloat(PIDs[i].kd);
        UART2PCChar('\t');
        UART2PCChar('\t');
    }
    UART2PCNewLine();
    i = 0;
    for (i = 0; i < 6; i++)
    {

        UART2PCString("max: \0");
        UART2PCFloat(PIDs[i].max);
        UART2PCChar('\t');
        UART2PCChar('\t');
    }
    UART2PCNewLine();
    i = 0;
    for (i = 0; i < 6; i++)
    {

        UART2PCString("min: \0");
        UART2PCFloat(PIDs[i].min);
        UART2PCChar('\t');
    }
    UART2PCNewLine();
    i = 0;
    for (i = 0; i < 6; i++)
    {

        UART2PCString("fc: \0");
        UART2PCFloat(PIDs[i].fc);
        UART2PCChar('\t');
        UART2PCChar('\t');
    }
    UART2PCNewLine();
    i = 0;
    UART2PCString("Enter new value in the form of name->property->value\0");
    UART2PCNewLine();
    UART2PCString("Example: roll->kp->.888\0");
    UART2PCNewLine();
}
void PID_Tune(int whichPID, char * prop, float val, pid_data * PIDs)
{
    if (whichPID == 6)
    {
        if (val == 0)
        {
            disarmMotors();
            return;
        }
        UART2PCNewLine();
        UART2PCFloat(val);
        UART2PCNewLine();
        editMainPWM(val);
        return;
    }
    else if ((strcmp(prop, "kp")) == 0)
    {
        PIDs[whichPID].kp = val;
    }
    else if ((strcmp(prop, "ki")) == 0)
    {
        PIDs[whichPID].ki = val;
    }
    else if ((strcmp(prop, "kd")) == 0)
    {
        PIDs[whichPID].kd = val;
    }
    else if ((strcmp(prop, "max")) == 0)
    {
        PIDs[whichPID].max = val;
    }
    else if ((strcmp(prop, "min")) == 0)
    {
        PIDs[whichPID].min = val;
    }
    else if ((strcmp(prop, "fc")) == 0)
    {
        PIDs[whichPID].fc = val;
    }
    else
    {
        UART2PCString("Incorrect format. Read the rules, dammit.\0");
    }
}


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
//#include <pthread.h>
//#include <mqueue.h>
//#include <semaphore.h>
//#include <time.h>
//#include <unistd.h>
//
///* TI-Driver Includes */
//#include <ti/drivers/GPIO.h>
//#include <ti/drivers/UART.h>
//#include <ti/drivers/Timer.h>
//#include <ti/display/Display.h>
//
///* SAP/DriverLib Includes */
//#include <ti/sap/sap.h>
//#include <ti/sbl/sbl.h>
//#include <ti/sbl/sbl_image.h>
//
///* Local Includes */
//#include "simple_application_processor.h"
//#include "Profile/simple_gatt_profile.h"
//#include "Board.h"
//#include "platform.h"
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
#include <IMU.h>
#include <motorMix.h>

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
    UART2PCString("Start: \0");
    I2C_init();
    UART2PCString("I2C: \0");
    initPWM(MOTOR_PWM_PERIOD, 0);
    UART2PCString("PWM: \0");
//    altimeter_init();
//    UART2PCString("ALT: \0");
    imu_init();
    UART2PCString("IMU: \0");
    int startup = 0;
    while (startup < 1000)
        startup++;
    //Update init IMU
    getZeroMeas(&_CFG);

    // currentIMU
    imuData cIMU = _CFG;

    int thrust = 20;

    // Init Controls
    cntrl_cmd controls;
    controls.thrust_cmd = 0;
    controls.roll_cmd = 0;
    controls.pitch_cmd = 0;
    controls.yaw_cmd = 0;

    // Init Cotors
    motor_mix motors;
    motors.thrust = 0;
    motors.roll = 0;
    motors.pitch = 0;
    motors.yaw = 0;

    motors.m0 = 0;
    motors.m1 = 0;
    motors.m2 = 0;
    motors.m3 = 0;

    motors.m0_duty = MIN_MAIN_MOTOR_PWM;
    motors.m1_duty = MIN_MAIN_MOTOR_PWM;
    motors.m2_duty = MIN_MAIN_MOTOR_PWM;
    motors.m3_duty = MIN_MAIN_MOTOR_PWM;
    UART2PCString("Pitch: ");
    int volatile c = 4800000;
    while (c != 0)
        c--;

    armMotors();
    editMainPWM(MIN_MAIN_MOTOR_PWM);
    c = 4800000;
    while (c <= 0)
        c--;
    printPIDs(PIDs);

    float dt = 100000000;
    int counter = 0;

    while (1)
    {
        //Update gyro

        get_gyro_data(&cIMU);
        get_mag_data(&cIMU);
        get_accel_data(&cIMU);

        //Compute roll
        //controls.roll_cmd = pid_compute(PIDs, 0.0f, cIMU.gyroRate.data[0], dt);

        convert_for_cntrl(&cIMU, &(cIMU.gyroRate), &(cIMU.mag), dt);

        // Update motor speeds

        controls.pitch_cmd = pid_compute(&PIDs[1], 0, cIMU.gyroRate.axis.pitch,
                                         dt);

        compute_motor_commands(&motors, 0, 0, controls.pitch_cmd, 0);
        editMotorPWM(0, motors.m0_duty);
        editMotorPWM(1, motors.m1_duty);
        editMotorPWM(2, motors.m2_duty);
        editMotorPWM(3, motors.m3_duty);


//        UART2PCString("pitch_cmd: ");
//        UART2PCFloat(controls.pitch_cmd);
//        UART2PCNewLine();

        if (counter >= 2500)
        {
            UART2PCString("M0 Duty: ");
            UART2PCFloat(motors.m0_duty);
            UART2PCNewLine();
            UART2PCString("M1 Duty: ");
            UART2PCFloat(motors.m1_duty);
            UART2PCNewLine();
            counter = 0;
        }
        else
        {

            counter++;
        }

        if (UART_HAS_MAIL)
        {
            if (strcmp(UART_MAIL, "exit") == 0)
                break;
            UART_getMail();
            UART2PCNewLine();
            updatePIDs(PIDs);
            printPIDs(PIDs);

        }

    }
    disarmMotors();
    while (1);
    return 0;
}

void updatePIDs(pid_data * PIDs)
{
    char * token;
    char * PIDName;
    char * PIDProp;
    float PIDVal;
    int c = 0;
    token = strtok(UART_MAIL, ",");
    PIDName = token;
    c++;
    while (token != NULL)
    {
        token = strtok(NULL, ",");
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
            else if (strcmp(PIDName, "a") == 0)
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


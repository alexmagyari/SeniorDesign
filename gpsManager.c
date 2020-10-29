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
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <i2cManager.h>
#include <uartManager.h>
#include <gpsManager.h>

char* GPS_Input (void) {


    MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, slaveAdd);
    MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_MODE);
    MAP_I2C_enableModule(EUSCI_B0_BASE);

    char* a;
    while (1){
        char result[70] = "$GPGGA,";
        char* sample = "$GNGGA";
        unsigned int i = 0;
        char label[10] = "";
        char r = MAP_I2C_masterReceiveSingleByte(EUSCI_B0_BASE);
        if(r=='$'){
            while (i<6){
                strncat(label, &r, 1);
                r = MAP_I2C_masterReceiveSingleByte(EUSCI_B0_BASE);
                i++;
            }
            if(strcmp(label,sample) == 0){
                i = 0;
                while(i<66){
                    r = MAP_I2C_masterReceiveSingleByte(EUSCI_B0_BASE);
                    strncat(result, &r, 1);
                    i++;
                }
            a = result;
            break;
            }
        }
    }
    return a;
}

char* getLati(char c[]) {
    unsigned int length = strlen(c);
    int comma=0;
    char lati[100]="";
    unsigned int i = 0;
    for (i = 0; i<length; i++){
        if ( c[i] == ','){
            comma++;
        }
        if(c[i] == ',' && comma == 2){
            i++;
            comma = 1;

            while (c[i] != ','){
                char r = c[i];
                strncat(lati,&r,1);
                i++;
            }
            char* r = lati;
            //printString(lati);
            return r;
        }
    }

}

char* getLatDirection(char c[]){
    unsigned int len = strlen(c);
    int comma = 0;
    char direction[100] = "";
    unsigned int i = 0;
    for ( i = 0; i<len; i++){
        if (c[i] == ','){
            comma++;
        }
        if (c[i] == ',' && comma == 3){
            i++;
            strncat(direction, &c[i], 1);
            char* r = direction;
            return r;
        }
    }
}

char* getLongi(char c[]) {
    unsigned int length = strlen(c);
    int comma = 0;
    char longi[100] = "";
    unsigned int i = 0;
    for ( i = 0; i < length; i++){
        if(c[i] == ','){
            comma++;
        }
        if(c[i] == ',' && comma == 4){
            i++;
            comma++;
            while(c[i] != ','){
                char r = c[i];
                strncat(longi, &r, 1);
                i++;
            }
            char* r = longi;
            return r;
        }
    }
}

char* getLonDirection(char c[]){
    unsigned int len = strlen(c);
    int comma = 0;
    char direction[100] = "";
    unsigned int i = 0;
    for ( i = 0; i<len; i++){
        if (c[i] == ','){
            comma++;
        }
        if (c[i] == ',' && comma == 5){
            i++;
            strncat(direction, &c[i],1);
            char* r = direction;
            return r;
        }
    }
}

char* getAlti(char c[]){
    char* r;
    unsigned int len = strlen(c);
    int comma = 0;
    char altitude[100] = "";
    unsigned int i = 0;
    for ( i = 0; i<len; i++){
        if (c[i] == ','){
            comma++;
        }
        if (c[i] == ',' && comma == 9){
            i++;
            while (c[i] != ','){
                strncat(altitude, &c[i], 1);
                i++;
            }
            r = altitude;

        }
    }
    return r;

}
float ToLatDecimal(char c[], char d){
    int len = strlen(c);
    char degree[50] = "";
    char decimal[50] = "";
    unsigned int i = 0;
    for ( i = 0; i < len; i++){
        if (i == 0 || i == 1) {
            strncat(degree, &c[i], 1);
        }
        if (i >= 2){
            strncat(decimal, &c[i], 1);
        }
    }

    float Degree = strtof(degree, NULL);
    float Decimal = strtof(decimal, NULL);

    float result = Degree + (Decimal/60);
    if (d == 'W') {
        result = result * (-1);
    }
    return result;
}

float ToLonDecimal(char c[], char d){
    int len = strlen(c);
    char degree[50] = "";
    char decimal[50] = "";
    unsigned int i = 0;
    for ( i = 0; i < len; i++){
        if (i == 0 || i == 1 || i == 2) {
            strncat(degree, &c[i], 1);
        }
        if (i >= 3){
            strncat(decimal, &c[i], 1);
        }
    }

    float Degree;
    Degree = strtof(degree, NULL);
    float Decimal = strtof(decimal, NULL);

    float result = Degree + (Decimal/60);
    if (d == 'S') {
        result = result * (-1);
    }
    return result;
}

void gpsTake(struct location gps){
    char* input = GPS_Input();
    char s[100];
    char temp[20];
    char t1[20];
    strcpy(s, input);

        strcpy(gps.latitude, getLati(s));
        strcpy(gps.longitude, getLongi(s));
        strcpy(gps.altitude, getAlti(s));
        strcpy(gps.lonDirection, getLonDirection(s));
        strcpy(gps.latDirection, getLatDirection(s));
        strcpy(temp, gps.latitude);
        strcpy(t1, gps.latDirection);
        gps.LatDecimal = ToLatDecimal(temp, t1);
        strcpy(temp, gps.longitude);
        strcpy(t1, gps.lonDirection);
        gps.LonDecimal = ToLonDecimal(temp, t1);
}



/*--------------------------------------------------
 * Copy following codes to main to retrieve the data
 * -------------------------------------------------
        I2C_GPS_init(0x42);

        char* input = GPS_Input();
        char s[100];
        char temp[20];
        char t1 [20];
        strcpy(s, input);
            struct location gps;
            strcpy(gps.latitude, getLati(s));
            strcpy(gps.longitude, getLongi(s));
            strcpy(gps.altitude, getAlti(s));
            strcpy(gps.lonDirection, getLonDirection(s));
            strcpy(gps.latDirection, getLatDirection(s));
            strcpy(temp, gps.latitude);
            strcpy(t1, gps.latDirection);
            gps.LatDecimal = ToLatDecimal(temp, t1);
            strcpy(temp, gps.longitude);
            strcpy(t1, gps.lonDirection);
            gps.LonDecimal = ToLonDecimal(temp, t1);
 */
/*-----------------------------------------------------
 * Copy following codes to main to test with terminal via UART
 * ----------------------------------------------------
    UART_init();
    I2C_GPS_init(0x42);

    char* input = GPS_Input();
    char s[100];
    char temp[20];
    char t1 [20];
    strcpy(s, input);
        struct location gps;
        strcpy(gps.latitude, getLati(s));
        strcpy(gps.longitude, getLongi(s));
        strcpy(gps.altitude, getAlti(s));
        strcpy(gps.lonDirection, getLonDirection(s));
        strcpy(gps.latDirection, getLatDirection(s));
        strcpy(temp, gps.latitude);
        strcpy(t1, gps.latDirection);
        gps.LatDecimal = ToLatDecimal(temp, t1);
        strcpy(temp, gps.longitude);
        strcpy(t1, gps.lonDirection);
        gps.LonDecimal = ToLonDecimal(temp, t1);

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
 */

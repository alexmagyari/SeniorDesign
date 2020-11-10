/*
 * gpsManager.h
 *
 *  Created on: Oct 18, 2020
 *      Author: Sang Nguyen
 */

#ifndef GPSMANAGER_H_
#define GPSMANAGER_H_

//Declare the gps structure
struct location {
    char longitude[20];
    char lonDirection[2];
    char latitude[20];
    char latDirection[2];
    char altitude[20];
    float LonDecimal;
    float LatDecimal;
    float AltDecimal;
};

//Read the output from GPS module and retrieve $GPGGA NMEA Format
char* GPS_Input (void);

//Read the GPS input and return Latitude in degree form
char* getLati(char c[]);

//Read the GPS input and return Latitude direction
//Direction is E(east) or W(west);
char* getLatDirection(char c[]);

//Read the GPS input and return Longitude in degree form
char* getLongi (char c[]);

//Read the GPS input and return Longitude Direction
//Direction is N(north) or S(south)
char* getLonDirection(char c[]);

//Read GPS input and return Altitude in degree form
char* getAlti(char c[]);

//Convert Latitude to decimal form and fit in x-y plane
float ToLatDecimal(char c[], char d);

//Convert Longitude to decimal form and fit in x-y plane
float ToLonDecimal(char c[], char d);

struct location gpsTake();


#endif /* GPSMANAGER_H_ */

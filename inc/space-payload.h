/*
 * =====================================================================================
 *
 *       Filename:  space-payload.h
 *
 *    Description:  Custom functions for use with space-payload.cpp
 *
 *        Version:  1.0
 *        Created:  07/10/2014 07:04:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  AUSTIN HUBBELL
 *   Organization:  Space Concordia
 *
 * =====================================================================================
 */

#ifndef SPACE_PAYLOAD_H
#define SPACE_PAYLOAD_H

#define LOG_PATH "/home/logs"
#define PROCESS "payload"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
//TODO: Include correct I2C libraries
//#include <linux/i2c-dev.h>

using namespace std;

//Prototype all functions here

//Log array of strings using shakespeare
int logToShakespeare (string *peakTime, string *peakVal) {
  //Check for null pointers
  if (peakTime && peakVal) {
    //Check if dimensions of arrays match
    if (sizeof(peakTime) == sizeof(peakVal)) {
      for (int i=0; i<sizeof(peakTime); i++) {
        string dataString = peakTime[i] + peakVal[i];
        //TODO: set log priority to appropriate value
        Shakespeare::log_shorthand(LOG_PATH, Shakespeare::NOTICE, PROCESS, dataString);
      }
      //Sucess
      return 0;
    }
    else {
      //TODO: change to appropriate data error code
      return 2;
    }
  }
  //TODO: change to appropriate null pointer error code
  return 1;
}

//Note: I2C reading/connecting adapted from
// http://elinux.org/Interfacing_with_I2C_Devices#Reading_from_the_ADC

//Connect to I2C bus, return I2C handler
int connectToI2C(int deviceAddress, string i2cBus) {
  return 0;
}

//Returns one one 12-bit reading in the form of a char array
char *readFromI2C (int i2cHandle) {
  return 0;
}

//TODO: Implement check event occurred function
int checkEventOccurred () {

  return 0;
}

//TODO: Implement activate geiger function
int activateGeiger () {
  return 0;
}

#endif /* !SPACE_PAYLOAD_H */

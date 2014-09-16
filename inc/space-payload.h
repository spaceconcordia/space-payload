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
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

//Prototype all functions here

//Function for reading in data from a text file
//Input file is assumed to be in the same directory as this program
string *readBinaryData (string fileName, const long numLines) {
  string data[numLines] = {"\0"};
  string line;
  ifstream dataFile(fileName);
  if(dataFile.is_open()) {
    long count = 0;
    while(getline(dataFile, line)) {
      data[count] = line;
      count += 1;
    }
    dataFile.close();
    return data;
  }
  else {
    //TODO: Correct for proper file error code
    string errorString[1] = {"OPEN_FILE_ERROR"};
    return errorString;
  }
}



//Reshape a one-dimensional array into a two-dimensional array (non-jagged assumed)
string **reshape (string *oneDimData, unsigned long numRows, unsigned long numColumns) {
  unsigned long row = 0;
  unsigned long column = 0;
  string **twoDimData;

  for (int i=0; i<sizeof(oneDimData); i++) {
    if(row < (numRows)) {
      twoDimData[row][column] = oneDimData[i];
      row += 1;
      continue;
    }
    if(row == (numRows-1)) {
      twoDimData[row][column] = oneDimData[i];
      row = 0;
      column += 1;
    }
  }
  return twoDimData;
}

//Log array of strings using shakespeare
int logToShakespeare (string *peakTime, string *peakVal) {
  //Check for null pointers
  if(peakTime && peakVal) {
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
  int file;
  char *filename;
  strcpy(filename, i2cBus.c_str());

  if ((file = open(filename, O_RDONLY)) < 0) {
    //TODO: Produce error code here
    return 1;
  }
  else if (ioct1(file, I2C_SLAVE, deviceAddress)){
    return 2;
  }
  else {
    return file;
  }
}

//Returns one one 12-bit reading in the form of a char array
char *readFromI2C (int i2cHandle) {
  //TODO: 12 chars for buffers?
  char rxBuffer[12] = "\0";
  char txBuffer[12] = "\0";

  if ((i2cHandle == 1) || (i2cHandle == 2) || (read(i2cHandle,rxBuffer,2) != 2)) {
    //TODO: set proper error code for fail to read I2C bus
    rxBuffer[0] = "0x55";
    return rxBuffer;
  }
  else {
    //TODO: Read from I2C and calculate voltage
  }
}

//TODO: Implement check event occurred function
int checkEventOccurred () {

  return 0;
}

//TODO: Implement activate geiger function
int activateGeiger () {

}

#endif /* !SPACE_PAYLOAD_H */

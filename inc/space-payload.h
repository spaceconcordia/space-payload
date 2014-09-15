/*
 * =====================================================================================
 *
 *       Filename:  space-payload.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  07/10/2014 07:04:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
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
string *readBinaryData (string fileName, long numLines) {
  string data[numLines];
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
    return 1;
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





#endif /* !SPACE_PAYLOAD_H */

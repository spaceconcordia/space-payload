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

using namespace std;

//Prototype all functions here

//Function for reading in data from a text file
//Input file is assumed to be in the same directory as this program
unsigned long long *readBinaryData (string fileName, long numLines) {
  unsigned long long data[numLines];
  string line;
  ifstream dataFile(fileName);
  if(dataFile.is_open()) {
    unsigned long count = 0;
    while(getline(dataFile, line)) {
      data[count] = atoi(line.c_str());
    }
    dataFile.close();
  }
  else {
    //TODO: Else condition if file doesn't open (if data is not available?)
  }
  unsigned long long *dataPointer = data;
  return dataPointer;
}

//Reshape a one-dimensional array into a two-dimensional array (non-jagged assumed)
unsigned long long **reshape (unsigned long long *oneDimData, unsigned long numRows, unsigned long numColumns) {
  unsigned long row = 0;
  unsigned long column = 0;
  unsigned long long **twoDimData;

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
  unsigned long long **twoDimDataPointer = twoDimData;
  return twoDimDataPointer;
}

/*

//Finds the maximum value in dataArray, and the corresponding time element
//The elements in "timeArray" must correspond with the values in "dataArray"
float findMaxTimeAndVal[2] (float dataArray[], float timeArray) {
  float maxVal = dataArray[0];
  int maxValTime = timeArray[0];

  for(int i=1; j<dataArray.length; i++) {
    if(peakData[i] > maxVal) {
      maxVal = peakData[i];
      maxValTime = timeArray[j];
    }
  }

  float returnArray[] = {maxValTime, maxVal};

  return returnArray;
}

*/



#endif /* !SPACE_PAYLOAD_H */

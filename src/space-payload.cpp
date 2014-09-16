/*
Consat-1 Payload Simulation Version 5
Author: Austin Hubbell (September 2014)

Note:
-"detection time" is the time between the geiger counter being activated and a
 peak being read in from I2C
-One "measurement" is a single peak value and its detection time

Pseudo-Code:
-Activate geiger counter
-Wait for peak reading from I2C
-Record the peak value and detection time in temporary variable
-Log group of measurements to shakespeare

Questions:

-How many measurements should be taken in between shakespeare logging (NUM_MEASUREMENTS)?
-Time difference is being recorded using clock ticks, but can the date/time be attached to
 each group of measurements from a higher process?
-Will another process determine if Consat-1 is in anomoly and start payload?

*/


#include <iostream>
#include "../inc/space-payload.h"
#include <string.h>
//#include "shakespeare.h"

using namespace std;

#define RAW_PEAKS_FILE_PATH "rawPeaksData1.txt"
#define NUM_MEASUREMENTS 1000 //Number of measurements to take in a row
#define DEAD_TIME_SEC 0.002 //2ms dead time
//TODO: Define proper I2C bus address
#define I2C_BUS_ADDRESS 0x51
//TODO: Define proper Geiger counter device address
#define I2C_DEVICE_ADDRESS 0x99
#define I2C_PACKET_LENGTH 12

int main(int argc, const char * argv[])
{
  //Timing variables
  clock_t start_time;
  clock_t elapsed_time;

  string tempBinaryPeakData[NUM_MEASUREMENTS];
  string tempTimeData[NUM_MEASUREMENTS];

  //Run the payload NUM_MEASUREMENTS times
  for (int i=0; i<NUM_MEASUREMENTS; i++) {
    //Geiger counter "turn on", record time it was "turned on"
    activateGeiger();
    start_time = clock();
    bool eventFlag = false;
    while (eventFlag == false) {
      //Check if an event has occurred
      if (checkEventOccurred() == 0) {
        //Set event flag true to move to next measurement
        eventFlag = true;

        //Once event is detected, determine elapsed time for detection
        elapsed_time = clock() - start_time;

        //Begin 2ms dead time
        start_time = clock();
        while((clock() - start_time) < (DEAD_TIME_SEC*CLOCKS_PER_SEC)) {
          //Do logging in here so this time isn't wasted?
        }
        //Log peak magnitude and detection times to temporary variables
        tempTimeData[i] = ((float)elapsed_time/CLOCKS_PER_SEC).c_str();
        tempBinaryPeakData[i] = readFromI2C(connectToI2C(I2C_DEVICE_ADDRESS, I2C_BUS_ADDRESS));
      }
    }
  }
  //Log all peak magnitude and detection times for all measurements using Shakspeare
  logToShakespeare(tempTimeData, tempBinaryPeakData);

  cout << "Space-Payload Terminated Successfully!\n";
  return 0;
}

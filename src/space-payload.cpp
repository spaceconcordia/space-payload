/*
Consat-1 Payload Simulation Version 5
Author: Austin Hubbell (September 2014)

Note:
-"detection time" is the time between the geiger counter being activated and a
 peak being read in from I2C
-One "measurement" is a single peak value and its detection time
-size_t not unsigned
-Turning on the payload board and G_M counter is GPIO not I2C
-Make I2C read function and place in utls folder in space lib
-Add I2Cio library cpp to space-lib/
-Add date time ot each measurement or group of measurements

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
-List of error codes for entire project needed so new ones can be made spacedecl.h
-Error codes in CPP file

*/


#include <iostream>
#include <SpaceDecl.h>
#include <shakespeare.h>
#include "../inc/space-payload.h"
#include <string.h>

using namespace std;

#define RAW_PEAKS_FILE_PATH "rawPeaksData1.txt"
#define NUM_MEASUREMENTS 1000 //Number of measurements to take in a row
#define DEAD_TIME_SEC 0.002 //2ms dead time
//TODO: Define proper I2C bus address
#define GPIO_BUS_ADDRESS "0x51"
//TODO: Define proper Geiger counter device address
#define ACTIVATE_PAYLOAD_BOARD_GPIO_ADDRESS "0x00"
#define GPIO_PACKET_LENGTH 12

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
    if (activateGeiger() != CS1_SUCCESS) {
      //TODO: Proper error code for geiger counter fail
      cout << "Geiger counter activation error";
      continue;
    }

    start_time = clock();
    bool eventFlag = false;

    while (eventFlag == false) {
      //Check if an event has occurred
      if (checkEventOccurred() == CS1_SUCCESS) {
        //Set event flag true to move to next measurement after loop body runs
        eventFlag = true;

        //Once event is detected, determine elapsed time for detection
        elapsed_time = clock() - start_time;

        //Begin 2ms dead time
        start_time = clock();
        while ((clock() - start_time) < (DEAD_TIME_SEC*CLOCKS_PER_SEC)) {
          //Do logging in here so this time isn't wasted?
        }
        //Log peak magnitude and detection times to temporary variables
        tempTimeData[i] = ((float)elapsed_time/CLOCKS_PER_SEC);

        //Check if GPIO available, if so attempt to read data
        int connectStatus = connectToGPIO();
        if (connectStatus == CS1_SUCCESS) {
          //TODO: Pass tempBinaryPeakData[i] to readFromGPIO()
          int readStatus = readFromGPIO();
          if (readStatus != CS1_SUCCESS) {
            cout << "Failed to read from GPIO";
          }
        }
        else {
          cout << "Failed to connect to GPIO\n";
        }
      }
    }
  }
  //Log all peak magnitude and detection times for all measurements using Shakspeare
  if (logToShakespeare(tempTimeData, tempBinaryPeakData) == 0) {
    cout << "Space-Payload Terminated Successfully!\n";
    return CS1_SUCCESS;
  }
  else {
    cout << "Shakespeare logging error";
    //TODO: Proper shakespeare logging error code
    return 2;
  }
}

//Log array of strings using shakespeare
int logToShakespeare (string *peakTime, string *peakVal) {
  //Check for null pointers
  if (peakTime && peakVal) {
    //Check if dimensions of arrays match
    if (sizeof(peakTime) == sizeof(peakVal)) {
      for (size_t i=0; i<sizeof(peakTime); i++) {
        string dataString = peakTime[i] + peakVal[i];
        //TODO: set log priority to appropriate value
        Shakespeare::log_shorthand(LOG_PATH, Shakespeare::NOTICE, PROCESS, dataString);
      }
      //Sucess
      return CS1_SUCCESS;
    }
    else {
      //TODO: change to appropriate data error code
      return 2;
    }
  }
  //TODO: change to appropriate null pointer error code
  return 1;
}

//Connect to GPIO bus
int connectToGPIO () {

  return CS1_SUCCESS;
}

//Expects one 12-bit reading from GPIO in the form of a char array
int readFromGPIO () {

  return CS1_SUCCESS;
}

//TODO: Implement check event occurred function
int checkEventOccurred () {

  return CS1_SUCCESS;
}

//TODO: Implement activate geiger function
int activateGeiger () {

  return CS1_SUCCESS;
}

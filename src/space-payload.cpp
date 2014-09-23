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
-Will another process determine if Consat-1 is in anomoly and start payload?
-List of error codes for entire project needed so new ones can be made spacedecl.h
-Error codes in CPP file

TODO:
-Implement space-lib/utls/src Date.cpp BuildDateTimePreciseString()
  -Include utls in payload Makefile
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
#define DATE_NUM_CHARS 100

//Payload-specific error codes
#define GEIGER_ACTIVATION_FAIL "0x02"
#define GPIO_READ_FAIL "0x03"
#define GPIO_CONNECT_FAIL "0x04"

//General error codes
#define SHAKESPEARE_LOG_FAIL "0x05"

int main(int argc, const char * argv[])
{
  //Timing variables
  clock_t start_time;
  clock_t elapsed_time;

  char tempBinaryPeakData[NUM_MEASUREMENTS][GPIO_PACKET_LENGTH];
  char tempTimeData[NUM_MEASUREMENTS][DATE_NUM_CHARS];

  //Run the payload NUM_MEASUREMENTS times
  for (int i=0; i<NUM_MEASUREMENTS; i++) {
    //Geiger counter "turn on", record time it was "turned on"
    if (activateGeiger() != CS1_SUCCESS) {
      //Log error code with priority ERROR
      char err[5] = {0};
      strcpy(err, GEIGER_ACTIVATION_FAIL);
      logErrorToShakespeare(err);
      //Move to next measurement attempt
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
        float detectionTime = ((float)elapsed_time/CLOCKS_PER_SEC);
        sprintf(tempTimeData[i], "%f", detectionTime);

        //Check if GPIO available, if so attempt to read data
        int connectStatus = connectToGPIO();
        if (connectStatus == CS1_SUCCESS) {
          //Read frmo GPIO, store results in tempBinaryPeakData
          int readStatus = readFromGPIO(tempBinaryPeakData[i]);
          if (readStatus != CS1_SUCCESS) {
            char err[5] = {0};
            strcpy(err, GPIO_READ_FAIL);
            logErrorToShakespeare(err);
          }
        }
        else {
          char err[5] = {0};
          strcpy(err, GPIO_CONNECT_FAIL);
          logErrorToShakespeare(err);
        }
      }
    }
  }
  //Log all peak magnitude and detection times for all measurements using Shakspeare
  for (int i=0; i<NUM_MEASUREMENTS; i++) {
    char *temp = strcat(tempTimeData[i], " ");
    char *timeAndPeakData = strcat(temp, tempBinaryPeakData[i]);
    if (logNoticeToShakespeare(timeAndPeakData) == CS1_SUCCESS) {
      cout << "Space-Payload Terminated Successfully!\n";
      return CS1_SUCCESS;
    }
    else {
      cout << "Payload Terminated: Logging Error";
    }
  }
}

//Log char array with "NOTICE" priority using shakespeare
int logNoticeToShakespeare (char *data) {
  //Check for null pointer
  if (data) {
    Shakespeare::log_shorthand(LOG_PATH, Shakespeare::NOTICE, PROCESS, data);
    return CS1_SUCCESS;
  }
  else {
    char err[5] = {0};
    strcpy(err, SHAKESPEARE_LOG_FAIL);
    logErrorToShakespeare(err);
    return 2;
  }
}

//Log char array with "NOTICE" priority using shakespeare
int logErrorToShakespeare (char *data) {
  //Check for null pointer
  if (data) {
    Shakespeare::log_shorthand(LOG_PATH, Shakespeare::ERROR, PROCESS, data);
    return CS1_SUCCESS;
  }
  else {
    char err[5] = {0};
    strcpy(err, SHAKESPEARE_LOG_FAIL);
    logErrorToShakespeare(err);
    return 2;
  }
}

//Connect to GPIO bus
int connectToGPIO () {

  return CS1_SUCCESS;
}

//Expects one 12-bit reading from GPIO in the form of a char array
int readFromGPIO (char *data) {

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

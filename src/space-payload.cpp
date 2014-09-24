/*
Consat-1 Payload Software Version 1
Author: Austin Hubbell (September 2014)

Note:
-"detection time" is the time between the geiger counter being activated and a
peak being detected
-One "measurement" is a single peak magnitude and its corresponding detection time
-Turning on the payload board and G_M counter is GPIO not I2C
-Make I2C read function and place in utls folder in space lib
-Add I2Cio library cpp to space-lib/

Pseudo-Code:
1. Activate geiger counter
2. Wait to detect peak
3. Record peak detection time in temporary variable
4. Record peak magnitude in temporary variable
5. Log measurement variables to shakespeare

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
#define SHAKESPEARE_NULL_DATA_FAIL "0x05"
#define SHAKESPEARE_PRIORITY_FORMAT_FAIL "0x06"

int main(int argc, const char * argv[])
{
  //Main peak and detection time variables;
  char binaryPeakMagData[GPIO_PACKET_LENGTH];
  char detectionTimeData[DATE_NUM_CHARS];

  //Geiger counter "turn on", record time it was activated
  if (activateGeiger() != CS1_SUCCESS) {
    //Log error code with priority ERROR
    char err[5] = {0};
    strcpy(err, GEIGER_ACTIVATION_FAIL);
    logToShakespeare(err, "ERROR");
  }
  //If activateGeiger() returns success, start timer
  clock_t start_time = clock();
  bool eventFlag = false;

  //Continue checking for an event until checkEventOccurred() returns success
  while (eventFlag == false) {
    //Check if an event has occurred
    if (checkEventOccurred() == CS1_SUCCESS) {
      //Once event is detected, determine elapsed time for detection
      clock_t elapsed_time = clock() - start_time;

      //Store peak detection time to temporary variable
      float detectionTime = ((float)elapsed_time/CLOCKS_PER_SEC);
      sprintf(detectionTimeData, "%f", detectionTime);

      //Check if GPIO available, if so attempt to read peak magnitude data
      int connectStatus = connectToGPIO();
      if (connectStatus == CS1_SUCCESS) {
        //Read from GPIO, store results in binaryPeakMagData
        int readStatus = readFromGPIO(binaryPeakMagData);
        //If reading from GPIO fails log error
        if (readStatus != CS1_SUCCESS) {
          char err[5] = {0};
          strcpy(err, GPIO_READ_FAIL);
          logToShakespeare(err, "ERROR");
        }
      }
      // If connecting to GPIO fails log error
      else {
        char err[5] = {0};
        strcpy(err, GPIO_CONNECT_FAIL);
        logToShakespeare(err, "ERROR");
      }

      //2ms dead time
      start_time = clock();
      while ((clock() - start_time) < (DEAD_TIME_SEC*CLOCKS_PER_SEC)) {
      }

      //Set event flag true to stop checking for events after loop body runs
      eventFlag = true;
    }
  }

  //Log all peak magnitude and detection times using Shakspeare
    char *temp = strcat(detectionTimeData, " ");
    char *timeAndPeakData = strcat(temp, binaryPeakMagData);
    if (logToShakespeare(timeAndPeakData, "NOTICE") == CS1_SUCCESS) {
      cout << "Space-Payload Terminated Successfully!\n";
      return CS1_SUCCESS;
    }
    else {
      cout << "Payload Terminated: Check log for error";
      return 1;
    }
}

//-------------------------------------Custom Functions--------------------------------------

//Log char array with given priority using shakespeare
int logToShakespeare (char *data, string priority) {
  //Check for null pointer
  if (data) {
    //If the data is flagged as "NOTICE" priority, log it as such
    if (priority.compare("NOTICE") == 0) {
      Shakespeare::log_shorthand(LOG_PATH, Shakespeare::NOTICE, PROCESS, data);
      return CS1_SUCCESS;
    }
    //If the data is flagged as "ERROR" priority, log it as such
    else if (priority.compare("ERROR") == 0) {
      Shakespeare::log_shorthand(LOG_PATH, Shakespeare::ERROR, PROCESS, data);
      return CS1_SUCCESS;
    }
    //If data is neither "NOTICE" nor "ERROR", log a priority formatting error
    else {
      char err[5] = {0};
      strcpy(err, SHAKESPEARE_PRIORITY_FORMAT_FAIL);
      Shakespeare::log_shorthand(LOG_PATH, Shakespeare::ERROR, PROCESS, err);
      return 2;
    }
  }
  //If data is null, log a null data error
  else {
    char err[5] = {0};
    strcpy(err, SHAKESPEARE_NULL_DATA_FAIL);
    Shakespeare::log_shorthand(LOG_PATH, Shakespeare::ERROR, PROCESS, err);
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

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
#define GEIGER_SWITCH_GPIO_ADDRESS "/dev/gpios/consat/GPIO_./value"
#define GPIO_PACKET_LENGTH 12
#define DATE_NUM_CHARS 10

//Payload-specific error codes
#define GEIGER_ACTIVATION_FAIL "2"
#define GPIO_READ_FAIL "3"
#define GPIO_CONNECT_FAIL "4"

//General error codes
#define SHAKESPEARE_NULL_DATA_FAIL "5"
#define SHAKESPEARE_PRIORITY_FORMAT_FAIL "6"
#define NULL_FILE_POINTER_FAIL "7"
#define CLOCK_FUNCTION_FAIL "8"

int main(int argc, const char * argv[])
{
  //Check main function parameters
  if (argc != 1 || (sizeof(argv)/sizeof(*argv)) != 1) {
    cout << "Main function does not take any arugments";
    return 1;
  }

  //Main peak and detection time variables;
  char binaryPeakMagData[GPIO_PACKET_LENGTH];
  char detectionTimeData[DATE_NUM_CHARS];

  //Geiger counter "turn on", record time it was activated
  if (setGeigerState(true, GEIGER_SWITCH_GPIO_ADDRESS) != CS1_SUCCESS) {
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
      snprintf(detectionTimeData, sizeof(float), "%f", detectionTime);

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

      //Turn "off" Geiger counter
      setGeigerState(false, GEIGER_SWITCH_GPIO_ADDRESS);
      //Execute 2ms dead time
      deadTime(DEAD_TIME_SEC);

      //Set event flag true to stop checking for events after loop body runs
      eventFlag = true;
    }
  }

  //Log peak magnitude and detection times using Shakspeare
  char *temp = strcat(detectionTimeData, " ");
  char *timeAndPeakData = strcat(temp, binaryPeakMagData);
  if (logToShakespeare(timeAndPeakData, "NOTICE") == CS1_SUCCESS) {
    cout << "Space-Payload Terminated Successfully!\n";
    return CS1_SUCCESS;
  }
  else {
    cout << "Payload Terminated: Check log for specific error";
    return 1;
  }
}

//-------------------------------------Custom Functions--------------------------------------

//Log char array with given priority using shakespeare
char logToShakespeare (char *data, string priority) {
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
char connectToGPIO () {

  return CS1_SUCCESS;
}

//Expects one 12-bit reading from GPIO in the form of a char array
char readFromGPIO (char *data) {
  //TODO: For debugging purposes only
  string otherVar = data;

  return CS1_SUCCESS;
}

//TODO: Implement check event occurred function
char checkEventOccurred () {

  return CS1_SUCCESS;
}

//Function to turn Geiger GPIO pin on/off
char setGeigerState (bool state, string gpioAddress) {
  //If true, activate Geiger using GPIO
  char pinValue;
  if (state == true) {
    //TODO: Check "on" value is 1
    pinValue = '1';
  }
  else if (state == false) {
    //TODO: Check "off" value is 0
    pinValue = '0';
  }
  FILE* gpioSysFile;
  gpioSysFile=fopen(gpioAddress.c_str(),"w");

  //If file pointer not null, set GPIO to "pinValue"
  if (gpioSysFile!=NULL) {
    fputc(pinValue,gpioSysFile);
    fclose(gpioSysFile);
    return CS1_SUCCESS;
  }
  //If file pointer is null, log erorr to shakespeare and return error code
  else {
    char err[5] = {0};
    strcpy(err, NULL_FILE_POINTER_FAIL);
    Shakespeare::log_shorthand(LOG_PATH, Shakespeare::ERROR, PROCESS, err);
    //Return single char error code from string
    return GPIO_CONNECT_FAIL[0];
  }
}

//Function for executing a certain duration of "dead time" in sec
char deadTime (float sec) {
  if (clock() != -1) {
    time_t start_time = clock();
    while ((clock() - start_time) < (sec*CLOCKS_PER_SEC)) {
      //Do nothing during this itme
    }
    return CS1_SUCCESS;
  }
  else {
    //Return single char error code from string
    return CLOCK_FUNCTION_FAIL[0];
  }
}

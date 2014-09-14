/*

Consat-1 Payload Simulation Version 2
Author: Austin Hubbell (August 2014)

Note:
-One "measurement" is a single peak value
-One "transmission" is a group of peak values sent in between dead times
-The simulation uses one peak per time iteration, so peak frequency is
determined by the number of measurements/transmissions

Pseudo-Code:
-Find the maximum value of a certain group of digital signals
-Record the frequency of events (frequency of the digital signals)
-Log input signal (from amplifier going to geiger counter)
-Logging should occur after each transmission (group of measurements)

Questions:
-The values being read in from Gieger counter, are they 8-byte floats?
-How many measurements will be sent inbetween deadtimes (NUM_MEASUREMENTS)?
-How should the rate of radiation events be determined, is there a
timestamp attached to each data element or should the program request
the current time from the clock and record it? Timestamp
-How to get the current time from onboard RTC?
-What to do if there are two max peaks of the same value in one transmission?

TODO:
-Figure out how to process binary data (how to consider a bitstring as
a number and not an array of characters), easy if conversion is left
out of simulation
-Implement logging using shakespeare
-Add timestamp to each measurement using millisecond precision
stop 2ms

maketime()

check if in anomoly
*/


#include <iostream>
#include <fstream>
#include "../inc/space-payload.h"

using namespace std;

#define LOG_PATH "/home/logs"
#define PROCESS "payload"
#define RAW_PEAKS_FILE_PATH "rawPeaksData1.txt"

int main(int argc, const char * argv[])
{
  //Number of data elements sent in each transmission, total number of transmissions
  const int NUM_MEASUREMENTS = 1000;
  const int NUM_TRANSMISSIONS = 5;


  //TODO: Use I2C to retrieve data and store in binaryPeakData array
  //1-dimensional array storing peak magnitude values
  unsigned long long *binaryPeaks;
  binaryPeaks = readBinaryData(RAW_PEAKS_FILE_PATH, NUM_MEASUREMENTS*NUM_TRANSMISSIONS);
  //2-dimensional array, each column is one "transmission"
  unsigned long long **binaryPeakData = reshape(binaryPeaks, NUM_MEASUREMENTS, NUM_TRANSMISSIONS);

  //Time between geiger counter activation and event detection
  float eventTimes[NUM_MEASUREMENTS][NUM_TRANSMISSIONS] = {0};

  //------------------Payload processing logic begins here----------------------
  //Timing variables
  clock_t start_time;
  clock_t elapsed_time;


  for (int i=0; i<(sizeof(binaryPeakData[0])/sizeof(long)); i++) {
    unsigned long long tempBinaryPeakData[sizeof(binaryPeakData)/sizeof(binaryPeakData[0])];
    float tempTimeData[sizeof(binaryPeakData)/sizeof(binaryPeakData[0])];
    for(int j=0; j<(sizeof(binaryPeakData)/sizeof(binaryPeakData[0])); j++) {
      //TODO: Activate geiger counter

      start_time = clock();

      //TODO: Check if an event has occurred

      //Once event is detected, determine elapsed time for detection
      elapsed_time = clock() - start_time;

      //Begin 2ms dead time
      start_time = clock();
      while((clock() - start_time) < 2e-3*CLOCKS_PER_SEC) {
        //Do logging in here so this time isn't wasted?
      }

      //Log peak magnitude and detection times to temporary variables
      tempTimeData[j] = ((float)elapsed_time)/CLOCKS_PER_SEC;
      tempBinaryPeakData[j] = binaryPeakData[j][i];
    }
    //TODO: Log all data for this transmission using shakespeare
  }

    cout << "Space-Payload Terminated Successfully!\n";
    return 0;
}

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
 -The values being read in from Gieger counter, are they 8-byte doubles?
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
 
*/


#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <array>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

#define RAW_PEAKS_FILE_PATH "file path here"

int main(int argc, const char * argv[])
{
 
    //Number of data elements sent in each transmission, number of transmissions
    //NOTE: These values determine the "length" of the simulation
    const int NUM_MEASUREMENTS = 1000;
    const int NUM_TRANSMISSIONS = 5;
    
    //Creates an array with each column corresponding to one transmission and
    //each row within a colum being one measurement in that transmission
    double analogPeaks[NUM_MEASUREMENTS][NUM_TRANSMISSIONS] = {0};
    
    //TODO: Get peaks data using I2C instead of text file
    string line;
    ifstream rawPeaksFile("rawPeaksData.txt");
    if(rawPeaksFile.is_open()) {
        //TODO: Ensure parsing is correct for the data coming in
        int row = 0;
        int column = 0;
        while(getline(rawPeaksFile, line)) {
            if(row < (NUM_MEASUREMENTS-1)) {
                analogPeaks[row][column] = atof(line);
                row += 1;
                continue;
            }
            if(row == (NUM_MEASUREMENTS-1)) {
                analogPeaks[row][column] = atof(line);
                row = 0;
                column += 1;
            }
        }
        rawPeaksFile.close();
    }
    
    //Fill time array with values (each column corresponds to one "transmission")
    int time[NUM_MEASUREMENTS][NUM_TRANSMISSIONS] = {0};
    int count = 0;
    
    for (int i=0; i<NUM_TRANSMISSIONS; i++) {
        for (int j=0; j<NUM_MEASUREMENTS; j++) {
            time[j][i] = count;
            count += 1;
        }
    }
    
    //Array to store the max value of each transmission
    double maxPeaks[NUM_TRANSMISSIONS][2] = {0};
    //For simplification purposes, the data is assumed to have already been
    //converted from bitstrings to parsed decimal numbers
    for (int i=0;i<NUM_TRANSMISSIONS; i++) {
        //Create array of radiation data with associated timestamps for this
        //specific transmission
        int timeData[NUM_MEASUREMENTS] = {0};
        for (int j=0; j<NUM_MEASUREMENTS; j++) {
            timeData[j] = time[j][i];
        }
        
        double peakData[NUM_MEASUREMENTS] = {0};
        for(int j=0; j<NUM_MEASUREMENTS; j++) {
            peakData[j] = analogPeaks[j][i];
        }
    
        //Compare all elements of peakData to find the max
        //After this is run, the max will be stored in "peakVal"
        double maxPeakVal = peakData[0];
        int maxPeakValTime = timeData[0];
        for (int j=1; j<NUM_MEASUREMENTS; j++) {
            if (peakData[j] > maxPeakVal) {
                maxPeakVal = peakData[j];
                maxPeakValTime = timeData[j];
            }
        }
    
        //Log the peak value and time to the permanent storage array
        //TODO: shakespeare logging here
        maxPeaks[i][0] = maxPeakValTime;
        maxPeaks[i][1] = maxPeakVal;
    
        cout << maxPeakVal << endl;
        cout << maxPeakValTime << endl;
    }
    
    //TODO: Log "analogPeaks" and "maxPeaks" using shakespeare
    
    cout << "Simulation Terminated Successfully!\n";
    return 0;
}


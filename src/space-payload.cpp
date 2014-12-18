#include <stdio.h>
#include <stdlib.h>
#include "SpaceDecl.h"
#include "shakespeare.h"

#define LOG_DIR "/home/logs/"
#define PAYLOAD cs1_systems[CS1_PAYLOAD] // DON'T EVER CHANGE THIS

using namespace std;

int SvetlanasFunction(char * input_data, char * output_data) {
    // does stuff
    return 0;
}

int main()
{
  // main function variables 
  int exitStatus=0;

  short int value = 2;

  string converted_string = atoi(value);

  // write to log via shakespeare
  Shakespeare::log(Shakespeare::NOTICE, PAYLOAD, converted_string);

  // use the Shakespeare::NOTICE argument as the prirotiy when logging binary data (telemetry)

  short int example_binary_value = 3; // you would have calculated this programmatically

  // NOTICE that the second argument is CS1_PAYLOAD, not PAYLOAD. This is because the binnary
  // logging function requires an ID for the subsystem, not the ASCII encoded name. So for all
  // binary logging, use the command as seen below
  Shakespeare::log_bin(Shakespeare::NOTICE, CS1_PAYLOAD, value);

  return exitStatus;
}

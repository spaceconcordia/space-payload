#include "gtest/gtest.h"

class space-payload_Test : public ::testing::Test
{
    protected:
    virtual void SetUp() { }
    int z=0;
};
//++++++++++++++++++++++++++++++++++++++logToShakespeare() Testing++++++++++++++++++++++++++++++++++++++++

// The same process that leads to the final char array (timeAndPeakData) for logging is repeated here,
// assuming proper data sent to binaryPeakMagData and detecionTime, then the logToShakespeare() function is called with
// to log
TEST_F(space-payload_Test, logCorrectDataNoticeShakespeare)
{
    clock_t start_time = clock();
    //The expected data from I2C should be a 12-bit char array
    char binaryPeakMagData[12 + 1] = {0,0,0,0,1,0,1,0,1,0,1,0};
    char detectionTimeData[10];
    //Determine etermine elapsed time for detection
    clock_t elapsed_time = clock() - start_time;

    //Store peak detection time to temporary variable
    float detectionTime = ((float)elapsed_time/CLOCKS_PER_SEC);
    sprintf(detectionTimeData, "%f", detectionTime);

    //Log peak magnitude and detection times using Shakspeare
    char *temp = strcat(detectionTimeData, " ");
    char *timeAndPeakData = strcat(temp, binaryPeakMagData);

    int logReturn = logToShakespeare(timeAndPeakData, "NOTICE");
    ASSERT_EQ(
        CS1_SUCCESS, // expected
        logReturn // actual
    );
}

TEST_F(space-payload_Test, logWrongLengthtDataNoticeShakespeare)
{
    clock_t start_time = clock();
    //The expected data from I2C should be a 12-bit char array
    //This array is of the same length but only contains 11 elements
    char binaryPeakMagData[12 + 1] = {0,0,0,0,1,0,1,0,1,0,1};
    char detectionTimeData[10];
    //Determine etermine elapsed time for detection
    clock_t elapsed_time = clock() - start_time;

    //Store peak detection time to temporary variable
    float detectionTime = ((float)elapsed_time/CLOCKS_PER_SEC);
    sprintf(detectionTimeData, "%f", detectionTime);

    //Log peak magnitude and detection times using Shakspeare
    char *temp = strcat(detectionTimeData, " ");
    char *timeAndPeakData = strcat(temp, binaryPeakMagData);

    int logReturn = logToShakespeare(timeAndPeakData, "NOTICE");
    ASSERT_EQ(
        CS1_SUCCESS, // expected
        logReturn // actual
    );
}

//---------------------------------------------------------------------------------------------------------


//++++++++++++++++++++++++++++++++++setGeigerState() testing ++++++++++++++++++++++++++++++++++++++++++++++

//Function is called to turn "on" Geiger counter GPIO pin at correct GPIO address
TEST_F(space-payload_Test, correctGPIOsetGeigerStateOn)
{
    int setGeigerStateReturn = setGeigerState(true, GEIGER_SWITCH_GPIO_ADDRESS)

    ASSERT_EQ(
        CS1_SUCCESS, // expected
        setGeigerStateReturn // actual
    );
}

//Function is called to turn "off" Geiger counter GPIO pin at correct GPIO address
TEST_F(space-payload_Test, correctGPIOsetGeigerStateOn)
{
    int setGeigerStateReturn = setGeigerState(false, GEIGER_SWITCH_GPIO_ADDRESS)

    ASSERT_EQ(
        CS1_SUCCESS, // expected
        setGeigerStateReturn // actual
    );
}

//Function is called to turn "on" Geiger counter GPIO pin at incorrect GPIO address
TEST_F(space-payload_Test, correctGPIOsetGeigerStateOn)
{
    //Incorrect GPIO address, should be /dev/gpios/consat/GPIO_./value
    string wrongAddress = "/dev/gpios/consat/gpio_./value";

    //Pass incorrect address to setGeigerState() function, record return value
    int geigerStateReturn = setGeigerState(true, wrongAddress);

    ASSERT_EQ(
        GPIO_CONNECT_FAIL, // expected
        geigerStateReturn // actual
    );
}

//Function is called to turn "off" Geiger counter GPIO pin at incorrect GPIO address
TEST_F(space-payload_Test, correctGPIOsetGeigerStateOn)
{
    //Incorrect GPIO address, should be /dev/gpios/consat/GPIO_./value
    string wrongAddress = "/dev/gpios/consat/gpio_./value";

    //Pass incorrect address to setGeigerState() function, record return value
    int geigerStateReturn = setGeigerState(false, wrongAddress);

    ASSERT_EQ(
        GPIO_CONNECT_FAIL, // expected
        geigerStateReturn // actual
    );
}

//---------------------------------------------------------------------------------------------------------

/*
 * =====================================================================================
 *
 *       Filename:  space-payload.h
 *
 *    Description:  Custom functions for use with space-payload.cpp
 *
 *        Version:  1.0
 *        Created:  07/10/2014 07:04:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  AUSTIN HUBBELL
 *   Organization:  Space Concordia
 *
 * =====================================================================================
 */

#ifndef SPACE_PAYLOAD_H
#define SPACE_PAYLOAD_H

#define LOG_PATH "/home/logs"
#define PROCESS "payload"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

//Log char array with specified priority using shakespeare
char logToShakespeare (char *data, string priority);

//Execute specified duration of dead time
char deadTime (float sec);

//Check if GPIO bus available, if so connect to it
//TODO: is this necessary?
char connectToGPIO ();

//Returns one one 12-bit reading in the form of a char array
char readFromGPIO (char *data);

//Checks if an event has been detected
char checkEventOccurred ();

//Turns "on" the Geiger counter
char setGeigerState (bool state, string gpioAddress);

#endif /* !SPACE_PAYLOAD_H */

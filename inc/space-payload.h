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
//TODO: Include correct I2C libraries
//#include <linux/i2c-dev.h>

using namespace std;

//Prototype all functions here

//Log char array with priority NOTICE  using shakespeare
int logNoticeToShakespeare (char *data);

//Log char array with priority ERROR  using shakespeare
int logErrorToShakespeare (char *data);

//Check if GPIO bus available, if so connect to it
//TODO: is this necessary?
int connectToGPIO ();

//Returns one one 12-bit reading in the form of a char array
int readFromGPIO (char *data);

//TODO: Implement check event occurred function
int checkEventOccurred ();

//TODO: Implement activate geiger function
int activateGeiger ();

#endif /* !SPACE_PAYLOAD_H */

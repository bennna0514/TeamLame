/*
 * commandParser.c - This file contains
 * the implementation of commandParser
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for tolower

#include "TCB.h"
#include "dataStructs.h"
#include "startup.h"
#include "scheduler.h"
#include "satComsParse.h"

extern bool warningBattTemp;

// CommandParser specific globals
extern char ack[3];

extern TaskQueue queue;
extern bool commandOn;
extern bool display;
/*
@DANIEL: thruster command is already in the shared variables
// extern unsigned int thrusterCommand;
*/

#ifdef WHEN_YOURE_READY 
// Initializes Hardware components. True if successful, false otherwise.
bool InitHardware();
// Terminates hardware components. True if successful, false otherwise.
bool CloseHardware();
// Adds measurement tasks. True if successful, false otherwise.
bool AddMeasureTasks();
// Removes measurement tasks. True if successful, false otherwise.
bool RemoveMeasureTasks();

// Returns true if the payload is valid for either thruster command
// or measurement, and false otherwise.
bool isValidPayload(char *payload);
#endif

void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;

    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}

void commandParser(void *cmdStruct) {
    // 1. Assign the data of cData into local variables
    cmdData *cData = (cmdData*)cmdStruct;
    char *received = cData->received;
    char *transmit = cData->transmit;
	unsigned int *thrusterCommand = cData->thrusterCommandPtr;
	bool *commandOn = cData->commandOnPtr;
	
    // 2. Parse the input 
	// @DANIEL We can format it as F12345, without any spaces?
    char cmd = tolower(received[0]);  // e.g. 'M', 'T', 'D', etc.
    char *payload = &received[1];  // e.g. '12345', 'F' (for fuel level), etc
	
	printf("cmd = %c, payload = %s\n", cmd, payload);
	#ifdef WHEN_YOURE_READY	
	switch(cmd) {
      case 't':
        // Thruster Command!
        if (isValidPayload(payload)) {
          ack[0] = 'A';
          maskBit(&thrusterCommand);
        } else {
          ack[0] = 'E';
        }
        ack[2] = 'T';
        *transmit = '\0';
        break;
      case 'm':
        // Measurement Command!
        ack[0] = isValidPayload(payload) ? 'A' : 'E';
        ack[2] = 'M';
        *transmit = *payload;  // e.g. 'F' (fuel level), 'B' (battery level), etc
        break;
      case 's':
        // Start Command!
        ack[0] = (InitHardware() && AddMeasureTasks()) ? 'A' : 'E';
            // Attempt to initialize the PWM + ADC and add measurement tasks.
            // 'A' if successful, 'E' otherwise.
        ack[2] = 'S';
        *transmit = '\0';
        break;
      case 'p':
        // Pause Command!
        ack[0] = (CloseHardware() && RemoveMeasureTasks()) ? 'A' : 'E';
          // Attempt to terminate the PWM + ADC and remove measurement tasks.
          // 'A' if successful, 'E' otherwise.
        ack[2] = 'P';
        *transmit = '\0';
        break;
      case 'd':
        // Display Command
        ack[0] = 'A';  // always successful (?)
        ack[2] = 'D';
        *transmit = '\0';
        display = !display;  // see extern above
        break;
      default:
        // Bad command (e.g. user enters 'Z' <something>)
        ack[0] = 'E';
        ack[2] = 'X';
        *transmit = '\0';
        break;
    }
	#endif
    *commandOn = false;
	
  }

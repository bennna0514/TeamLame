/*
 * satelliteComs.c - This file contains
 * the implementation of satelliteComs
 * (see satelliteComs.h for specification)
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
#include <time.h>

#include "TCB.h"
#include "dataStructs.h"
#include "satelliteComs.h"
#include "nonBlockingKeys.h"
#include "satelliteVehicle.h"
#include "satComsParse.h"

#define MAX 65536 // upper bound for 16 bit
#define BUF_SIZE 16
#define CMD_SIZE 20

// from warningAlarm
extern bool warningBattTemp;
extern bool everythingsMELTING;

// from vehicleComs
extern bool getFrequencies;

void satelliteComs(void *satStruct) {
	/*
	// Only runs this function every global cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	*/
    // 1. Assign the data of sData into local variables
    satData *sData = (satData*)satStruct;
    bool *fuelLow = sData->fuelLowPtr;
    bool *batteryLow = sData->batteryLowPtr;
    bool *solarPanelState = sData->solarPanelStatePtr;
    int *processImage = sData->processImagePtr;
    unsigned int *batteryLvl = sData->batteryLvlPtr;
    unsigned short *fuelLvl = sData->fuelLvlPtr;
    unsigned short *pConsume = sData->pConsumePtr;
    unsigned int *thrusterCommand = sData->thrusterCommandPtr;
    char *command = sData->commandPtr;
    char *response = sData->responsePtr;
    unsigned int *distance = sData->distancePtr;	
    unsigned int *batteryTmp1 = sData->batteryTmp1;
    unsigned int *batteryTmp2 = sData->batteryTmp2;
    char *received = sData->received;
    char *transmit = sData->transmit;
	bool *commandOn = sData->commandOnPtr;
	unsigned int *pirateDistance = sData->pirateDistancePtr;
    char *ack = sData->ack;	

    // 2. Opens psuedoterminal for earth display terminal
	static int fd1;
	static int firstTime = 1;
	if (firstTime == 1) {
        fd1 = open("/dev/pts/1", O_WRONLY);
	    if (!fd1) {
		    fprintf(stderr, "SATCOMS: couldn't open fd1 earth terminal\n");
		    exit(EXIT_FAILURE);
	    }
		dprintf(fd1, "\033[2J"); // clears display
		dprintf(fd1, "\033[1;1H"); //sets cursor to top left
	    firstTime--;
	    dprintf(fd1, "\e[?25l"); // hides cursor
		
	}
	
	// 3. Reads user input
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	char pString[CMD_SIZE];
    pString[0] = '\0';
	#ifdef TEST
    static int call = 0;
	printf("call = %d\n", call);
	if (call == 0) {
		pString[0] = 'S';
			call++;
		
	} 
	printf("pString: %s\n", pString);
	#endif
    if(fgets(pString, CMD_SIZE, stdin) != NULL) {
        // remove newline
        pString[strcspn(pString, "\n")] = 0;	
        if (validCmd(pString[0])) {
		  strncpy(received, pString, CMD_SIZE);
		  *commandOn = true; // Appends command parser next cycle
        } else if (strstr(pString, "clear")) {
		    printf("\033[2J");
		    printf("\033[1;1H");
		} else {
		    *received = '\0';
			if (checkAll(pString[0])) {
				int i;
				for (i = strlen(pString); i >= 0; i--) {
				  ungetc(pString[i], stdin);
				}
			} else {
				*transmit = 'E';
				printf("Transmit: %c\n", *transmit);
			}
        }  
    }
    // 3. Writes necessary information to terminal
    clearScreen(fd1, *transmit);
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    char *fuelString = (*fuelLow)?        RED"YES" RST:GRN"NO"RST;
    char *battString = (*batteryLow)?     RED"YES" RST:GRN"NO"RST;
    char *tempString = (warningBattTemp)? RED"HOT!"RST:GRN"OK"RST;
	
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    dprintf(fd1, "\033[1;1H");
    dprintf(fd1, BOLD"EARTH REMOTE TERMINAL\n");
    dprintf(fd1, "%d-%d-%d %d:%d:%2d\n", 
	             tm.tm_year + 1950, 
				 tm.tm_mon + 1, 
				 tm.tm_mday, 
				 tm.tm_hour, 
				 tm.tm_min, 
				 tm.tm_sec);
    dprintf(fd1, "Operator:          Xxpu$$y$layer69xX"RST"\n\n");	
    dprintf(fd1, "Battery Low:       %3s     \n"
	             "Fuel Low:          %3s     \n"
    	         "Battery Over Temp: %4s     \n", 
				 battString, fuelString, tempString);
	
    // 4. Checks if measurements are active, if not, returns	
	if (*transmit == 'Q') {
		dprintf(fd1, YEL"Measurements are currently inactive. Press %c to activate.\n"RST, START);
		return;
	}
	
	// 5.1 Checks if in stage 2 of overheating, flashes terminal window red and yellow
	if (everythingsMELTING) {
	    static int flash = 0;
		if (flash == 1) {
		    printf(RED_BG"\e[H\e[2J");
		} else {
			printf(YEL_BG"\e[H\e[2J");
		}
	    printf(BLK"Lmao whatever dude. The satellite's melting now. Feel free to press 'a'.\n"RST);		
		flash = 1 - flash;
	// 5.2 Checks if in stage 1 of overheating, colors terminal window yellow
	} else if (warningBattTemp) {
		printf(YEL_BG"\e[H\e[2J");
		printf(BLK"Battery overheating! Press 'a' to acknowledge!\n"RST);
	}
	
	// 6. Checks and prints what commandParser sends back
	if (strstr(ack, "A") || strstr(ack, "E")) {
		dprintf(fd1, BOLD"\nReceived from Parser: %s\nNow displaying....:\n"RST, ack);
	}
	
	// 7. Checks and prints to terminal the information requested by user
    int i;	
	switch(*transmit) {	
		case SHOW_FUEL:
		    if (*fuelLvl > 50) {
			    dprintf(fd1, "Fuel Level: "GRN"%3hu      \n"RST, *fuelLvl);
			} else if (*fuelLvl < 10) {
			    dprintf(fd1, "Fuel Level: "RED"%3hu      \n"RST, *fuelLvl);				
			} else {
			    dprintf(fd1, "Fuel Level: "YEL"%3hu      \n"RST, *fuelLvl);
			}
			break;
		case SHOW_BATT:
		    if (*batteryLvl > 18) {
			    dprintf(fd1, "Battery Level: "GRN"%3hu     \n"RST, *batteryLvl);
			} else if (*batteryLvl < 4) {
			    dprintf(fd1, "Battery Level: "RED"%3hu     \n"RST, *batteryLvl);				
			} else {
			    dprintf(fd1, "Battery Level: "YEL"%3hu     \n"RST, *batteryLvl);
			}		
			break;
        case SHOW_PCON:
		    dprintf(fd1, "Power Consumed: %2hu  \n", *pConsume);
            break;
		case SHOW_TEMP:		
		    dprintf(fd1, "Battery Temp 1: %2d    \n"
			             "Battery Temp 2: %2d    \n", 
						 *batteryTmp1, *batteryTmp2);
			break;					
		case SHOW_PANEL:
		    dprintf(fd1, "Solar Panels: %9s\n", solarPanelString);
			break;			
		case SHOW_DIST:
		    if (*distance < 500) {
		        dprintf(fd1, "Transport Distance: "RED"%4d\n"RST, *distance);
			} else {
		        dprintf(fd1, "Transport Distance: %4d\n", *distance);				
			}
			break;			
		case SHOW_IMAG:
		    if (!getFrequencies) {
		        dprintf(fd1, YEL"No image captured\n"RST);
				
			} else {
				dprintf(fd1, "Image Frequencies:\n");		
				for (i = 0; i < 16; i++) {
					dprintf(fd1, "%5hu ", processImage[i]);
					if ((i+1) % 4 == 0) {
						dprintf(fd1, "\n");
					}
				}
			}
			break;			
		case SHOW_PIRATE:
		    if (*pirateDistance < 5) {
		        dprintf(fd1, "Pirate Distance: "RED"%4hu\n"RST, *pirateDistance);
			} else if (*pirateDistance < 30) {
		        dprintf(fd1, "Pirate Distance: "YEL"%4hu\n"RST, *pirateDistance);				
			} else {
		        dprintf(fd1, "Pirate Distance: "WHT"%4hu\n"RST, *pirateDistance);								
			}
			break;			
	}
	
	// 8. Prints vehicle response 
	if (strstr(response, "A")) {  
	    dprintf(fd1, "\nVehicle Response: %c %c\n", *response, *command);
    }

}

// Clears screen (but only for a different input)
void clearScreen(int fd1, char c){
	static char prev;
	if (prev != c) {
        dprintf(fd1, "\033[2J");
        dprintf(fd1, "\033[1;1H");
	    prev = c;
	}
}

// Checks if the command is a valid "received" command for commandParser
bool validCmd(char c) {
	return c == START ||
	       c == STOP ||
		   c == DISPLAY ||
		   c == THRUSTER ||
		   c == MEASURE;
}
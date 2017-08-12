#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdbool.h>
#include "nonBlockingKeys.h"
#include "scheduler.h"
#include "startup.h"

bool checkAll(char c) {
	return satVehicleCmd(c) ||
	       consoleModeCmd(c)||
		   motorSpeedCmd(c) ||
		   warningCmd(c) ||
		   payloadCmd(c);
	
}

bool satVehicleCmd(char c) {
	return c == FORWARD         || 
	       c == BACK            || 
		   c == LEFT            || 
		   c == RIGHT           || 
		   c == DRILL_DOWN      || 
		   c == DRILL_UP        ||
		   c == START_IMAGE     ||
		   c == SEND_IMAGE;
}

bool consoleModeCmd(char c) {
	return c == SATELLITESTATUS || 
	       c == ANNUNCIATION;
}

extern TaskQueue queue;
extern TCB solarPanelControlTCB;

bool motorSpeedCmd(char c) {
        //unsigned int taskQueue = NumTasksInTaskQueue(queue) + 1;
	if (ContainsTCB(queue, &solarPanelControlTCB)) {
		return c == SPEEDINC ||
		       c == SPEEDDEC;
	}
	return false;
        /* 
	return c == SPEEDINC ||
	       c == SPEEDDEC;
        */
}

bool warningCmd(char c) {
	return c == OVERHEAT_ACK;
}

bool payloadCmd(char c) {
	return c == THRUSTER ||
	       c == MEASURE;
	
}

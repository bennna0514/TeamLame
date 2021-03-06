#ifndef NONBLOCK
#define NONBLOCK
#define NB_DISABLE 1
#define NB_ENABLE 0

// commands for consoleDisplay
#define SATELLITESTATUS 'z'
#define ANNUNCIATION 'x'

// commands for vehicleComs
#define FORWARD 'F'
#define BACK 'B'
#define LEFT 'L'
#define RIGHT 'R'
#define DRILL_DOWN 'Y' // changed from D to avoid conflict
#define DRILL_UP 'H'
#define START_IMAGE 'U' // changed from S to avoid conflict with START
#define SEND_IMAGE 'I'

// commands for warningAlarm
#define OVERHEAT_ACK 'a'

// commands for keyboardConsole
#define SPEEDINC 'i'
#define SPEEDDEC 'd'

// commands for Earth to Satellite
#define START 'S'
#define STOP 'P'
#define DISPLAY 'D'
#define THRUSTER 'T'
#define MEASURE 'M'

// commands for pirates
#define PHASORS_BITCH 'p'
#define PHOTONS_BITCH 'o'

// miscellaneous commands
#define CLEAR 'c'

/*
  @param satelliteComs
    Struct containing necessary
    data for the satelliteComs
  @modifies
    *sData.thrusterCommand
  @effects
    changes thrusterCommand based
    on random integer from rand2.c
*/

/*
  @param thrusterCommand
    16-bit instruction of the form:
    [X X X X X X X] [Y Y Y Y] [Z Z Z Z]
       duration     magnitude direction
  @modifies
    *thrusterCommand
  @effects
    Masks the last 4 bits to be 00XX
*/

bool consoleModeCmd(char c);
bool satVehicleCmd(char c);
bool motorSpeedCmd(char c);
bool warningCmd(char c);
bool payloadCmd(char c);
bool piratesCmd(char c);
bool checkAll(char c);
bool miscCmd(char c);


#endif

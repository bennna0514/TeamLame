// Structs associated with each task hold the shared data
#ifndef DATASTRUCTS
#define DATASTRUCTS
#include "Bool.h"  // need to include Bool.h for the Bool type

struct powerSubsystemData 
{
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
};
typedef struct powerSubsystemData powerData;


struct thrusterSubsystemData
{
  unsigned int *thrusterCommandPtr;
  unsigned short *fuelLvlPtr;
};
typedef struct thrusterSubsystemData thrustData;

struct satelliteComsData
{
  Bool *fuelLowPtr;
  Bool *batteryLowPtr;
  Bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
  unsigned int *thrusterCommandPtr;
};
typedef struct satelliteComsData satData;

struct consoleDisplayData
{
  Bool *fuelLowPtr;
  Bool *batteryLowPtr;
  Bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
};
typedef struct consoleDisplayData consoleData;

struct warningAlarmData
{
  Bool *fuelLowPtr;
  Bool *batteryLowPtr;
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
};
typedef struct warningAlarmData warnData;

#endif
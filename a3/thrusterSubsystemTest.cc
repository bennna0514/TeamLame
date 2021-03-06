/*

  thrusterSubsystemTest - This file contains
  unit tests for the thrusterSubsystem.

*/

#include "gtest/gtest.h"
extern "C" {
    #include <stdint.h>
    #include "./thrusterSubsystem.h"
    #include "dataStructs.h"
}

// This tests the "parseCommands" method in "thrusterSubsystem.c"
TEST(ThrusterSubsystemTest, Test_ParseCommands) {
  // 0. Declare test variables
  uint8_t left = 0x00;
  uint8_t up = 0x02;
  uint8_t down = 0x03;
  uint8_t thrust_dir[3] = {left, up, down};

  uint8_t off = 0x00;
  uint8_t twenty_prct = 0x03;
  uint8_t max = 0x0F;
  uint8_t magnitude[3] = {off, twenty_prct, max};

  uint8_t zero_sec = 0x00;
  uint8_t oneeighty_sec  = 0xB4;
  uint8_t twofiftyfive_sec = 0xFF;
  uint8_t duration[3] = {zero_sec, oneeighty_sec, twofiftyfive_sec};

  // 1. Construct thrust commands from
  // all combinations of test variables
  unsigned int thrusterCommands[3][3][3];
  int i, j, k;
  for(i = 0; i < 3; i++) {
    uint8_t dir = thrust_dir[i];
    for(j = 0; j < 3; j++) {
      uint8_t mag = magnitude[j];
      for(k = 0; k < 3; k++) {
        uint8_t dur = duration[k];
        unsigned int thrusterCommand = (dur << 8)|(mag << 4)|(dir);
        thrusterCommands[i][j][k] = thrusterCommand;
      }
    }
  }

  // 2. Cycle through all thrusterCommands and
  // compare to returned value.
  cleanCommands cc;
  for(i = 0; i < 3; i++) {
    uint8_t dir = thrust_dir[i];
    for(j = 0; j < 3; j++) {
      uint8_t mag = magnitude[j];
      for(k = 0; k < 3; k++) {
        uint8_t dur = duration[k];
        unsigned int thrusterCommand = thrusterCommands[i][j][k];
        parseCommands(&thrusterCommand, &cc);
        ASSERT_EQ(cc.duration, dur);
        ASSERT_EQ(cc.thruster_dir, dir);
        ASSERT_EQ(cc.magnitude, mag);
      }
    }
  }
}

// This tests the "thrusterSubsystem" method in "thrusterSubsystem.c"
TEST(ThrusterSubsystemTest, Test_ThrusterSubsystem) {
  // 1. Set up data struct
  unsigned int thrusterCommand = 0xFFF0;
  unsigned short fuelLvl = 100;
  thrustData tData = {&thrusterCommand, &fuelLvl};
  void *tDataPtr = (void*)&tData;
  // 2. Store absolute cost for each iteration
  double actual_cost = 0.49132966499999997;
  double actual_fuelLvl = 100.00;
  // 3. Store the difference between the
  // rounded and actual cost.
  double diff, fuelAsDouble;
  while(fuelLvl > 0) {
    thrusterSubsystem(tDataPtr);
    fuelAsDouble = (double)fuelLvl;
    diff = fuelAsDouble - actual_fuelLvl;
    diff = diff < 0 ? diff * -1 : diff;
    ASSERT_TRUE(diff <= 0.51);
    actual_fuelLvl -= actual_cost;
  }
}

/*

  powerSubsystemTest - This file contains
  unit tests for the powerSubsystem.

*/

#include "gtest/gtest.h"
extern "C" {
    #include "./powerSubsystem.h"
}


// This tests the "powerConsumption" method in "powerSubsystem"
TEST(PowerSubsystemTest, Test_PowerConsumption) {

  // Convenience variables
  unsigned short curr_pConsume = 0;
  unsigned short *curr_pConsumePtr = &curr_pConsume;
  unsigned short old_pConsume = *curr_pConsumePtr;
  int numCall;

  // Execute call 1
  powerConsumption(curr_pConsumePtr);

  for(numCall = 1; numCall < 40; numCall++) {
    // Case 1: If pConsume <= 10 (w/o double reverse)
    // or pConsume < 5 (w/ double reverse)
    if(numCall < 20 || numCall >= 29) {
      // Case 1.1: If call == even -> +2
      if(numCall % 2 == 0) {
        ASSERT_EQ(old_pConsume + 2, *curr_pConsumePtr);
      }
      // Case 1.2: If call == odd -> -1
      else {
        ASSERT_EQ(old_pConsume - 1, *curr_pConsumePtr);
      }
    }
    // Case 2: If pConsume > 10 (11) (w/ reverse)
    else if(numCall >= 20 && numCall < 29){
      // Case 2.1: If call == even -> -2
      if(numCall % 2 == 0) {
        ASSERT_EQ(old_pConsume - 2, *curr_pConsumePtr);
      }
      // Case 2.2: If call == odd -> +1
      else {
        ASSERT_EQ(old_pConsume + 1, *curr_pConsumePtr);
      }
    }
    powerConsumption(curr_pConsumePtr);
    old_pConsume = *curr_pConsumePtr;
  }

}

// This tests the "powerGeneration" method in "powerSubsystem"
TEST(PowerSubsystemTest, Test_PowerGeneration) {

  // NOTE: Assumes powerConsumption is always 0.

  // Convenience variables
  unsigned short curr_pGenerate = 0;
  unsigned short *curr_pGeneratePtr = &curr_pGenerate;
  unsigned short old_pGenerate = *curr_pGeneratePtr;
  unsigned short curr_batteryLvl = 0;
  unsigned short *curr_batteryLvlPtr = &curr_batteryLvl;

  int numCall;

  // Execute call 1
  powerGeneration(curr_pGeneratePtr, curr_batteryLvlPtr);

  for(numCall = 1; numCall < 80; numCall++) {
    // Case 1: batteryLvl <= 50%
    if(numCall < 35) {
      // Case 1.1: If call == even -> +2
      if(numCall % 2 == 0) {
        ASSERT_EQ(old_pGenerate + 2, *curr_pGeneratePtr);
      }
      // Case 1.2: If call == odd -> +1
      else {
        ASSERT_EQ(old_pGenerate + 1, *curr_pGeneratePtr);
      }
    }
    // Case 2: 50% < batteryLvl <= 95%
    if(numCall < 75 && numCall >= 35) {
      // Case 1.1: If call == even -> +2
      if(numCall % 2 == 0) {
        ASSERT_EQ(old_pGenerate + 2, *curr_pGeneratePtr);
      }
      // Case 1.2: If call == odd -> +0
      else {
        ASSERT_EQ(old_pGenerate, *curr_pGeneratePtr);
      }
    }
    // Case 3: batteryLvl > 95%
    if(numCall >= 75) {
      ASSERT_EQ(old_pGenerate, *curr_pGeneratePtr);
      *curr_batteryLvlPtr -= *curr_pGeneratePtr;
    }
    // Add pGenerate to batteryLvl
    *curr_batteryLvlPtr += *curr_pGeneratePtr;
    old_pGenerate = *curr_pGeneratePtr;

    ASSERT_TRUE(*curr_batteryLvlPtr < 100);

    powerGeneration(curr_pGeneratePtr, curr_batteryLvlPtr);

  }

}
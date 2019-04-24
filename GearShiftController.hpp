#pragma once

#include <stdint.h>
#include <Arduino.h>

#include "SharedDefinitions.hpp"
#include "SharedStructDefinitions.hpp"

#include <Servo.h>
#include <string.h>

#define START_GEAR 1

#define GEAR_1_DOWN 14 
#define GEAR_2_DOWN 32 
#define GEAR_3_DOWN 44
#define GEAR_4_DOWN 54
#define GEAR_5_DOWN 60

#define GEAR_1_UP 14
#define GEAR_2_UP 30 
#define GEAR_3_UP 37
#define GEAR_4_UP 49
#define GEAR_5_UP 60

#define LINEAR_ACTUATOR_PIN 13

#define SDRD_SHIFT_DELAY 5000

class GearShiftController {
  private:
    unsigned long lastShiftTime;
  
    Servo linAct;
    uint8_t gearArrUp[5]   = {GEAR_1_UP, GEAR_2_UP, GEAR_3_UP, GEAR_4_UP, GEAR_5_UP};
    uint8_t gearArrDown[5] = {GEAR_1_DOWN, GEAR_2_DOWN, GEAR_3_DOWN, GEAR_4_DOWN, GEAR_5_DOWN};
  
    uint8_t currentGear;
    
    uint8_t setGear(uint8_t gear) {
      Serial.println("setGear called");
      if (millis() < lastShiftTime + SDRD_SHIFT_DELAY) {
        return currentGear;
      }
      
      if (gear < currentGear) {
        linAct.write(gearArrUp[gear-1]);
        lastShiftTime = millis();        
      } else if (gear > currentGear) {
        linAct.write(gearArrDown[gear-1]);
        lastShiftTime = millis();        
      }
      
      currentGear = gear;
	  
	  return currentGear;
    }
  public:
    void updateShiftPoints(ShiftPointArr shiftPointArr) {
      memcpy(gearArrUp,   shiftPointArr.gearArrUp,   sizeof(gearArrUp));
      memcpy(gearArrDown, shiftPointArr.gearArrDown, sizeof(gearArrUp));
    }
    
    uint8_t getGear() {
      return currentGear;
    }
    uint8_t shiftUp() {
      if (currentGear >= MAX_GEAR) {
        return setGear(MAX_GEAR);
      } else {        
        return setGear(currentGear+1);
      }
    }
    uint8_t shiftDown() {
      if (currentGear <= MIN_GEAR) {
        return setGear(MIN_GEAR);
      } else {        ;
        return setGear(currentGear-1);
      }  
    }
    GearShiftController() {
      //currentGear = START_GEAR;
    }
    void setup() {
     currentGear = START_GEAR;
     lastShiftTime = millis();
     linAct.attach(LINEAR_ACTUATOR_PIN, 1000, 2000);
     linAct.write(gearArrUp[START_GEAR-1]);
    }
    
};

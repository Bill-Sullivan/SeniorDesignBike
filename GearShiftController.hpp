#pragma once

#include <stdint.h>
#include <Arduino.h>

#include <Servo.h>
#include <string.h>

#define UP_PIN   2
#define DOWN_PIN 3

#define MAX_GEAR 5
#define MIN_GEAR 1

#define START_GEAR 1

#define GEAR_1_DOWN 14 
#define GEAR_2_DOWN 32 
#define GEAR_3_DOWN 44
#define GEAR_4_DOWN 58
#define GEAR_5_DOWN 82


#define GEAR_1_UP 14
#define GEAR_2_UP 30 
#define GEAR_3_UP 37
#define GEAR_4_UP 48
#define GEAR_5_UP 82


class GearShiftController {
  private:
    Servo linAct;
    int gearArrUp[5]   = {GEAR_1_UP, GEAR_2_UP, GEAR_3_UP, GEAR_4_UP, GEAR_5_UP};
    int gearArrDown[5] = {GEAR_1_DOWN, GEAR_2_DOWN, GEAR_3_DOWN, GEAR_4_DOWN, GEAR_5_DOWN};
  
    uint8_t currentGear;
    
    uint8_t setGear(uint8_t gear) {
      if (gear < currentGear) {
        linAct.write(gearArrUp[gear-1]);
      } else if (gear > currentGear) {
        linAct.write(gearArrDown[gear-1]);
      }
      
      currentGear = gear;	  
      Serial.print("Gear: ");
      Serial.println(gear);
	  
	  return currentGear;
    }
  public:
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
     linAct.attach(13, 1000, 2000);
     linAct.write(gearArrUp[START_GEAR-1]); 
    }
    
};

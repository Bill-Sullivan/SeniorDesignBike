#pragma once

#include <stdint.h>
#include <Arduino.h>

#include "CentralObject.hpp"
#include "SharedEnums.hpp"

#define SDRD_SHIFT_RANGE 5

#define HIGH_TORQUE_SHIFT_THRESHHOLD 6800
#define LOW_TORQUE_SHIFT_THRESHHOLD  6300


class ShiftLogic {
  protected:
  CentralObject* centralObject;
  public:

  ShiftLogic(CentralObject* _centralObject) {
    centralObject = _centralObject;
  }
  
  void classMain() {
    if (centralObject->getMode() == automatic) { 
      double cadence = centralObject->getCadence();
      double targetCadence = centralObject->getTargetCadence();

      #if defined(STRICT_SDRD_ADHERENCE)
      if (cadence  > targetCadence + SDRD_SHIFT_RANGE and centralObject->getCadence() > 0 ) {        
        centralObject->setGear(centralObject->getGear() + 1);        
      } else if (cadence < targetCadence - SDRD_SHIFT_RANGE and centralObject->getCadence() > 0) {
        centralObject->setGear(centralObject->getGear() - 1);
      }
      #else
      uint16_t torque = centralObject->getTorque();
      if (cadence > 0.0) {
        if (torque < LOW_TORQUE_SHIFT_THRESHHOLD) {
          if (cadence < targetCadence + SDRD_SHIFT_RANGE) {
            centralObject->shiftDown();
          } else {
            centralObject->shiftUp();
          }
        } else if (torque > HIGH_TORQUE_SHIFT_THRESHHOLD) {
          if (cadence > targetCadence - SDRD_SHIFT_RANGE) {
            centralObject->shiftUp();
          }
        } else if (cadence  > targetCadence + SDRD_SHIFT_RANGE) {
          centralObject->shiftUp();
        } else if (cadence < targetCadence - SDRD_SHIFT_RANGE) {
          centralObject->shiftDown();
        }
      }      
      #endif      
    }
    
    else if (centralObject->getMode() == manual) {
       
    }
    delay(500);
    
  }
};

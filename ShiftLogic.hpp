#pragma once

#include <stdint.h>
#include <Arduino.h>

#include "CentralObject.hpp"
#include "SharedEnums.hpp"

#define SDRD_SHIFT_RANGE 5

class CentralObject;

class ShiftLogic {
  protected:
  static CentralObject* centralObject;
  public:
  static void classMain() {
    if (centralObject->getMode() == automatic) {
      if (centralObject->getCadence() > centralObject->getTargetCadence() + SDRD_SHIFT_RANGE) {
        centralObject->setGear(centralObject->getGear() + 1);
      } else if (centralObject->getCadence() < centralObject->getTargetCadence() - SDRD_SHIFT_RANGE) {
        centralObject->setGear(centralObject->getGear() - 1);
      }
    } 
    
    else if (centralObject->getMode() == manual) {
      delay(500);
    }
    
  }
};

#pragma once

#include <stdint.h>
#include <Arduino.h>

#include "CentralObject.hpp"
#include "SharedEnums.hpp"

#define SDRD_SHIFT_RANGE 5


class ShiftLogic {
  protected:
  CentralObject* centralObject;
  public:

  ShiftLogic(CentralObject* _centralObject) {
    centralObject = _centralObject;
  }
  
  void classMain() {
    if (centralObject->getMode() == automatic) { 
      if (centralObject->getCadence() > centralObject->getTargetCadence() + SDRD_SHIFT_RANGE and centralObject->getCadence() > 0 ) {
        centralObject->setGear(centralObject->getGear() + 1);
        Serial.println("Auto UP");
      } else if (centralObject->getCadence() < centralObject->getTargetCadence() - SDRD_SHIFT_RANGE and centralObject->getCadence() > 0) {
        centralObject->setGear(centralObject->getGear() - 1);
        Serial.println("Auto Down");
      }
    } 
    
    else if (centralObject->getMode() == manual) {
       
    }
    delay(500);
    
  }
};

#pragma once

#include <stdint.h>
#include <Arduino.h>

#include "Displayer.hpp"
#include "GearShiftController.hpp"
#include "SharedEnums.hpp"
#include "SharedStructDefinitions.hpp"
#include "SharedDefinitions.hpp"
#include "LCD.hpp"

class CentralObject {
  protected:
    double cadence;
    double targetCadence;
    uint8_t gear;
    shiftMode mode;
    bool shiftModeTransistioned;

    double torque;
    
    Buttons buttons;

    
    GearShiftController  gearShiftController;

    bool uIWasHere = 0;
    
  public:
    void updateShiftPoints(ShiftPointArr shiftPointArr) {
      gearShiftController.updateShiftPoints(shiftPointArr);
    }
  
    //Displayer* pDisplayer;
    LCD* pDisplayer;
  
    bool setMode(shiftMode _mode);
    bool setCadence(double _cadence);
    bool setTargetCadence(double _targetCadence);
    bool setGear(uint8_t _gear);
    bool setTorque(uint16_t _torque);

    shiftMode getMode();
    double   getCadence();
    double   getTargetCadence();
    uint8_t  getGear();
    uint16_t getTorque();

    bool shiftUp();
    bool shiftDown();

    void automaticModeUI();
    void manualModeUI();

     void classMain() {
     while(1) {
      if (mode == automatic) {        
        automaticModeUI();
      } else if (mode == manual) {
        manualModeUI();        
      }
      if (gearShiftController.getGear() < gear) {
        gear = gearShiftController.shiftUp();
      } else if (gearShiftController.getGear() > gear) {
        gear = gearShiftController.shiftDown();
      }
      

      pDisplayer->updateCadence(cadence);
      pDisplayer->updateGear(gear);
      pDisplayer->updateTargetCadence(targetCadence);
      pDisplayer->updateMode(mode);      
      
      delay(1000);              // wait for a half second  
     }
    }
     void updateButtons(Buttons _buttons) {
      if (_buttons.up     == pressed)  {
        buttons.up     = pressed;
        uIWasHere = true;
      }
      if (_buttons.down   == pressed) buttons.down   = _buttons.down;
      if (_buttons.select == pressed) buttons.select = _buttons.select;
      
    }
     void setupClass() {      
      buttons.up     = notPressed;
      buttons.down   = notPressed;      
      buttons.select = notPressed;

      gearShiftController.setup();
      mode = manual;
      shiftModeTransistioned = false;

      pDisplayer->setupDisplayer();      

      cadence = 70;
      targetCadence = 70;
      gear = 1;
      
      return;
    }
    CentralObject(LCD* lcd) {
      pDisplayer = lcd;
      mode = manual;
      gear = 1;
    }
};

double CentralObject::getCadence() {
  return cadence;
}
shiftMode CentralObject::getMode() {
  return mode;
}
double CentralObject::getTargetCadence() {
  return targetCadence;
}
uint8_t CentralObject::getGear() {
  return gearShiftController.getGear();
}
uint16_t CentralObject::getTorque() {
  return gear;
}

bool CentralObject::setTorque(uint16_t _torque) {
  torque = _torque;
}

bool CentralObject::setMode(shiftMode _mode) { 
  if (mode != _mode) {
    buttons.up     = notPressed;
    buttons.down   = notPressed;      
    buttons.select = notPressed;
    shiftModeTransistioned = true;
  } 
  mode = _mode;
}
bool CentralObject::setCadence(double _cadence) {
  cadence = _cadence;
}
bool CentralObject::setTargetCadence(double _targetCadence) {
  targetCadence = _targetCadence;
}
bool CentralObject::setGear(uint8_t _gear) {
  if (MIN_GEAR <= _gear and _gear <= MAX_GEAR) {
    gear = _gear;
  }  
}

bool CentralObject::shiftUp() {
  setGear(gear+1);
}

bool CentralObject::shiftDown() {
  setGear(gear-1);
}

void CentralObject::automaticModeUI() {

      static bool selectPressedOnce = false;

      if (buttons.select == pressed and selectPressedOnce == false) {
        selectPressedOnce = true;
        buttons.select = notPressed;
        buttons.up     = notPressed;
        buttons.down   = notPressed;
      }
      if (selectPressedOnce == true) {
        if (buttons.up == pressed) {
          targetCadence += 1.0;
          buttons.up = notPressed;
        }  else if (buttons.down == pressed) {
          targetCadence -= 1.0;
          buttons.down = notPressed;
        }
      }        
        
        if (buttons.select == pressed and selectPressedOnce == true and getCadence() <= 5) {          
          pDisplayer->showDesiredCadenceSetMessage();
          buttons.select = notPressed;
          selectPressedOnce = false;
        } else {
          if (getCadence() > 5 && shiftModeTransistioned == true) {
            targetCadence = cadence;
            pDisplayer->updateTargetCadence(targetCadence);
            //pDisplayer->showDesiredCadenceSetMessage();
            shiftModeTransistioned = false;
          }
        }                
}

void CentralObject::manualModeUI() {
       if (buttons.up == pressed) {
          gear = gearShiftController.shiftUp();
          buttons.up = notPressed;
        }  else if (buttons.down == pressed) {
          gear = gearShiftController.shiftDown();
          buttons.down = notPressed;
        }
}

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

    Buttons buttons;

    
    GearShiftController  gearShiftController;

    bool uIWasHere = 0;
    
  public:
  
    //Displayer* pDisplayer;
    LCD* pDisplayer;
  
    bool setMode(shiftMode _mode);
    bool setCadence(double _cadence);
    bool setTargetCadence(double _targetCadence);
    bool setGear(uint8_t _gear);

    shiftMode getMode();
    double getCadence();
    double getTargetCadence();
    uint8_t getGear();

      void automaticModeUI() {
        
      Serial.println("auto ui");
      static bool selectPressedOnce = false;

      if (buttons.select == pressed and selectPressedOnce == false) {
        Serial.println("Initial Select Pressed");
        selectPressedOnce = true;
        buttons.select = notPressed;
        buttons.up     = notPressed;
        buttons.down   = notPressed;
      }
      if (selectPressedOnce == true) {
        Serial.println("selectPressedOnce");
        if (buttons.up == pressed) {
          targetCadence += 1.0;
          buttons.up = notPressed;
        }  else if (buttons.down == pressed) {
          targetCadence -= 1.0;
          buttons.down = notPressed;
        }
        Serial.println(targetCadence, 10);
      } else {
        Serial.println("no selectPressedOnce");
      }
        
        
        if (buttons.select == pressed and selectPressedOnce == true and getCadence() <= 5) {
          Serial.println("showDesiredCadenceSetMessage");
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
       void manualModeUI() {
      
    }

     void classMain() {
     while(1) {      
      //Serial.println("Central Object");            
      if (mode == manual) {
        //Serial.println("Manual");
        //Serial.println((uint32_t)&buttons.up);
        /*
        if (buttons.up == pressed) Serial.println("UP Pressed");
        if (buttons.up == notPressed) Serial.println("UP Not Pressed");
        if (buttons.up == held) Serial.println("UP held");
        */
        if (buttons.up == pressed) {
          Serial.println("Shift Up");
          gear = gearShiftController.shiftUp();
          buttons.up = notPressed;
        }  else if (buttons.down == pressed) {
          Serial.println("Shift Down");
          gear = gearShiftController.shiftDown();
          buttons.down = notPressed;
        }      
      }
      if (mode == automatic) {        
        automaticModeUI();
      } else if (mode == manual) {
        manualModeUI();        
      }

      pDisplayer->updateCadence(cadence);
      pDisplayer->updateGear(gear);
      pDisplayer->updateTargetCadence(targetCadence);
      pDisplayer->updateMode(mode);

      
      
      delay(1000);              // wait for a half second  
     }
    }
     void updateButtons(Buttons _buttons) {
      //buttons = _buttons;
      //Serial.println("Updated");
      if (_buttons.up     == pressed)  {
        buttons.up     = pressed;
        //Serial.println((uint32_t)&buttons.up);
        //if (buttons.up == pressed) Serial.println("UP Recieved");
        //if (buttons.up == notPressed) Serial.println("UP Recieved not Pressed");
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
  return gear;
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

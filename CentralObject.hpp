#pragma once

#include <stdint.h>
#include <Arduino.h>

#include "Displayer.hpp"
#include "GearShiftController.hpp"
#include "SharedEnums.hpp"
#include "SharedStructDefinitions.hpp"
#include "LCD.hpp"


class CentralObject {
  protected:
    double cadence;
    double targetCadence;
    uint8_t gear;
    shiftMode mode;

    Buttons buttons;

    Displayer* pDisplayer;
    GearShiftController  gearShiftController;

    bool uIWasHere = 0;
    
  public:
    bool setMode(shiftMode _mode);
    bool setCadence(double _cadence);
    bool setTargetCadence(double _targetCadence);
    bool setGear(uint8_t _gear);

    shiftMode getMode();
    double getCadence();
    double getTargetCadence();
    uint8_t getGear();

     void classMain() {
     while(1) {
      digitalToggle(LED_RED); // Toggle LED
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
          gearShiftController.shiftUp();
          buttons.up = notPressed;
        }  else if (buttons.down == pressed) {
          Serial.println("Shift Down");
          gearShiftController.shiftDown();
          buttons.down = notPressed;
        }      
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

      pDisplayer->setupDisplayer();      

      cadence = 70;
      targetCadence = 70;
      gear = 1;
      
      return;
    }
    CentralObject(Displayer* lcd) {
      pDisplayer = lcd;
    }
};

bool CentralObject::setMode(shiftMode _mode) {
  mode = _mode;
}

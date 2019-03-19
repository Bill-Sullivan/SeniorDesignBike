#pragma once

#include <stdint.h>
#include <Arduino.h>

#include "CentralObject.hpp"
#include "SharedEnums.hpp"
#include "SharedStructDefinitions.hpp"

#define UP_PIN     10
#define DOWN_PIN   11
#define SELECT_PIN  6 

#define MODE_PIN 9

Buttons* pButtons;

void upInterupt();
void downInerupt();

class UserInputMonitor {
  protected:
     CentralObject* centralObject;
     Buttons buttons;
    
     shiftMode modeSwitch;
    Button pollButton(uint8_t pin) {
      static bool pressedButton[256];      
      if(digitalRead(pin) == LOW && pressedButton[pin] == false) {
        pressedButton[pin] = true;
        //Serial.println("Pressed");
        return pressed;
      } else if (digitalRead(pin) == LOW) {
        //Serial.println("held");
        return held;
      } else if (digitalRead(pin) == HIGH) {
        pressedButton[pin] = false;
        //Serial.println("Not Pressed");
        return notPressed;
      }
    }
  public:  
    void pollButtons() {
      Button tempUp;
      Button tempDown;
      Button tempSelect;
      
      tempUp = pollButton(UP_PIN);
      tempDown = pollButton(DOWN_PIN);
      tempSelect = pollButton(SELECT_PIN);
      
      if (tempUp != held) {
        buttons.up = tempUp;
        if (tempUp == pressed) Serial.println("UP");
      }
      if (tempDown != held) {
        buttons.down = tempDown;        
        if (tempDown == pressed) Serial.println("DOWN");
      }
      if (tempSelect != held) {
        buttons.select = tempSelect;        
        if (tempSelect == pressed) Serial.println("SELECT");
      }   
      
    }
    void pollSwitch() {
      if (digitalRead(MODE_PIN) == HIGH) {
        modeSwitch = manual;
      } else  {
        modeSwitch = automatic;
      }
    }
  
     bool setMode(shiftMode _mode) {      
      centralObject->setMode(_mode);      
    }
     void setTargetCadence(float _targetCadence) {
      centralObject->setTargetCadence(_targetCadence);
    }

     void setupClass() {
      Serial.println("Setup UIM");

      pinMode(DOWN_PIN, INPUT);
      pinMode(UP_PIN, INPUT);
      pinMode(SELECT_PIN, INPUT);
      pinMode(MODE_PIN, INPUT);
      
      
      buttons.up     = notPressed;
      buttons.down   = notPressed;
      buttons.select = notPressed;
      pButtons       = &buttons;
      modeSwitch = manual;
      Serial.println("Setup UIM");
    }

     void classMain() {   
      //Serial.println("called before update");   
      pollButtons();
      pollSwitch();
      if (buttons.up == pressed and buttons.down == pressed) {
        buttons.up     = notPressed;
        buttons.down   = notPressed;
      }
      //if(buttons.up == pressed) Serial.println("pressed before update");
      centralObject->updateButtons(buttons);
      buttons.up     = indeterminate;
      buttons.down   = indeterminate;
      buttons.select = indeterminate;
      setMode(modeSwitch);
      
      delay(100);
    }
    UserInputMonitor(CentralObject* _CentralObject) {
      centralObject = _CentralObject;
    }
};

#define INTERUPT_DELAY 1000

void downInerupt() {
  //delay(INTERUPT_DELAY);
  for (uint32_t i = 0; i < 16000000; i++) asm("");
  Serial.println("Down");
  pButtons->down = pressed;
}

void upInterupt() {
  //delay(INTERUPT_DELAY );  
  for (uint16_t i = 0; i < 16000000; i++) asm("");
  Serial.println("UP");
  pButtons->up = pressed;
  
}

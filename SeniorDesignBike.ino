#pragma once

//#define STRICT_SDRD_ADHERENCE

#include <Arduino.h>
#include "CentralObject.hpp"
#include "UserInput.hpp"
#include "CadenceSensor.hpp"
#include "ShiftLogic.hpp"
#include "BluetoothOutput.hpp"
#include "TorqueReader.hpp"

LCD lcd;

CentralObject centralObject(&lcd);
UserInputMonitor userInputMonitor(&centralObject);
ShiftLogic shiftLogic(&centralObject);
BluetoothOutput bluetoothOutput(&centralObject);
TorqueReader torqueReader(&centralObject);
CadenceSensor cadenceSensor(&centralObject, &torqueReader);

void callCenralObjectClassMain() {
  centralObject.classMain();
}

void callUserInputMonitorClassMain() {
  userInputMonitor.classMain();
}

void callLCDClassMain() {
  lcd.classMain();
}

void callSensorClassMain() {
  cadenceSensor.classMain();
}

void callShiftLogicClassMain() {
  shiftLogic.classMain();
}
void callBluetoothOutputClassMain() {
  bluetoothOutput.classMain();
}

void setup() 
{
  Serial.begin(115200);
  while ( !Serial && millis() < 2000) delay(10);   // for nrf52840 with native usb
  analogReadResolution(14);
  Serial.println("Start");
  centralObject.setupClass();
  userInputMonitor.setupClass();
  bluetoothOutput.setupClass();
  lcd.setupDisplayer();

  // Create loop2() using Scheduler to run in 'parallel' with loop()
  Scheduler.startLoop (callCenralObjectClassMain);
  Scheduler.startLoop (callUserInputMonitorClassMain);
  Scheduler.startLoop (callLCDClassMain);
  Scheduler.startLoop (callSensorClassMain);
  Scheduler.startLoop (callShiftLogicClassMain);
  Scheduler.startLoop (callBluetoothOutputClassMain);
}


/**
 * Toggle led1 every 1 second
 */
void loop() 
{
  digitalToggle(LED_BLUE); // Toggle LED
  delay(500);              // wait for a half second  
}

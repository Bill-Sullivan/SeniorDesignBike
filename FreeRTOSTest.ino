#pragma once

#include <Arduino.h>
#include "CentralObject.hpp"
#include "UserInput.hpp"
#include "CadenceSensor.hpp"

LCD lcd;

CentralObject centralObject(&lcd);
UserInputMonitor userInputMonitor(&centralObject);
CadenceSensor cadenceSensor(&centralObject);


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

void setup() 
{
  Serial.begin(115200);
  while ( !Serial && millis() < 2000) delay(10);   // for nrf52840 with native usb
  Serial.println("Start");
  centralObject.setupClass();
  userInputMonitor.setupClass();

  // Create loop2() using Scheduler to run in 'parallel' with loop()
  Scheduler.startLoop (callCenralObjectClassMain);
  Scheduler.startLoop (callUserInputMonitorClassMain);
  Scheduler.startLoop (callLCDClassMain);
  Scheduler.startLoop (callSensorClassMain);
}

/**
 * Toggle led1 every 1 second
 */
void loop() 
{
  digitalToggle(LED_BLUE); // Toggle LED
  delay(500);              // wait for a half second  
}

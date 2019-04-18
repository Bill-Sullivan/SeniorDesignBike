#pragma once

#include "CentralObject.hpp"
#include "SharedEnums.hpp"

#include "SampleAverager.hpp"
#include "TorqueReader.hpp"

//#define SENSOR_MIN_THRESHOLD (1024 /8)
#define SENSOR_MIN_THRESHOLD (16384 /8)

class CadenceSensor {
  protected:
  CentralObject* pCentralObject;
  uint32_t timeSince =0;
  SampleAverager averager;
  uint8_t lastValue = LOW;
  double cadence;
  PedalDirection pedalDirection;
  TorqueReader* pTorqueReader;
  
  public: 
  CadenceSensor(CentralObject* _pCentralObject, TorqueReader* _pTorqueReader) {
    pCentralObject = _pCentralObject;
    pTorqueReader = _pTorqueReader;
  }
  
  void classMain() {
  
  if (lastValue == LOW && analogRead(A0) > SENSOR_MIN_THRESHOLD) {
    //To Do: when detect rising edge on cosine, check if sine is high or low and one way is forwards and the other is backwards
    // if sin tansitions high and cosin is high the we're going forward
    
    if (analogRead(A1) > SENSOR_MIN_THRESHOLD) {
      pedalDirection = backword;
    } else {
      pedalDirection = forward;
    }
    
    int pulseWidth = millis() - timeSince;
    
    timeSince = millis();
    lastValue = HIGH;    
    
    cadence = (60.0 * 1000.0) / ((double)(pulseWidth)) ; //convert miliseconds/segment crossing to rotations/ second
    cadence /= 16;   // convert rotations/ second to rotations/ minute
  
    averager.addSample(cadence);
    pTorqueReader->measureTorque();
    
  } else if (lastValue == HIGH && analogRead(A0) < SENSOR_MIN_THRESHOLD) {
    int pulseWidth = millis() - timeSince;
    
    timeSince = millis();
    lastValue = LOW; 
  
    cadence = (60.0 * 1000.0) / ((double)(pulseWidth)) ; //convert miliseconds/segment crossing to rotations/ second
    cadence /= 16;   // convert rotations/ second to rotations/ minute
  
    averager.addSample(cadence);
    pTorqueReader->measureTorque();
  } else if (millis() - timeSince > 1000){
      cadence = 0.0;
  }  

  if (cadence == 0.0) {
     pCentralObject->setCadence(cadence);
  } else if (pedalDirection == forward) {
    pCentralObject->setCadence(averager.getAverage());
  } else if (pedalDirection == backword) {
    pCentralObject->setCadence(-1.0*averager.getAverage());
  }
  delay(1);
 }
};

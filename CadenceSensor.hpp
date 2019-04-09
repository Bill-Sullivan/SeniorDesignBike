#pragma once

#include "CentralObject.hpp"
#include "SharedEnums.hpp"

class SampleAverager {
  private:
    double samples[16];
    uint8_t currSample = 0;
  public:
    double getAverage() {
      double total = 0.0;
      for (double sample : samples) {
        total += sample;
      }
      total /= 16;
      return total;
    }
    double addSample(double sample) {
      samples[currSample] = sample;
      currSample++;
      currSample = currSample % 16;
    }
};

#define SENSOR_MIN_THRESHOLD (1024 /8)

class CadenceSensor {
  protected:
  CentralObject* pCentralObject;
  uint32_t timeSince =0;
  SampleAverager averager;
  uint8_t lastValue = LOW;
  double cadence;
  PedalDirection pedalDirection;
  
  public: 
  CadenceSensor(CentralObject* _pCentralObject) {
    pCentralObject = _pCentralObject;
  }

  
  void classMain() {
  
  
  //Serial.println("Sensor");
  
  if (lastValue == LOW && analogRead(A0) > SENSOR_MIN_THRESHOLD) {
    //To Do: when detect rising edge on cosine, check if sine is high or low and one way is forwards and the other is backwards
    // if sin tansitions high and cosin is high the we're going forward
    
    if (analogRead(A1) > SENSOR_MIN_THRESHOLD) {
      pedalDirection = backword;
      //Serial.println("backword");      
    } else {
      pedalDirection = forward;
      //Serial.println("forward");
    }
    
    //Serial.println(timeSince);
    //Serial.println(millis());
    int pulseWidth = millis() - timeSince;
    
    timeSince = millis();
    lastValue = HIGH;    
    
    cadence = (60.0 * 1000.0) / ((double)(pulseWidth)) ; //convert miliseconds/segment crossing to rotations/ second
    cadence /= 16;   // convert rotations/ second to rotations/ minute
    
    //currentCadence.setValue((uint8_t)cadence); 
    //Serial.println((uint8_t)cadence);
    //Serial.println(pulseWidth);
    //Serial.println(cadence, 10);
    //char bufferName[40];
    //sprintf(bufferName, "%f\n", cadence);
    //Serial.println(bufferName);
    //Serial.println();
  
    averager.addSample(cadence);
    
  } else if (lastValue == HIGH && analogRead(A0) < SENSOR_MIN_THRESHOLD) {
    //Serial.println(timeSince);
    //Serial.println(millis());
    int pulseWidth = millis() - timeSince;
    
    timeSince = millis();
    lastValue = LOW; 
  
    cadence = (60.0 * 1000.0) / ((double)(pulseWidth)) ; //convert miliseconds/segment crossing to rotations/ second
    cadence /= 16;   // convert rotations/ second to rotations/ minute
    
    //currentCadence.setValue((uint8_t)cadence); 
    //Serial.println((uint8_t)cadence);
    //Serial.println(pulseWidth);
    //Serial.println(cadence, 10);
    //char bufferName[40];
    //sprintf(bufferName, "%f\n", cadence);
    //Serial.println(bufferName);
    //Serial.println();
  
    averager.addSample(cadence);
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

#pragma once

#include "CentralObject.hpp"
#include "SharedEnums.hpp"

#include "SampleAverager.hpp"

class TorqueReader {
  protected:
    SampleAverager averager;
    CentralObject* pCentralObject;
  public:
  TorqueReader(CentralObject* _pCentralObject) {
    pCentralObject = _pCentralObject;
  }

  
  void measureTorque();
    
};

void TorqueReader::measureTorque() {
  uint16_t torque = analogRead(A2);
  averager.addSample((double)torque);
  
  pCentralObject->setTorque(averager.getMin());
  //delay(500);
}

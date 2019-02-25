
#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "CentralObject.hpp"
#include "SharedEnums.hpp"

class Displayer {
  protected:
  public:
  virtual bool updateCadence(double _cadence);
  virtual bool updateGear(uint8_t _gear);
  virtual bool updateTargetCadence (double _targetCadence);
  virtual bool updateMode(shiftMode _mode);
  virtual bool setupDisplayer();
  
  
};

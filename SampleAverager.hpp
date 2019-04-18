#pragma once

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
    double getMin() {
      double currentMax = 100000000000000000000.0;
      for (double val : samples) {
        if (val < currentMax) {
          currentMax = val;
        }
      }
      
      return currentMax;
    }
};

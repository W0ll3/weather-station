#pragma once
#include <Arduino.h>

class MillisTimer {
  unsigned long prev = 0;
  unsigned long interval;

public:
  explicit MillisTimer(unsigned long interval) : interval(interval) {}

  bool ready() {
    unsigned long now = millis();
    if (now - prev >= interval) { 
      unsigned long missed = now - prev;
      unsigned long missedIntervals = missed / interval;
      prev += missedIntervals * interval; 
      return true; 
    } 
    return false;
  }

  void reset() { 
    prev = millis(); 
  }

  void setInterval(unsigned long ms) { 
    interval = ms; 
  }
};
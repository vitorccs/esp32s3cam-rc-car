#ifndef PWM_LED_H
#define PWM_LED_H
#include <Arduino.h>

class PwmLed
{
public:
  uint8_t dutyCycle = 0;

  PwmLed(uint8_t pin);
  void turnHigh();
  void turnLow();
  void turnOff();
  void setDutyCycle(uint8_t dutyCycle);

private:
  static const int pwmFreq = 5000;
  static const int pwmResolution = 8;
  static const uint8_t channel = 0;
  uint8_t pin;
};

#endif

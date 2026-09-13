#ifndef PWM_LED_H
#define PWM_LED_H
#include <Arduino.h>

class PwmLed
{
public:
  uint8_t dutyCycle = 0;

  PwmLed(uint8_t pin);

  // Must be called AFTER the camera initialization
  // to prevent issues with PWM channels/timers
  void init();

  void turnHigh();
  void turnLow();
  void turnOff();
  void setDutyCycle(uint8_t dutyCycle);

private:
  static const int pwmFreq = 5000;
  static const int pwmResolution = 8;
  // PWM channel is being set to 2 because
  // camera XCLK uses timer 0 (Channel 0 and 1)
  static const uint8_t channel = 2;
  static const uint8_t lowDuty = 40;  // 0 to 255
  static const uint8_t highDuty = 255; // 0 to 255
  uint8_t pin;
  bool initialized = false;
};

#endif

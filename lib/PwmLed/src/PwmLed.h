#ifndef PWM_LED_H
#define PWM_LED_H
#include <Arduino.h>

class PwmLed
{
public:
  uint8_t dutyCycle = 0;

  PwmLed(uint8_t pin);

  // Must be called from setup(), AFTER the camera has been initialized:
  // esp_camera_init() takes over LEDC channel 0 / timer 0 for the XCLK, so
  // configuring this channel any earlier would be silently overwritten.
  void init();

  void turnHigh();
  void turnLow();
  void turnOff();
  void setDutyCycle(uint8_t dutyCycle);

private:
  static const int pwmFreq = 5000;
  static const int pwmResolution = 8;
  // Channel 2 lives on timer 1. Channel 0 (and 1, which shares timer 0 with it)
  // must be left alone: the camera drives its 20 MHz XCLK there.
  static const uint8_t channel = 2;
  static const uint8_t lowDuty = 40;  // 0 to 255
  static const uint8_t highDuty = 255; // 0 to 255
  uint8_t pin;
  bool initialized = false;
};

#endif

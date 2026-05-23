#ifndef DIGITAL_LED_H
#define DIGITAL_LED_H
#include <Arduino.h>

class DigitalLed
{
public:
  uint8_t state = LOW;

  DigitalLed(uint8_t pin);
  void turnOn();
  void turnOff();
  void setInverted(bool inverted);
  void setState(uint8_t state);

private:
  uint8_t pin;
  bool inverted = false;
};

#endif
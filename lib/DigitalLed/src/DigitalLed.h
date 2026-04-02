#ifndef DIGITALLED_H
#define DIGITALLED_H
#include <Arduino.h>
#include <Led.h>

class DigitalLed : public Led
{
public:
  uint8_t state = LOW;

  DigitalLed(uint8_t pin);
  void turnOn() override;
  void turnOff() override;
  void setState(uint8_t state) override;

private:
  uint8_t pin;
};

#endif
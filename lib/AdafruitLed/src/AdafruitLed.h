#ifndef ADAFRUITLED_H
#define ADAFRUITLED_H
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Led.h>

class AdafruitLed : public Led
{
public:
  boolean state = false;
  uint8_t red = 255, green = 255, blue = 255; 

  AdafruitLed(uint8_t pin);
  void init();
  void turnOn() override;
  void turnOff() override;
  void setState(uint8_t state) override;
  void setColor(uint8_t red, uint8_t green, uint8_t blue);

private:
  uint8_t pin;
  uint8_t numLeds = 1;
  Adafruit_NeoPixel led;
};

#endif
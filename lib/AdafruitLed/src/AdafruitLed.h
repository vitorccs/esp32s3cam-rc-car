#ifndef ADAFRUIT_LED_H
#define ADAFRUIT_LED_H
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class AdafruitLed
{
public:
  boolean state = false;
  uint8_t red = 255, green = 255, blue = 255;
  uint8_t brightness = 255;

  AdafruitLed(uint8_t pin);
  void init();
  void turnOn();
  void turnOff();
  void setState(uint8_t state);
  void setColor(uint8_t red, uint8_t green, uint8_t blue);
  void setGreen();
  void setLowBrightness();

private:
  uint8_t pin;
  uint8_t numLeds = 1;
  Adafruit_NeoPixel led;
};

#endif
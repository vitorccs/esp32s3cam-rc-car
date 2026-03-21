#include <Arduino.h>
#include <AdafruitLed.h>

AdafruitLed::AdafruitLed(uint8_t pin) : pin(pin),
                                        led(numLeds, pin, NEO_GRB + NEO_KHZ800)
{
}

void AdafruitLed::init()
{
  led.begin();
  led.clear();
  led.show();
}

void AdafruitLed::turnOn()
{
  setState(true);
}

void AdafruitLed::turnOff()
{
  setState(false);
}

void AdafruitLed::setColor(uint8_t red,
                           uint8_t green,
                           uint8_t blue)
{
  this->red = red;
  this->green = green;
  this->blue = blue;
}

void AdafruitLed::setState(boolean state)
{
  this->state = state;

  if (state)
  {
    led.setPixelColor(0, led.Color(this->red, this->green, this->blue));
    led.show();
  }
  else
  {
    led.clear();
    led.show();
  }
}
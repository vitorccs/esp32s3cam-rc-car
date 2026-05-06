#include <Arduino.h>
#include <DigitalLed.h>

DigitalLed::DigitalLed(uint8_t pin)
{
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void DigitalLed::turnOn()
{
  if (inverted)
  {
    setState(LOW);
  }
  else
  {
    setState(HIGH);
  }
}

void DigitalLed::turnOff()
{
   if (inverted)
  {
    setState(HIGH);
  }
  else
  {
    setState(LOW);
  }
}

void DigitalLed::setInverted(bool inverted)
{
  this->inverted = inverted;
}

void DigitalLed::setState(uint8_t state)
{
  this->state = state;
  digitalWrite(this->pin, this->state);
}
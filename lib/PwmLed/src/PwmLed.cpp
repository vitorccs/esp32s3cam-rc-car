#include <Arduino.h>
#include <PwmLed.h>

PwmLed::PwmLed(uint8_t pin)
{
  this->pin = pin;
  ledcSetup(channel, pwmFreq, pwmResolution);
  ledcAttachPin(pin, channel);
}

void PwmLed::turnHigh()
{
  setDutyCycle(highDuty);
}

void PwmLed::turnLow()
{
  setDutyCycle(lowDuty);
}

void PwmLed::turnOff()
{
  setDutyCycle(0);
}

void PwmLed::setDutyCycle(uint8_t dutyCycle)
{
  this->dutyCycle = dutyCycle;
  ledcWrite(channel, this->dutyCycle);
}

#include <Arduino.h>
#include <PwmLed.h>

PwmLed::PwmLed(uint8_t pin)
{
  this->pin = pin;
}

void PwmLed::init()
{
  if (this->initialized)
  {
    return;
  }

  ledcSetup(channel, pwmFreq, pwmResolution);
  ledcAttachPin(this->pin, channel);

  this->initialized = true;

  turnOff();
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

  if (!this->initialized)
  {
    return;
  }

  ledcWrite(channel, this->dutyCycle);
}

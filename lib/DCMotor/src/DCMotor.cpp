#include <Arduino.h>
#include <DCMotor.h>

uint8_t DCMotor::nextChannel = DCMotor::firstChannel;

DCMotor::DCMotor(uint8_t pinIn1,
                 uint8_t pinIn2)
{
    this->pinIn1 = pinIn1;
    this->pinIn2 = pinIn2;
}

/**
 * Sets up the two LEDC channels once. Previously this class relied on
 * analogWrite(), which re-runs ledcSetup() + ledcAttachPin() on every single
 * call - i.e. it reconfigured the LEDC timer four times per joystick command.
 *
 * analogWrite() also hardcoded 1000 Hz, which is why that is the default here:
 * higher frequencies are quieter but cost torque on slow H-bridges.
 */
void DCMotor::init(uint32_t pwmFreq)
{
    if (this->initialized)
    {
        return;
    }

    this->pwmFreq = pwmFreq;
    this->channelIn1 = nextChannel--;
    this->channelIn2 = nextChannel--;

    ledcSetup(this->channelIn1, pwmFreq, pwmResolution);
    ledcAttachPin(this->pinIn1, this->channelIn1);

    ledcSetup(this->channelIn2, pwmFreq, pwmResolution);
    ledcAttachPin(this->pinIn2, this->channelIn2);

    this->initialized = true;

    this->stop();
}

/** Speed must be between 0 and 100 */
void DCMotor::backward(uint8_t speed)
{
    setSpeed(speed);
    write(this->absSpeed, 0);
}

/** Speed must be between 0 and 100 */
void DCMotor::forward(uint8_t speed)
{
    setSpeed(speed);
    write(0, this->absSpeed);
}

void DCMotor::setSpeed(uint8_t speed)
{
    this->absSpeed = map(speed, 0, 100, 0, this->maxAbsSpeed);

    if (this->absSpeed <= this->ignoreAbsSpeed)
    {
        this->absSpeed = 0;
    }

    if (this->absSpeed > 0 && this->absSpeed <= this->minAbsSpeed)
    {
        this->absSpeed = minAbsSpeed;
    }
}

void DCMotor::setMinAbsSpeed(uint8_t absSpeed)
{
    this->minAbsSpeed = absSpeed;
}

void DCMotor::stop()
{
    setSpeed(0);
    write(0, 0);
}

void DCMotor::write(uint8_t duty1, uint8_t duty2)
{
    if (!this->initialized)
    {
        return;
    }

    ledcWrite(this->channelIn1, duty1);
    ledcWrite(this->channelIn2, duty2);
}

#ifndef DCMOTOR_H
#define DCMOTOR_H
#include <Arduino.h>

class DCMotor
{
public:
    DCMotor(uint8_t pinIn1, uint8_t pinIn2);

    // Must be called from setup(), AFTER the camera has been initialized.
    // See the LEDC channel map note below.
    // pwmFreq drives the torque/noise trade-off - see MOTOR_PWM_FREQ in Config.h.
    void init(uint32_t pwmFreq);

    void backward(uint8_t speed = 100);
    void forward(uint8_t speed = 100);
    void setMinAbsSpeed(uint8_t absSpeed);
    void stop();

private:
    // LEDC channels are handed out from the top (7, 6, 5, 4) so we never
    // collide with the camera XCLK (channel 0 / timer 0) nor with the front
    // LED (channel 2 / timer 1). Channel to timer map: 0,1 -> timer 0;
    // 2,3 -> timer 1; 4,5 -> timer 2; 6,7 -> timer 3.
    static const uint8_t firstChannel = 7;
    static uint8_t nextChannel;

    static const int pwmResolution = 8;

    uint32_t pwmFreq = 1000;
    uint8_t pinIn1;
    uint8_t pinIn2;
    uint8_t channelIn1 = 0;
    uint8_t channelIn2 = 0;
    bool initialized = false;
    uint8_t absSpeed = 0;
    uint8_t maxAbsSpeed = 255;
    uint8_t minAbsSpeed = 50;
    uint8_t ignoreAbsSpeed = 30;

    void setSpeed(uint8_t speed);
    void write(uint8_t duty1, uint8_t duty2);
};

#endif

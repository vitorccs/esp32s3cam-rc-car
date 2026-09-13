#ifndef DCMOTOR_H
#define DCMOTOR_H
#include <Arduino.h>

class DCMotor
{
public:
    DCMotor(uint8_t pinIn1, uint8_t pinIn2);

    // Must be called from setup(), AFTER the camera has been initialized.
    // See the LEDC channel map note below.
    void init();

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

    // 20 kHz is above the audible range, which removes the whine the H-bridge
    // produced at the 1 kHz default used by analogWrite().
    static const int pwmFreq = 20000;
    static const int pwmResolution = 8;

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

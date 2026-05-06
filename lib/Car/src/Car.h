#ifndef THE_CAR_H
#define THE_CAR_H
#include <Arduino.h>
#include <DCMotor.h>
#include <PwmLed.h>

class Car
{
public:
    Car(DCMotor &m1,
        DCMotor &m2,
        PwmLed &fLed);
    Car(const Car &other) = delete;
    Car &operator=(const Car &other) = delete;
    void backward(uint8_t speed = 100);
    void backwardLeft(uint8_t speed = 100);
    void backwardRight(uint8_t speed = 100);
    void forward(uint8_t speed = 100);
    void forwardLeft(uint8_t speed = 100);
    void forwardRight(uint8_t speed = 100);
    void frontLedHigh();
    void frontLedLow();
    void frontLedOff();
    void setMinAbsSpeed(uint8_t absSpeed);
    void stop();
    void turn(int16_t leftSpeed, int16_t rightSpeed);
    void turnLeft(uint8_t speed = 100);
    void turnRight(uint8_t speed = 100);

private:
    DCMotor motor1;
    DCMotor motor2;
    PwmLed *frontLed;
};
#endif
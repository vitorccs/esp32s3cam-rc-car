#ifndef WEB_JOYSTICK_HANDLER_H
#define WEB_JOYSTICK_HANDLER_H
#include <Arduino.h>
#include <Car.h>
#include <JoyCoords.h>

class WebJoystickHandler {
public:
    WebJoystickHandler(Car &car);
    void handle(JoyCoords coords);
    void frontLightsHigh();
    void frontLightsLow();
    void frontLightsOff();
    void setDebug(bool enable);
private:
    Car &_car;
    bool debug = false;
    void debugMovement(JoyCoords coords, String direction);
};
#endif
#include <Arduino.h>
#include <Car.h>
#include <WebJoystickHandler.h>
#include <JoyCoords.h>

WebJoystickHandler::WebJoystickHandler(Car &car) : _car(car)
{
}

void WebJoystickHandler::setDebug(bool enable)
{
    this->debug = enable;
}

void WebJoystickHandler::handle(const JoyCoords &coords)
{
    const char *direction = coords.direction;

    // clamp: the speed arrives from the network and feeds uint8_t parameters
    const uint8_t speed = (uint8_t)constrain(coords.speed, 0, 100);

    if (strcmp(direction, "N") == 0)
    {
        debugMovement(coords, "Forward");
        this->_car.forward(speed);
        return;
    }

    if (strcmp(direction, "S") == 0)
    {
        debugMovement(coords, "Backward");
        this->_car.backward(speed);
        return;
    }

    if (strcmp(direction, "W") == 0)
    {
        debugMovement(coords, "Left");
        this->_car.turnLeft(speed);
        return;
    }

    if (strcmp(direction, "E") == 0)
    {
        debugMovement(coords, "Right");
        this->_car.turnRight(speed);
        return;
    }

    if (strcmp(direction, "NW") == 0)
    {
        debugMovement(coords, "Forward Left");
        this->_car.forwardLeft(speed);
        return;
    }

    if (strcmp(direction, "NE") == 0)
    {
        debugMovement(coords, "Forward Right");
        this->_car.forwardRight(speed);
        return;
    }

    if (strcmp(direction, "SW") == 0)
    {
        debugMovement(coords, "Backward Left");
        this->_car.backwardLeft(speed);
        return;
    }

    if (strcmp(direction, "SE") == 0)
    {
        debugMovement(coords, "Backward Right");
        this->_car.backwardRight(speed);
        return;
    }

    debugMovement(coords, "Stop");
    this->_car.stop();
}

void WebJoystickHandler::frontLightsHigh()
{
    this->_car.frontLedHigh();
}

void WebJoystickHandler::frontLightsLow()
{
    this->_car.frontLedLow();
}

void WebJoystickHandler::frontLightsOff()
{
    this->_car.frontLedOff();
}

void WebJoystickHandler::debugMovement(const JoyCoords &coords,
                                       const char *movement)
{
    if (!this->debug)
    {
        return;
    }

    Serial.printf("(speed: %d, direction: %s) %s\n",
                  coords.speed,
                  coords.direction,
                  movement);
}

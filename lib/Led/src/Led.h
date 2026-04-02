#ifndef LED_H
#define LED_H

class Led
{
public:
    virtual ~Led() {}
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual void setState(uint8_t state) = 0;
};

#endif

#ifndef JOYCOORDS_H
#define JOYCOORDS_H

#include <Arduino.h>

struct JoyCoords
{
    // the cardinal point (C, N, NE, E, SE, S, SW, W, NW)
    char direction[3];

    // the motion speed (from 0 to 100)
    int16_t speed;
};
#endif

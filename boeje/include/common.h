#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

typedef struct packedData
{
    float biggestAccel;      // biggest acceleration recorded since last transmission (should contain the length of the acceleration vector)
    float batteryVoltage;    // battery voltage, contains the calculated current
    uint16_t lampCycles;     // count of times we've seen the lamp turn on and off since last transmission
    float lat;
    float lon;
} packedData;

#endif
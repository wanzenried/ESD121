#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

typedef struct packedData
{
    float biggestAccel;      // biggest acceleration recorded since last transmission (should contain the length of the acceleration vector)
    float batteryVoltage;    // battery voltage, stored as a number between 0-1023, same as analogread output
    uint16_t lampCycles;     // count of times we've seen the lamp turn on and off since last transmission
    float lat;
    float lon;
} packedData;


#endif
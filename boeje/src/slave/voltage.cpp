#include <slave/voltage.h>

const uint8_t voltagePin = A0;

uint8_t sampleCount = 1;
uint8_t sampleDelay = 20;

void voltageSetup(uint8_t samples, uint8_t sDelay)
{
    pinMode(voltagePin, INPUT);
    sampleCount = samples;
    sampleDelay = sDelay;
}

void updateVoltage(float *output)
{
    uint32_t sum = 0;

    for (uint8_t i = 0; i < sampleCount; i++)
    {
        sum += analogRead(voltagePin);
        delay(sampleDelay);
    }

    uint16_t gns = (sum / sampleCount); //taking the average of samples, should be multiplied by approx 0.02205 to get the correct voltage
    float gnsReal = gns*0.02205;
    *output = gnsReal;
    
}
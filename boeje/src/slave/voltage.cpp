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

void updateVoltage(uint16_t *output)
{
    uint32_t sum = 0;

    for (uint8_t i = 0; i < sampleCount; i++)
    {
        sum += analogRead(voltagePin);
        delay(sampleDelay);
    }

    *output = (sum / sampleCount); //taking the average of samples

    float voltage = *output * 0.02205;  //convert result from 0-1023 to 0-18
    Serial.println(voltage);
    
    
}
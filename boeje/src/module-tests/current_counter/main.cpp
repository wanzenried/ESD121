#include <Arduino.h>
#include <modules/current.h>

uint16_t lampCounter = 0;

void setup()
{
    Serial.begin(9600);
    currentSetup();
}

void loop()
{
    uint16_t old = lampCounter;
    updateCurrent(&lampCounter);

    if (old != lampCounter)
    {
        Serial.println(lampCounter);
    }

    delay(5);
}
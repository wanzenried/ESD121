/*
*   Use this program with a serial plotter
*   To see acceleration magnitude over time
*/


#include <Arduino.h>
#include <modules/accel.h>

float mag = 0.0;

void setup()
{
    Serial.begin(9600);
    accelSetup();

}

void loop()
{
    updateAccel(&mag);
    Serial.print(">magnitude:");
    Serial.println(mag);
    mag = 0.0;

}
/*
*   Use this program with a serial plotter
*   To see voltage over time
*/

#include <Arduino.h>
#include <modules/voltage.h>

float voltage = 0.0;

void setup()
{
    Serial.begin(9600);
    voltageSetup(10,2);

}

void loop()
{
    updateVoltage(&voltage);
    Serial.print("Voltage:");
    Serial.println(voltage);

}
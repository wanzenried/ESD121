//  outputs readings from the current sensor every 250 ms
//  this test does not use the code we wrote for the current sensor,
//  as that code only counts how many times the current rises above a specified level

#include <Arduino.h>

const uint8_t currentPin = A1;

const float voltageConstant = 0.0045;

void setup()
{
    pinMode(currentPin, INPUT);
    Serial.begin(9600);
}

void loop()
{
    float voltage = analogRead(currentPin) * voltageConstant;   //transform analog read (0-1023) to voltage (0-5V)

    float current = voltage / 10.0; //  1V meassured corresponds to 0.1A, 2V to 0.2A and so on

    Serial.print(">Current: ");
    Serial.print(current, 4);
    Serial.print(",500mA:0.500,0mA:0.0");
    Serial.println();
    delay(10);
}
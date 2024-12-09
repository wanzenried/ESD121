#include <Arduino.h>

#include <modules/gps.h>



float lat = 0.0;
float lon = 0.0;

void setup()
{
    Serial.begin(9600);
    gpsSetup();
    Serial.println("lat,lon");
}


void loop()
{
    if (updateGps(&lat, &lon) == 1)
    {
        Serial.print(lat, 6);
        Serial.print(",");
        Serial.println(lon, 6);
    }
    delay(10);
}
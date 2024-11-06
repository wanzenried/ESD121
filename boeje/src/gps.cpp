#include <Arduino.h>

#include <SoftwareSerial.h>

SoftwareSerial ss(19,18);

void setup(){
    Serial.begin(9600);
    ss.begin(9600);
    Serial.println("setup done");
}

void loop(){
    while (ss.available()>0){
        Serial.println("GPS opfanger satellitter");
        byte gpsData = ss.read();
        
        Serial.println(gpsData);
        Serial.println("Dette er efter gpsdata");
    }
}
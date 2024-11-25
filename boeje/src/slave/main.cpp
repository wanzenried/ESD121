#include <Arduino.h>
#include <stdlib.h>

//our libraries / includes
#include <apc220.h>

#include <slave/accel.h>
#include <slave/gps.h>
#include <slave/current.h>
#include <slave/voltage.h>
#include <slave/radio.h>


//defines:



//function prototypes:

void dumpValues();

//global variables:

uint16_t biggestAccel = 0;      // biggest acceleration recorded since last transmission (should contain the length of the acceleration vector)
uint16_t batteryVoltage = 0;    //battery voltage, stored as a number between 0-1023, same as analogread output
uint16_t lampCycles = 0;        //count of times we've seen the lamp turn on and off since last transmission
char geohash[12];               //todo: geohash of our location this will be hardcoded until we implement the geohash function



void setup() {

    // start serial communication with pc
    Serial.begin(9600);

    Serial.println("setting dummy values for internal vaiables");
    biggestAccel = 987;
    batteryVoltage = 603;
    lampCycles = 1200;
    sprintf(geohash, "12345678901");

    Serial.println(biggestAccel);
    Serial.println(batteryVoltage);
    Serial.println(lampCycles);
    Serial.println(geohash);

    int err = beginRadio();

    if (err != 0)
    {
        Serial.print("Could not configure radio, err: ");
        Serial.println(err);

        while (true) {} // stop other code from running if we receive an error
    }
}

void loop() {

    // If there is communication, handle it and send back a response if neccesary
    int command = recieveCommand();

    switch (command)
    {
    case 1:     //send back stored values
        dumpValues();
        break;

    case 2:     //resend last message

        break;
    
    default:
        break;
    }

    //!Write functions here

    //Get Gps data
    updateGps(geohash);

    //Get battery voltage
    updateVoltage(&batteryVoltage);

    //Meassure current
    updateCurrent(&lampCycles);

    //Get acceleration
    updateAccel(&biggestAccel);
    

}


void dumpValues()
{
    Serial.println(biggestAccel);
    Serial.println(batteryVoltage);
    Serial.println(lampCycles);
    Serial.println(geohash);


}


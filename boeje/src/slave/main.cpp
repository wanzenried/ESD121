#include <Arduino.h>
#include <stdlib.h>

//our libraries / includes
#include <apc220.h>
#include <common.h>
#include <slave/accel.h>
#include <slave/current.h>
#include <slave/voltage.h>
#include <slave/radio.h>

//we can only use the gps module if serialdebug is disabled
//#define SERIALDEBUG
#ifndef SERIALDEBUG
#include <slave/gps.h>
#endif

//defines:


//function prototypes:

void packValues(packedData *data, char *buffer);

//global variables:

packedData data = {0,0,0,0,0};

uint8_t sender = 0;

char dataPck[sizeof(packedData) + 6];               //2 bytes for each of accel, voltage and cycles, 4 bytes for each of lat and lon. this is the minimum, so just a bit more to be sure
uint8_t dataLen = 0;



void setup() {

    #ifdef SERIALDEBUG
    // start serial communication with pc
    Serial.begin(9600);



    Serial.println("setting dummy values for internal vaiables");
    #endif

    data.lat = 12.345678;
    data.lon = 23.456789;

    #ifdef SERIALDEBUG
    Serial.println(data.biggestAccel);
    Serial.println(data.batteryVoltage);
    Serial.println(data.lampCycles);
    Serial.println(data.lat);
    Serial.println(data.lon);
    #endif

    //split16bit(0xaaff, geohash, 0);

    voltageSetup(10, 2);

    currentSetup();

    accelSetup();

    #ifndef SERIALDEBUG
    gpsSetup();
    #endif
    
    int err = beginRadio();

    if (err != 0)
    {
        #ifdef SERIALDEBUG
        Serial.print("Could not configure radio, err: ");
        Serial.println(err);
        #endif

        #ifndef SERIALDEBUG
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        #endif

        while (true) {} // stop other code from running if we receive an error
    }
}

void loop() {

    // If there is communication, handle it and send back a response if neccesary

    switch (recieveCommand(&sender))
    {
    case VARDMP:     //send back stored values
        packValues(&data, dataPck);
        sendMsg(sender, dataPck, dataLen);
        data.lampCycles = 0; //Resettes efter værdien er sendt afsted
        data.biggestAccel = 0.0;
        break;

    case RESEND:     //resend last message
        sendMsg(sender, dataPck, dataLen);
        break;
    
    case NOCMD:

        break;
    
    default:
        break;
    }

    //!Write functions here

    #ifndef SERIALDEBUG
    //Get Gps data
    updateGps(&data.lat, &data.lon);
    #endif

    //Get battery voltage
    updateVoltage(&data.batteryVoltage);

    //Meassure current
    updateCurrent(&data.lampCycles);

    //Get acceleration
    updateAccel(&data.biggestAccel);
    

}


void packValues(packedData *data, char *buffer)
{
    #ifdef SERIALDEBUG
    Serial.println(data->biggestAccel);
    Serial.println(data->batteryVoltage);
    Serial.println(data->lampCycles);
    Serial.println(data->lat);
    Serial.println(data->lon);
    #endif

    dataLen = sizeof(packedData);

    union u
    {
        packedData data;
        byte byteArr[sizeof(packedData)];
    } u;
    
    u.data = *data;

    for (int i = 0; i < sizeof(packedData); i++)
    {
        buffer[i] = u.byteArr[i];
    }

}
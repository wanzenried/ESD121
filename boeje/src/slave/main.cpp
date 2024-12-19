#include <Arduino.h>
#include <stdlib.h>

//our libraries / includes
#include <apc220.h>
#include <common.h>
#include <modules/accel.h>
#include <modules/current.h>
#include <modules/voltage.h>
#include <modules/radio.h>

//we can only use the gps module if serialdebug is disabled
//#define SERIALDEBUG
#ifndef SERIALDEBUG
#include <modules/gps.h>
#endif

//defines:


//function prototypes:

void packValues(packedData *data, byte *buffer);

//global variables:

packedData data = {0,0,0,0,0};

uint8_t sender = 0;

byte dataPck[sizeof(packedData) + 6];               //2 bytes for each of accel, voltage and cycles, 4 bytes for each of lat and lon. this is the minimum, so just a bit more to be sure
uint8_t dataLen = 0;



void setup() {

    #ifdef SERIALDEBUG
    // start serial communication with pc
    Serial.begin(9600);

    #endif

    voltageSetup(10, 2);

    currentSetup();

    accelSetup();

    #ifndef SERIALDEBUG
    gpsSetup();
    #endif
    
    int err = beginRadio();

    if (err != 0)
    {
        pinMode(LED_BUILTIN, OUTPUT);

        #ifdef SERIALDEBUG
        Serial.print("Could not configure radio, err: ");
        Serial.println(err);
        #endif



        // stop other code from running if we receive an error
        while (true)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    }
}

void loop() {

    // If there is communication, handle it and send back a response if neccesary

    switch (recieveCommand(&sender))
    {
    case VARDMP:     //send back stored values
        packValues(&data, dataPck);
        sendMsg(sender, dataPck, dataLen);
        data.lampCycles = 0; //Resettes efter vaerdien er sendt afsted
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

    #ifndef SERIALDEBUG
    //Get Gps data
    updateGps(&data.lat, &data.lon);
    #endif

    //Get battery voltage
    updateVoltage(&data.batteryVoltage);

    //Meassure current
    updateCurrent(&data.lampCycles);

    //Get acceleration
    float mag = getMagnitude();
    if (mag > data.biggestAccel)
    {
        data.biggestAccel = mag;
    }

}


void packValues(packedData *data, byte *buffer)
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
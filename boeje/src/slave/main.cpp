#include <Arduino.h>
#include <stdlib.h>

//our libraries / includes
#include <apc220.h>
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

void packValues(uint16_t biggestAccel, uint16_t batteryVoltage, uint16_t lampCycles, float lat, float lon);
void split16bit(uint16_t n, char *buffer, uint8_t *index);
void splitFloat(float f, char *buffer, uint8_t *index);

//global variables:

uint16_t biggestAccel = 0;      // biggest acceleration recorded since last transmission (should contain the length of the acceleration vector)
uint16_t batteryVoltage = 0;    //battery voltage, stored as a number between 0-1023, same as analogread output
uint16_t lampCycles = 0;        //count of times we've seen the lamp turn on and off since last transmission
float lat = 0.0;
float lon = 0.0;

uint8_t sender = 0;
char dataPck[20];               //2 bytes for each of accel, voltage and cycles, 4 bytes for each of lat and lon. this is the minimum, so just a bit more to be sure
uint8_t dataLen = 0;



void setup() {

    #ifdef SERIALDEBUG
    // start serial communication with pc
    Serial.begin(9600);



    Serial.println("setting dummy values for internal vaiables");
    #endif
    biggestAccel = 987;
    batteryVoltage = 603;
    lat = 12.345678;
    lon = 23.456789;

    #ifdef SERIALDEBUG
    Serial.println(biggestAccel);
    Serial.println(batteryVoltage);
    Serial.println(lampCycles);
    Serial.println(lat);
    Serial.println(lon);
    #endif

    //split16bit(0xaaff, geohash, 0);

    voltageSetup(10, 2);

    currentSetup();

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
        packValues(biggestAccel, batteryVoltage, lampCycles, lat, lon);
        sendMsg(sender, dataPck, dataLen);
        lampCycles = 0; //Resettes efter værdien er sendt afsted
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
    updateGps(&lat, &lon);
    #endif

    //Get battery voltage
    updateVoltage(&batteryVoltage);

    //Meassure current
    updateCurrent(&lampCycles);

    //Get acceleration
    updateAccel(&biggestAccel);
    

}


void packValues(uint16_t biggestAccel, uint16_t batteryVoltage, uint16_t lampCycles, float lat, float lon)
{
    #ifdef SERIALDEBUG
    Serial.println(biggestAccel);
    Serial.println(batteryVoltage);
    Serial.println(lampCycles);
    Serial.println(lat);
    Serial.println(lon);
    #endif

    //char data[18]; //two bytes for each number, and 12 for the geohash
    dataLen = 0;

    split16bit(biggestAccel, dataPck, &dataLen);
    split16bit(batteryVoltage, dataPck, &dataLen);
    split16bit(lampCycles, dataPck, &dataLen);
    splitFloat(lat, dataPck, &dataLen);
    splitFloat(lon, dataPck, &dataLen);

}

void split16bit(uint16_t n, char *buffer, uint8_t *index)
{
    buffer[*index] = n & 0xFF;
    *index +=1;
    buffer[*index] = (n>>8) & 0xFF;
    *index +=1;
    return;
}

void splitFloat(float f, char *buffer, uint8_t *index)
{
    
    union {
        float floatval;
        byte temparray[4];
    } u;

    u.floatval = f;

    for (size_t i = 0; i < 4; i++)
    {
        buffer[*index] = u.temparray[i];
        *index +=1;
    }
    
    
    
}

//Dummy slave module that just send random values back when requested

#include <Arduino.h>
#include <common.h>

//APC pins
#define APC_VCC 13  //5v

#include <modules/radio.h>


void packValues(packedData *data, byte *buffer);

packedData data = {1,2,3,4,5};

uint8_t sender = 0;

byte dataPck[sizeof(packedData) + 6];               //2 bytes for each of accel, voltage and cycles, 4 bytes for each of lat and lon. this is the minimum, so just a bit more to be sure
uint8_t dataLen = 0;

void setup()
{
    Serial.begin(9600);

    // turn on power to radio
    pinMode(APC_VCC, OUTPUT);
    digitalWrite(APC_VCC, HIGH);

    int err = beginRadio();

    if (err != 0)
    {
        Serial.print("Could not configure radio, err: ");
        Serial.println(err);

        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);

        while (true) {} // stop other code from running if we receive an error
    }
    pinMode(A0, INPUT);
    randomSeed(analogRead(A0));

    data.batteryVoltage = random(1,30);
    data.biggestAccel = random(1,30);
    data.lampCycles = random(1,30);
    data.lat = random(1,30);
    data.lon = random(1,30);
}

void loop()
{
    // If there is communication, handle it and send back a response if neccesary

    switch (recieveCommand(&sender))
    {
    case VARDMP:     //send back stored values
        packValues(&data, dataPck);
        sendMsg(sender, dataPck, dataLen);
        data.batteryVoltage +=1;
        data.biggestAccel +=1;
        data.lampCycles +=1;
        data.lat +=1;
        data.lon +=1;
        break;

    case RESEND:     //resend last message
        sendMsg(sender, dataPck, dataLen);
        break;
    
    case NOCMD:

        break;
    
    default:
        break;
    }
}

void packValues(packedData *data, byte *buffer)
{
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
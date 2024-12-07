#include <Arduino.h>
#include <apc220.h>
#include <stdlib.h>
#include <common.h>

//defines:

//APC pins
#define APC_VCC 13  //5v
#define APC_EN  12  //[output] HIGH enable, LOW disable
#define APC_RXD 11  //connected to TX on board
#define APC_TXD 10  //connected to RX on board
#define APC_AUX 9   //[input] HIGH when receiving data, else low
#define APC_SET 8   //[output] HIGH for normal function, LOW to change settings


//function prototypes:

void decode(byte *data);

//global variables:

const uint8_t ID = 0; //this device id


byte sInput[50];
uint8_t sLen = 0;

byte rInput[50];
uint8_t rLen = 0;

softAPC radio(APC_TXD, APC_RXD, APC_EN, APC_SET, APC_AUX, ID);

void setup() {

    // start serial communication with pc
    Serial.begin(9600);

    // turn on power to radio
    pinMode(APC_VCC, OUTPUT);
    digitalWrite(APC_VCC, HIGH);

    // start and configure radio
    radio.begin(9600);
    int err = radio.config(433000, 3, 9, 3, 0);

    if (err != 0)
    {
        Serial.print("Could not configure radio, err: ");
        Serial.println(err);

        while (true) {} // stop other code from running if we receive an error
    }
    Serial.println("ID,Voltage,Acceleration,LampCycles,Lat,Long");
}

unsigned long lastSend = 0;
unsigned long sendDelay = 3000;

void loop() {
    if (Serial.available() > 0)
    {
        uint8_t nId = Serial.parseInt();
        sLen = Serial.readBytesUntil('\n', sInput, 45);
        sInput[sLen] = '\0';

        //Serial.println((char)sInput);
        
        radio.sendMsg(nId, sInput, sLen);

    }

    if (millis() - lastSend > sendDelay)
    {
        byte msg[] = "vd";
        radio.sendMsg(1, msg, 2);
        lastSend = millis();
    }
    

    while (radio.available() > 0)
    {
        if(radio.recieveMsg(rInput, &rLen) == 0)
        {
            decode(rInput);
        }
    }
}

void decode(byte *data)
{
    union u
    {
        packedData data;
        byte byteArr[sizeof(packedData)];
    } u;

    for (int i = 0; i < sizeof(packedData); i++)
    {
       u.byteArr[i] = data[i+1];
    }
    
    Serial.print(data[0]);

    Serial.print(",");
    Serial.print(u.data.batteryVoltage);
    Serial.print(",");
    Serial.print(u.data.biggestAccel, 2);
    Serial.print(",");
    Serial.print(u.data.lampCycles);
    Serial.print(",");
    
    Serial.print(u.data.lat, 6);
    Serial.print(",");
    Serial.println(u.data.lon, 6);

}
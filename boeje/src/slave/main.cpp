#include <Arduino.h>
#include <stdlib.h>

//our libraries / includes
#include <apc220.h>

#include <slave/accel.h>
#include <slave/gps.h>
#include <slave/current.h>
#include <slave/voltage.h>


//defines:

//APC pins
#define APC_EN  7  //[output] HIGH enable, LOW disable
#define APC_RXD 8  //connected to TX on board
#define APC_TXD 9  //connected to RX on board
#define APC_AUX 10   //[input] HIGH when receiving data, else low
#define APC_SET 11   //[output] HIGH for normal function, LOW to change settings

//function prototypes:

void sendMsg(uint8_t recipientId, byte *msg, uint8_t msgLen);
void recieveMsg(char *buffer, uint8_t *bufferLen);
void decode();
void dumpValues();

//global variables:

const uint8_t ID = 1; //this device id

uint16_t biggestAccel = 0;      // biggest acceleration recorded since last transmission (should contain the length of the acceleration vector)
uint16_t batteryVoltage = 0;    //battery voltage, stored as a number between 0-1023, same as analogread output
uint16_t lampCycles = 0;        //count of times we've seen the lamp turn on and off since last transmission
char geohash[12];               //todo: geohash of our location this will be hardcoded until we implement the geohash function


char rInput[50];
uint8_t rLen;


softAPC radio(APC_TXD, APC_RXD, APC_EN, APC_SET, APC_AUX);

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


    // start and configure radio
    radio.begin(9600);
    //Serial.println(radio.getTimeout());
    int err = radio.config(433000, 3, 9, 3, 0);

    if (err != 0)
    {
        Serial.print("Could not configure radio, err: ");
        Serial.println(err);

        while (true) {} // stop other code from running if we receive an error
    }
}

void loop() {

    // If there is communication, handle it and send back a response if neccesary
    while (radio.available() > 0)
    {
        if (radio.read() == '[')
        {
            recieveMsg(rInput, &rLen);
        }
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

void decode()
{
    uint8_t sendId = rInput[0];

    Serial.print("Sender id: ");
    Serial.println(sendId);

    if(rInput[1] == 'v' && rInput[2] == 'd')
    {
        dumpValues();
    }
    else
    {
    Serial.print("msg: ");
    Serial.println(rInput+1);
    }


}

void dumpValues()
{
    Serial.println(biggestAccel);
    Serial.println(batteryVoltage);
    Serial.println(lampCycles);
    Serial.println(geohash);


}


// Send a message over the radio, i dont think messages should be longer than 100 bytes at most, but maybe we can squeze more through if we need it
void sendMsg(uint8_t recipientId, byte *msg, uint8_t msgLen)
{
    
    uint8_t transmissionLen = msgLen + 2;  // length of msg + 1 byte for sender id + 1 byte for reciever id

    radio.write('[');               // specifies start of message
    radio.write(transmissionLen);
    radio.write(recipientId);
    radio.write(ID);
    radio.write(msg, msgLen);       // send message, this way of doing it does not care if there is 0x00 chars in the middle of msg
    radio.write(']');               // specifies end of message
    
}

void recieveMsg(char *buffer, uint8_t *bufferLen)
{

    Serial.println("Recieved start of transmission");

    int transmissionLen = radio.read();

    Serial.print("Transmission is of length ");
    Serial.println(transmissionLen);

    uint8_t recvId = radio.read();      //first byte of transmission is the id of the device that is supposed to recieve the message
    transmissionLen--;

    Serial.print("Transmission is intended for device id: ");
    Serial.println(recvId);

    if(recvId != ID)
    {
        Serial.println("Transmission is not intended for me");
        return;
    }


    *bufferLen = radio.readBytes(buffer,transmissionLen);

    Serial.print("bytes recieved: ");
    Serial.println(rLen);

    buffer[*bufferLen] = '\0';
    


    if (radio.read() == ']')
    {
        Serial.println("Recieved end of transmission");
        decode();
    }
    else
    {
        Serial.println("Sumting wrogn");
    }
}
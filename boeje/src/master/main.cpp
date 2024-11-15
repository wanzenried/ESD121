#include <Arduino.h>
#include <apc220.h>
#include <stdlib.h>

//defines:

//APC pins
#define APC_VCC 13  //5v
#define APC_EN  12  //[output] HIGH enable, LOW disable
#define APC_RXD 11  //connected to TX on board
#define APC_TXD 10  //connected to RX on board
#define APC_AUX 9   //[input] HIGH when receiving data, else low
#define APC_SET 8   //[output] HIGH for normal function, LOW to change settings

//function prototypes:

void sendMsg(uint8_t recipientId, char *msg);

//global variables:

const int8_t ID = 0; //this device id



softAPC radio(APC_TXD, APC_RXD, APC_EN, APC_SET, APC_AUX);

void setup() {

    // start serial communication with pc
    Serial.begin(9600);

    // turn on power to radio
    pinMode(APC_VCC, OUTPUT);
    digitalWrite(APC_VCC, HIGH);

    // start and configure radio
    radio.begin(9600);
    int err = radio.config(433000, 2, 9, 3, 0);

    if (err != 0)
    {
        Serial.print("Could not configure radio, err: ");
        Serial.println(err);

        while (true) {} // stop other code from running if we receive an error
    }
}

void loop() {
    
}


void sendMsg(uint8_t recipientId, char *msg)
{
    
    uint8_t transmissionLen = strlen(msg) + 2;  // length of msg + 1 byte for sender id + 1 byte for reciever id 

    char transmission[transmissionLen + 3];        // string to store transmission before we send it + 2 bytes for start and stop chars + 1 byte for string terminator

    sprintf(transmission, "[%c%c%s%c]",transmissionLen, recipientId, msg, ID);

    radio.write(transmission);
    
}
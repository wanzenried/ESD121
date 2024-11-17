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

void sendMsg(uint8_t recipientId, byte *msg, uint8_t msgLen);
void recieveMsg(char *buffer, uint8_t *bufferLen);
void decode();

//global variables:

const uint8_t ID = 1; //this device id


char rInput[50];
uint8_t rLen;


softAPC radio(APC_TXD, APC_RXD, APC_EN, APC_SET, APC_AUX);

void setup() {

    // start serial communication with pc
    Serial.begin(9600);
    Serial.println(Serial.getTimeout());

    // turn on power to radio
    pinMode(APC_VCC, OUTPUT);
    digitalWrite(APC_VCC, HIGH);

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
    while (radio.available() > 0)
    {
        if (radio.read() == '[')
        {
            recieveMsg(rInput, &rLen);
        }
        
    }
    
    
}

void decode()
{
    uint8_t recId = rInput[0];
    uint8_t sendId = rInput[1];

    Serial.print("recipient id: ");
    Serial.println(recId);
    Serial.print("Sender id: ");
    Serial.println(sendId);
    Serial.print("msg: ");
    Serial.println(rInput+2);

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
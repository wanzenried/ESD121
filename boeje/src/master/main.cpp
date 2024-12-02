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


typedef struct packedData
{
    float biggestAccel;      // biggest acceleration recorded since last transmission (should contain the length of the acceleration vector)
    uint16_t batteryVoltage;    // battery voltage, stored as a number between 0-1023, same as analogread output
    uint16_t lampCycles;        // count of times we've seen the lamp turn on and off since last transmission
    float lat;
    float lon;
} packedData;

//function prototypes:

void sendMsg(uint8_t recipientId, byte *msg, uint8_t msgLen);
void decode(byte *data);

//global variables:

const uint8_t ID = 0; //this device id


byte sInput[50];
uint8_t sLen = 0;

byte rInput[50];
uint8_t rLen = 0;

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
    int err = radio.config(433000, 3, 9, 3, 0);

    if (err != 0)
    {
        Serial.print("Could not configure radio, err: ");
        Serial.println(err);

        while (true) {} // stop other code from running if we receive an error
    }
}

unsigned long lastSend = 0;
unsigned long sendDelay = 1000;

void loop() {
    if (Serial.available() > 0)
    {
        uint8_t nId = Serial.parseInt();
        sLen = Serial.readBytesUntil('\n', sInput, 45);
        sInput[sLen] = '\0';

        //Serial.println((char)sInput);
        
        sendMsg(nId, sInput, sLen);

    }

    if (millis() - sendDelay > lastSend)
    {
        byte msg[] = "vd";
        sendMsg(1, msg, 2);
    }
    

    while (radio.available() > 0)
    {
        if (radio.read() != '[')
        {
            break;
        }
        rLen = 0;
        delay(5);
        uint8_t transmissionLen = radio.read();
        Serial.println(transmissionLen, HEX);

        uint8_t recipient = radio.read();
        transmissionLen--;
        Serial.print("transmission intended for: ");
        Serial.println(recipient, HEX);

        uint8_t sender = radio.read();
        transmissionLen--;
        Serial.print("transmission sent from: ");
        Serial.println(sender, HEX);

        rLen = radio.readBytes(rInput, transmissionLen);
        delay(5);

        if (radio.read() == ']')
        {
            Serial.println("End of transmission");
            decode(rInput);
        }

    }
    
    
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

void decode(byte *data)
{
    union u
    {
        packedData data;
        byte byteArr[sizeof(packedData)];
    } u;

    for (int i = 0; i < sizeof(packedData); i++)
    {
       u.byteArr[i] = data[i];
    }

    Serial.println("data from transmission");
    Serial.println(u.data.batteryVoltage);
    Serial.println(u.data.biggestAccel);
    Serial.println(u.data.lampCycles);
    Serial.println(u.data.lat);
    Serial.println(u.data.lon);
    

}
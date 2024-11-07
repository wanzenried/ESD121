#include <Arduino.h>
#include <apc220cfg.h>
#include <SoftwareSerial.h>

// Parts of this code is adapted from JensD at
// https://jensd.dk/doc/arduino/apc220/apc220code.html

void sendMsg();
void listen();

//uncomment the next line to upload the listener version of the program
//#define LISTEN


// pins used by the APC
// if we use these pins it can be put directly into the arduino, no breadboard needed. very nice for testing :)
#define APC_VCC 13
#define APC_EN  12
#define APC_RXD 11
#define APC_TXD 10
#define APC_AUX 9
#define APC_SET 8

SoftwareSerial S0(APC_TXD,APC_RXD); // RX , TX (yes this is supposed to be the opposite to what is on the apc220 board...)

apc220cfg apc(&S0, APC_SET); // serial "instance", set pin

char c[20];

void setup() {
    Serial.begin(9600); //communication with computer
    S0.begin(9600);     //communication with apc220
    S0.setTimeout(3000);

    pinMode(APC_VCC, OUTPUT);
    digitalWrite(APC_VCC, HIGH);    //turn on power to the apc

    pinMode(APC_EN, OUTPUT);
    digitalWrite(APC_EN, LOW);     //enable the apc

    pinMode(APC_SET, OUTPUT);
    digitalWrite(APC_SET, HIGH);

    //todo: test if this value is correct
    delay(300); //delay to stabilize radio 




}

void loop() {

#ifdef LISTEN
    listen();
#else
    sendMsg();
    delay(1000);
#endif

    
}

void sendMsg() {
    Serial.println("sending message...");
    S0.println("hello world!");
}

void listen() {
    if (S0.available() > 0){

        S0.readBytesUntil('\n',c, 19);

        Serial.print("i recieved:  ");
        Serial.println(c);
    }

}
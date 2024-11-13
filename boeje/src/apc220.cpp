#include <Arduino.h>
#include <apc220.h>

// Parts of this code is adapted from JensD at
// https://jensd.dk/doc/arduino/apc220/apc220code.html

void sendMsg();
void listen();

//uncomment the next line to upload the listener version of the program
//#define LISTEN


// pins used by the APC
// if we use these pins it can be put directly into the arduino, no breadboard needed. very nice for testing :)
#define APC_VCC 13  //5v
#define APC_EN  12  //should be left floating
#define APC_RXD 11  //connected to TX on board
#define APC_TXD 10  //connected to RX on board
#define APC_AUX 9   //should be left floating
#define APC_SET 8   //todo: figure out how important this pin is, for now it looks like it needs to be set high


softAPC apc220(APC_TXD, APC_RXD, APC_EN, APC_SET, APC_AUX);

char c[20];

void setup() {
    Serial.begin(9600); //communication with computer
    apc220.begin(9600);     //communication with apc220
    apc220.setTimeout(3000);

    pinMode(APC_VCC, OUTPUT);
    digitalWrite(APC_VCC, HIGH);    //turn on power to the apc

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
    apc220.println("hello world!");
}

void listen() {
    if (apc220.available() > 0){

        apc220.readBytesUntil('\n',c, 19);

        Serial.print("i recieved:  ");
        Serial.println(c);
    }

}
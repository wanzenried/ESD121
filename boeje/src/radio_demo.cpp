#include <Arduino.h>
#include <stdlib.h>
#include <apc220.h>


//Defines

#define ID 0xaa

//APC pins
#define APC_VCC 13  //5v
#define APC_EN  12  //[output] HIGH enable, LOW disable
#define APC_RXD 11  //connected to TX on board
#define APC_TXD 10  //connected to RX on board
#define APC_AUX 9   //[input] HIGH when receiving data, else low
#define APC_SET 8   //[output] HIGH for normal function, LOW to change settings

// function prototypes
void config();
void sendMsg(char *msg);
void receiveMsg();



// global variables

softAPC apc220(APC_TXD, APC_RXD, APC_EN, APC_SET, APC_AUX);

char x[50];

long wait = 600;

long lastMillis;


void config()
{
    Serial.println("starting config of apc220 module");

    pinMode(APC_VCC, OUTPUT);
    digitalWrite(APC_VCC, HIGH);

    apc220.begin(9600);

    apc220.config(433000, 2, 9, 3, 0);

    apc220.setTimeout(400);

}

void sendMsg(char *msg)
{
    Serial.print("Sending:   ");
    Serial.println(msg);
    apc220.println(msg);

}

void receiveMsg()
{
    if (apc220.available() > 0){

        apc220.readBytesUntil('\n',x, 50);

        Serial.print("Recieved:  ");
        Serial.println(x);
    }
}

void setup() {
    Serial.begin(9600);

    config();

    lastMillis = millis();

}

void loop() {

    if(millis() - lastMillis > wait){
        if (apc220.available() > 0)
        {}
        else
        {
            sprintf(x, "%i time is: %ld long long long ", ID, lastMillis);
            sendMsg(x);

            lastMillis = millis();
        }
        
    }

    receiveMsg();
    
}
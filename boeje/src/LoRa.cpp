#include <Arduino.h>

void sendBesked(int a);
void printBesked(int a);

void setup() {
Serial.begin(9600);

}

void loop() {
sendBesked(45);
delay(500);  
// check om vi har modtaget en besked

//hvis vi har modtaget besked

// modtagBesked(beskeden)
}

void sendBesked(int a) {
    Serial.print("Transmitting: ");
    Serial.print(a);
    Serial.print("\n");

    //på en eller anden måde sende beskeden

}

void printBesked(int a) {
    Serial.print("Received: ");
    Serial.print(a);
    Serial.print("\n");
}
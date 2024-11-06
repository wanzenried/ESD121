#include <Arduino.h>
#include <RadioLib.h>

#define rst 9
#define cs 10
#define DIO0 2
#define DIO1 3

void printBesked(int a);

RFM96 radio = new Module(cs, DIO0, rst, DIO1);


void setup() {
Serial.begin(9600);

Serial.println("setting up LoRa module");

int state = radio.begin();
if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

}

void loop() {
// check om vi har modtaget en besked

//hvis vi har modtaget besked

// modtagBesked(beskeden)
}

void printBesked(int a) {
    Serial.print("Received: ");
    Serial.print(a);
    Serial.print("\n");
}
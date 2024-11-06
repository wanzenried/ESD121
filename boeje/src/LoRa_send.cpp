#include <Arduino.h>
#include <RadioLib.h>

#define rst 9
#define cs 10
#define DIO0 2
#define DIO1 3

void sendBesked(int a);

int value = 0;

RFM96 radio = new Module(cs, DIO0, rst, DIO1);


void setup() {
Serial.begin(9600);

Serial.println("setting up LoRa module");

int state = radio.begin(868.0);
if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

}

void loop() {
  if (value > 50)
  {
    value = 0;
  }
  sendBesked(value);

  value++;

  delay(1000);

}

void sendBesked(int a) {
    Serial.print("Transmitting: ");
    Serial.print("hello world");
    Serial.print(" ... ");

    int status = radio.transmit("hello world");

    if (status == RADIOLIB_ERR_NONE)
    {
      Serial.println(F(" success!"));

      Serial.print("datarate");
      Serial.print(radio.getDataRate());
      Serial.println(" bps");
    }
    else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(status);

    }
    

    //på en eller anden måde sende beskeden

}

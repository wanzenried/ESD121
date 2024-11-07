#include <Arduino.h>

#include "apc220cfg.h"

SoftwareSerial soft01(10, 11);  // RX, TX

// just an easy interface to configure an apc220 on the port

apc220cfg soft01cfg(&soft01, 8);

// --------------------------^-- dig pin to force apc in prog mode
//--------------------^--------- reference to serial port


char x[34];


void setapc220pin8to13(const char cfg[])
{

	// we need to give radio power and enable by dig pins 12 and 13
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH); // power on

	pinMode(12, OUTPUT);
	digitalWrite(12, HIGH); // enable radio

	delay(500); // to stabilize radio - too much time but ...
	soft01cfg.wrCfg(cfg); //

	// show new config

	if (0 == soft01cfg.rdCfg(x)) {
		// we did read a config
		Serial.print("yes: ");
		Serial.println(x);
	}
	else {
		Serial.println("hmm");
		// nope
	}

}


void setup() {
	int res;
	// put your setup code here, to run once:
	Serial.begin(9600); // initalise direct
	soft01.begin(9600); // initalise direct


	// SOFTSERIAL

	setapc220pin8to13("412000 2 9 3 0");

}


// or just very simple
//void setup() {
//  int res;
//  // put your setup code here, to run once:
//  Serial.begin(9600); // initalise direct
//  soft01.begin(9600); // initalise direct
//
//  // we need to give radio power and enable by dig pins 12 and 13
//  pinMode(13, OUTPUT);
//  digitalWrite(13, HIGH); // power on
//
//  pinMode(12, OUTPUT);
//  digitalWrite(12, HIGH); // enable radio
//
//  delay(500); // to stabilize radio - too much time but ...
//  soft01cfg.wrCfg("412000 2 9 3 0"); //
//
//}



void loop() {}
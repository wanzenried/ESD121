/*
   Single Read

   simple example to test the HMC5983 sensor from Honeywell

   (c) 2014 David Cuartielles, Arduino LLC
*/

#include <HMC5983.h>
#include <Wire.h>

HMC5983 compass;

void setup () {
  Serial.begin(9600);
  Serial.println(" heading  /  x y z composanter");
  delay(3000);
  compass.begin(); // use "true" if you need some debug information
}

void loop() {
  float x, y, z;
  float c = -999;
//  c = compass.read();
  c = compass.readAll(&x, &y, &z);
  if (c == -999) {
    Serial.print("Reading error, discarded");
    return;
  } else {
    Serial.print(c);
  }
  Serial.print(" ");
  Serial.print(x); Serial.print(" ");
  Serial.print(y); Serial.print(" ");
  Serial.println(z); 
}

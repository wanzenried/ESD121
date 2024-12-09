#include <Arduino.h>
#include <modules/accel_ny.h>

vec3D accel;

void setup()
{
  Serial.begin(9600);

  if (accelSetup() != 0)
  {
    Serial.println("error during setup");
    while (true){}
  }
  
}

void loop()
{
  getAccel(&accel);

  Serial.print(">Magnitude:");
  Serial.print(accel.magnitude);
  Serial.print(",aX:");
  Serial.print(accel.x);
  Serial.print(",aY:");
  Serial.print(accel.y);
  Serial.print(",aZ:");
  Serial.println(accel.z);

  delay(10);
  
}
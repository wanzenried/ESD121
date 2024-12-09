#include <Arduino.h>
#include <modules/accel.h>

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

  if (accel.magnitude > 5.0)
  {
    Serial.println("big acceleration detected");
  }
  

  delay(10);
  
}
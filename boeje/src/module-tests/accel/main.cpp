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
  Serial.print(accel.z);
  Serial.println(",1G:9.81,2G:19.62,Tresh:5.0");


  if (accel.magnitude > 5.0)
  {
    Serial.println("big acceleration detected");
  }
  

  delay(10);
  
}
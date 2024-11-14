#include <Wire.h>
#include <bmp085.h>

/*
1. call bmp085_init
   - parameter pressure at sealevel (in Pascal) or 0
     if 0 then 101325 (std pressure at sea level) is used

2. Measurement by
  a. call bmp085_measure()
  b. now pressure and temperature is in float variables bmp085_temp and bmp085_pres
*/


void setup()
{
  Serial.begin(9600);
  Wire.begin();

  bmp085Init(101300.0); // ress at sealevel today
}

float temp, atm, alt, pres;


#define LOOPTIME 200
unsigned long t1, t3;

void loop()
{
float tryk, temp, hojde;
  bmp085Measure(&temp, &tryk, &hojde);

  Serial.print("temp(C) "); Serial.print(temp);
  Serial.print("\t tryk(Pa) "); Serial.print(tryk);
  Serial.print("\t hojde(m)"); Serial.println(hojde);
  delay(100);
}




#include <Wire.h>
#include <bmp085.h>

/*
1. call bmp085_init
   - parameter pressure at sealevel (in Pascal) or 0
     if 0 then 101325 (std pressure at sea level) is used

2. Measurement by
  a. call bmp085_measure()
  b. now pressure and temperature is in float variables bmp085_temp and bmp085_pres

The program dumps a oneliner with numbers alone

"height"


You can use the Arduino Serialplotter to show the data

NB NB  Serial communication is 115200
       It takes approx 100 msec for one loop

*/


void setup()
{
  Serial.begin(115200);
  Wire.begin();

  bmp085Init(101300.0); // ress at sealevel today
delay(100);
}

float temp, atm, alt, pres;


#define LOOPTIME 200
unsigned long t1, t3;

void loop()
{
float tryk, temp, hojde;
  bmp085Measure(&temp, &tryk, &hojde);
 
  Serial.println(hojde);
  delay(10);
}




#include <Wire.h>
#include <bmp085.h>

// NB NB find atm pressure at sealevel for today and adjust SEALEVEL below

#define SEALEVEL 102320.0

// ---

#define LOOPTIME 5
#define AVERSIZE 20

float ff[AVERSIZE];

void initAverageArray()
{
  static float pres, temp, alt;
  for (int i = 0; i < AVERSIZE; i++) {  // do AVERSIZE start measurements
    bmp085Measure(&temp, &pres, &alt);
    delay(LOOPTIME);
    ff[i] = hojde;
  }
}

float calcAverage(float ff[])
{
  float mean = 0.0;

  for (int i = 0; i < AVERSIZE; i++)
    mean += ff[i];

  return (mean / AVERSIZE);
}

void setup()
{
  Serial.begin(57600);
  Wire.begin();
  delay(10);

  bmp085Init(SEALEVEL); // ress at sealevel today
  delay(10);

  initAverageArray();
}


void loop()
{
  // placed here with static to guard by use of scoperules
  static int index = 0;

  static float pres, temp, alt;
  static float averageAltitude;
  int  ttt; // just 1 meter marker  // int so we only get 2 instead of 2.543 m :-) a hackk

  bmp085Measure(&temp, &pres, &alt);
  // overwrite oldest value
  ff[index] = alt;
  index++;
  if (AVERSIZE <= index)  // wrap around
    index = 0;


  averageAltitude = calcAverage(ff);

  Serial.print(alt); Serial.print(" ");
  Serial.print(averageAltitude); Serial.print(" ");
  ttt = averageAltitude;
  Serial.print(ttt); Serial.print(" ");
  Serial.println(ttt + 1);
  delay(LOOPTIME);
}

 /* ende */

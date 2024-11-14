/* mod by JDN */
#ifndef BMP085

#define BMP085

/*
1. call bmp085_init
   - parameter pressure at sealevel (in Pascal) or 0
     if 0 then 101325 (std pressure at sea level) is used

2. Measurement by
  a. call bmp085_measure()
  b. now pressure and temperature is in float variables bmp085_temp and bmp085_pres
*/


extern float bmp085_temp,bmp085_pres,bmp085_height, bmp085_heightscale;

void bmp085Init(float bmp085_heightscale);


void bmp085Measure(float *temp,float *press, float *height);

#endif

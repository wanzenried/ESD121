/*
Get pressure, altitude, and temperature from the BMP085.
Serial.print it out at 9600 baud to serial monitor.
Fixed for Arduino 1.0+ by iLabBali.com
Based largely on code by Jim Lindblom via the repost at
http://ilabbali.com/code/Arduino_BMP085.cpp
http://bildr.org/2011/06/bmp085-arduino/
mod by JDN
*/

#include <Wire.h>
#include <Arduino.h>
#include "bmp085.h"

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

float bmp085_temp,bmp085_pres,bmp085_height, bmp085_heightscale;

const unsigned char OSS = 2;  // Oversampling Setting

// Calibration values
// static means they are only visible within this file
static int16_t ac1,ac2,ac3;
static uint16_t ac4,ac5,ac6;
static int16_t b1,b2;
static int32_t b5;
static int16_t mb,mc,md;

 

// Read 1 char from the BMP085 at 'address'
uint8_t bmp085Read(uint8_t address)
{
    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(BMP085_ADDRESS, 1);
    while(!Wire.available()) {};
    return Wire.read();
}

// Read 2 chars from the BMP085
// First char will be from 'address'
// Second char will be from 'address'+1
int16_t bmp085ReadInt(int8_t address)
{
   uint8_t msb, lsb;

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();

    Wire.requestFrom(BMP085_ADDRESS, 2);
    while(Wire.available()<2);
    msb = Wire.read();
    lsb = Wire.read();

    return (int16_t) msb<<8 | lsb;
}


// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
static void bmp085Calibrate()
{
    ac1 = bmp085ReadInt(0xAA);
    ac2 = bmp085ReadInt(0xAC);
    ac3 = bmp085ReadInt(0xAE);
    ac4 = bmp085ReadInt(0xB0);
    ac5 = bmp085ReadInt(0xB2);
    ac6 = bmp085ReadInt(0xB4);
    b1 = bmp085ReadInt(0xB6);
    b2 = bmp085ReadInt(0xB8);
    mb = bmp085ReadInt(0xBA);
    mc = bmp085ReadInt(0xBC);
    md = bmp085ReadInt(0xBE);
}

void bmp085Init(float heightSc)
{
    if (1 < heightSc) {
        bmp085_heightscale = heightSc;
    } else {
        bmp085_heightscale = 101325;    // std pressure at sea level ;-)
    }

    bmp085Calibrate();
}

// Calculate temperature in deg C
float bmp085GetTemperature(uint16_t ut)
{
    int32_t x1, x2;

    x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
    x2 = ((long)mc << 11)/(x1 + md);
    b5 = x1 + x2;

    float temp = ((b5 + 8)>>4);
    temp = temp /10;

    return temp;
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
float bmp085GetPressure(unsigned long up)
{
    int32_t x1, x2, x3, b3, b6, p;
    uint32_t b4, b7;
    float ppp;
    b6 = b5 - 4000;
    // Calculate
    x1 = (b2 * (b6 * b6)>>12)>>11;
    x2 = (ac2 * b6)>>11;
    x3 = x1 + x2;
    b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

    // Calculate B4
    x1 = (ac3 * b6)>>13;
    x2 = (b1 * ((b6 * b6)>>12))>>16;
    x3 = ((x1 + x2) + 2)>>2;
    b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

    b7 = ((unsigned long)(up - b3) * (50000>>OSS));
    if (b7 < 0x80000000) {
        p = (b7<<1)/b4;
    } else {
        p = (b7/b4)<<1;
    }

    x1 = (p>>8) * (p>>8);
    x1 = (x1 * 3038)>>16;
    x2 = (-7357 * p)>>16;
    p += (x1 + x2 + 3791)>>4;

    ppp = 1.0*p;

    return ppp;
}

float calcAltitude(float pressure,float seaLevel)
{
    float A = (1.0 * pressure)/seaLevel;
    float B = 1.0/5.25588;
    float C = pow(A,B);
    C = 1.0 - C;
    C = C /0.0000225577;

    return C;
}


// Read the uncompensated temperature value
static uint16_t bmp085ReadUT()
{
    uint16_t ut;

    // Write 0x2E into Register 0xF4
    // This requests a temperature reading
    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write((char)0xF4);
    Wire.write((char)0x2E);
    Wire.endTransmission();

    // Wait at least 4.5ms
    delay(5);

    // Read two chars from registers 0xF6 and 0xF7
    ut = bmp085ReadInt(0xF6);
    return ut;
}

// Read the uncompensated pressure value
static uint32_t bmp085ReadUP()
{

    uint8_t msb, lsb, xlsb;
    uint32_t up = 0;

    // Write 0x34+(OSS<<6) into register 0xF4
    // Request a pressure reading w/ oversampling setting
    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(0xF4);
    Wire.write(0x34 + (OSS<<6));
    Wire.endTransmission();

    // Wait for conversion, delay time dependent on OSS
    delay(2 + (3<<OSS));

    // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
    msb = bmp085Read(0xF6);
    lsb = bmp085Read(0xF7);
    xlsb = bmp085Read(0xF8);

    up = (((uint32_t) msb << 16) | ((uint32_t) lsb << 8) | (uint32_t) xlsb) >> (8-OSS);

    return up;
}

void bmp085Measure(float *temp, float *press,float*height)
{
    float pp;
    bmp085_temp = *temp   = bmp085GetTemperature(bmp085ReadUT());
    bmp085_pres = *press = bmp085GetPressure(bmp085ReadUP());
    bmp085_height = *height = calcAltitude(bmp085_pres,bmp085_heightscale);
}




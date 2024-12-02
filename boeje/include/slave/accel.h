#include <Arduino.h>
#include <Wire.h>

typedef struct vec3D{
float x;
float y;
float z;
float magnitude;
}vec3D;

void accelSetup();
void updateAccel(float *output);
void writeRegister(uint8_t reg, uint8_t value);
int8_t readRegister(uint8_t addr);
int16_t accelX();
int16_t accelY();
int16_t accelZ();
void filterGravity(int_fast16_t rawAx, int16_t rawAy, int16_t rawAz);
vec3D calculateVectorAcc(int_fast16_t rawAx, int16_t rawAy, int16_t rawAz);
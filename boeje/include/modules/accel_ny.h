#include <Arduino.h>
#include <MPU6050.h>

typedef struct vec3D{
float x;
float y;
float z;
float magnitude;
}vec3D;


int accelSetup();
void updateAccel();
void getAccel(vec3D *output);
float getMagnitude();
void updateGravity(vec3D *gravity , int16_t ax, int16_t ay, int16_t az);
#include <modules/accel.h>


///#define CALIBRATEACC
#ifndef CALIBRATEACC
    #define CUSTOMOFFSET
    #ifdef CUSTOMOFFSET
    const int16_t aXOffset = -2662;
    const int16_t aYOffset = -5;
    const int16_t aZOffset = 1032;
    #endif
#endif

const float G = 9.81;
const float GS = sq(G);

const uint8_t accelRange = 2;   // 0 = +-2G, 1  = +-4G, 2 = +-8G, 3 = +-16G
const int16_t gScale = 16384 >> accelRange;
const float scaleFactor = G / gScale;
const float filterConstant = 0.8;


vec3D gravity = {1,1,1,G};
vec3D Accel = {0,0,0,0};

MPU6050 mpu;

int accelSetup()
{
    Wire.begin();
    mpu.initialize();

    if (!mpu.testConnection())
    {
        return 1;
    }

    #ifdef CALIBRATEACC
    Serial.println("calibrating mpu6050, program will terminate afterwards. press any key to continue");
    while (Serial.available()) Serial.read();
    while (!Serial.available()) {}
    while (Serial.available()) Serial.read();

    mpu.CalibrateAccel(8);
    delay(100);
    mpu.PrintActiveOffsets();
    while(true) {}
    #endif
    #ifdef CUSTOMOFFSET
    mpu.setXAccelOffset(aXOffset);
    mpu.setYAccelOffset(aYOffset);
    mpu.setZAccelOffset(aZOffset);
    #endif

    mpu.setFullScaleAccelRange(accelRange);

    return 0;
}

void updateAccel()
{
    int16_t aX, aY, aZ;

    mpu.getAcceleration(&aX, &aY, &aZ);
    updateGravity(&gravity ,aX, aY, aZ);

    Accel.x = aX * scaleFactor - gravity.x;
    Accel.y = aY * scaleFactor - gravity.y;
    Accel.z = aZ * scaleFactor - gravity.z;
    Accel.magnitude = sqrt(sq(Accel.x)+sq(Accel.y)+sq(Accel.z));

}

void getAccel(vec3D *output)
{
    updateAccel();
    *output = Accel;
}

float getMagnitude()
{
    updateAccel();
    return Accel.magnitude;
}

void updateGravity(vec3D *gravity , int16_t ax, int16_t ay, int16_t az)
{
    float gx = gravity->x * filterConstant + (1-filterConstant) * ax * scaleFactor;
    float gy = gravity->y * filterConstant + (1-filterConstant) * ay * scaleFactor;
    float gz = gravity->z * filterConstant + (1-filterConstant) * az * scaleFactor;

    float s = G / (sqrt(sq(gx) + sq(gy) + sq(gz)));

    gravity->x = gx*s;
    gravity->y = gy*s;
    gravity->z = gz*s;
}
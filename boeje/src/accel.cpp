#include <Arduino.h>
#include <Wire.h>
//#include "I2Cdev.h"
//#include <MPU6050.h>
#include <mpu6050_customlib.h>

//Defines

#define MPU6050_ADDR 0x68
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
#define ACCEL_CONFIG 0x1C


typedef struct vector{
float x;
float y;
float z;
float magnitude;
}vector;

void mpuSetup();
int16_t accelX();
int16_t accelY();
int16_t accelZ();
void writeRegister(uint8_t addr, uint8_t val);
int8_t readRegister(uint8_t addr);
vector calculateVectorAcc(int_fast16_t rawAx, int16_t rawAy, int16_t rawAz);
void filterGravity(int_fast16_t rawAx, int16_t rawAy, int16_t rawAz);

// Variabler til vektor
float gravityX = 0;
float gravityY = 0;
float gravityZ = 0;
const float filterconstant = 0.8; //Lavpasfilter-konstant
const float scaleFactor = 9.81 / 4096.0; // Konvertering til 2g-kraft


//MPU6050 mpu;

void mpuSetup()
{
    writeRegister(PWR_MGMT_1, 0x80);
    delay(200);
    readRegister(PWR_MGMT_1);
    

    writeRegister(PWR_MGMT_1, 0b00001000);
    delay(10);
    readRegister(PWR_MGMT_1);

    writeRegister(PWR_MGMT_2, 0b00000111);
    delay(10 );
    readRegister(PWR_MGMT_2);
    
    //  configure accelrerometer to +-8G sensitivity
    writeRegister(ACCEL_CONFIG, 0b00010000);
}


void writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(true);
}

int8_t readRegister(uint8_t reg)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 1, true);
    int8_t value = Wire.read();
    return value;
}

int16_t accelX()
{
    int8_t high_byte = readRegister(0x3B);
    int8_t low_byte = readRegister(0x3C);

    int16_t combined_value = (high_byte<<8) + low_byte;
    return combined_value;
}

int16_t accelY()
{
    int8_t high_byte = readRegister(0x3d);
    int8_t low_byte = readRegister(0x3e);

    int16_t combined_value = (high_byte<<8) + low_byte;
    return combined_value;
}

int16_t accelZ()
{
    int8_t high_byte =readRegister(0x3F);
    int8_t low_byte = readRegister(0x40);

    int16_t combined_value = (high_byte<<8) + low_byte;
    return combined_value;
}

void setup() {
    Serial.begin(19200);
    Wire.begin();

    mpuSetup();

    //get offset values
    float offsetX, offsetY, offsetZ;
    offsetX = offsetY = offsetZ = 0;
    int count = 100;
    for (int i = 0; i < count; i++)
    {
        offsetX += accelX() * scaleFactor;
        offsetY += accelY() * scaleFactor;
        offsetZ += accelZ() * scaleFactor;
        delay(2);
    }

    offsetX = offsetX/count;
    offsetY = offsetY/count;
    offsetZ = offsetZ/count;

    Serial.println(offsetX, 4);
    Serial.println(offsetY, 4);
    Serial.println(offsetZ, 4);
    
    delay(5000);

}

void loop() {
    // Get acceleration data raw
    int32_t rawAx = 0;
    int32_t rawAy = 0;
    int32_t rawAz = 0;

    int count = 10;
    for (int i = 0; i < count; i++)
    {
        rawAx += accelX();
        rawAy += accelY();
        rawAz += accelZ();
        delayMicroseconds(1100);
    }

    rawAx = rawAx/count;
    rawAy = rawAy/count;
    rawAz = rawAz/count;

    

    //Gravity-filter
    filterGravity(rawAx,rawAy, rawAz);

    //Calculate vectoracceleration
    vector accelVector = calculateVectorAcc(rawAx, rawAy, rawAz);

    //print values of the accelvector
    Serial.print("Acceleration vector:<");
    Serial.print(accelVector.x, 2); Serial.print(", ");
    Serial.print(accelVector.y, 2); Serial.print(", ");
    Serial.print(accelVector.z, 2); Serial.print("> m/s^2");

    //print vector length
    Serial.print(", Magnitude: ");
    Serial.print(accelVector.magnitude, 3);
    Serial.println("m/s^2");

    delay(50);

}

// Function to filtrate away the gravity contribute with lowpass
void filterGravity(int_fast16_t rawAx, int16_t rawAy, int16_t rawAz)
{
    // Convert raw to m/s^2
    float ax = rawAx * scaleFactor;
    float ay = rawAy * scaleFactor;
    float az = rawAz * scaleFactor;

    //Lowpassfilter for the gravity estimat
    gravityX = filterconstant * gravityX + (1 - filterconstant)*ax;
    gravityY = filterconstant * gravityY + (1 - filterconstant)*ay;
    gravityZ = filterconstant * gravityZ + (1 - filterconstant)*az;

    gravityX = max(-9.81, min(9.81, gravityX));
    gravityY = max(-9.81, min(9.81, gravityY));
    gravityZ = max(-9.81, min(9.81, gravityZ));
}

// Function for calculating the accelerationvector
vector calculateVectorAcc(int_fast16_t rawAx, int16_t rawAy, int16_t rawAz)
{

    // remove the gravity contribute
    float ax = rawAx * scaleFactor - gravityX;
    float ay = rawAy * scaleFactor - gravityY;
    float az = rawAz * scaleFactor - gravityZ;

    //Calculate magnitude
    float magnitude = sqrt(ax*ax+ay*ay+az*az);

    vector v = {ax, ay, az, magnitude};

    //return vector
    return v;

}
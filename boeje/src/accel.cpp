#include <Arduino.h>
#include <Wire.h>
#include "I2Cdev.h"
#include <MPU6050.h>

MPU6050 mpu;

// Get acceleration data
int16_t ax = mpu.getAccelerationX();
int16_t ay = mpu.getAccelerationY();
int16_t az = mpu.getAccelerationZ();

// Get gyroscope data
int16_t gx = mpu.getRotationX();
int16_t gy = mpu.getRotationY();
int16_t gz = mpu.getRotationZ();

void setup() {
    Serial.begin(19200);
    Wire.begin();

    // Initialize MPU6050
    mpu.initialize();

    if (!mpu.testConnection()) {
        Serial.println("MPU6050 connection failed");
        while (1); // Halt if MPU6050 is not connected
    }
}

void loop() {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    float gyroX = gx / 131.0;
    float gyroY = gy / 131.0;
    float gyroZ = gz / 131.0;

    Serial.println("Gyro x: ");
    Serial.println(gyroX);

    Serial.println("Gyro y: ");
    Serial.println(gyroY);

    Serial.println("Gyro z: ");
    Serial.println(gyroZ);

    delay(500);
}
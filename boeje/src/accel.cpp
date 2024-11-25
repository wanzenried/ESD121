#include <Arduino.h>
#include <Wire.h>
//#include "I2Cdev.h"
//#include <MPU6050.h>
#include <mpu6050_customlib.h>


void mpuSetup();
int16_t accelX();
int16_t accelY();
int16_t accelZ();
void writeRegister(uint8_t addr, uint8_t val);
int8_t readRegister(uint8_t addr);

//Defines

#define MPU6050_ADDR 0x68
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C



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
}

void writeRegister(uint8_t reg, uint8_t value)
{
Serial.print("writing value:");
Serial.print(value, HEX);
Serial.print("into register:");
Serial.println(reg, HEX);
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
uint8_t value = Wire.read();
Serial.print("read value:");
Serial.println(value,HEX);
return value;
}

int16_t accelX()
{
uint8_t high_byte = readRegister(0x3B);
uint8_t low_byte = readRegister(0x3C);

uint16_t combined_value = (high_byte<<8) + low_byte;
return combined_value;
}

int16_t accelY()
{
uint8_t high_byte = readRegister(0x3d);
uint8_t low_byte = readRegister(0x3e);

uint16_t combined_value = (high_byte<<8) + low_byte;
return combined_value;
}

int16_t accelZ()
{
uint8_t high_byte =readRegister(0x3F);
uint8_t low_byte = readRegister(0x40);

uint16_t combined_value = (high_byte<<8) + low_byte;
return combined_value;
}

void setup() {
    Serial.begin(19200);
    Wire.begin();

    mpuSetup();

/*
    // Initialize MPU6050
    mpu.initialize();

    if (!mpu.testConnection()) {
        Serial.println("MPU6050 connection failed");
        while (1); // Halt if MPU6050 is not connected
    }
    */
}

void loop() {
    // Get acceleration data
    int16_t ax = accelX();
    int16_t ay = accelY();
    int16_t az = accelZ();
/*
    // Get gyroscope data
    int16_t gx = mpu.getRotationX();
    int16_t gy = mpu.getRotationY();
    int16_t gz = mpu.getRotationZ();
*/
    //mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    /*float gyroX = gx / 131.0;
    float gyroY = gy / 131.0;
    float gyroZ = gz / 131.0;*/
    
    print_acceleration_data(&ax, &ay, &az);

}
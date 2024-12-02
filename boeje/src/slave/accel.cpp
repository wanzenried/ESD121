#include <slave/accel.h>

#define MPU6050_ADDR 0x68
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
#define ACCEL_CONFIG 0x1C

// Variabler til vektor
float gravityX = 0;
float gravityY = 0;
float gravityZ = 0;
const float filterconstant = 0.8; //Lavpasfilter-konstant
const float scaleFactor = 9.81 / 4096; // Konvertering til 2g-kraft

void accelSetup()
{
    Wire.begin();
    delay(10);
    writeRegister(PWR_MGMT_1, 0x80);
    delay(200);
    //readRegister(PWR_MGMT_1);
    

    writeRegister(PWR_MGMT_1, 0b00001000);
    delay(10);
    //readRegister(PWR_MGMT_1);

    writeRegister(PWR_MGMT_2, 0b00000111);
    delay(10);
    //readRegister(PWR_MGMT_2);

    //  configure accelrerometer to +-8G sensitivity
    writeRegister(ACCEL_CONFIG, 0b00010000);
}

void updateAccel(float *output)
{
    int16_t rawAx = accelX();
    int16_t rawAy = accelY();
    int16_t rawAz = accelZ();

    filterGravity(rawAx,rawAy, rawAz);

    vec3D accelVector = calculateVectorAcc(rawAx, rawAy, rawAz);

    if (accelVector.magnitude > *output)
    {
        *output = accelVector.magnitude;
    }

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
    uint8_t value = Wire.read();
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

}

vec3D calculateVectorAcc(int_fast16_t rawAx, int16_t rawAy, int16_t rawAz)
{

    // remove the gravity contribute
    float ax = rawAx * scaleFactor - gravityX;
    float ay = rawAy * scaleFactor - gravityY;
    float az = rawAz * scaleFactor - gravityZ;

    //Calculate magnitude
    float magnitude = sqrt((ax*ax)+(ay*ay)+(az*az));

    vec3D v = {ax, ay, az, magnitude};

    //return vector
    return v;

}
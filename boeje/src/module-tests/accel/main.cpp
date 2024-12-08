#include <Wire.h>
#include <MPU6050.h>

// Create an MPU6050 object
MPU6050 mpu;

// Define the scale factor for ±2g (16384 for 16-bit raw data)
//const float accelScale = 16384.0;
const float accelScale = 8192.0;


void setup() {
  // Start serial communication
  Serial.begin(9600);
  Serial.println("welcome");
  
  // Initialize the MPU6050
  Wire.begin();
  mpu.initialize();

  
  // Check if the sensor is connected correctly
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);  // Hang if connection fails
  }

  mpu.setFullScaleAccelRange(1);

  //mpu.PrintActiveOffsets();
  //delay(100);
  //mpu.CalibrateAccel(7);

  mpu.setXAccelOffset(-420);
  mpu.setYAccelOffset(581);
  mpu.setZAccelOffset(1430);

  mpu.PrintActiveOffsets();
  delay(3000);
}

void loop() {
  // Variables to store raw acceleration data
  int16_t ax, ay, az;
  
  // Read acceleration data from the MPU6050
  mpu.getAcceleration(&ax, &ay, &az);
  
  // Convert raw data to 'g' values
  float ax_g = ax / accelScale;
  float ay_g = ay / accelScale;
  float az_g = az / accelScale;
  
  // Calculate the magnitude of the acceleration vector
  float magnitude = sqrt(ax_g * ax_g + ay_g * ay_g + az_g * az_g);
  
  // Output the data in the desired format with 'g' units
  Serial.print(">aX:");
  Serial.print(ax_g, 4);  // Print with 4 decimal places
  Serial.print(",aY:");
  Serial.print(ay_g, 4);  // Print with 4 decimal places
  Serial.print(",aZ:");
  Serial.print(az_g, 4);  // Print with 4 decimal places
  Serial.print(",Mag:");
  Serial.println(magnitude, 4);  // Print the magnitude of the vector
  
  // Wait before the next reading
  delay(25);
}

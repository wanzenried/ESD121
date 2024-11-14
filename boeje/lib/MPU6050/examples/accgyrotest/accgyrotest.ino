 //JDN 2020
 

#include "MPU6050.h"
 
 #include "Wire.h"
 


MPU6050 accelgyro(0x68);

// if AD0 pin on MPU is high i2c address is high = 0x69
// so change 0x68 above to 0x69
//
// Try cansati2cscanner to see which address is used
// so you have to specify i2c address as below


int16_t gx, gy, gz;
int16_t ax, ay, az;



// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


#define LED_PIN 13
bool blinkState = false;






void handleCalib()
{

  Serial.println("Updating internal sensor offsets...");
  // -76    -2359   1688    0   0   0
  Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
  Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
  Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
  Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
  Serial.print("\n");
  //  accelgyro.setXGyroOffset(220);
  //  accelgyro.setYGyroOffset(76);
  //  accelgyro.setZGyroOffset(-85);
  Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
  Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
  Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
  Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
  Serial.print("\n");
}


void setup() {
  Serial.begin(38400); // start serial port

  Serial.println("cansatMPU6050accgyro test");


  Wire.begin();  // start i2c bnetwork


  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  if (accelgyro.testConnection()) {
    Serial.println("MPU6050 ok ");
  }
  else {
    Serial.println("MPU6050 fail ");
    while (1);  // stop here
  }
  handleCalib();  // if you want to see printout of offsets

  //accelgyro.setI2CBypassEnabled(true);
  // configure Arduino LED for
  pinMode(LED_PIN, OUTPUT);
}

void loop() {

  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  accelgyro.getAcceleration(&ax, &ay, &az);
  accelgyro.getRotation(&gx, &gy, &gz);

  
  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");         
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");            
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);
  
  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
}

/*


        void getMotion9(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* mx, int16_t* my, int16_t* mz);
        void getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
        void getAcceleration(int16_t* x, int16_t* y, int16_t* z);
        int16_t getAccelerationX();
        int16_t getAccelerationY();
        int16_t getAccelerationZ();

        void getRotation(int16_t* x, int16_t* y, int16_t* z);
        int16_t getRotationX();
        int16_t getRotationY();
        int16_t getRotationZ();

        // GYRO_CONFIG register
        uint8_t getFullScaleGyroRange();
        void setFullScaleGyroRange(uint8_t range);
        #define MPU6050_GYRO_FS_250         0x00  
        #define MPU6050_GYRO_FS_500         0x01
        #define MPU6050_GYRO_FS_1000        0x02
        #define MPU6050_GYRO_FS_2000        0x03

        // ACC_CONFIG register
        uint8_t getFullScaleAccelRange();
        void setFullScaleAccelRange(uint8_t range);
        #define MPU6050_ACCEL_FS_2          0x00  +- 2g  ~= + 32767, - 32767   - 16 bit int)
        #define MPU6050_ACCEL_FS_4          0x01
        #define MPU6050_ACCEL_FS_8          0x02
        #define MPU6050_ACCEL_FS_16         0x03

        int16_t getTemperature();


*/

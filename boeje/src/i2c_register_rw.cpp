#include <Arduino.h>
#include <Wire.h>

void readRegister(int addr, int bytes);
int writeRegister(int addr, int val);

const int I2CADDR = 0x68;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Wire.begin();
  //Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  //Wire.write(0x6B);                  // Talk to the register 6B
  //Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  //Wire.endTransmission(true);        //end the transmission

  //writeRegister(0x6b, 0x00);

  delay(300);
  Serial.println("the following commands can be used");
  Serial.println("read: 'r a b' where a is the adress and b is the amount of bytes to read");
  Serial.println("write: 'w a v' where a is the adress and v is the value to write");

  //readRegister(0x43, 6);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0) {
    char c = Serial.read();
    int addr = Serial.parseInt();
    int b = Serial.parseInt();

    if (Serial.read() == '\n') {

      if (addr < 0 || b < 0) {
        Serial.println("values must be minimum 0");
        break;
      }

      if (addr > 255 || b > 255) {
        Serial.println("values cannot be bigger than 255");
        break;
      }

      if (c == 'w') {
        writeRegister(addr, b);
        break;
      } else if (c == 'r') {
        readRegister(addr, b);
        break;
      }

      Serial.println(c);
      Serial.println("unknown command, use 'w' or 'r'");
      break;
    }
  }
}

void readRegister(int addr, int bytes) {
  Serial.print("reading ");
  Serial.print(bytes);
  Serial.print(" bytes, starting from adress: 0x");
  Serial.println(addr, HEX);

  Wire.beginTransmission(I2CADDR);
  Wire.write(addr);
  Wire.endTransmission(false);
  int n = Wire.requestFrom(I2CADDR, bytes, true);

  for (int i = 0; i < n; i++) {
    int val = Wire.read();  //reads one byte, the address is automatically incremented
    Serial.print("0x");
    Serial.print(addr + i, HEX);
    Serial.print(" : hex ( 0x");
    Serial.print(val, HEX);
    Serial.print(" ) , bin ( 0b");
    Serial.print(val, BIN);
    Serial.println(" )");
  }
}

int writeRegister(int addr, int val) {
  Serial.print("writing 0x");
  Serial.print(val, HEX);
  Serial.print(" to adress: 0x");
  Serial.println(addr, HEX);

  if (val > 0xff)  //Value is too big return an error
  {
    return -1;
  }
  if (val < 0) {
    return -2;
  }

  Wire.beginTransmission(I2CADDR);
  Wire.write(addr);
  Wire.write(val);
  Wire.endTransmission(true);
  return 0;
}
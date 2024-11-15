#include <mpu6050_customlib.h>




unsigned long zeroStartTime = 0;
bool zeroTimerStarted = false;
bool warningActive = false;

void print_acceleration_data(int16_t* x, int16_t* y, int16_t* z) {
    Serial.print("Accel x: ");
    Serial.print((*x / (16384>>1)) * 9.81);
    Serial.print("\n");

    Serial.print("Accel y: ");
    Serial.print((*y / (16384>>1)) * 9.81);
    Serial.print("\n");

    Serial.print("Accel z: ");
    Serial.print((*z / (16384>>1)) * 9.81);
    Serial.print("\n");

    Serial.println("-------------");

    if (*z <= 0) {
        if (!zeroTimerStarted) {
            zeroStartTime = millis();
            zeroTimerStarted = true;
        } else if (millis() - zeroStartTime >= 30000) {
                warningActive = true;
        }
    } else {
        zeroTimerStarted = false;
        warningActive = false;
    }

    if (warningActive) {
        Serial.println("Warning: The Z axis of the bouy has been 0 or negative for more than 30 seconds!");
    }

    delay(500);
}
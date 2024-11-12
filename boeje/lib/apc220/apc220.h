

#ifndef _APC220_H_
#define _APC220_H_

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdlib.h>

class softAPC : public SoftwareSerial
{
private:
    uint8_t _EN;
    uint8_t _AUX;
    uint8_t _SET;

    int calcBaudFromParam(uint8_t param);
public:
    softAPC(int rxPin, int txPin, int en, int set, int aux);
    void begin(long speed);
    int config(uint32_t freq, uint8_t airBaud, uint8_t power, uint8_t baud, uint8_t parity);

};





#endif
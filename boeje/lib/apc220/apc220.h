

#ifndef _APC220_H
#define _APC220_H

#include <Arduino.h>
#include <apc220cfg.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <string.h>

class apc220
{
private:
    //control pins
    uint8_t _enPin;
    uint8_t _setPin;
    uint8_t _auxPin;

    //serial interfaces
    HardwareSerial *hardware;
    SoftwareSerial *software;
    int serialType;                 //0 for hardware serial, 1 for software serial

    //apc220cfg *apcConf;
public:
    apc220(int rx, int tx, int en, int set, int aux);
    apc220(HardwareSerial *s,int en, int set, int aux);

    void begin();

    int config(uint32_t frequency, uint8_t rfBaud, uint8_t power, uint8_t baud, uint8_t parity);
    int send(char *msg);
    int receive(char *reply);

    ~apc220();
};



#endif
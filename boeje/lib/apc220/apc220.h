/*
*   Library for using APC220 radio module
*   This library extends softwareSerial
*
*   parts of the code are adapted from code written by Jens Dalsgaard @ https://jensd.dk/doc/wless/apc220-radio/index.html
*   Here you can also find the datasheet for the radio module
*
*   Jacob Wanzenried 13/11/2024
*/

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

    bool interruptActive = false;
    volatile bool receiving = false;

    int calcBaudFromParam(uint8_t param);
    void wrConfig(char *cmd, char* reply);
    void rdConfig(char *reply);
    void enterSetMode();
    void exitSetMode();

    void checkReceiving();
public:
    softAPC(int rxPin, int txPin, int en, int set, int aux);
    void begin(long speed);
    int config(uint32_t freq, uint8_t airBaud, uint8_t power, uint8_t baud, uint8_t parity);
    int safeSend(char *msg);

};





#endif
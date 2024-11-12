

#ifndef _APC220_H_
#define _APC220_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

class softAPC : public SoftwareSerial
{
private:
    int _EN;
    int _AUX;
    int _SET;
public:
    softAPC(int rxPin, int txPin, int en, int set, int aux);
    void begin(long speed);
    //int config();

};





#endif
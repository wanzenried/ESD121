#include <apc220.h>

apc220::apc220(int rx, int tx, int en, int set, int aux)
{
    software = &SoftwareSerial(rx,tx);
    serialType = 1;

    _enPin = en;
    _setPin = en;
    _auxPin = en;

    pinMode(_enPin, OUTPUT);
    pinMode(_setPin, OUTPUT);
    pinMode(_auxPin, INPUT);
}

apc220::apc220(HardwareSerial *s, int en, int set, int aux)
{
    hardware = s;
    serialType = 0;

    _enPin = en;
    _setPin = set;
    _auxPin = aux;

    pinMode(_enPin, OUTPUT);
    pinMode(_setPin, OUTPUT);
    pinMode(_auxPin, INPUT);
}

void apc220::begin()
{
    digitalWrite(_enPin, HIGH);
    digitalWrite(_setPin, HIGH);
    //start serial communication at standard data rate (9600)
    if (serialType == 0)
    {
        hardware->begin(9600);
    }
    else
    {
        software->begin(9600);
    }
    delay(100); //minimum from datasheet is 50ms
}


int apc220::config(uint32_t frequency, uint8_t rfBaud, uint8_t power, uint8_t baud, uint8_t parity)
{
    int err = 0;
    if (frequency > 999999)
    {
        err += 1;
    }

    if (rfBaud > 9)
    {
        err += 2;
    }

    if (power > 9)
    {
        err += 4;
    }

    if (baud > 9)
    {
        err += 8;
    }

    if (parity)
    {
        err += 16;
    }

    if (err!= 0)
    {
        return err;
    }
    
    char cmd[15];

    sprintf(cmd, "%i %i %i %i %i", frequency, rfBaud, power, baud, parity);
    
    
    apc220cfg *cfg;
    if (serialType == 0)
    {
        cfg = &apc220cfg(hardware, _setPin);
    }
    else
    {
        cfg = &apc220cfg(software, _setPin);
    }


    
    
}

int apc220::send(char *msg)
{

}
int apc220::receive(char *reply){

}
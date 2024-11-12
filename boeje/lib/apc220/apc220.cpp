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

/*  configures the apc220, the following values are allowed
*   frequency   : 418000 - 455000
*   rfBaud      : 1 - 4
*   power       : 0 - 9
*   baud        : 0 - 6
*   parity      : 0 - 2
*/
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

    uint16_t baudRate;

    switch (baud)
    {
    case 0:
    baudRate=1200;
        break;
    case 1:
    baudRate=2400;
        break;
    case 2:
    baudRate=4800;
        break;
    case 3:
    baudRate=9600;
        break;
    case 4:
    baudRate=19200;
        break;
    case 5:
    baudRate=38400;
        break;
    case 6:
    baudRate=57600;
        break;
    
    default:
    return 32;
        break;
    }
    
    char cmd[15];

    sprintf(cmd, "%i %i %i %i %i", frequency, rfBaud, power, baud, parity);
    
    
    apc220cfg *cfg;
    if (serialType == 0)
    {
        hardware->begin(9600);
        cfg = &apc220cfg(hardware, _setPin);
    }
    else
    {
        software->begin(9600);
        cfg = &apc220cfg(software, _setPin);
    }

    cfg->wrCfg(cmd);

    if (serialType == 0)
    {
        hardware->begin(baudRate);
    }
    else
    {
        software->begin(baudRate);
    }

}

int apc220::send(char *msg)
{
    if (serialType == 0)
    {
        hardware->println(msg);
    }
    else
    {
        software->println(msg);
    }

}
int apc220::receive(char *reply, uint16_t len){

}
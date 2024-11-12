#include <apc220.h>


softAPC::softAPC(int rxPin, int txPin, int en, int set, int aux) :
    SoftwareSerial(rxPin, txPin)

{
    _EN = en;
    pinMode(_EN, OUTPUT);
    _SET = set;
    pinMode(_SET, OUTPUT);
    _AUX = aux;
    pinMode(_AUX, INPUT);
}

void softAPC::begin(long speed)
{
    digitalWrite(_EN, HIGH);
    digitalWrite(_SET, HIGH);
    delay(100); //time to let the module start up. double the value in the datasheet
    SoftwareSerial::begin(speed);
}

int softAPC::config(uint32_t freq, uint8_t airBaud, uint8_t power, uint8_t baud, uint8_t parity)
{
    //Return an error message, and abort, if any of the numbers are too long
    int err = 0;
    if (freq > 455000)
    {
        err += 1;
    }
    if (airBaud > 4)
    {
        err += 2;
    }
    if (power > 9)
    {
        err += 4;
    }
    if (baud > 6)
    {
        err += 8;
    }
    if (parity > 2)
    {
        err += 16;
    }

    if (err > 0)
    {
        return err;
    }

    int newBaud = calcBaudFromParam(baud);

    char cmd[15];

    sprintf(cmd, "%d %d %d %d %d", freq, airBaud, power, baud, parity);
    

}

int softAPC::calcBaudFromParam(uint8_t param)
{
    if (param > 6)
    {
        return -1;
    }

    if (param == 6)
    {
        return 57600;
    }

    return (1200 << param);

}

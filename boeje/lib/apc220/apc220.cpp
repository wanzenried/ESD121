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
    delay(100); //double the value in the datasheet
    SoftwareSerial::begin(speed);

}

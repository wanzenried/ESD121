#include <slave/current.h>

const uint8_t currentPin = A1;
const uint8_t interruptPin = 2;
volatile bool registreret = false;

void currentSetup()
{
    pinMode(currentPin, INPUT);
    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), registrestroem, RISING);

}


void updateCurrent(uint16_t *output)
{
    if (registreret == true)
    {
        *output++;
        registreret = false;
    }
    
}

void registrestroem()
{
    if(analogRead(currentPin) > 600){
        registreret = true;
    }
    

}
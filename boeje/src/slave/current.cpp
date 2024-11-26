#include <slave/current.h>

const uint8_t currentPin = A1;
const uint8_t interruptPin = 2;
volatile bool registreret = false;

void currentSetup()
{
    pinMode(currentPin, INPUT);
    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), registrestroem, RISING); //denne funktion afbryder koden og kører funktionen, hvorefte den oprindelige kode fortsætter

}


void updateCurrent(uint16_t *output)
{
    //Serial.println(analogRead(currentPin));
    if (registreret == true)
    {
        *output +=1;
        registreret = false;
    }
    
}

void registrestroem()
{
    if(analogRead(currentPin) > 600){ //Målet er at måle om spændingen er større end 3 V, og da 0-5 V på anlogread svarer til 0-1023, så bliver 3/5*1023=613, cirka 600.
        registreret = true;
    }
    

}
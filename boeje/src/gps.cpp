
#include <Arduino.h>

void setup()
{
  Serial.begin(9600);  // Sætter baud rate i forhold til databladet for gps modul
  Serial3.begin(9600); //Sætter baud rate i forhold til databladet for gps modul| Serial 3 er defineret som pins tx3 pin 14 og rx3 pin 15. HUSK!!! At rx til tx og tx til rx, ellers kommunikere modulet ikke med boardet.
}

void loop()
{
  // Denne while loop tjekker om serial1 er available, hvilket egentlig betyder at den tjekker om GPS modulet får signal med satellit.
  while(Serial3.available()>0){ //Det der tjekker om gps har signal til satellit
    Serial.write(Serial3.read()); //Printer vores serial1, hvilket egentlig er det vi reciever fra gps
  }
}
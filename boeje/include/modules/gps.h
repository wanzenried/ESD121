#include <Arduino.h>
#include <stdlib.h>
#include <string.h>


void updateGps(float *latOutput, float *lonOutput);
float omregngrader(char *Koordinaterstring, char retning);
int nmeaparser(char *nmeasat, char *nmeaarr[]);
bool getkoordinater(char *ting[], char **latitude, char **longitude, char **nord, char **oest);
void nmeaprinter(char *nmeasat, float *latOuput, float *lonOutput);
void getnmeasat(float *latOutput, float *lonOutput);
void gpsSetup();
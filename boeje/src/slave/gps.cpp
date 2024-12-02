#include <slave/gps.h>



// Definere variabler vi skal bruge


float laengdegrad[50];
float breddegrad[50];
int taeller = 0;


// Omregner nmea format koordinat til decimal grader, da det er lettere at forstå for mennesker

void gpsSetup(){
    Serial.begin(9600);
}

void updateGps(float *latOutput, float *lonOutput)
{
    getnmeasat(latOutput, lonOutput);
}


float omregngrader(char *Koordinaterstring, char retning) {
    float koordinaterFLOAT = atof(Koordinaterstring); 

    int graderint = int(koordinaterFLOAT / 100); 

    float minutterFLOAT = koordinaterFLOAT - (graderint * 100);  

    float decimalgrader = graderint + (minutterFLOAT / 60.0);  

    if (retning == 'S' || retning == 'W') { 
        decimalgrader = -decimalgrader;
    }
    return decimalgrader;
}
// Funktioner der parser nmea sætning (Erstatter , med null-terminator for at lave en ny string)
int nmeaparser(char *nmeasat, char *nmeaarr[]) {

    int nmeataeller = 0;
    char *point = nmeasat;
    nmeaarr[nmeataeller++] = point;

    while (*point && nmeataeller < 10) {

        if (*point == ',') {

            *point = '\0';

            point++;

            nmeaarr[nmeataeller++] = point;

        } else {
            point++;
        }
    }
    return nmeataeller;
}
// Gemmer koordinater i arrays, og er bool for at lave return statement så man kan lave if statement
bool getkoordinater(char *ting[], char **latitude, char **longitude, char **nord, char **oest) {

    *latitude = ting[1];
    *longitude = ting[3];

    *nord = ting[2];
    *oest = ting[4];

    return (*latitude && **latitude && *longitude && **longitude && **nord && *nord && **oest && *oest);
}
// Gemmer decimal grader i arrays
/*void gemkoordinateriarray(float lat, float lon) {

    if (taeller < 50) {

        breddegrad[taeller] = lat;
        laengdegrad[taeller] = lon;
        taeller++;
    }
}*/
// Beregner gennemsnit af decimal grader gemt i array
/*void gennemsnitsberegner(float *genLat, float *genLon) {

    if (taeller == 0) {

        *genLat = 0;
        *genLon = 0;

        return;
    }

    float sumLat = 0, sumLon = 0;

    for (int i = 0; i < taeller; i++) {

        sumLat += breddegrad[i];
        sumLon += laengdegrad[i];
    }

    *genLat = sumLat / taeller;
    *genLon = sumLon / taeller;
}*/
//Printer gennemsnit... Duhhh
/*void printgennemsnit() {
    float genLat, genLon;
    gennemsnitsberegner(&genLat, &genLon);

    Serial.print("Gennemsnit Latitude: ");
    Serial.println(genLat, 6);

    Serial.print("Gennemsnit Longitude: ");
    Serial.println(genLon, 6);

    taeller = 0; 
}*/
//Samlede funktion der caller de fleste funktioner, for at printe koordinater + gennemsnit efter 50 samples eller når data bliver utilgængeligt
void nmeaprinter(char *nmeasat, float *latOutput, float *lonOutput) {

    if (strncmp(nmeasat, "$GPGLL", 6) == 0) { 

        //Serial.println(nmeasat);
        char *ting[10];
        nmeaparser(nmeasat, ting);

        char *latitude;
        char *longitude;
        char *nord;
        char *oest;

        if (getkoordinater(ting, &latitude, &longitude, &nord, &oest)) {

            *latOutput = omregngrader(latitude, nord[0]);
            *lonOutput = omregngrader(longitude, oest[0]);


            /*gemkoordinateriarray(latOutput, lonOutput);

            if (taeller == 50) { 
                printgennemsnit();
            }*/

        } /*else { 

            Serial.println("Faerre end 50 samples");

            if (taeller > 0) { 
                printgennemsnit();
            }
        } */
    }
}

void getnmeasat(float *latOutput, float *lonOutput) {

    static char nmea[100]; 
    static size_t tjekarray = 0;

    while (Serial.available() > 0) {
        char c = Serial.read(); 

        if (tjekarray < sizeof(nmea) - 1) {

            nmea[tjekarray] = c; 
            tjekarray++;

            if (c == '\n') { 

                nmea[tjekarray] = '\0'; 
                nmeaprinter(nmea, latOutput, lonOutput); 
                tjekarray = 0; 
            }
        } else {
            tjekarray = 0; 
        }
    }
}
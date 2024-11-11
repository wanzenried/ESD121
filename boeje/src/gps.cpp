#include<stdlib.h>
#include<string.h>
#include<Arduino.h>

float omregngrader(char *Koordinaterstring,char retning){ //Vi bruger en pointer til en array med forskellige strings, Det bliver mere klart jo længere ned vi kommer :)
  float koordinaterFLOAT = atof(Koordinaterstring); // Konverter string til float (Library)

  int graderint = int(koordinaterFLOAT/100); // vi får graderne ved at dividere med 100 grundet formatet. Vi konverter til integer fordi vi ikke gider at regne med float har :)

  float minutterFLOAT = koordinaterFLOAT-(graderint*100);//Adskille grader fra minutter.

  float decimalgrader = graderint+(minutterFLOAT/60.0); // Omregner nmea til decimal grader

  if (retning == 'S'||retning == 'W'){ // Dette kommer ikke til at blive brugt i vores eksempler, men det kan bruges til et mere generaliserbar produkt :)
    decimalgrader = -decimalgrader;
  }

  return decimalgrader;
  }

void nmeatjekker(char *nmeasat){
  if (strncmp(nmeasat, "$GPGLL", 6) == 0){// Den linje kode sammenligner med brug af (strncmp) som outputter 0 hvis det er ens. 6 taller tjekker hvor mange karaktere den skal tjekke. hvilket er 6 da der er 6 strings i $GPGLL. Vi tjekker denne da der er bestemt format i $gpgll formatet, som letgøre kodning
    Serial.print(nmeasat);
    char *ting[10]; // Et array af pointer der peger på hver begyndelse i en sætning. f.ek.s En sætning bliver gemt i arr[0], og en anden bliver gemt i arr[1]

    int tingtaeller = 0; // Tæller antallet af sætninger

    char *point = nmeasat; // Kigger på starten af nmea, og bliver brugt til at gentage igennem stringen.
    
    ting[tingtaeller++] = point; //Starter ved første sætning

    while(*point && tingtaeller < 10){ // Dette er sandt indtil at pointerne enten peger på en null terminator "\0" eller tingtaeller er større end 10.

      if(*point ==','){ // Hvis pointer finder noget i arrayet med et komma. Dette gøres fordi vi kender til hvordan nmea formatet ser ud. Kort forklaret bliver de forskellige parametre markeret ved et komma.

        *point = '\0'; // Udskifter komma med null terminator. Vi deferencer pointer da den skal skifte værdi i hukommelse
        point++; // Increment pointer fra null terminator

        ting[tingtaeller++] = point; // Incrementer point, så den starter ved en ny sætning
      }

      else{
        point++; // Bliver ved med at gå igennem sætningen hvis ikke den finder et komma
      }
    } 

    char *latitude = ting[1]; //Attribuere latitude til den anden sætning i ting array.
    char *nord = ting[2]; //Det samme som overstående kommentar, bare det 3 sætning i array
    char *longitude = ting[3]; // same bare fjerde
    char *oest = ting[4]; // Same bare femte

    if(latitude && *latitude && nord && *nord && longitude && *longitude && oest && *oest){ //Tjekker bare om pointers !=0 og at strings (dem uden snefnug*) har en værdi, altså de ikke er tomme. Vi bruger LOGisk AND da alle burde være sande

      float lat = omregngrader(latitude, nord[0]); // indsætter vores ting i omregngrader funktionen vi har kigget på før :)

      float longituder = omregngrader(longitude, oest[0]); // same som overstående

      Serial.println("Jeg er her ved indsamling af koordinater og print af omregning"); // Debugging

      Serial.println("Latitude: ");
      Serial.println(lat, 6);

      Serial.println("Longitude: ");
      Serial.println(longituder, 6);
    } 
    else{
      Serial.println("Ikke eksisterende data ENDNU"); // Hvis ikke if statement er sandt, så må det være fordi der ikke er nok data endnu
    }
  }
}

void setup()
{
  Serial.begin(9600);  // Sætter baud rate i forhold til databladet for gps modul
  Serial3.begin(9600); //Sætter baud rate i forhold til databladet for gps modul| Serial 3 er defineret som pins tx3 pin 14 og rx3 pin 15. HUSK!!! At rx til tx og tx til rx, ellers kommunikere gps modulet ikke med boardet.
}

void loop(){ // Det her segment er mega fucked og grimt at læse. Bare stol på kommentarene.
  static char nmea[100]; //et array der gemmer hvert enkel nmea karakter der bliver spyttet ud
  static size_t tjekarray=0; // Går igennem array, altså funktionen for dette er at tracke hvorhenne man er i nmea array

  while(Serial3.available()>0){ // While gps modulet kommunikere med arduino. Den er static for at beholde værdierne igennem forskellige funktioner. (LIdt ligesom matching i rust tror jeg);

    char c = Serial3.read(); // Gemmer det gps modulet spytter ud i en char variabel

    if(tjekarray<sizeof(nmea)-1){ //Så vi ikke går over vores nmea array 

      nmea[tjekarray]=c; //Går videre i array
      tjekarray++;

      if (c=='\n'){// Den her tjekker om nmea sætningen er slut, det var lidt et gæt om det var \n der viste slutningen.

        nmea[tjekarray]='\0'; // Null terminate stringen, altså gør at den bare tager en nmea protokol.

        nmeatjekker(nmea); // Får nmea ind igennem nmeatjekker funktionen

        tjekarray=0; // næste sætning
      }
    }
    else{
      tjekarray=0; //Er her for at vi ikke går over nmea array
    }
  }
}
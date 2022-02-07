#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/time.h>
#include <string.h>
#include "checkPara.h"

int main(int argc, char *argv[])
{
      char c, d;

      if( access( "/home/pi/E3dcGui/parameter.h", F_OK ) != -1){
        printf("Datei parameter.h ist vorhanden.\n");
      }
      else{
        printf("Fehler: parameter.h ist nicht vorhanden.\n        Du musst die Datei erstellen.\n");
        printf("Oder soll jetzt kopiert werden? [j/n] :");
        c = getchar();
        getchar();
        if(c == 'j' || c == 'J' || c == 'y' || c == 'Y'){
          system("cp parameter.h.temp parameter.h");
          printf("Die Datei wurde aus der 'parameter.h.temp' kopiert\n        und kann nun bearbeitet werden.\n");
        }
        else {
          printf("Du musst nun die Datei selber erstelln,\n        z.B. mit 'cp parameter.h.temp parameter.h'\n");
        }
      }

      if( access( "/home/pi/E3dcGui/parameterHM.h", F_OK ) != -1){
        printf("Datei parameterHM.h ist vorhanden.\n");
      }
      else{
        printf("Fehler: parameterHM.h ist nicht vorhanden.\n        Du musst die Datei erstellen.\n");
        printf("Oder soll jetzt kopiert werden? [j/n] :");
        d = getchar();
        if(d == 'j' || d == 'J' || d == 'y' || d == 'Y'){
          system("cp parameterHM.h.temp parameterHM.h");
          printf("Die Datei wurde aus der 'parameterHM.h.temp' kopiert\n        und kann nun bearbeitet werden.\n");
        }
        else {
          printf("Du musst nun die Datei selber erstelln,\n        z.B. mit 'cp parameterHM.h.temp parameterHM.h'\n");
        }
      }
      checkDefine(1);
}

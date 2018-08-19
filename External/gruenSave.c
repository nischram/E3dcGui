#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <time.h>
#include <linux/input.h>
#include <linux/ioctl.h>
#include <sys/sysinfo.h>
#include "../parameter.h"
#include "../Frame/framebuffer.c"
#include "../Frame/DrawImage.h"
#include "../Frame/DrawCorner.c"
#include "../funktion.h"
#include "Gruenbeck.h"

void readWriteSendGB(char *name, char *unit, char *para, int pos, int ise)
{
  char value[128];
  readGB(para, value);
  writeGruenRAM(pos, atof(value));
  if (GruenbeckHM == 1 && ise > 0){
    printsendHM(ise, value);
  }
  printf("\n%s: %s %s\n", name, value, unit);
}
void main()
{
  char value[128];
  readWriteSendGB("Verbrauch", "l", GB_CONSUMPTION, PosGB_CONSUMPTION, ISE_GB_CONSUMPTION);
  readWriteSendGB("Regenerationsschritt", " ", GB_CURRENT_REGENERATION, PosGB_CURRENT_REGENERATION, ISE_GB_CURRENT_REGENERATION);
  readWriteSendGB("Anlagenkapazitaet", "%", GB_PERCENT_REGENERATION, PosGB_PERCENT_REGENERATION, ISE_GB_PERCENT_REGENERATION);
  readWriteSendGB("Aktueller Druchfluss", "qm", GB_CURRENT_FLOW, PosGB_CURRENT_FLOW, ISE_GB_CURRENT_FLOW);
  readWriteSendGB("Restkapazitaet", "qm", GB_R_CAPACITY, PosGB_R_CAPACITY, ISE_GB_R_CAPACITY);
  readWriteSendGB("Letzte Regeneration vor", "Std", GB_LAST_REGENERATION, PosGB_LAST_REGENERATION, ISE_GB_LAST_REGENERATION);
  readWriteSendGB("Status letzte eMail", " ", GB_STATUS_EMAIL, PosGB_STATUS_EMAIL, ISE_GB_STATUS_EMAIL);
  snprintf(value, (size_t)8, "%i", OK);
  if (ISE_GB_PI_SEND_OK > 0){
    printsendHM(ISE_GB_PI_SEND_OK, value);
  }
}

/*
gcc -g -o start  start.c
*/
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "checkPara.h"
#include "Frame/touch.h"
#include "Frame/touch.c"
#include "Frame/framebuffer.c"
#include "Frame/DrawCorner.c"
#include "Frame/DrawImage.h"
#include "funktion.h"

int main()
{
  //Parameter einbinden, checken oder default setzen
	checkDefinePara(1);

  sleep(2);
  if(GUI == 1){
    if(E3DC_S10 ==1){
      system("/home/pi/E3dcGui/RscpMain &");
    }
    if (E3DC_LED == 1){
      system("sudo /home/pi/E3dcGui/External/LedMain &");
    }
    if (Homematic_GUI == 1){
      system("/home/pi/E3dcGui/screenSave &");
      system("/home/pi/E3dcGui/screenSaveHM &");
    }
    else
      system("/home/pi/E3dcGui/screenSave &");
    sleep (4);
    system("/home/pi/E3dcGui/GuiMain &");
    system("/home/pi/E3dcGui/watchdog &");
  }
  else{
    system("/home/pi/E3dcGui/RscpMain &");
    system("/home/pi/E3dcGui/watchdog &");
    if(WALLBOX_SEND == 1){
      system("/home/pi/E3dcGui/Rscp/wbCheckHM &");
    }
    if (E3DC_LED == 1){
      system("sudo /home/pi/E3dcGui/External/LedMain &");
    }
  }
}

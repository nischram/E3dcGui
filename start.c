/*
gcc -g -o start  start.c
*/
#include "parameter.h"

int main()
{
  sleep(4);
  if(GUI == 1){
    if(E3DC_S10 ==1){
      system("/home/pi/E3dcGui/RscpMain &");
    }
    system("/home/pi/E3dcGui/screenSave &");
    sleep (2);
    system("/home/pi/E3dcGui/GuiMain &");
    system("/home/pi/E3dcGui/watchdog &");
  }
  else{
    system("/home/pi/E3dcGui/RscpMain &");
    system("/home/pi/E3dcGui/watchdog &");
  }
}

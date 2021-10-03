#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/ioctl.h>
#include <sys/sysinfo.h>
#include "../checkPara.h"
#include "../Frame/touch.h"
#include "../Frame/touch.c"
#include "../Frame/framebuffer.c"
#include "../Frame/DrawSkala.c"
#include "../Frame/DrawLine.c"
#include "../Frame/DrawSOC.c"
#include "../Frame/DrawImage.h"
#include "../Frame/DrawNetImage.h"
#include "../Frame/DrawCorner.c"
#include "../funktion.h"
#include "actionCheckHM.h"

//####################################
int main(){
	//Parameter einbinden, checken oder default setzen
	checkDefinePara(0);

	char OUT[128];

//####################################################
	//Wallbox or EP-Reserve HM read and senden
	if(WALLBOX_ACTION == 1 || EP_RESERVE_ACTION == 1){
		while(1){
			actionCheckAll();
			sleep(HM_Intervall);
		}
	}
}
// /home/pi/E3dcGui/Rscp/RscpSet -wb -sonne 8 -BtCno -BbCyes -swPh
// /home/pi/E3dcGui/Rscp/RscpSet -ep 10000 3230 // Testaufruf. Dieser Aufruf schreibt 3230W in EP.

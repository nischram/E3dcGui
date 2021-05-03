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
#include "../parameter.h"
#include "../Frame/touch.h"
#include "../Frame/touch.c"
#ifdef FONT_8X8
	#include "../Frame/framebuffer.c"
#endif
#ifdef FONT_8X16
	#include "../Frame/framebuffer8x16.c"
#endif
#include "../Frame/DrawSkala.c"
#include "../Frame/DrawLine.c"
#include "../Frame/DrawSOC.c"
#include "../Frame/DrawImage.h"
#include "../Frame/DrawNetImage.h"
#include "../Frame/DrawCorner.c"
#include "../funktion.h"

//####################################
int main(){
	char OUT[128];
	char wallboxSendNow[20], wallboxSendMode[20], wallboxSendCurrent[20], wallboxSendBtC[20], wallboxSendBbC[20];
	char WbMode[24],WbBtC[24],WbBbC[24];
	int WbCurrent, wallboxSendTime = 0;
//####################################################
	//Wallbox HM read and senden
	if(WALLBOX_SEND == 1){
		while(1){
			read_HM(ISE_WB_SEND_NOW, 4, wallboxSendNow);
			if (strcmp ("true",wallboxSendNow) == 0){
				read_HM(ISE_WB_SEND_MODE, 4, wallboxSendMode);
				read_HM(ISE_WB_SEND_CURRENT, 2, wallboxSendCurrent);
				read_HM(ISE_WB_SEND_BTC, 4, wallboxSendBtC);
				read_HM(ISE_WB_SEND_BBC, 4, wallboxSendBbC);
				if(strcmp ("true",wallboxSendMode) == 0) snprintf (WbMode, (size_t)128, "-sonne");
				else snprintf (WbMode, (size_t)128, "-mix");
				WbCurrent = atoi(wallboxSendCurrent);
				if(strcmp ("true",wallboxSendBtC) == 0) snprintf (WbBtC, (size_t)128, "-BtCyes");
				else snprintf (WbBtC, (size_t)128, "-BtCno");
				if(strcmp ("true",wallboxSendBbC) == 0) snprintf (WbBbC, (size_t)128, "-BbCyes");
				else snprintf (WbBbC, (size_t)128, "-BbCno");
				snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpWb %s %i %s %s", WbMode, WbCurrent, WbBtC, WbBbC);
				system(OUT);
				//snprintf (OUT, (size_t)128, "true");
				printsendHM(ISE_WB_SEND_NOW, "false");
			}
			sleep(WALLBOX_INTERVAL);
		}
	}
}

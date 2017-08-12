/*
gcc -g -o screenSave  screenSave.c
*/

#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "parameter.h"
#include "parameterHM.h"
#include "Frame/touch.h"
#include "Frame/touch.c"
#include "Frame/framebuffer.c"
#include "Frame/DrawCorner.c"
#include "funktion.h"

int main()
{
	signal(SIGINT, INThandler);

	int  xres,yres;

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	int screenChange;

	system("fbset -fb /dev/fb0 -depth 16");    //Umschalten auf 16Bit Display
	system("setterm -cursor off");             //Courser Abschalten

	int screenShutdown = ShutdownRun;

	char Value[20];

	screenOn();

	int Screen[ScreenMAX];


	if (openTouchScreen() == 1)
		perror("error opening touch screen");

	getTouchScreenDetails(&screenXmin,&screenXmax,&screenYmin,&screenYmax);

	framebufferInitialize(&xres,&yres);

	scaleXvalue = ((float)screenXmax-screenXmin) / xres;
	printf ("X Scale Factor = %f\n", scaleXvalue);
	scaleYvalue = ((float)screenYmax-screenYmin) / yres;
	printf ("Y Scale Factor = %f\n", scaleYvalue);

	int buttonCordsSaveHalb[4] = {5,110,400,280};
	int	buttonSaveHalb= BUTTON_OFF;
	int buttonTimerSaveHalb = mymillis();
	int buttonCordsHMrefresh[4] = {760,440,20,20};
	int buttonTimerHMrefresh = mymillis();

	//Homematic Touch-Button
	int buttonCordsParty[4] = {S7-3,R2,Fw+6,21+3};
	int	buttonParty= BUTTON_OFF;
	int buttonTimerParty = mymillis();
	int buttonCordsKueche[4] = {(S6-3),(R2),(Fw+6),(21+3)};
	int	buttonKueche= BUTTON_OFF;
	int buttonTimerKueche = mymillis();
	int buttonCordsBrunnen[4] = {(S8-3),(R4),(Fw+6),(21+3)};
	int	buttonBrunnen= BUTTON_OFF;
	int buttonTimerBrunnen = mymillis();
	int buttonCordsMac[4] = {(S8-3),(R1),(Fw+6),(21+3)};
	int	buttonMac= BUTTON_OFF;
	int buttonTimerMac = mymillis();
	int buttonCordsOGWL[4] = {(S6-3),(R1),(Fw+6),(21+3)};
	int	buttonOGWL= BUTTON_OFF;
	int buttonTimerOGWL = mymillis();
	int buttonCordsJOGWST[4] = {(S7-3),(R1),(Fw+6),(21+3)};
	int	buttonJOGWST= BUTTON_OFF;
	int buttonTimerJOGWST = mymillis();

	while(1){
		getTouchSample(&rawX, &rawY, &rawPressure);
		scaledX = rawX/scaleXvalue;
		scaledY = rawY/scaleYvalue;

		Screen[ScreenChange] = readScreen(ScreenChange);
		screenChange = Screen[ScreenChange];
		Screen[ScreenShutdown] = readScreen(ScreenShutdown);
		screenShutdown = Screen[ScreenShutdown];

		switch(screenChange){
			case ScreenAktuell:{
				//siehe screnSave.c
				break; // ScreenAktuell
			}
			case ScreenLangzeit:{
				//siehe screnSave.c
				break; // ScreenLangzeit
			}
			case ScreenSetup:{
				//siehe screnSave.c
				break; // ScreenSetup
			}
			case ScreenMonitor:{
				//siehe screnSave.c
				break; // ScreenMonitor
			}
			case ScreenHM:{
				if((scaledX  > buttonCordsSaveHalb[X] && scaledX < (buttonCordsSaveHalb[X]+buttonCordsSaveHalb[W])) && (scaledY > buttonCordsSaveHalb[Y] && scaledY < (buttonCordsSaveHalb[Y]+buttonCordsSaveHalb[H]))){
					if (mymillis() - buttonTimerSaveHalb > 500)
					if(buttonSaveHalb){
						screenOff();
						buttonSaveHalb= BUTTON_OFF;
						buttonTimerSaveHalb = mymillis();
					}
					else{
						screenOn();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						buttonSaveHalb= BUTTON_ON;
						buttonTimerSaveHalb = mymillis();
					}
				}
				if((scaledX  > buttonCordsHMrefresh[X] && scaledX < (buttonCordsHMrefresh[X]+buttonCordsHMrefresh[W])) && (scaledY > buttonCordsHMrefresh[Y] && scaledY < (buttonCordsHMrefresh[Y]+buttonCordsHMrefresh[H]))){
					if (mymillis() - buttonTimerHMrefresh > 500){
						buttonTimerHMrefresh = mymillis();
						drawSquare(760,440,20,20,LIGHT_GREEN);
						drawCorner(760,440,20,20,WHITE);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenChange, ScreenHM);
					}
				}
				if((scaledX  > buttonCordsParty[X] && scaledX < (buttonCordsParty[X]+buttonCordsParty[W])) && (scaledY > buttonCordsParty[Y] && scaledY < (buttonCordsParty[Y]+buttonCordsParty[H]))){
					if (mymillis() - buttonTimerParty > 500)
					if(buttonParty){
						buttonParty= BUTTON_OFF;
						buttonTimerParty = mymillis();
						drawSquare(S7-3,R2,Fw+6,21+3,LTGREY);
						printsendHM(ISE_OGParty, "false");
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenChange, ScreenHM);
					}
					else{
						buttonParty= BUTTON_ON;
						buttonTimerParty = mymillis();
						drawSquare(S7-3,R2,Fw+6,21+3,LIGHT_GREEN);
						printsendHM(ISE_OGParty, "true");
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenChange, ScreenHM);
					}
				}
				if((scaledX  > buttonCordsKueche[X] && scaledX < (buttonCordsKueche[X]+buttonCordsKueche[W])) && (scaledY > buttonCordsKueche[Y] && scaledY < (buttonCordsKueche[Y]+buttonCordsKueche[H]))){
					if (mymillis() - buttonTimerKueche > 500){
						buttonTimerKueche = mymillis();
						drawSquare(S6-3,R2,Fw+6,21+3,LIGHT_GREEN);
						printsendHM(ISE_Send_OGKSpul, "true");
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenChange, ScreenHM);
					}
				}
				if((scaledX  > buttonCordsBrunnen[X] && scaledX < (buttonCordsBrunnen[X]+buttonCordsBrunnen[W])) && (scaledY > buttonCordsBrunnen[Y] && scaledY < (buttonCordsBrunnen[Y]+buttonCordsBrunnen[H]))){
					if (mymillis() - buttonTimerBrunnen > 500)
					if(buttonBrunnen){
						buttonBrunnen= BUTTON_OFF;
						buttonTimerBrunnen = mymillis();
						drawSquare(S8-3,R4,Fw+6,21+3,LTGREY);
						printsendHM(ISE_Brunnen, "false");
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenChange, ScreenHM);
					}
					else{
						buttonBrunnen= BUTTON_ON;
						buttonTimerBrunnen = mymillis();
						drawSquare(S8-3,R4,Fw+6,21+3,LIGHT_BLUE);
						printsendHM(ISE_Brunnen, "true");
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenChange, ScreenHM);
					}
				}
				if((scaledX  > buttonCordsMac[X] && scaledX < (buttonCordsMac[X]+buttonCordsMac[W])) && (scaledY > buttonCordsMac[Y] && scaledY < (buttonCordsMac[Y]+buttonCordsMac[H]))){
					if (mymillis() - buttonTimerMac > 500){
						buttonTimerMac = mymillis();
						char MacSchrank[20];
						read_HM(ISE_MacSchrankValue, 4, MacSchrank);
						if (strcmp ("true",MacSchrank) == 0){
							drawSquare(S8-3,R1,Fw+6,21+3,LIGHT_GREEN);
							printsendHM(ISE_MacSchrank, "false");
						}
						else if (strcmp ("fals",MacSchrank) == 0){
							drawSquare(S8-3,R1,Fw+6,21+3,LIGHT_RED);
							printsendHM(ISE_MacSchrank, "true");
						}
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenChange, ScreenHM);
					}
				}
				if((scaledX  > buttonCordsOGWL[X] && scaledX < (buttonCordsOGWL[X]+buttonCordsOGWL[W])) && (scaledY > buttonCordsOGWL[Y] && scaledY < (buttonCordsOGWL[Y]+buttonCordsOGWL[H]))){
					if (mymillis() - buttonTimerOGWL > 500){
						buttonTimerOGWL = mymillis();
						char OGWLicht[20];
						read_HM(ISE_OGWLicht, 1, OGWLicht);
						if ((strcmp ("1",OGWLicht) == 0) || (strcmp ("2",OGWLicht) == 0) || (strcmp ("3",OGWLicht) == 0)){
							drawSquare(S6-3,R1,Fw+6,21+3,GREY);
							printsendHM(ISE_OGWLicht, "0");
						}
						else if (strcmp ("0",OGWLicht) == 0){
							drawSquare(S6-3,R1,Fw+6,21+3,GREEN);
							printsendHM(ISE_OGWLicht, "1");
						}
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenChange, ScreenHM);
					}
				}
				if((scaledX  > buttonCordsJOGWST[X] && scaledX < (buttonCordsJOGWST[X]+buttonCordsJOGWST[W])) && (scaledY > buttonCordsJOGWST[Y] && scaledY < (buttonCordsJOGWST[Y]+buttonCordsJOGWST[H]))){
					if (mymillis() - buttonTimerJOGWST > 500){
						buttonTimerJOGWST = mymillis();
						char OGWJalSt[20];
						readJalou_HM(ISE_OGWJalSt, OGWJalSt);																//Jalousie Wert aus der Homematic Abfrage
						if (strcmp ("100",OGWJalSt) == 0){
							drawSquare(S7-3,R1,Fw+6,21+3,GREEN);
							printsendHM(ISE_OGWJalSt, "0.50");
						}
						else {
							drawSquare(S7-3,R1,Fw+6,21+3,GREY);
							printsendHM(ISE_OGWJalSt, "1.00");
						}
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenChange, ScreenHM);
					}
				}
				break; // ScreenHM
			}
			default:{
				//siehe screnSave.c
			}
		} // Switch screenChange
	}  //while
} //main

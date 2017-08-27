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

	//Umschalten auf 16Bit Display
	char OUT[128];
	snprintf (OUT, (size_t)128, "fbset -fb %s -depth 16", framebuffer_File);
	system(OUT);

	int screenShutdown = ShutdownRun;

	char Value[20];

	screenOn();

	int Screen[ScreenMAX];

	if(E3DC_S10 ==1)
		writeScreen(ScreenChange, ScreenAktuell);
	else if(Homematic_GUI == 1)
		writeScreen(ScreenChange, ScreenHM);
	else
		writeScreen(ScreenChange, ScreenMonitor);
	writeScreen(ScreenCounter, 0);
	writeScreen(ScreenSaver, false);
	writeScreen(ScreenShutdown, ShutdownRun);
	writeScreen(ScreenHistory, historyOff);
	makeHistory();

  writeLegende(SOC, true);
  writeLegende(Solar, true);
  writeLegende(Home, true);
  writeLegende(NetIn, true);
  writeLegende(NetOut, true);
  writeLegende(BatIn, false);
  writeLegende(BatOut, false);
	writeLegende(ADD, false);

	if (openTouchScreen() == 1)
		perror("error opening touch screen");

	getTouchScreenDetails(&screenXmin,&screenXmax,&screenYmin,&screenYmax);

	framebufferInitialize(&xres,&yres);

	scaleXvalue = ((float)screenXmax-screenXmin) / xres;
	printf ("X Scale Factor = %f\n", scaleXvalue);
	scaleYvalue = ((float)screenYmax-screenYmin) / yres;
	printf ("Y Scale Factor = %f\n", scaleYvalue);

	int buttonCordsSave[4] = {5,110,790,280};
	int buttonTimerSave = mymillis();
	int buttonCordsSaveHalb[4] = {5,110,400,280};
	int	buttonSaveHalb= BUTTON_OFF;
	int buttonTimerSaveHalb = mymillis();
	int buttonCordsAktuell[4] = {270,10,80,90};
	int buttonTimerAktuell = mymillis();
	int buttonCordsLangzeit[4] = {360,10,80,90};
	int buttonTimerLangzeit = mymillis();
	int buttonCordsSetup[4] = {180,10,80,90};
	int buttonTimerSetup = mymillis();
	int buttonCordsMonitor[4] = {450,10,80,90};
	int buttonTimerMonitor = mymillis();
	int buttonCordsHM[4] = {540,10,80,90};
	int buttonTimerHM = mymillis();
	int buttonCordsHistory[4] = {360,410,80,25};
	int buttonTimerHistory = mymillis();

	int buttonCordsSD[4] = {S1,R2-20,180,30};
	int buttonTimerSD = mymillis();
	int buttonCordsSDN[4] = {S1,R2-20,180,30};
	int buttonTimerSDN = mymillis();
	int buttonCordsSRS[4] = {S1,R3-20,180,30};
	int buttonTimerSRS = mymillis();
	int buttonCordsHRS[4] = {S1,R4-20,180,30};
	int buttonTimerHRS = mymillis();
	int buttonCordsBrigh25[4] = {(S4+105),(R1),(Fw+6),(21+3)};
	int buttonTimerBrigh25 = mymillis();
	int buttonCordsBrigh76[4] = {(S4+160),(R1),(Fw+6),(21+3)};
	int buttonTimerBrigh76 = mymillis();
	int buttonCordsBrigh150[4] = {(S4+215),(R1),(Fw+6),(21+3)};
	int buttonTimerBrigh150 = mymillis();
	int buttonCordsBrigh255[4] = {(S4+270),(R1),(Fw+6),(21+3)};
	int buttonTimerBrigh255 = mymillis();

	int buttonCordsLeSOC[4] = {364,438,50,30};
	int buttonTimerLeSOC = mymillis();
	int buttonCordsLeSolar[4] = {417,438,50,30};
	int buttonTimerLeSolar = mymillis();
	int buttonCordsLeHome[4] = {470,438,50,30};
	int buttonTimerLeHome = mymillis();
	int buttonCordsLeNetIn[4] = {523,438,50,30};
	int buttonTimerLeNetIn = mymillis();
	int buttonCordsLeNetOut[4] = {576,438,50,30};
	int buttonTimerLeNetOut = mymillis();
	int buttonCordsLeBatIn[4] = {629,438,50,30};
	int buttonTimerLeBatIn = mymillis();
	int buttonCordsLeBatOut[4] = {682,438,50,30};
	int buttonTimerLeBatOut = mymillis();
	int buttonCordsLeADD[4] = {735,438,50,30};
	int buttonTimerLeADD = mymillis();

	while(1){
		getTouchSample(&rawX, &rawY, &rawPressure);
		scaledX = rawX/scaleXvalue;
		scaledY = rawY/scaleYvalue;

		Screen[ScreenChange] = readScreen(ScreenChange);
		screenChange = Screen[ScreenChange];
		Screen[ScreenShutdown] = readScreen(ScreenShutdown);
		screenShutdown = Screen[ScreenShutdown];

		if(E3DC_S10 ==1){
			if((scaledX  > buttonCordsAktuell[X] && scaledX < (buttonCordsAktuell[X]+buttonCordsAktuell[W])) && (scaledY > buttonCordsAktuell[Y] && scaledY < (buttonCordsAktuell[Y]+buttonCordsAktuell[H]))){
				if (mymillis() - buttonTimerAktuell > 500){
					buttonTimerAktuell = mymillis();
					writeScreen(ScreenChange, ScreenAktuell);
					writeScreen(ScreenCounter, 0);
					writeScreen(ScreenSaver, false);
					writeScreen(ScreenShutdown, ShutdownRun);
				}
			}
			if((scaledX  > buttonCordsLangzeit[X] && scaledX < (buttonCordsLangzeit[X]+buttonCordsLangzeit[W])) && (scaledY > buttonCordsLangzeit[Y] && scaledY < (buttonCordsLangzeit[Y]+buttonCordsLangzeit[H]))){
				if (mymillis() - buttonTimerLangzeit > 500){
					buttonTimerLangzeit = mymillis();
					writeScreen(ScreenChange, ScreenLangzeit);
					writeScreen(ScreenCounter, 0);
					writeScreen(ScreenSaver, false);
					writeScreen(ScreenShutdown, ShutdownRun);
				}
			}
			if((scaledX  > buttonCordsMonitor[X] && scaledX < (buttonCordsMonitor[X]+buttonCordsMonitor[W])) && (scaledY > buttonCordsMonitor[Y] && scaledY < (buttonCordsMonitor[Y]+buttonCordsMonitor[H]))){
				if (mymillis() - buttonTimerMonitor > 500){
					buttonTimerMonitor = mymillis();
					writeScreen(ScreenChange, ScreenMonitor);
					writeScreen(ScreenCounter, 0);
					writeScreen(ScreenSaver, false);
					writeScreen(ScreenShutdown, ShutdownRun);
				}
			}
		}
		if((scaledX  > buttonCordsSetup[X] && scaledX < (buttonCordsSetup[X]+buttonCordsSetup[W])) && (scaledY > buttonCordsSetup[Y] && scaledY < (buttonCordsSetup[Y]+buttonCordsSetup[H]))){
			if (mymillis() - buttonTimerSetup > 500){
				buttonTimerSetup = mymillis();
				writeScreen(ScreenChange, ScreenSetup);
				writeScreen(ScreenCounter, 0);
				writeScreen(ScreenSaver, false);
				writeScreen(ScreenShutdown, ShutdownRun);
			}
		}

		if(Homematic_GUI == 1){
			if((scaledX  > buttonCordsHM[X] && scaledX < (buttonCordsHM[X]+buttonCordsHM[W])) && (scaledY > buttonCordsHM[Y] && scaledY < (buttonCordsHM[Y]+buttonCordsHM[H]))){
				if (mymillis() - buttonTimerHM > 500){
					buttonTimerHM = mymillis();
					writeScreen(ScreenChange, ScreenHM);
					writeScreen(ScreenCounter, 0);
					writeScreen(ScreenSaver, false);
					writeScreen(ScreenShutdown, ShutdownRun);
				}
			}
		}
		switch(screenChange){
			case ScreenAktuell:{
				if((scaledX  > buttonCordsSave[X] && scaledX < (buttonCordsSave[X]+buttonCordsSave[W])) && (scaledY > buttonCordsSave[Y] && scaledY < (buttonCordsSave[Y]+buttonCordsSave[H]))){
					if (mymillis() - buttonTimerSave > 500){
						buttonTimerSave = mymillis();
						int state = readScreen(ScreenState);
						if(state == ScreenOff){
							screenOn();
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
						}
						else{
							screenOff();
							writeScreen(ScreenSaver, true);
						}
					}
				}
				if (historyAktiv == true){
					if((scaledX  > buttonCordsHistory[X] && scaledX < (buttonCordsHistory[X]+buttonCordsHistory[W])) && (scaledY > buttonCordsHistory[Y] && scaledY < (buttonCordsHistory[Y]+buttonCordsHistory[H]))){
						if (mymillis() - buttonTimerHistory > 500){
							buttonTimerHistory = mymillis();
							Screen[ScreenHistory] = readScreen(ScreenHistory);
							if (Screen[ScreenHistory] == today){
								writeScreen(ScreenHistory, yesterday);
							}
							else if (Screen[ScreenHistory] == yesterday){
								writeScreen(ScreenHistory, historyOff);
							}
							else if (Screen[ScreenHistory] == historyOff){
								writeScreen(ScreenHistory, today);
							}
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
					}
				}
				break; // ScreenAktuell
			}
			case ScreenLangzeit:{
				if((scaledX  > buttonCordsSave[X] && scaledX < (buttonCordsSave[X]+buttonCordsSave[W])) && (scaledY > buttonCordsSave[Y] && scaledY < (buttonCordsSave[Y]+buttonCordsSave[H]))){
					if (mymillis() - buttonTimerSave > 500){
						buttonTimerSave = mymillis();
						int state = readScreen(ScreenState);
						if(state == ScreenOff){
							screenOn();
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
						}
						else{
							screenOff();
							writeScreen(ScreenSaver, true);
						}
					}
				}
				if((scaledX  > buttonCordsLeSOC[X] && scaledX < (buttonCordsLeSOC[X]+buttonCordsLeSOC[W])) && (scaledY > buttonCordsLeSOC[Y] && scaledY < (buttonCordsLeSOC[Y]+buttonCordsLeSOC[H]))){
					if (mymillis() - buttonTimerLeSOC > 300){
						buttonTimerLeSOC = mymillis();
						changeLegende(SOC);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsLeSolar[X] && scaledX < (buttonCordsLeSolar[X]+buttonCordsLeSolar[W])) && (scaledY > buttonCordsLeSolar[Y] && scaledY < (buttonCordsLeSolar[Y]+buttonCordsLeSolar[H]))){
					if (mymillis() - buttonTimerLeSolar > 300){
						buttonTimerLeSolar = mymillis();
						changeLegende(Solar);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsLeHome[X] && scaledX < (buttonCordsLeHome[X]+buttonCordsLeHome[W])) && (scaledY > buttonCordsLeHome[Y] && scaledY < (buttonCordsLeHome[Y]+buttonCordsLeHome[H]))){
					if (mymillis() - buttonTimerLeHome > 300){
						buttonTimerLeHome = mymillis();
						changeLegende(Home);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsLeNetIn[X] && scaledX < (buttonCordsLeNetIn[X]+buttonCordsLeNetIn[W])) && (scaledY > buttonCordsLeNetIn[Y] && scaledY < (buttonCordsLeNetIn[Y]+buttonCordsLeNetIn[H]))){
					if (mymillis() - buttonTimerLeNetIn > 300){
						buttonTimerLeNetIn = mymillis();
						changeLegende(NetIn);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsLeNetOut[X] && scaledX < (buttonCordsLeNetOut[X]+buttonCordsLeNetOut[W])) && (scaledY > buttonCordsLeNetOut[Y] && scaledY < (buttonCordsLeNetOut[Y]+buttonCordsLeNetOut[H]))){
					if (mymillis() - buttonTimerLeNetOut > 300){
						buttonTimerLeNetOut = mymillis();
						changeLegende(NetOut);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsLeBatIn[X] && scaledX < (buttonCordsLeBatIn[X]+buttonCordsLeBatIn[W])) && (scaledY > buttonCordsLeBatIn[Y] && scaledY < (buttonCordsLeBatIn[Y]+buttonCordsLeBatIn[H]))){
					if (mymillis() - buttonTimerLeBatIn > 300){
						buttonTimerLeBatIn = mymillis();
						changeLegende(BatIn);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsLeBatOut[X] && scaledX < (buttonCordsLeBatOut[X]+buttonCordsLeBatOut[W])) && (scaledY > buttonCordsLeBatOut[Y] && scaledY < (buttonCordsLeBatOut[Y]+buttonCordsLeBatOut[H]))){
					if (mymillis() - buttonTimerLeBatOut > 300){
						buttonTimerLeBatOut = mymillis();
						changeLegende(BatOut);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if(Additional == 1){
					if((scaledX  > buttonCordsLeADD[X] && scaledX < (buttonCordsLeADD[X]+buttonCordsLeADD[W])) && (scaledY > buttonCordsLeADD[Y] && scaledY < (buttonCordsLeADD[Y]+buttonCordsLeADD[H]))){
						if (mymillis() - buttonTimerLeADD > 300){
							buttonTimerLeADD = mymillis();
							changeLegende(ADD);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
						}
					}
				}
				break; // ScreenLangzeit
			}
			case ScreenSetup:{
				if(screenShutdown == ShutdownRun){
					if((scaledX  > buttonCordsSD[X] && scaledX < (buttonCordsSD[X]+buttonCordsSD[W])) && (scaledY > buttonCordsSD[Y] && scaledY < (buttonCordsSD[Y]+buttonCordsSD[H]))){
						if (mymillis() - buttonTimerSD > 600){
							buttonTimerSD = mymillis();
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownSD);
						}
					}
				} // if ScreenShutdownRun
				if((scaledX  > buttonCordsSRS[X] && scaledX < (buttonCordsSRS[X]+buttonCordsSRS[W])) && (scaledY > buttonCordsSRS[Y] && scaledY < (buttonCordsSRS[Y]+buttonCordsSRS[H]))){
					if (mymillis() - buttonTimerSRS > 3000){
						buttonTimerSRS = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						writeScreen(ScreenShutdown, ShutdownSRS);
						sleep (2);
						killall();
						system("/home/pi/E3dcGui/start &");
						drawSquare(2,2,800,480,LTGREY);
						system("killall -9 screenSave screenSaveHM");
						return 0;
					}
				}
				if((scaledX  > buttonCordsHRS[X] && scaledX < (buttonCordsHRS[X]+buttonCordsHRS[W])) && (scaledY > buttonCordsHRS[Y] && scaledY < (buttonCordsHRS[Y]+buttonCordsHRS[H]))){
					if (mymillis() - buttonTimerHRS > 3000){
						buttonTimerHRS = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						writeScreen(ScreenShutdown, ShutdownHRS);
						sleep (2);
						killall();
						drawSquare(2,2,800,480,BLACK);
						system("sudo reboot");
						system("killall -9 screenSave screenSaveHM");
						return 0;
					}
				}
				//Helligkeit
				if((scaledX  > buttonCordsBrigh25[X] && scaledX < (buttonCordsBrigh25[X]+buttonCordsBrigh25[W])) && (scaledY > buttonCordsBrigh25[Y] && scaledY < (buttonCordsBrigh25[Y]+buttonCordsBrigh25[H]))){
					if (mymillis() - buttonTimerBrigh25 > 500){
						buttonTimerBrigh25 = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						setBrightness(25);
					}
				}
				if((scaledX  > buttonCordsBrigh76[X] && scaledX < (buttonCordsBrigh76[X]+buttonCordsBrigh76[W])) && (scaledY > buttonCordsBrigh76[Y] && scaledY < (buttonCordsBrigh76[Y]+buttonCordsBrigh76[H]))){
					if (mymillis() - buttonTimerBrigh76 > 500){
						buttonTimerBrigh76 = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						setBrightness(76);
					}
				}
				if((scaledX  > buttonCordsBrigh150[X] && scaledX < (buttonCordsBrigh150[X]+buttonCordsBrigh150[W])) && (scaledY > buttonCordsBrigh150[Y] && scaledY < (buttonCordsBrigh150[Y]+buttonCordsBrigh150[H]))){
					if (mymillis() - buttonTimerBrigh150 > 500){
						buttonTimerBrigh150 = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						setBrightness(150);
					}
				}
				if((scaledX  > buttonCordsBrigh255[X] && scaledX < (buttonCordsBrigh255[X]+buttonCordsBrigh255[W])) && (scaledY > buttonCordsBrigh255[Y] && scaledY < (buttonCordsBrigh255[Y]+buttonCordsBrigh255[H]))){
					if (mymillis() - buttonTimerBrigh255 > 500){
						buttonTimerBrigh255 = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						setBrightness(255);
					}
				}//Ende Helligkeit
				if(screenShutdown == ShutdownSD){
					if((scaledX  > buttonCordsSDN[X] && scaledX < (buttonCordsSDN[X]+buttonCordsSDN[W])) && (scaledY > buttonCordsSDN[Y] && scaledY < (buttonCordsSDN[Y]+buttonCordsSDN[H]))){
						if (mymillis() - buttonTimerSDN > 3000){
							buttonTimerSDN = mymillis();
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownSDN);
							killall();
							sleep (2);
							drawSquare(2,2,800,480,BLACK);
							system("sudo shutdown -h now");
							system("killall -9 screenSave screenSaveHM");
							return 0;
						}
					}
				} // if screenShutdownSD
				break; // ScreenSetup
			}
			case ScreenMonitor:{
				if((scaledX  > buttonCordsSave[X] && scaledX < (buttonCordsSave[X]+buttonCordsSave[W])) && (scaledY > buttonCordsSave[Y] && scaledY < (buttonCordsSave[Y]+buttonCordsSave[H]))){
					if (mymillis() - buttonTimerSave > 500){
						buttonTimerSave = mymillis();
						int state = readScreen(ScreenState);
						if(state == ScreenOff){
							screenOn();
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
						}
						else{
							screenOff();
							writeScreen(ScreenSaver, true);
						}
					}
				}
				break; // ScreenMonitor
			}
			case ScreenHM:{
				// Alle Touchfuktionen für den Homematic-Scree werden in der Datei screenSaveHM.c weiter verarbeitet
				break; // ScreenHM
			}
			default:{
				writeScreen(ScreenChange, ScreenAktuell);
			}
		} // Switch screenChange
	}  //while
} //main

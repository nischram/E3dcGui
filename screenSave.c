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
	checkDefinePara(0);

	picturePosition();
	makeScreen();
	makeLegende();
	makeUnixtime();
	makeTo();
	writeTo(PosToEpSet,2000);

	signal(SIGINT, INThandler);

	int  xres,yres;

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	int screenChange;

	//Umschalten auf 16Bit Display
	char OUT[128];
	char WbMode[24],WbBtC[24],WbBbC[24];
	int WbCurrent;
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
		writeScreen(ScreenChange, ScreenSetup);   //writeScreen(ScreenChange, ScreenWetter);
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
	int buttonCordsSetup[4] = {Picture1,PictureLine1,PictureW,PictureH};
	int buttonTimerSetup = mymillis();
	int buttonCordsWetter[4] = {Picture2,PictureLine1,PictureW,PictureH};
	int buttonTimerWetter = mymillis();
	int buttonCordsAktuell[4] = {Picture3,PictureLine1,PictureW,PictureH};
	int buttonTimerAktuell = mymillis();
	int buttonCordsLangzeit[4] = {Picture4,PictureLine1,PictureW,PictureH};
	int buttonTimerLangzeit = mymillis();
	int buttonCordsWallbox[4] = {Picture5,PictureLine1,PictureW,PictureH};
	int buttonTimerWallbox = mymillis();
	int buttonCordsMonitor[4] = {Picture6,PictureLine1,PictureW,PictureH};
	int buttonTimerMonitor = mymillis();
	int buttonCordsSmart[4] = {Picture7,PictureLine1,PictureW,PictureH};
	int buttonTimerSmart = mymillis();
	int buttonCordsHM[4] = {Picture8,PictureLine1,PictureW,PictureH};
	int buttonTimerHM = mymillis();
	int buttonCordsGB[4] = {Picture9,PictureLine1,PictureW,PictureH};
	int buttonTimerGB = mymillis();
	int buttonCordsMuell[4] = {Picture10,PictureLine1,PictureW,PictureH};
	int buttonTimerMuell = mymillis();
	int buttonCordsHistory[4] = {360,410,80,25};
	int buttonTimerHistory = mymillis();

	int buttonCordsSD[4] = {SP1,RP2-20,180,30};
	int buttonTimerSD = mymillis();
	int buttonCordsSDN[4] = {SP1,RP2-20,180,30};
	int buttonTimerSDN = mymillis();
	int buttonCordsSRS[4] = {SP1,RP3-20,180,30};
	int buttonTimerSRS = mymillis();
	int buttonCordsHRS[4] = {SP1,RP4-20,180,30};
	int buttonTimerHRS = mymillis();
	int buttonCordsSStop[4] = {SP1,RP5-20,180,30};
	int buttonTimerSStop = mymillis();
	int buttonCordsBrigh25[4] = {(SP4+105),(RP1),(FPw+6),(21+3)};
	int buttonTimerBrigh25 = mymillis();
	int buttonCordsBrigh76[4] = {(SP4+160),(RP1),(FPw+6),(21+3)};
	int buttonTimerBrigh76 = mymillis();
	int buttonCordsBrigh150[4] = {(SP4+215),(RP1),(FPw+6),(21+3)};
	int buttonTimerBrigh150 = mymillis();
	int buttonCordsBrigh255[4] = {(SP4+270),(RP1),(FPw+6),(21+3)};
	int buttonTimerBrigh255 = mymillis();
	int buttonCordsPIR[4] = {(SP6-20), RP2,(FPw+6),(21+3)};
	int buttonTimerPIR = mymillis();

	int buttonCordsLeSOC[4] = {LF01,438,50,30};
	int buttonTimerLeSOC = mymillis();
	int buttonCordsLeSolar[4] = {LF02,438,50,30};
	int buttonTimerLeSolar = mymillis();
	int buttonCordsLeHome[4] = {LF03,438,50,30};
	int buttonTimerLeHome = mymillis();
	int buttonCordsLeNetIn[4] = {LF04,438,50,30};
	int buttonTimerLeNetIn = mymillis();
	int buttonCordsLeNetOut[4] = {LF05,438,50,30};
	int buttonTimerLeNetOut = mymillis();
	int buttonCordsLeBatIn[4] = {LF06,438,50,30};
	int buttonTimerLeBatIn = mymillis();
	int buttonCordsLeBatOut[4] = {LF07,438,50,30};
	int buttonTimerLeBatOut = mymillis();
	int buttonCordsLeADD[4] = {LF08,438,50,30};
	int buttonTimerLeADD = mymillis();
	int buttonCordsLeWbAll[4] = {LF09,438,50,30};
	int buttonTimerLeWbAll = mymillis();
	int buttonCordsLeWbSolar[4] = {LF10,438,50,30};
	int buttonTimerLeWbSolar = mymillis();

	int buttonCordsMinus10[4] = {WBCURX-58-58-58, WBCURY,54,30};
	int buttonTimerMinus10 = mymillis();
	int buttonCordsMinus2[4] = {WBCURX-58-58, WBCURY,54,30};
	int buttonTimerMinus2 = mymillis();
	int buttonCordsMinus[4] = {WBCURX-58, WBCURY,54,30};
	int buttonTimerMinus = mymillis();
	int buttonCordsPlus[4] = {WBCURX+82, WBCURY,54,30};
	int buttonTimerPlus = mymillis();
	int buttonCordsPlus2[4] = {WBCURX+82+58, WBCURY,54,30};
	int buttonTimerPlus2 = mymillis();
	int buttonCordsPlus10[4] = {WBCURX+82+58+58, WBCURY,54,30};
	int buttonTimerPlus10 = mymillis();
	int buttonCordsWbMode[4] = {WBMODEX,WBMODEY,82,30};
	int buttonTimerWbMode = mymillis();
	int buttonCordsWbBtC[4] = {WBBTCX,WBBTCY,82,30};
	int buttonTimerWbBtC = mymillis();
	int buttonCordsWbBbC[4] = {WBBBCX,WBBBCY,82,30};
	int buttonTimerWbBbC = mymillis();
	int buttonCordsWbSwPh[4] = {WBPHX,WBPHY,82,30};
	int buttonTimerWbSwPh = mymillis();
	int buttonCordsWbStop[4] = {WBSTOPX,WBSTOPY,82,30};
	int buttonTimerWbStop = mymillis();

	int buttonCordsMinus500[4] = {EPSETS1-2, EPSETR1-2,82,32};
	int buttonTimerMinus500 = mymillis();
	int buttonCordsPlus500[4] = {EPSETS3-2, EPSETR1-2,82,32};
	int buttonTimerPlus500 = mymillis();
	int buttonCordsMinus2000[4] = {EPSETS1-2, EPSETR2-2,82,32};
	int buttonTimerMinus2000 = mymillis();
	int buttonCordsPlus2000[4] = {EPSETS3-2, EPSETR2-2,82,32};
	int buttonTimerPlus2000 = mymillis();
	int buttonCordsMinus10000[4] = {EPSETS1-2, EPSETR3-2,82,32};
	int buttonTimerMinus10000 = mymillis();
	int buttonCordsPlus10000[4] = {EPSETS3-2, EPSETR3-2,82,32};
	int buttonTimerPlus10000 = mymillis();
	int buttonCordsEpSet[4] = {EPSETS2-2, EPSETR1-2,82,32};
	int buttonTimerEpSet = mymillis();
	int buttonCordsEpSetOnOff[4] = {EPSWX-2, EPSWY-2,82,32};
	int buttonTimerEpSetOnOff = mymillis();

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
			if(Wallbox ==1){
				if((scaledX  > buttonCordsWallbox[X] && scaledX < (buttonCordsWallbox[X]+buttonCordsWallbox[W])) && (scaledY > buttonCordsWallbox[Y] && scaledY < (buttonCordsWallbox[Y]+buttonCordsWallbox[H]))){
					if (mymillis() - buttonTimerWallbox > 500){
						buttonTimerWallbox = mymillis();
						writeScreen(ScreenChange, ScreenWallbox);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						writeScreen(ScreenShutdown, ShutdownRun);
					}
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
		if(wetterGui ==1){
			if((scaledX  > buttonCordsWetter[X] && scaledX < (buttonCordsWetter[X]+buttonCordsWetter[W])) && (scaledY > buttonCordsWetter[Y] && scaledY < (buttonCordsWetter[Y]+buttonCordsWetter[H]))){
				if (mymillis() - buttonTimerWetter > 500){
					buttonTimerWetter = mymillis();
					writeScreen(ScreenChange, ScreenWetter);
					writeScreen(ScreenCounter, 0);
					writeScreen(ScreenSaver, false);
					writeScreen(ScreenShutdown, ShutdownRun);
				}
			}
		}
		if(useAktor == 1 && useDHT == 1){
			if((scaledX  > buttonCordsSmart[X] && scaledX < (buttonCordsSmart[X]+buttonCordsSmart[W])) && (scaledY > buttonCordsSmart[Y] && scaledY < (buttonCordsSmart[Y]+buttonCordsSmart[H]))){
				if (mymillis() - buttonTimerSmart > 500){
					buttonTimerSmart = mymillis();
					writeScreen(ScreenChange, ScreenSmart);
					writeScreen(ScreenCounter, 0);
					writeScreen(ScreenSaver, false);
					writeScreen(ScreenShutdown, ShutdownRun);
				}
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
		if(Gruenbeck == 1){
			if((scaledX  > buttonCordsGB[X] && scaledX < (buttonCordsGB[X]+buttonCordsGB[W])) && (scaledY > buttonCordsGB[Y] && scaledY < (buttonCordsGB[Y]+buttonCordsGB[H]))){
				if (mymillis() - buttonTimerGB > 500){
					buttonTimerGB = mymillis();
					writeScreen(ScreenChange, ScreenGB);
					writeScreen(ScreenCounter, 0);
					writeScreen(ScreenSaver, false);
					writeScreen(ScreenShutdown, ShutdownRun);
				}
			}
		}
		if(Abfuhrkalender == 1){
			if((scaledX  > buttonCordsMuell[X] && scaledX < (buttonCordsMuell[X]+buttonCordsMuell[W])) && (scaledY > buttonCordsMuell[Y] && scaledY < (buttonCordsMuell[Y]+buttonCordsMuell[H]))){
				if (mymillis() - buttonTimerMuell > 500){
					buttonTimerMuell = mymillis();
					writeScreen(ScreenChange, ScreenMuell);
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
				if(Wallbox == 1){
					if((scaledX  > buttonCordsLeWbAll[X] && scaledX < (buttonCordsLeWbAll[X]+buttonCordsLeWbAll[W])) && (scaledY > buttonCordsLeWbAll[Y] && scaledY < (buttonCordsLeWbAll[Y]+buttonCordsLeWbAll[H]))){
						if (mymillis() - buttonTimerLeWbAll > 300){
							buttonTimerLeWbAll = mymillis();
							changeLegende(WbAll);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
						}
					}
					if((scaledX  > buttonCordsLeWbSolar[X] && scaledX < (buttonCordsLeWbSolar[X]+buttonCordsLeWbSolar[W])) && (scaledY > buttonCordsLeWbSolar[Y] && scaledY < (buttonCordsLeWbSolar[Y]+buttonCordsLeWbSolar[H]))){
						if (mymillis() - buttonTimerLeWbSolar > 300){
							buttonTimerLeWbSolar = mymillis();
							changeLegende(WbSolar);
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
						if(pidCheck("screenSaveHM") != 0) system("killall -9 screenSaveHM");
						if(pidCheck("screenSave") != 0) system("killall -9 screenSave");
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
						if(pidCheck("screenSaveHM") != 0) system("killall -9 screenSaveHM");
						if(pidCheck("screenSave") != 0) system("killall -9 screenSave");
						return 0;
					}
				}
				if((scaledX  > buttonCordsSStop[X] && scaledX < (buttonCordsSStop[X]+buttonCordsSStop[W])) && (scaledY > buttonCordsSStop[Y] && scaledY < (buttonCordsSStop[Y]+buttonCordsSStop[H]))){
					if (mymillis() - buttonTimerSStop > 3000){
						buttonTimerSStop = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						writeScreen(ScreenShutdown, ShutdownSStop);
						sleep (2);
						drawSquare(2,2,800,480,BLACK);
						system("/home/pi/E3dcGui/stop &");
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
				if((scaledX  > buttonCordsPIR[X] && scaledX < (buttonCordsPIR[X]+buttonCordsPIR[W])) && (scaledY > buttonCordsPIR[Y] && scaledY < (buttonCordsPIR[Y]+buttonCordsPIR[H]))){
					if (mymillis() - buttonTimerPIR > 500){
						buttonTimerPIR = mymillis();
						changePirUse();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}//Ende PIR
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
							if(pidCheck("screenSaveHM") != 0) system("killall -9 screenSaveHM");
							if(pidCheck("screenSave") != 0) system("killall -9 screenSave");
							return 0;
						}
					}
				} // if screenShutdownSD
				break; // ScreenSetup
			}
			case ScreenWetter:{
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
				break; // ScreenWetter
			}
			case ScreenSmart:{
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
				break; // ScreenSmart
			}
			case ScreenWallbox:{
				if((scaledX  > buttonCordsMinus10[X] && scaledX < (buttonCordsMinus10[X]+buttonCordsMinus10[W])) && (scaledY > buttonCordsMinus10[Y] && scaledY < (buttonCordsMinus10[Y]+buttonCordsMinus10[H]))){
					if (mymillis() - buttonTimerMinus10 > 500){
						buttonTimerMinus10 = mymillis();
						if((readRscpWb(PosWbCurrent)-10) >= 6){
							DrawImage("Switch/Send", WBCURX, WBCURY);
							WbCurrent = readRscpWb(PosWbCurrent) -10;
							if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
							else snprintf (WbMode, (size_t)128, "-sonne");
							if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
							else snprintf (WbBtC, (size_t)128, "-BtCyes");
							if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
							else snprintf (WbBbC, (size_t)128, "-BbCyes");
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -no &", WbMode, WbCurrent, WbBtC, WbBbC);
							system(OUT);
						}
					}
				}
				if((scaledX  > buttonCordsMinus2[X] && scaledX < (buttonCordsMinus2[X]+buttonCordsMinus2[W])) && (scaledY > buttonCordsMinus2[Y] && scaledY < (buttonCordsMinus2[Y]+buttonCordsMinus2[H]))){
					if (mymillis() - buttonTimerMinus2 > 500){
						buttonTimerMinus2 = mymillis();
						if((readRscpWb(PosWbCurrent)-2) >= 6){
							DrawImage("Switch/Send", WBCURX, WBCURY);
							WbCurrent = readRscpWb(PosWbCurrent) -2;
							if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
							else snprintf (WbMode, (size_t)128, "-sonne");
							if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
							else snprintf (WbBtC, (size_t)128, "-BtCyes");
							if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
							else snprintf (WbBbC, (size_t)128, "-BbCyes");
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -no &", WbMode, WbCurrent, WbBtC, WbBbC);
							system(OUT);
						}
					}
				}
				if((scaledX  > buttonCordsMinus[X] && scaledX < (buttonCordsMinus[X]+buttonCordsMinus[W])) && (scaledY > buttonCordsMinus[Y] && scaledY < (buttonCordsMinus[Y]+buttonCordsMinus[H]))){
					if (mymillis() - buttonTimerMinus > 500){
						buttonTimerMinus = mymillis();
						if((readRscpWb(PosWbCurrent)-1) >= 6){
							DrawImage("Switch/Send", WBCURX, WBCURY);
							WbCurrent = readRscpWb(PosWbCurrent) -1;
							if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
							else snprintf (WbMode, (size_t)128, "-sonne");
							if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
							else snprintf (WbBtC, (size_t)128, "-BtCyes");
							if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
							else snprintf (WbBbC, (size_t)128, "-BbCyes");
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -no &", WbMode, WbCurrent, WbBtC, WbBbC);
							system(OUT);
						}
					}
				}
				if((scaledX  > buttonCordsPlus[X] && scaledX < (buttonCordsPlus[X]+buttonCordsPlus[W])) && (scaledY > buttonCordsPlus[Y] && scaledY < (buttonCordsPlus[Y]+buttonCordsPlus[H]))){
					if (mymillis() - buttonTimerPlus > 500){
						buttonTimerPlus = mymillis();
						if((readRscpWb(PosWbCurrent)+1) <= 32){
							DrawImage("Switch/Send", WBCURX, WBCURY);
							WbCurrent = readRscpWb(PosWbCurrent) +1;
							if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
							else snprintf (WbMode, (size_t)128, "-sonne");
							if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
							else snprintf (WbBtC, (size_t)128, "-BtCyes");
							if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
							else snprintf (WbBbC, (size_t)128, "-BbCyes");
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -no &", WbMode, WbCurrent, WbBtC, WbBbC);
							system(OUT);
						}
					}
				}
				if((scaledX  > buttonCordsPlus2[X] && scaledX < (buttonCordsPlus2[X]+buttonCordsPlus2[W])) && (scaledY > buttonCordsPlus2[Y] && scaledY < (buttonCordsPlus2[Y]+buttonCordsPlus2[H]))){
					if (mymillis() - buttonTimerPlus2 > 500){
						buttonTimerPlus2 = mymillis();
						if((readRscpWb(PosWbCurrent)+2) <= 32){
							DrawImage("Switch/Send", WBCURX, WBCURY);
							WbCurrent = readRscpWb(PosWbCurrent) +2;
							if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
							else snprintf (WbMode, (size_t)128, "-sonne");
							if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
							else snprintf (WbBtC, (size_t)128, "-BtCyes");
							if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
							else snprintf (WbBbC, (size_t)128, "-BbCyes");
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -no &", WbMode, WbCurrent, WbBtC, WbBbC);
							system(OUT);
						}
					}
				}
				if((scaledX  > buttonCordsPlus10[X] && scaledX < (buttonCordsPlus10[X]+buttonCordsPlus10[W])) && (scaledY > buttonCordsPlus10[Y] && scaledY < (buttonCordsPlus10[Y]+buttonCordsPlus10[H]))){
					if (mymillis() - buttonTimerPlus10 > 500){
						buttonTimerPlus10 = mymillis();
						if((readRscpWb(PosWbCurrent)+2) <= 32){
							DrawImage("Switch/Send", WBCURX, WBCURY);
							WbCurrent = readRscpWb(PosWbCurrent) +10;
							if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
							else snprintf (WbMode, (size_t)128, "-sonne");
							if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
							else snprintf (WbBtC, (size_t)128, "-BtCyes");
							if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
							else snprintf (WbBbC, (size_t)128, "-BbCyes");
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -no &", WbMode, WbCurrent, WbBtC, WbBbC);
							system(OUT);
						}
					}
				}
				if((scaledX  > buttonCordsWbMode[X] && scaledX < (buttonCordsWbMode[X]+buttonCordsWbMode[W])) && (scaledY > buttonCordsWbMode[Y] && scaledY < (buttonCordsWbMode[Y]+buttonCordsWbMode[H]))){
					if (mymillis() - buttonTimerWbMode > 500){
						buttonTimerWbMode = mymillis();
						DrawImage("Switch/Send", WBMODEX, WBMODEY);
						WbCurrent = readRscpWb(PosWbCurrent);
						if(readRscpWb(PosWbMode)==1) snprintf (WbMode, (size_t)128, "-mix");
						else snprintf (WbMode, (size_t)128, "-sonne");
						if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
						else snprintf (WbBtC, (size_t)128, "-BtCyes");
						if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
						else snprintf (WbBbC, (size_t)128, "-BbCyes");
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -no &", WbMode, WbCurrent, WbBtC, WbBbC);
						system(OUT);
					}
				}
				if((scaledX  > buttonCordsWbBtC[X] && scaledX < (buttonCordsWbBtC[X]+buttonCordsWbBtC[W])) && (scaledY > buttonCordsWbBtC[Y] && scaledY < (buttonCordsWbBtC[Y]+buttonCordsWbBtC[H]))){
					if (mymillis() - buttonTimerWbBtC > 500){
						DrawImage("Switch/Send", WBBTCX, WBBTCY);
						WbCurrent = readRscpWb(PosWbCurrent);
						if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
						else snprintf (WbMode, (size_t)128, "-sonne");
						if(readRscpWb(PosWbBtC)==1) {
							snprintf (WbBtC, (size_t)128, "-BtCno");
							if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
							else snprintf (WbBbC, (size_t)128, "-BbCyes");
						}
						else {
							snprintf (WbBtC, (size_t)128, "-BtCyes");
							snprintf (WbBbC, (size_t)128, "-BbCno");
						}
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -no &", WbMode, WbCurrent, WbBtC, WbBbC);
						system(OUT);
						sleep(2);
						buttonTimerWbBtC = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						writeScreen(ScreenShutdown, ShutdownRun);
					}
				}
				if((scaledX  > buttonCordsWbBbC[X] && scaledX < (buttonCordsWbBbC[X]+buttonCordsWbBbC[W])) && (scaledY > buttonCordsWbBbC[Y] && scaledY < (buttonCordsWbBbC[Y]+buttonCordsWbBbC[H]))){
					if (mymillis() - buttonTimerWbBbC > 500){
						DrawImage("Switch/Send", WBBBCX, WBBBCY);
						WbCurrent = readRscpWb(PosWbCurrent);
						if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
						else snprintf (WbMode, (size_t)128, "-sonne");
						if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
						else snprintf (WbBtC, (size_t)128, "-BtCyes");
						if(readRscpWb(PosWbBbC)==1) snprintf (WbBbC, (size_t)128, "-BbCno");
						else {
							snprintf (WbBtC, (size_t)128, "-BtCno");
							snprintf (WbBbC, (size_t)128, "-BbCyes");
						}
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -no &", WbMode, WbCurrent, WbBtC, WbBbC);
						system(OUT);
						sleep(2);
						buttonTimerWbBbC = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						writeScreen(ScreenShutdown, ShutdownRun);
					}
				}
				if((scaledX  > buttonCordsWbSwPh[X] && scaledX < (buttonCordsWbSwPh[X]+buttonCordsWbSwPh[W])) && (scaledY > buttonCordsWbSwPh[Y] && scaledY < (buttonCordsWbSwPh[Y]+buttonCordsWbSwPh[H]))){
					if (mymillis() - buttonTimerWbSwPh > 500){
						DrawImage("Switch/Send", WBPHX, WBPHY);
						WbCurrent = readRscpWb(PosWbCurrent);
						if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
						else snprintf (WbMode, (size_t)128, "-sonne");
						if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
						else snprintf (WbBtC, (size_t)128, "-BtCyes");
						if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
						else snprintf (WbBbC, (size_t)128, "-BbCyes");
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -swPh &", WbMode, WbCurrent, WbBtC, WbBbC);
						system(OUT);
						sleep(2);
						buttonTimerWbSwPh = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						writeScreen(ScreenShutdown, ShutdownRun);
					}
				}
				if((scaledX  > buttonCordsWbStop[X] && scaledX < (buttonCordsWbStop[X]+buttonCordsWbStop[W])) && (scaledY > buttonCordsWbStop[Y] && scaledY < (buttonCordsWbStop[Y]+buttonCordsWbStop[H]))){
					if (mymillis() - buttonTimerWbStop > 500){
						if (readRscpWb(PosWbLED_BAT)==1){
							DrawImage("Switch/Send", WBSTOPX, WBSTOPY);
							WbCurrent = readRscpWb(PosWbCurrent);
							if(readRscpWb(PosWbMode)==0) snprintf (WbMode, (size_t)128, "-mix");
							else snprintf (WbMode, (size_t)128, "-sonne");
							if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
							else snprintf (WbBtC, (size_t)128, "-BtCyes");
							if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
							else snprintf (WbBbC, (size_t)128, "-BbCyes");
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s %s -stop &", WbMode, WbCurrent, WbBtC, WbBbC);
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbStop = mymillis();
					}
				}
				break; // ScreenWallbox
			}
			case ScreenMonitor:{
				if((scaledX  > buttonCordsSaveHalb[X] && scaledX < (buttonCordsSaveHalb[X]+buttonCordsSaveHalb[W])) && (scaledY > buttonCordsSaveHalb[Y] && scaledY < (buttonCordsSaveHalb[Y]+buttonCordsSaveHalb[H]))){
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
				if((scaledX  > buttonCordsMinus500[X] && scaledX < (buttonCordsMinus500[X]+buttonCordsMinus500[W])) && (scaledY > buttonCordsMinus500[Y] && scaledY < (buttonCordsMinus500[Y]+buttonCordsMinus500[H]))){
					if (mymillis() - buttonTimerMinus500 > 500){
						buttonTimerMinus500 = mymillis();
						if(readTo(PosToEpSet)-500 >= 0){
							writeTo(PosToEpSet,readTo(PosToEpSet)-500);
						}
					}
				}
				if((scaledX  > buttonCordsPlus500[X] && scaledX < (buttonCordsPlus500[X]+buttonCordsPlus500[W])) && (scaledY > buttonCordsPlus500[Y] && scaledY < (buttonCordsPlus500[Y]+buttonCordsPlus500[H]))){
					if (mymillis() - buttonTimerPlus500 > 500){
						buttonTimerPlus500 = mymillis();
						if(readTo(PosToEpSet)+500 <= readTo(PosToEpMax)){
							writeTo(PosToEpSet,readTo(PosToEpSet)+500);
						}
					}
				}
				if((scaledX  > buttonCordsMinus2000[X] && scaledX < (buttonCordsMinus2000[X]+buttonCordsMinus2000[W])) && (scaledY > buttonCordsMinus2000[Y] && scaledY < (buttonCordsMinus2000[Y]+buttonCordsMinus2000[H]))){
					if (mymillis() - buttonTimerMinus2000 > 2000){
						buttonTimerMinus2000 = mymillis();
						if(readTo(PosToEpSet)-2000 >= 0){
							writeTo(PosToEpSet,readTo(PosToEpSet)-2000);
						}
					}
				}
				if((scaledX  > buttonCordsPlus2000[X] && scaledX < (buttonCordsPlus2000[X]+buttonCordsPlus2000[W])) && (scaledY > buttonCordsPlus2000[Y] && scaledY < (buttonCordsPlus2000[Y]+buttonCordsPlus2000[H]))){
					if (mymillis() - buttonTimerPlus2000 > 500){
						buttonTimerPlus2000 = mymillis();
						if(readTo(PosToEpSet)+2000 <= readTo(PosToEpMax)){
							writeTo(PosToEpSet,readTo(PosToEpSet)+2000);
						}
					}
				}
				if((scaledX  > buttonCordsMinus10000[X] && scaledX < (buttonCordsMinus10000[X]+buttonCordsMinus10000[W])) && (scaledY > buttonCordsMinus10000[Y] && scaledY < (buttonCordsMinus10000[Y]+buttonCordsMinus10000[H]))){
					if (mymillis() - buttonTimerMinus10000 > 10000){
						buttonTimerMinus10000 = mymillis();
						if(readTo(PosToEpSet)-10000 >= 0){
							writeTo(PosToEpSet,readTo(PosToEpSet)-10000);
						}
					}
				}
				if((scaledX  > buttonCordsPlus10000[X] && scaledX < (buttonCordsPlus10000[X]+buttonCordsPlus10000[W])) && (scaledY > buttonCordsPlus10000[Y] && scaledY < (buttonCordsPlus10000[Y]+buttonCordsPlus10000[H]))){
					if (mymillis() - buttonTimerPlus10000 > 500){
						buttonTimerPlus10000 = mymillis();
						if(readTo(PosToEpSet)+10000 <= readTo(PosToEpMax)){
							writeTo(PosToEpSet,readTo(PosToEpSet)+10000);
						}
					}
				}
				if((scaledX  > buttonCordsEpSet[X] && scaledX < (buttonCordsEpSet[X]+buttonCordsEpSet[W])) && (scaledY > buttonCordsEpSet[Y] && scaledY < (buttonCordsEpSet[Y]+buttonCordsEpSet[H]))){
					if (mymillis() - buttonTimerEpSet > 500){
						buttonTimerEpSet = mymillis();
						DrawImage("Switch/Send", EPSETS2, EPSETR1);
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -ep %i %i &", readRscp(PosEpReservMaxW),readTo(PosToEpSet));
						system(OUT);
					}
				}
				if((scaledX  > buttonCordsEpSetOnOff[X] && scaledX < (buttonCordsEpSetOnOff[X]+buttonCordsEpSetOnOff[W])) && (scaledY > buttonCordsEpSetOnOff[Y] && scaledY < (buttonCordsEpSetOnOff[Y]+buttonCordsEpSetOnOff[H]))){
					if (mymillis() - buttonTimerEpSetOnOff > 500){
						buttonTimerEpSetOnOff = mymillis();
						DrawImage("Switch/Send", EPSETS2, EPSETR1);
						if(readRscp(PosEpReservW) == 0){
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -ep %i %i &", readRscp(PosEpReservMaxW),readTo(PosToEpSet));
						}
						else {
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -ep %i %i &", readRscp(PosEpReservMaxW),0);
						}
						system(OUT);
					}
				}
				break; // ScreenMonitor
			}
			case ScreenHM:{
				// Alle Touchfuktionen für den Homematic-Scree werden in der Datei screenSaveHM.c weiter verarbeitet
				break; // ScreenHM
			}
			case ScreenGB:{
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
				break; // ScreenGB
			}
			case ScreenMuell:{
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
				break; // ScreenMuell
			}
			default:{
				writeScreen(ScreenChange, ScreenAktuell);
			}
		} // Switch screenChange
	}  //while
} //main

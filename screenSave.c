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
#include <unistd.h>
#include <sys/sysinfo.h>
#include <math.h>
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
	writeTo(PosToBlCharge, round(readRscp(PosBlCharge)/500)*500);
	writeTo(PosToBlDischarge, round(readRscp(PosBlDischarge)/500)*500);
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
	char WbMode[24],WbBtC[24],WbBbC[24],WbMode[24];
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
	int buttonCordsFunktion[4] = {Picture6,PictureLine1,PictureW,PictureH};
	int buttonTimerFunktion = mymillis();
	int buttonCordsMonitor[4] = {Picture7,PictureLine1,PictureW,PictureH};
	int buttonTimerMonitor = mymillis();
	int buttonCordsSmart[4] = {Picture8,PictureLine1,PictureW,PictureH};
	int buttonTimerSmart = mymillis();
	int buttonCordsHM[4] = {Picture9,PictureLine1,PictureW,PictureH};
	int buttonTimerHM = mymillis();
	int buttonCordsGB[4] = {Picture10,PictureLine1,PictureW,PictureH};
	int buttonTimerGB = mymillis();
	int buttonCordsMuell[4] = {Picture11,PictureLine1,PictureW,PictureH};
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
	int buttonCordsWbPer90[4] = {WBPERLX,WBPER9Y,54,30};
	int buttonTimerWbPer90 = mymillis();
	int buttonCordsWbPer80[4] = {WBPERRX,WBPER9Y,54,30};
	int buttonTimerWbPer80 = mymillis();
	int buttonCordsWbPer70[4] = {WBPERLX,WBPER7Y,54,30};
	int buttonTimerWbPer70 = mymillis();
	int buttonCordsWbPer60[4] = {WBPERRX,WBPER7Y,54,30};
	int buttonTimerWbPer60 = mymillis();
	int buttonCordsWbPer50[4] = {WBPERLX,WBPER5Y,54,30};
	int buttonTimerWbPer50 = mymillis();
	int buttonCordsWbPer40[4] = {WBPERRX,WBPER5Y,54,30};
	int buttonTimerWbPer40 = mymillis();
	int buttonCordsWbPer30[4] = {WBPERLX,WBPER3Y,54,30};
	int buttonTimerWbPer30 = mymillis();
	int buttonCordsWbPer20[4] = {WBPERRX,WBPER3Y,54,30};
	int buttonTimerWbPer20 = mymillis();
	int buttonCordsWbPer10[4] = {WBPERLX,WBPER1Y,54,30};
	int buttonTimerWbPer10 = mymillis();
	int buttonCordsWbPer0[4] = {WBPERRX,WBPER1Y,54,30};
	int buttonTimerWbPer0 = mymillis();
	int buttonCordsWbBaM[4] = {WBMODEX+58+58-3,WBMODEY,54,30};
	int buttonTimerWbBaM = mymillis();

	int buttonCordsBlSetOnOff[4] = {BLS2-2, BLR1-2,82,32};
	int buttonTimerBlSetOnOff = mymillis();
	int buttonCordsBlCSet[4] = {BLS3-2, BLR4-2,82,32};
	int buttonTimerBlCSet = mymillis();
	int buttonCordsBlDSet[4] = {BLS3-2, BLR7-2,82,32};
	int buttonTimerBlDSet = mymillis();
	int buttonCordsBlCMinus500[4] = {BLS1-2, BLR3-2,82,32};
	int buttonTimerBlCMinus500 = mymillis();
	int buttonCordsBlCPlus500[4] = {BLS2-2, BLR3-2,82,32};
	int buttonTimerBlCPlus500 = mymillis();
	int buttonCordsBlCMinus2000[4] = {BLS1-2, BLR4-2,82,32};
	int buttonTimerBlCMinus2000 = mymillis();
	int buttonCordsBlCPlus2000[4] = {BLS2-2, BLR4-2,82,32};
	int buttonTimerBlCPlus2000 = mymillis();
	int buttonCordsBlDMinus500[4] = {BLS1-2, BLR6-2,82,32};
	int buttonTimerBlDMinus500 = mymillis();
	int buttonCordsBlDPlus500[4] = {BLS2-2, BLR6-2,82,32};
	int buttonTimerBlDPlus500 = mymillis();
	int buttonCordsBlDMinus2000[4] = {BLS1-2, BLR7-2,82,32};
	int buttonTimerBlDMinus2000 = mymillis();
	int buttonCordsBlDPlus2000[4] = {BLS2-2, BLR7-2,82,32};
	int buttonTimerBlDPlus2000 = mymillis();

	int buttonCordsEpMinus500[4] = {EPSETS1-2, EPSETR1-2,82,32};
	int buttonTimerEpMinus500 = mymillis();
	int buttonCordsEpPlus500[4] = {EPSETS3-2, EPSETR1-2,82,32};
	int buttonTimerEpPlus500 = mymillis();
	int buttonCordsEpMinus2000[4] = {EPSETS1-2, EPSETR2-2,82,32};
	int buttonTimerEpMinus2000 = mymillis();
	int buttonCordsEpPlus2000[4] = {EPSETS3-2, EPSETR2-2,82,32};
	int buttonTimerEpPlus2000 = mymillis();
	int buttonCordsEpMinus10000[4] = {EPSETS1-2, EPSETR3-2,82,32};
	int buttonTimerEpMinus10000 = mymillis();
	int buttonCordsEpPlus10000[4] = {EPSETS3-2, EPSETR3-2,82,32};
	int buttonTimerEpPlus10000 = mymillis();
	int buttonCordsEpSet[4] = {EPSETS2-2, EPSETR1-2,82,32};
	int buttonTimerEpSet = mymillis();
	int buttonCordsEpSetOnOff[4] = {EPSWX-2, EPSWY-2,82,32};
	int buttonTimerEpSetOnOff = mymillis();

	int buttonCordsIdleMonCharge[4] = {ISP1-2-4, IRC+IB-2,82,32};
	int buttonTimerIdleMonCharge = mymillis();
	int buttonCordsIdleTueCharge[4] = {ISP2-2-4, IRC+IB-2,82,32};
	int buttonTimerIdleTueCharge = mymillis();
	int buttonCordsIdleWedCharge[4] = {ISP3-2-4, IRC+IB-2,82,32};
	int buttonTimerIdleWedCharge = mymillis();
	int buttonCordsIdleThuCharge[4] = {ISP4-2-4, IRC+IB-2,82,32};
	int buttonTimerIdleThuCharge = mymillis();
	int buttonCordsIdleFriCharge[4] = {ISP5-2-4, IRC+IB-2,82,32};
	int buttonTimerIdleFriCharge = mymillis();
	int buttonCordsIdleSatCharge[4] = {ISP6-2-4, IRC+IB-2,82,32};
	int buttonTimerIdleSatCharge = mymillis();
	int buttonCordsIdleSunCharge[4] = {ISP7-2-4, IRC+IB-2,82,32};
	int buttonTimerIdleSunCharge = mymillis();
	int buttonCordsIdleMonDischarge[4] = {ISP1-2-4, IRD+IB-2,82,32};
	int buttonTimerIdleMonDischarge = mymillis();
	int buttonCordsIdleTueDischarge[4] = {ISP2-2-4, IRD+IB-2,82,32};
	int buttonTimerIdleTueDischarge = mymillis();
	int buttonCordsIdleWedDischarge[4] = {ISP3-2-4, IRD+IB-2,82,32};
	int buttonTimerIdleWedDischarge = mymillis();
	int buttonCordsIdleThuDischarge[4] = {ISP4-2-4, IRD+IB-2,82,32};
	int buttonTimerIdleThuDischarge = mymillis();
	int buttonCordsIdleFriDischarge[4] = {ISP5-2-4, IRD+IB-2,82,32};
	int buttonTimerIdleFriDischarge = mymillis();
	int buttonCordsIdleSatDischarge[4] = {ISP6-2-4, IRD+IB-2,82,32};
	int buttonTimerIdleSatDischarge = mymillis();
	int buttonCordsIdleSunDischarge[4] = {ISP7-2-4, IRD+IB-2,82,32};
	int buttonTimerIdleSunDischarge = mymillis();

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
			if((scaledX  > buttonCordsFunktion[X] && scaledX < (buttonCordsFunktion[X]+buttonCordsFunktion[W])) && (scaledY > buttonCordsFunktion[Y] && scaledY < (buttonCordsFunktion[Y]+buttonCordsFunktion[H]))){
				if (mymillis() - buttonTimerFunktion > 500){
					buttonTimerFunktion = mymillis();
					if (readScreen(ScreenChange) != ScreenFunktion) writeScreen(ScreenChange, ScreenFunktion);
					else writeScreen(ScreenChange, ScreenIdlePeriods);
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
		if(useAktor == 1 || useDHT == 1){
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
					if (mymillis() - buttonTimerSRS > 2000){
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
					if (mymillis() - buttonTimerHRS > 2000){
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
					if (mymillis() - buttonTimerSStop > 2000){
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
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i -no &", WbMode, WbCurrent);
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
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i -no &", WbMode, WbCurrent);
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
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i -no &", WbMode, WbCurrent);
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
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i -no &", WbMode, WbCurrent);
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
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i -no &", WbMode, WbCurrent);
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
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i -no &", WbMode, WbCurrent);
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
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i -no &", WbMode, WbCurrent);
						system(OUT);
					}
				}
				if((scaledX  > buttonCordsWbBtC[X] && scaledX < (buttonCordsWbBtC[X]+buttonCordsWbBtC[W])) && (scaledY > buttonCordsWbBtC[Y] && scaledY < (buttonCordsWbBtC[Y]+buttonCordsWbBtC[H]))){
					if (mymillis() - buttonTimerWbBtC > 500){
						DrawImage("Switch/Send", WBBTCX, WBBTCY);
						if(readRscpWb(PosWbBtC)==1) {
							snprintf (WbBtC, (size_t)128, "-BtCno");
							if(readRscpWb(PosWbBbC)==0) snprintf (WbBbC, (size_t)128, "-BbCno");
							else snprintf (WbBbC, (size_t)128, "-BbCyes");
						}
						else {
							snprintf (WbBtC, (size_t)128, "-BtCyes");
							snprintf (WbBbC, (size_t)128, "-BbCno");
						}
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbEMS %s %s &", WbBtC, WbBbC);
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
						if(readRscpWb(PosWbBtC)==0) snprintf (WbBtC, (size_t)128, "-BtCno");
						else snprintf (WbBtC, (size_t)128, "-BtCyes");
						if(readRscpWb(PosWbBbC)==1) snprintf (WbBbC, (size_t)128, "-BbCno");
						else {
							snprintf (WbBtC, (size_t)128, "-BtCno");
							snprintf (WbBbC, (size_t)128, "-BbCyes");
						}
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbEMS %s %s &", WbBtC, WbBbC);
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
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i -swPh &", WbMode, WbCurrent);
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
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i -stop &", WbMode, WbCurrent);
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbStop = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer90[X] && scaledX < (buttonCordsWbPer90[X]+buttonCordsWbPer90[W])) && (scaledY > buttonCordsWbPer90[Y] && scaledY < (buttonCordsWbPer90[Y]+buttonCordsWbPer90[H]))){
					if (mymillis() - buttonTimerWbPer90 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERLX-24, WBPER9Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 90 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer90 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer80[X] && scaledX < (buttonCordsWbPer80[X]+buttonCordsWbPer80[W])) && (scaledY > buttonCordsWbPer80[Y] && scaledY < (buttonCordsWbPer80[Y]+buttonCordsWbPer80[H]))){
					if (mymillis() - buttonTimerWbPer80 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERRX, WBPER9Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 80 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer80 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer70[X] && scaledX < (buttonCordsWbPer70[X]+buttonCordsWbPer70[W])) && (scaledY > buttonCordsWbPer70[Y] && scaledY < (buttonCordsWbPer70[Y]+buttonCordsWbPer70[H]))){
					if (mymillis() - buttonTimerWbPer70 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERLX-24, WBPER7Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 70 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer70 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer60[X] && scaledX < (buttonCordsWbPer60[X]+buttonCordsWbPer60[W])) && (scaledY > buttonCordsWbPer60[Y] && scaledY < (buttonCordsWbPer60[Y]+buttonCordsWbPer60[H]))){
					if (mymillis() - buttonTimerWbPer60 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERRX, WBPER7Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 60 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer60 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer50[X] && scaledX < (buttonCordsWbPer50[X]+buttonCordsWbPer50[W])) && (scaledY > buttonCordsWbPer50[Y] && scaledY < (buttonCordsWbPer50[Y]+buttonCordsWbPer50[H]))){
					if (mymillis() - buttonTimerWbPer50 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERLX-24, WBPER5Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 50 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer50 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer40[X] && scaledX < (buttonCordsWbPer40[X]+buttonCordsWbPer40[W])) && (scaledY > buttonCordsWbPer40[Y] && scaledY < (buttonCordsWbPer40[Y]+buttonCordsWbPer40[H]))){
					if (mymillis() - buttonTimerWbPer40 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERRX, WBPER5Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 40 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer40 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer30[X] && scaledX < (buttonCordsWbPer30[X]+buttonCordsWbPer30[W])) && (scaledY > buttonCordsWbPer30[Y] && scaledY < (buttonCordsWbPer30[Y]+buttonCordsWbPer30[H]))){
					if (mymillis() - buttonTimerWbPer30 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERLX-24, WBPER3Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 30 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer30 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer20[X] && scaledX < (buttonCordsWbPer20[X]+buttonCordsWbPer20[W])) && (scaledY > buttonCordsWbPer20[Y] && scaledY < (buttonCordsWbPer20[Y]+buttonCordsWbPer20[H]))){
					if (mymillis() - buttonTimerWbPer20 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERRX, WBPER3Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 20 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer20 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer10[X] && scaledX < (buttonCordsWbPer10[X]+buttonCordsWbPer10[W])) && (scaledY > buttonCordsWbPer10[Y] && scaledY < (buttonCordsWbPer10[Y]+buttonCordsWbPer10[H]))){
					if (mymillis() - buttonTimerWbPer10 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERLX-24, WBPER1Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 10 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer10 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbPer0[X] && scaledX < (buttonCordsWbPer0[X]+buttonCordsWbPer0[W])) && (scaledY > buttonCordsWbPer0[Y] && scaledY < (buttonCordsWbPer0[Y]+buttonCordsWbPer0[H]))){
					if (mymillis() - buttonTimerWbPer0 > 500){
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/Send", WBPERRX, WBPER1Y);
							snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbUntil 0 &");
							system(OUT);
							sleep(2);
							writeScreen(ScreenCounter, 0);
							writeScreen(ScreenSaver, false);
							writeScreen(ScreenShutdown, ShutdownRun);
						}
						buttonTimerWbPer0 = mymillis();
					}
				}
				if((scaledX  > buttonCordsWbBaM[X] && scaledX < (buttonCordsWbBaM[X]+buttonCordsWbBaM[W])) && (scaledY > buttonCordsWbBaM[Y] && scaledY < (buttonCordsWbBaM[Y]+buttonCordsWbBaM[H]))){
					if (mymillis() - buttonTimerWbBaM > 500){
						DrawImage("Switch/Send", WBMODEX+58+58-3, WBMODEY);
						if(readRscp(PosWbBaM)==0) snprintf (WbBaM, (size_t)128, "-BaMon");
						else snprintf (WbBaM, (size_t)128, "-BaMoff");
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbBaM %s &", WbBaM);
						system(OUT);
						sleep(2);
						buttonTimerWbBaM = mymillis();
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
						writeScreen(ScreenShutdown, ShutdownRun);
					}
				}
				break; // ScreenWallbox
			}
			case ScreenFunktion:{
				//Battery-Limits
				if((scaledX  > buttonCordsBlSetOnOff[X] && scaledX < (buttonCordsBlSetOnOff[X]+buttonCordsBlSetOnOff[W])) && (scaledY > buttonCordsBlSetOnOff[Y] && scaledY < (buttonCordsBlSetOnOff[Y]+buttonCordsBlSetOnOff[H]))){
					if (mymillis() - buttonTimerBlSetOnOff > 500){
						buttonTimerBlSetOnOff = mymillis();
						DrawImage("Switch/Send", BLS2, BLR1);
						if(readRscp(PosBlUsed)==0) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -bl -blYes %i %i &", readTo(PosToBlCharge),readTo(PosToBlDischarge));
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -bl -blNo %i %i &", readTo(PosToBlCharge),readTo(PosToBlDischarge));
						printf("%s", OUT);
						system(OUT);
					}
				}
				if((scaledX  > buttonCordsBlCSet[X] && scaledX < (buttonCordsBlCSet[X]+buttonCordsBlCSet[W])) && (scaledY > buttonCordsBlCSet[Y] && scaledY < (buttonCordsBlCSet[Y]+buttonCordsBlCSet[H]))){
					if (mymillis() - buttonTimerBlCSet > 500){
						buttonTimerBlCSet = mymillis();
						DrawImage("Switch/Send", BLS3+30, BLR4);
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -bl -blYes %i %i &",readTo(PosToBlCharge),readTo(PosToBlDischarge));
						printf("%s", OUT);
						system(OUT);
					}
				}
				if((scaledX  > buttonCordsBlDSet[X] && scaledX < (buttonCordsBlDSet[X]+buttonCordsBlDSet[W])) && (scaledY > buttonCordsBlDSet[Y] && scaledY < (buttonCordsBlDSet[Y]+buttonCordsBlDSet[H]))){
					if (mymillis() - buttonTimerBlDSet > 500){
						buttonTimerBlDSet = mymillis();
						DrawImage("Switch/Send", BLS3+30, BLR7);
						snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -bl -blYes %i %i &",readTo(PosToBlCharge),readTo(PosToBlDischarge));
						printf("%s", OUT);
						system(OUT);
					}
				}
				if((scaledX  > buttonCordsBlCMinus500[X] && scaledX < (buttonCordsBlCMinus500[X]+buttonCordsBlCMinus500[W])) && (scaledY > buttonCordsBlCMinus500[Y] && scaledY < (buttonCordsBlCMinus500[Y]+buttonCordsBlCMinus500[H]))){
					if (mymillis() - buttonTimerBlCMinus500 > 500){
						buttonTimerBlCMinus500 = mymillis();
						if(readTo(PosToBlCharge)-500 >= 0){
							writeTo(PosToBlCharge,readTo(PosToBlCharge)-500);
						}
					}
				}
				if((scaledX  > buttonCordsBlCPlus500[X] && scaledX < (buttonCordsBlCPlus500[X]+buttonCordsBlCPlus500[W])) && (scaledY > buttonCordsBlCPlus500[Y] && scaledY < (buttonCordsBlCPlus500[Y]+buttonCordsBlCPlus500[H]))){
					if (mymillis() - buttonTimerBlCPlus500 > 500){
						buttonTimerBlCPlus500 = mymillis();
						if(readTo(PosToBlCharge)+500 <= 65535){
							writeTo(PosToBlCharge,readTo(PosToBlCharge)+500);
						}
					}
				}
				if((scaledX  > buttonCordsBlCMinus2000[X] && scaledX < (buttonCordsBlCMinus2000[X]+buttonCordsBlCMinus2000[W])) && (scaledY > buttonCordsBlCMinus2000[Y] && scaledY < (buttonCordsBlCMinus2000[Y]+buttonCordsBlCMinus2000[H]))){
					if (mymillis() - buttonTimerBlCMinus2000 > 500){
						buttonTimerBlCMinus2000 = mymillis();
						if(readTo(PosToBlCharge)-2000 >= 0){
							writeTo(PosToBlCharge,readTo(PosToBlCharge)-2000);
						}
					}
				}
				if((scaledX  > buttonCordsBlCPlus2000[X] && scaledX < (buttonCordsBlCPlus2000[X]+buttonCordsBlCPlus2000[W])) && (scaledY > buttonCordsBlCPlus2000[Y] && scaledY < (buttonCordsBlCPlus2000[Y]+buttonCordsBlCPlus2000[H]))){
					if (mymillis() - buttonTimerBlCPlus2000 > 500){
						buttonTimerBlCPlus2000 = mymillis();
						if(readTo(PosToBlCharge)+2000 <= 65535){
							writeTo(PosToBlCharge,readTo(PosToBlCharge)+2000);
						}
					}
				}
				if((scaledX  > buttonCordsBlDMinus500[X] && scaledX < (buttonCordsBlDMinus500[X]+buttonCordsBlDMinus500[W])) && (scaledY > buttonCordsBlDMinus500[Y] && scaledY < (buttonCordsBlDMinus500[Y]+buttonCordsBlDMinus500[H]))){
					if (mymillis() - buttonTimerBlDMinus500 > 500){
						buttonTimerBlDMinus500 = mymillis();
						if(readTo(PosToBlDischarge)-500 >= 0){
							writeTo(PosToBlDischarge,readTo(PosToBlDischarge)-500);
						}
					}
				}
				if((scaledX  > buttonCordsBlDPlus500[X] && scaledX < (buttonCordsBlDPlus500[X]+buttonCordsBlDPlus500[W])) && (scaledY > buttonCordsBlDPlus500[Y] && scaledY < (buttonCordsBlDPlus500[Y]+buttonCordsBlDPlus500[H]))){
					if (mymillis() - buttonTimerBlDPlus500 > 500){
						buttonTimerBlDPlus500 = mymillis();
						if(readTo(PosToBlDischarge)+500 <= 65535){
							writeTo(PosToBlDischarge,readTo(PosToBlDischarge)+500);
						}
					}
				}
				if((scaledX  > buttonCordsBlDMinus2000[X] && scaledX < (buttonCordsBlDMinus2000[X]+buttonCordsBlDMinus2000[W])) && (scaledY > buttonCordsBlDMinus2000[Y] && scaledY < (buttonCordsBlDMinus2000[Y]+buttonCordsBlDMinus2000[H]))){
					if (mymillis() - buttonTimerBlDMinus2000 > 500){
						buttonTimerBlDMinus2000 = mymillis();
						if(readTo(PosToBlDischarge)-2000 >= 0){
							writeTo(PosToBlDischarge,readTo(PosToBlDischarge)-2000);
						}
					}
				}
				if((scaledX  > buttonCordsBlDPlus2000[X] && scaledX < (buttonCordsBlDPlus2000[X]+buttonCordsBlDPlus2000[W])) && (scaledY > buttonCordsBlDPlus2000[Y] && scaledY < (buttonCordsBlDPlus2000[Y]+buttonCordsBlDPlus2000[H]))){
					if (mymillis() - buttonTimerBlDPlus2000 > 500){
						buttonTimerBlDPlus2000 = mymillis();
						if(readTo(PosToBlDischarge)+2000 <= 65535){
							writeTo(PosToBlDischarge,readTo(PosToBlDischarge)+2000);
						}
					}
				}
				//EP
				if((scaledX  > buttonCordsEpMinus500[X] && scaledX < (buttonCordsEpMinus500[X]+buttonCordsEpMinus500[W])) && (scaledY > buttonCordsEpMinus500[Y] && scaledY < (buttonCordsEpMinus500[Y]+buttonCordsEpMinus500[H]))){
					if (mymillis() - buttonTimerEpMinus500 > 500){
						buttonTimerEpMinus500 = mymillis();
						if(readTo(PosToEpSet)-500 >= 0){
							writeTo(PosToEpSet,readTo(PosToEpSet)-500);
						}
					}
				}
				if((scaledX  > buttonCordsEpPlus500[X] && scaledX < (buttonCordsEpPlus500[X]+buttonCordsEpPlus500[W])) && (scaledY > buttonCordsEpPlus500[Y] && scaledY < (buttonCordsEpPlus500[Y]+buttonCordsEpPlus500[H]))){
					if (mymillis() - buttonTimerEpPlus500 > 500){
						buttonTimerEpPlus500 = mymillis();
						if(readTo(PosToEpSet)+500 <= readTo(PosToEpMax)){
							writeTo(PosToEpSet,readTo(PosToEpSet)+500);
						}
					}
				}
				if((scaledX  > buttonCordsEpMinus2000[X] && scaledX < (buttonCordsEpMinus2000[X]+buttonCordsEpMinus2000[W])) && (scaledY > buttonCordsEpMinus2000[Y] && scaledY < (buttonCordsEpMinus2000[Y]+buttonCordsEpMinus2000[H]))){
					if (mymillis() - buttonTimerEpMinus2000 > 2000){
						buttonTimerEpMinus2000 = mymillis();
						if(readTo(PosToEpSet)-2000 >= 0){
							writeTo(PosToEpSet,readTo(PosToEpSet)-2000);
						}
					}
				}
				if((scaledX  > buttonCordsEpPlus2000[X] && scaledX < (buttonCordsEpPlus2000[X]+buttonCordsEpPlus2000[W])) && (scaledY > buttonCordsEpPlus2000[Y] && scaledY < (buttonCordsEpPlus2000[Y]+buttonCordsEpPlus2000[H]))){
					if (mymillis() - buttonTimerEpPlus2000 > 500){
						buttonTimerEpPlus2000 = mymillis();
						if(readTo(PosToEpSet)+2000 <= readTo(PosToEpMax)){
							writeTo(PosToEpSet,readTo(PosToEpSet)+2000);
						}
					}
				}
				if((scaledX  > buttonCordsEpMinus10000[X] && scaledX < (buttonCordsEpMinus10000[X]+buttonCordsEpMinus10000[W])) && (scaledY > buttonCordsEpMinus10000[Y] && scaledY < (buttonCordsEpMinus10000[Y]+buttonCordsEpMinus10000[H]))){
					if (mymillis() - buttonTimerEpMinus10000 > 10000){
						buttonTimerEpMinus10000 = mymillis();
						if(readTo(PosToEpSet)-10000 >= 0){
							writeTo(PosToEpSet,readTo(PosToEpSet)-10000);
						}
					}
				}
				if((scaledX  > buttonCordsEpPlus10000[X] && scaledX < (buttonCordsEpPlus10000[X]+buttonCordsEpPlus10000[W])) && (scaledY > buttonCordsEpPlus10000[Y] && scaledY < (buttonCordsEpPlus10000[Y]+buttonCordsEpPlus10000[H]))){
					if (mymillis() - buttonTimerEpPlus10000 > 500){
						buttonTimerEpPlus10000 = mymillis();
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
				break; // ScreenFunktion
			}
			case ScreenIdlePeriods:{
				//IdlePeriods
				if((scaledX  > buttonCordsIdleMonCharge[X] && scaledX < (buttonCordsIdleMonCharge[X]+buttonCordsIdleMonCharge[W])) && (scaledY > buttonCordsIdleMonCharge[Y] && scaledY < (buttonCordsIdleMonCharge[Y]+buttonCordsIdleMonCharge[H]))){
					if (mymillis() - buttonTimerIdleMonCharge > 500){
						buttonTimerIdleMonCharge = mymillis();
						DrawImage("Switch/Send", ISP1-4, IRC + IB);
 						if(readIdle(PosMonCharge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -open -mon &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -lock -mon &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleTueCharge[X] && scaledX < (buttonCordsIdleTueCharge[X]+buttonCordsIdleTueCharge[W])) && (scaledY > buttonCordsIdleTueCharge[Y] && scaledY < (buttonCordsIdleTueCharge[Y]+buttonCordsIdleTueCharge[H]))){
					if (mymillis() - buttonTimerIdleTueCharge > 500){
						buttonTimerIdleTueCharge = mymillis();
						DrawImage("Switch/Send", ISP2-4, IRC + IB);
						if(readIdle(PosTueCharge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -open -tue &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -lock -tue &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleWedCharge[X] && scaledX < (buttonCordsIdleWedCharge[X]+buttonCordsIdleWedCharge[W])) && (scaledY > buttonCordsIdleWedCharge[Y] && scaledY < (buttonCordsIdleWedCharge[Y]+buttonCordsIdleWedCharge[H]))){
					if (mymillis() - buttonTimerIdleWedCharge > 500){
						buttonTimerIdleWedCharge = mymillis();
						DrawImage("Switch/Send", ISP3-4, IRC + IB);
						if(readIdle(PosWedCharge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -open -wed &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -lock -wed &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleThuCharge[X] && scaledX < (buttonCordsIdleThuCharge[X]+buttonCordsIdleThuCharge[W])) && (scaledY > buttonCordsIdleThuCharge[Y] && scaledY < (buttonCordsIdleThuCharge[Y]+buttonCordsIdleThuCharge[H]))){
					if (mymillis() - buttonTimerIdleThuCharge > 500){
						buttonTimerIdleThuCharge = mymillis();
						DrawImage("Switch/Send", ISP4-4, IRC + IB);
						if(readIdle(PosThuCharge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -open -thu &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -lock -thu &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleFriCharge[X] && scaledX < (buttonCordsIdleFriCharge[X]+buttonCordsIdleFriCharge[W])) && (scaledY > buttonCordsIdleFriCharge[Y] && scaledY < (buttonCordsIdleFriCharge[Y]+buttonCordsIdleFriCharge[H]))){
					if (mymillis() - buttonTimerIdleFriCharge > 500){
						buttonTimerIdleFriCharge = mymillis();
						DrawImage("Switch/Send", ISP5-4, IRC + IB);
						if(readIdle(PosFriCharge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -open -fri &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -lock -fri &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleSatCharge[X] && scaledX < (buttonCordsIdleSatCharge[X]+buttonCordsIdleSatCharge[W])) && (scaledY > buttonCordsIdleSatCharge[Y] && scaledY < (buttonCordsIdleSatCharge[Y]+buttonCordsIdleSatCharge[H]))){
					if (mymillis() - buttonTimerIdleSatCharge > 500){
						buttonTimerIdleSatCharge = mymillis();
						DrawImage("Switch/Send", ISP6-4, IRC + IB);
						if(readIdle(PosSatCharge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -open -sat &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -lock -sat &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleSunCharge[X] && scaledX < (buttonCordsIdleSunCharge[X]+buttonCordsIdleSunCharge[W])) && (scaledY > buttonCordsIdleSunCharge[Y] && scaledY < (buttonCordsIdleSunCharge[Y]+buttonCordsIdleSunCharge[H]))){
					if (mymillis() - buttonTimerIdleSunCharge > 500){
						buttonTimerIdleSunCharge = mymillis();
						DrawImage("Switch/Send", ISP7-4, IRC + IB);
						if(readIdle(PosSunCharge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -open -sun &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -charge -lock -sun &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleMonDischarge[X] && scaledX < (buttonCordsIdleMonDischarge[X]+buttonCordsIdleMonDischarge[W])) && (scaledY > buttonCordsIdleMonDischarge[Y] && scaledY < (buttonCordsIdleMonDischarge[Y]+buttonCordsIdleMonDischarge[H]))){
					if (mymillis() - buttonTimerIdleMonDischarge > 500){
						buttonTimerIdleMonDischarge = mymillis();
						DrawImage("Switch/Send", ISP1-4, IRD + IB);
						if(readIdle(PosMonDischarge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -open -mon &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -lock -mon &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleTueDischarge[X] && scaledX < (buttonCordsIdleTueDischarge[X]+buttonCordsIdleTueDischarge[W])) && (scaledY > buttonCordsIdleTueDischarge[Y] && scaledY < (buttonCordsIdleTueDischarge[Y]+buttonCordsIdleTueDischarge[H]))){
					if (mymillis() - buttonTimerIdleTueDischarge > 500){
						buttonTimerIdleTueDischarge = mymillis();
						DrawImage("Switch/Send", ISP2-4, IRD + IB);
						if(readIdle(PosTueDischarge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -open -tue &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -lock -tue &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleWedDischarge[X] && scaledX < (buttonCordsIdleWedDischarge[X]+buttonCordsIdleWedDischarge[W])) && (scaledY > buttonCordsIdleWedDischarge[Y] && scaledY < (buttonCordsIdleWedDischarge[Y]+buttonCordsIdleWedDischarge[H]))){
					if (mymillis() - buttonTimerIdleWedDischarge > 500){
						buttonTimerIdleWedDischarge = mymillis();
						DrawImage("Switch/Send", ISP3-4, IRD + IB);
						if(readIdle(PosWedDischarge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -open -wed &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -lock -wed &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleThuDischarge[X] && scaledX < (buttonCordsIdleThuDischarge[X]+buttonCordsIdleThuDischarge[W])) && (scaledY > buttonCordsIdleThuDischarge[Y] && scaledY < (buttonCordsIdleThuDischarge[Y]+buttonCordsIdleThuDischarge[H]))){
					if (mymillis() - buttonTimerIdleThuDischarge > 500){
						buttonTimerIdleThuDischarge = mymillis();
						DrawImage("Switch/Send", ISP4-4, IRD + IB);
						if(readIdle(PosThuDischarge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -open -thu &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -lock -thu &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleFriDischarge[X] && scaledX < (buttonCordsIdleFriDischarge[X]+buttonCordsIdleFriDischarge[W])) && (scaledY > buttonCordsIdleFriDischarge[Y] && scaledY < (buttonCordsIdleFriDischarge[Y]+buttonCordsIdleFriDischarge[H]))){
					if (mymillis() - buttonTimerIdleFriDischarge > 500){
						buttonTimerIdleFriDischarge = mymillis();
						DrawImage("Switch/Send", ISP5-4, IRD + IB);
						if(readIdle(PosFriDischarge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -open -fri &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -lock -fri &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleSatDischarge[X] && scaledX < (buttonCordsIdleSatDischarge[X]+buttonCordsIdleSatDischarge[W])) && (scaledY > buttonCordsIdleSatDischarge[Y] && scaledY < (buttonCordsIdleSatDischarge[Y]+buttonCordsIdleSatDischarge[H]))){
					if (mymillis() - buttonTimerIdleSatDischarge > 500){
						buttonTimerIdleSatDischarge = mymillis();
						DrawImage("Switch/Send", ISP6-4, IRD + IB);
						if(readIdle(PosSatDischarge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -open -sat &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -lock -sat &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				if((scaledX  > buttonCordsIdleSunDischarge[X] && scaledX < (buttonCordsIdleSunDischarge[X]+buttonCordsIdleSunDischarge[W])) && (scaledY > buttonCordsIdleSunDischarge[Y] && scaledY < (buttonCordsIdleSunDischarge[Y]+buttonCordsIdleSunDischarge[H]))){
					if (mymillis() - buttonTimerIdleSunDischarge > 500){
						buttonTimerIdleSunDischarge = mymillis();
						DrawImage("Switch/Send", ISP7-4, IRD + IB);
						if(readIdle(PosSunDischarge)) snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -open -sun &");
						else snprintf (OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -idle -discharge -lock -sun &");
						system(OUT);
						writeScreen(ScreenCounter, 0);
						writeScreen(ScreenSaver, false);
					}
				}
				break; // IdlePeriods
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
			/*	if((scaledX  > buttonCordsMinus500[X] && scaledX < (buttonCordsMinus500[X]+buttonCordsMinus500[W])) && (scaledY > buttonCordsMinus500[Y] && scaledY < (buttonCordsMinus500[Y]+buttonCordsMinus500[H]))){
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
				*/
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

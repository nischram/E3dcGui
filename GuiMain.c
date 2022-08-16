/*
gcc -g -o GuiMain  GuiMain.c -lwiringPi
*/
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
#include <wiringPi.h>
#include "checkPara.h"
#include "Frame/touch.h"
#include "Frame/touch.c"
#include "Frame/framebuffer.c"
#include "Frame/DrawSkala.c"
#include "Frame/DrawLine.c"
#include "Frame/DrawSOC.c"
#include "Frame/DrawImage.h"
#include "Frame/DrawNetImage.h"
#include "Frame/DrawCorner.c"
#include "funktion.h"
#include "Rscp/actionCheckHM.h"
#include "HMGui.h"
#include "External/WetterGui.h"
#include "External/MuellGui.h"
#include "External/dht.h"
#include "External/Aktor.h"
#include "External/Gruenbeck.h"

//####################################
int main(){
	//Parameter einbinden, checken oder default setzen
	checkDefinePara(0);

	picturePosition();
	writeScreen(ScreenCounter, 0);
	makeAktor();
	makeDHT11();
	if (Gruenbeck == 1){
		makeGruenRAM();
	}
	writeAktor(AktorPrioPosition, 1);

	signal(SIGINT, INThandler);

	int  xres,yres;

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	int GuiTime, change = 1, changeStop = 0, saveGBstate = 0;
	char batch[256], OUT [100],Path[100],Value[20],writeTxt[20],TimestampHM[20],RscpTimestamp[40],weatherTime[64], gruenTime[24], changepf[20];
  char TAG_EMS_OUT_DATE[20], TAG_EMS_OUT_TIME[20], serialnumber[17], swrelease[20];
	int counter, ScreenSaverCounter, HistoryCounter = 15, SmartCounter = 0;
	int UnixTime;
	int wbCheckSumOld = 0;
	char wallboxSendNow[20], wallboxSendMode[20], wallboxSendCurrent[20], wallboxSendBtC[20], wallboxSendBbC[20], wallboxSendStop[20], wallboxSendPhC[20];
	char WbMode[24],WbBtC[24],WbBbC[24],WbSet[24];
	int WbCurrent, actionSendTime = 0;

	screenOn();
	writeScreen(ScreenCounter, 0);

	int Screen[ScreenMAX];

	if (openTouchScreen() == 1)
		perror("error opening touch screen");

	getTouchScreenDetails(&screenXmin,&screenXmax,&screenYmin,&screenYmax);

	framebufferInitialize(&xres,&yres);

	scaleXvalue = ((float)screenXmax-screenXmin) / xres;
	printf ("X Scale Factor = %f\n", scaleXvalue);
	scaleYvalue = ((float)screenYmax-screenYmin) / yres;
	printf ("Y Scale Factor = %f\n", scaleYvalue);

	int wiringPi = OK;
	if ( wiringPiSetup() == -1 ){
		wiringPi = ERROR;
		printf("wiringPi error\n");
	}
	if (wiringPi == OK){
		pinMode( PirPin, INPUT );
		pullUpDnControl ( PirPin, PUD_DOWN);
		pinMode(Aktor1Pin, OUTPUT);
		pinMode(Aktor2Pin, OUTPUT);
		pinMode(Aktor3Pin, OUTPUT);
		pinMode(Aktor4Pin, OUTPUT);
		pinMode(Aktor5Pin, OUTPUT);
	}
	if (Gruenbeck == 1){
		readGruenbeck(gruenTime);
	}
	DEBUG("\n### Neustart ###\n");

//####################################
	while(1){
		time_t currentTime = time(NULL);
		struct tm *now;
		time( &currentTime );
		now = localtime( &currentTime );
		strftime (OUT,100,"%H:%M:%S ",now);
		DEBUG(OUT);

		system ("cp /mnt/RAMDisk/E3dcCache.txt /mnt/RAMDisk/E3dcGuiData.txt");
		system ("cp /mnt/RAMDisk/E3dcWbCache.txt /mnt/RAMDisk/E3dcGuiWbData.txt ");

		readRscpChar(TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME, serialnumber, swrelease);
		GuiTime = PiTime;

		int screenState = readScreen(ScreenState);
		int pirUse = readPirUse();
		if (pirUse == true){
			if (wiringPi == OK){
				if ( digitalRead( PirPin ) == true && screenState == ScreenOff){
					screenOn();
					writeScreen(ScreenCounter, 0);
					writeScreen(ScreenSaver, false);
				}
			}
		}

		if (Gruenbeck == 1){
			saveGruenbeck(gruenTime);
		}

		Screen[ScreenCounter] = readScreen(ScreenCounter);
		counter = Screen[ScreenCounter] -1;
		DEBUGint(Screen[ScreenCounter]);
		if(counter < 0)
			counter = 0;
		writeScreen(ScreenCounter, counter);

		Screen[ScreenChange] = readScreen(ScreenChange);
		int screenChange = Screen[ScreenChange];
		Screen[ScreenHistory] = readScreen(ScreenHistory);

//####################################
		switch(screenChange){
//####################################
			//Aktuell Grafik erstellen
			case ScreenAktuell:{
				GuiTime = RscpTime;
				if(counter == 0 ){
					writeScreen(ScreenCounter, 60);
					if(screenState == ScreenOn){
						drawMainScreen();                         // Hintergrundbild mit Bildern für den oberen Auswahlbereich erzeugen
						DrawImage("PviImage", 40, 90);
						int pmAktivPhases = readRscp(PosPMPhases);
						DrawNetImage(640, 90, pmAktivPhases);              //Einfügen der Grafik für NetImage oder NetImageOff je nach Status der Phasen am LM in der Frame/DrawNetImage.h
						DrawImage("BatImage", 60, 318);
						DrawImage("HomeImage", 640, 340);
						DrawImage("S10Image", 270, 110);
						DrawImage("Eigenstrom", 320, 270);
						DrawImage("Autarkie", 465, 270);
						if(Additional == 1)
							DrawImage("ExtImage", 40, 220);
						if(Wallbox == 1){
							if(readRscpWb(PosWbLED_ERR)==1)
								DrawImage("Wallbox/WbError", 650, 220);
							else if(readRscpWb(PosWbLED_SON)==1){
								if (readRscpWb(PosWbLED_BAT)==1)
									DrawImage("Wallbox/WbSunOn", 650, 220);
								else
									DrawImage("Wallbox/WbSunOff", 650, 220);
							}
							else {
								if (readRscpWb(PosWbLED_BAT)==1)
									DrawImage("Wallbox/WbMixOn", 650, 220);
								else
									DrawImage("Wallbox/WbMixOff", 650, 220);
							}
						}
						if (E3DC_S10 ==1 && historyAktiv == 1){
							if (Screen[ScreenHistory] == today)
								DrawImage("Yesterday", 370, 405);
							else if (Screen[ScreenHistory] == yesterday)
								DrawImage("HistoryOff", 370, 405);
							else
								DrawImage("Today", 370, 405);
						}
					}
				}
				DEBUG("Main ");
				if(screenState == ScreenOn){
					//Bildwechsel für > > >
					if(changeStop == 0){
						change ++;
						if(change > 3)change = 1;
						snprintf (changepf, (size_t)100, "Zahlen/pf%i", change);
					}
					else snprintf (changepf, (size_t)100, "Zahlen/pf0");
					//Programm-Start
					drawOutput(340,100,140,12, serialnumber, GREY);
					//PVI
					int TAG_PVI = readRscp(PosPVI);
					int TAG_PVIState = readRscp(PosPVIState);
					DEBUG("PVI ");
					DEBUGint(TAG_PVI);
					DEBUGint(TAG_PVIState);
					snprintf (OUT, (size_t)100, "%i W", TAG_PVI);
					if(TAG_PVIState >= 1){
						if(TAG_PVI > 50)
							drawNumber(160, 115, TAG_PVI, WATT, ORANGE);
						else
							drawNumber(160, 115, TAG_PVI, WATT, BLACK);
						if(TAG_PVI > 0)DrawImage(changepf, 180, 140);
						else DrawImage("Zahlen/pf0", 180, 140);
					}
					else
						DrawImage("Zahlen/pvOff", 180, 115);
					//Grid
					int TAG_Grid = readRscp(PosGrid);
					int TAG_PMState = readRscp(PosPMState);
					DEBUG("Grid ");
					DEBUGint(TAG_Grid);
					DEBUGint(TAG_PMState);
					if(TAG_PMState >= 1){
						if(TAG_Grid < 0){
							TAG_Grid = TAG_Grid * -1;
							if (TAG_Grid > 15){
								drawNumber(550, 115, TAG_Grid, WATT, CYAN);
								DrawImage(changepf, 570, 140);
							}
							else {
								drawNumber(550, 115, TAG_Grid, WATT, GREEN);
								DrawImage("Zahlen/pf0", 570, 140);

							}
						}
						else{
							if (TAG_Grid > 15){
								drawNumber(550, 115, TAG_Grid, WATT, BLUE);
								snprintf (OUT, (size_t)100, "%sn", changepf);
								DrawImage(OUT, 570, 140);
							}
							else{
								drawNumber(550, 115, TAG_Grid, WATT, GREEN);
								DrawImage("Zahlen/pf0", 570, 140);
							}
						}
					}
					else
						DrawImage("Zahlen/lmOff", 550, 115);
					//Home
					int TAG_Home = readRscp(PosHome);
					DEBUG("Home ");
					DEBUGint(TAG_Home);
					drawNumber(550, 360, TAG_Home, WATT, 0);
					DrawImage(changepf, 570, 385);
					//Battery
					int TAG_Bat = readRscp(PosBat);
					DEBUG("Bat ");
					DEBUGint(TAG_Bat);
					if(TAG_Bat < 0){
						TAG_Bat = TAG_Bat * -1;
						DrawImage(changepf, 180, 385);
					}
					else{
						if(TAG_Bat > 0){
							snprintf (OUT, (size_t)100, "%sn", changepf);
							DrawImage(OUT, 180, 385);
						}
						else DrawImage("Zahlen/pf0", 180, 385);
					}
					drawNumber(160, 360, TAG_Bat, WATT, BLACK);
					//Additional
					if(Additional == 1){
						int TAG_ADD = readRscp(PosADD);
						drawNumber(160, 236, TAG_ADD, WATT, BLACK);
						if(TAG_ADD > 15) DrawImage(changepf, 180, 261);
						else DrawImage("Zahlen/pf0", 180, 261);
					}
					//Wallbox
					if(Wallbox == 1){
						int TAG_WbAll = readRscp(PosWbAll);
						int TAG_WbSolar = readRscp(PosWbSolar);
						if(TAG_WbAll > 15) DrawImage(changepf, 570, 261);
						else DrawImage("Zahlen/pf0", 570, 261);
						drawNumber(550, 236, TAG_WbAll, WATT, BLACK);
						drawNumber(550, 276, TAG_WbSolar, WATT, ORANGE);
					}
					//SOC
					int TAG_SOC = read100Rscp(PosSOC);
					DEBUG("SOC ");
					DEBUGint(TAG_SOC);
					snprintf (OUT, (size_t)100, "%i %%", TAG_SOC);
					int TAG_BatState = readRscp(PosBatState);
					if(TAG_BatState >= 1){
						int SOCx = TAG_SOC * 0.82;
						int EpResx = readRscp(PosEpReserv);
						if(EpResx > 0){
							drawSquare(75,348,66,82,GREY);
							drawSquare(75,348+82-EpResx,66,EpResx,GREEN);
							drawSquare(75,348+82,66,9,GREEN);
						}
						drawSquare(84,348,48,82,WHITE);
						drawSquare(84,348+82-SOCx,48,SOCx,BLUE);
						drawNumber(80, 440, TAG_SOC, PERCENT, BLUE);
					}
					else{
						drawOutput(80,440,50,12,"OFF",RED);
					}
					//Autarky
					int TAG_Autarky = read100Rscp(PosAutarky);
					DEBUG("Autarky ");
					DEBUGint(TAG_Autarky);
					int Autarkyx = 2 * TAG_Autarky;
					drawSquare(405,366-200,60,200-Autarkyx,WHITE);
					drawSquare(405,366-Autarkyx,60,Autarkyx,GREEN);
					drawNumber(405, 140, TAG_Autarky, PERCENT, BLACK);
					//SelfConsumption
					int TAG_SelfCon = read100Rscp(PosSelfCon);
					DEBUG("SelfCon ");
					DEBUGint(TAG_SelfCon);
					int SelfConx = 2 * TAG_SelfCon;
					drawSquare(340,366-200,60,200-SelfConx,WHITE);
					drawSquare(340,366-SelfConx,60,SelfConx,LTGREY);
					drawNumber(340, 140, TAG_SelfCon, PERCENT, BLACK);
					//HistoryValues
					if(counter == 0 && Screen[ScreenHistory] > 0 && historyAktiv == 1){
						float historyPV = readHistory(dataPV, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "%.1f kWh", historyPV/1000);
						put_stringRGB(180, 155, OUT, 225, 122, 34);
						float historyGridIn = readHistory(dataGridIn, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "IN  %.1f kWh", historyGridIn/1000);
						put_stringRGB(550, 155, OUT, 0, 172, 255);
						float historyGridOut = readHistory(dataGridOut, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "OUT %.1f kWh", historyGridOut/1000);
						put_stringRGB(550, 175, OUT, 0, 0, 172);
						float historyHome = readHistory(dataHome, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "%.1f kWh", historyHome/1000);
						put_stringRGB(550, 400, OUT, 225, 30, 30);
						float historyBatIn = readHistory(dataBatIn, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "IN  %.1f kWh", historyBatIn/1000);
						put_stringRGB(180, 400, OUT, 0, 225, 64);
						float historyBatOut = readHistory(dataBatOut, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "OUT %.1f kWh", historyBatOut/1000);
						put_stringRGB(180, 420, OUT, 0, 172, 0);
					}
					DEBUG("RSCP-Ende ");
				}
				break;
			}
//####################################################
			//Langzeit Grafik erstellen
			case ScreenLangzeit:{
				GuiTime = 0;
	 		  //Legende
				int Legende [LegendeMAX];

				Legende[SOC] = readLegende(SOC);
				if (Legende[SOC] == 0)
					drawSquare(LF01,443,50,20, LTGREY);
				else
		 			drawSquareRGB(LF01,443,50,20, 64, 134, 64);
				put_string(LF01+2,443," SOC", WHITE);

				Legende[Solar] = readLegende(Solar);
	 			if (Legende[Solar] == 0)
	 				drawSquare(LF02,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(LF02,443,50,20, 225, 122, 34);
	 			put_string(LF02+2,443,"Solar", WHITE);

				Legende[Home] = readLegende(Home);
	 			if (Legende[Home] == 0)
	 				drawSquare(LF03,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(LF03,443,50,20, 225, 30, 30);
				put_string(LF03+1,443," Home", WHITE);

				Legende[NetIn] = readLegende(NetIn);
	 			if (Legende[NetIn] == 0)
	 				drawSquare(LF04,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(LF04,443,50,20, 0, 172, 255);
	 			put_string(LF04+3,443,"NetIn", WHITE);

				Legende[NetOut] = readLegende(NetOut);
	 			if (Legende[NetOut] == 0)
	 				drawSquare(LF05,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(LF05,443,50,20, 0, 0, 172);
	 			put_string(LF05+1,443,"NetOut", WHITE);

				Legende[BatIn] = readLegende(BatIn);
	 			if (Legende[BatIn] == 0)
	 				drawSquare(LF06,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(LF06,443,50,20, 0, 225, 64);
	 			put_string(LF06+3,443,"BatIn", WHITE);

				Legende[BatOut] = readLegende(BatOut);
	 			if (Legende[BatOut] == 0)
	 				drawSquare(LF07,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(LF07,443,50,20, 0, 172, 0);
	 			put_string(LF07+1,443,"BatOut", WHITE);

				if(Additional == 1 ){
					Legende[ADD] = readLegende(ADD);
		 			if (Legende[ADD] == 0)
		 				drawSquare(LF08,443,50,20, LTGREY);
		 			else
		 				drawSquareRGB(LF08,443,50,20, 172, 0, 172);
		 			put_string(LF08+2,443," ADD", WHITE);
				}
				else {
					Legende[ADD] = 0;
				}
				if(Wallbox == 1 ){
					Legende[WbSolar] = readLegende(WbSolar);
					Legende[WbAll] = readLegende(WbAll);
					if (Legende[WbAll] == 0)
		 				drawSquare(LF09,443,50,20, LTGREY);
		 			else
		 				drawSquareRGB(LF09,443,50,20, 0, 234, 234);
		 			put_string(LF09+2,443,"WBAll", WHITE);
		 			if (Legende[WbSolar] == 0)
		 				drawSquare(LF10,443,50,20, LTGREY);
		 			else
		 				drawSquareRGB(LF10,443,50,20, 234, 234, 0);
		 			put_string(LF10+2,443,"WbSun", WHITE);
				}
				else {
					Legende[WbSolar] = 0;
					Legende[WbAll] = 0;
				}

	 			if(counter == 0){
					drawMainScreen();
	 				time_t timeStamp;
	 				struct tm *now;
	 				time( &timeStamp );
	 				now = localtime( &timeStamp );
	 				char Hour [41];
	 				char Minute [41];
	 				strftime (Hour,40,"%H",now);
	 				strftime (Minute,40,"%M",now);
	 				int Hourint = atoi(Hour);
	 				int Minuteint = atoi(Minute);
	 				int x24h = ((Hourint * 60) + Minuteint) * 0.534;											//Berechnung für die 0:00 Uhr Linie in den Langzeitwerten
	 				skala();
	 				if(Legende[SOC] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/SOC900.txt");
	 					drawSOC(Path, 64, 134, 64);
	 				}
					if(Legende[Solar] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/Solar900.txt");
	 					drawLine(Path, 225, 122, 34, PowerMax);
	 				}
	 				if(Legende[Home] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/Home900.txt");
	 					drawLine(Path, 225, 30, 30, PowerMax);
	 				}
	 				if(Legende[NetIn] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/NetIn900.txt");
	 					drawLine(Path, 0, 172, 255, PowerMax);
	 				}
	 				if(Legende[NetOut] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/NetOut900.txt");
	 					drawLine(Path, 0, 0, 172, PowerMax);
	 				}
	 				if(Legende[BatIn] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/BatIn900.txt");
	 					drawLine(Path, 0, 225, 64, PowerMax);
	 				}
	 				if(Legende[BatOut] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/BatOut900.txt");
	 					drawLine(Path, 0, 172, 0, PowerMax);
	 				}
					if(Legende[ADD] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/Add900.txt");
	 					drawLine(Path, 225, 30, 30, PowerMax);
	 				}
					if(Legende[WbAll] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/WBAll900.txt");
	 					drawLine(Path, 0, 234, 234, PowerMax);
	 				}
					if(Legende[WbSolar] == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/WBSolar900.txt");
	 					drawLine(Path, 234, 234, 0, PowerMax);
	 				}
	 				drawSquareRGB(783-x24h, 430-330, 1, 330, 255, 172, 64);								//Ziechnen der 0:00 Uhr Linie in den Langzeitwerten
					writeScreen(ScreenCounter, 60);
				}
				break;
			}
//####################################################
			//Wallbox Grafik erstellen
			case ScreenWallbox:{
				GuiTime = RscpTime;
				if(counter == 0 ){
					writeScreen(ScreenCounter, 60);
					if(screenState == ScreenOn){
						drawMainScreen();
						drawSquare(WB1,RP1-20,740,353,GREY);
						drawCorner(WB1,RP1-20,740,353,WHITE);
						drawSquare(WB1+3,RP1+40,734,290,WHITE);
						drawCorner(WB1+3,RP1+40,734,290,GREY);
						DrawImage("Wallbox/Wallbox", 290, RP1-5);
					}
				}
				if(counter == 0 || screenState == ScreenOn){
					int TAG_WbAll = readRscp(PosWbAll);
					int TAG_WbSolar = readRscp(PosWbSolar);
					int WbGrid = TAG_WbAll - TAG_WbSolar;

					if (counter == 0 || wbCheckSumOld != readRscpWb(PosWbCheckSum)){
						if (readRscpWb(PosWbLED_BAT)==1){
							if(TAG_WbSolar > 200 && WbGrid < 200)
								DrawImage("Wallbox/CarSun",     WBCARX, WBCARY+93);
							else if(TAG_WbSolar < 200 && WbGrid > 200)
								DrawImage("Wallbox/CarGrid",    WBCARX, WBCARY+93);
							else
								DrawImage("Wallbox/CarMix",     WBCARX, WBCARY+93);
						}
						else if (readRscpWb(PosWbLocked)==1)
							DrawImage("Wallbox/CarLocked",  WBCARX, WBCARY+93);
						else if (readRscpWb(PosWbConnect)==1)
							DrawImage("Wallbox/CarConnect", WBCARX, WBCARY+93);
						else
							DrawImage("Wallbox/Car",        WBCARX, WBCARY+93);
						if(readRscpWb(PosWbLED_ERR)==1)
							DrawImage("Wallbox/Mon_WbError", WBCARX+265, WBCARY);
						else if(readRscpWb(PosWbLED_SON)==1){
							if (readRscpWb(PosWbLED_BAT)==1)
								DrawImage("Wallbox/Mon_WbSunOn", WBCARX+265, WBCARY);
							else
								DrawImage("Wallbox/Mon_WbSunOff", WBCARX+265, WBCARY);
						}
						else {
							if (readRscpWb(PosWbLED_BAT)==1)
								DrawImage("Wallbox/Mon_WbMixOn", WBCARX+265, WBCARY);
							else
								DrawImage("Wallbox/Mon_WbMixOff", WBCARX+265, WBCARY);
						}

						snprintf (OUT, (size_t)100, "Wallbox/Current/%iA", readRscpWb(PosWbCurrent));
						DrawImage(OUT, WBCURX, WBCURY);
						put_string(WBCURX-1,WBCURY+33,"Ladestrom",GREY);
						if((readRscpWb(PosWbCurrent)-10) >= 6)
							DrawImage("Switch/Minus10", WBCURX-58-58-58, WBCURY);
						else
							DrawImage("Switch/Minus10Off", WBCURX-58-58-58, WBCURY);
						if((readRscpWb(PosWbCurrent)-2) >= 6)
							DrawImage("Switch/Minus2", WBCURX-58-58, WBCURY);
						else
							DrawImage("Switch/Minus2Off", WBCURX-58-58, WBCURY);
						if((readRscpWb(PosWbCurrent)-1) >= 6)
							DrawImage("Switch/Minus", WBCURX-58, WBCURY);
						else
							DrawImage("Switch/MinusOff", WBCURX-58, WBCURY);
						if((readRscpWb(PosWbCurrent)+1) <= 32)
							DrawImage("Switch/Plus", WBCURX+82, WBCURY);
						else
							DrawImage("Switch/Plus", WBCURX+82, WBCURY);
						if((readRscpWb(PosWbCurrent)+2) <= 32)
							DrawImage("Switch/Plus2", WBCURX+82+58, WBCURY);
						else
							DrawImage("Switch/Plus2Off", WBCURX+82+58, WBCURY);
						if((readRscpWb(PosWbCurrent)+10) <= 32)
							DrawImage("Switch/Plus10", WBCURX+82+58+58, WBCURY);
						else
							DrawImage("Switch/Plus10Off", WBCURX+82+58+58, WBCURY);

						if (readRscpWb(PosWbMode)==1)
							DrawImage("Switch/On", WBMODEX, WBMODEY);
						else
							DrawImage("Switch/Off", WBMODEX, WBMODEY);
						put_string(WBMODEX-8,WBMODEY+33,"Sonnenmodus",GREY);
						if (readRscpWb(PosWbBtC)==1){
							DrawImage("Switch/On", WBBTCX, WBBTCY);
							DrawImage("Switch/90percent", WBPERLX, WBPER9Y);
							DrawImage("Switch/80percent", WBPERRX, WBPER9Y);
							DrawImage("Switch/70percent", WBPERLX, WBPER7Y);
							DrawImage("Switch/60percent", WBPERRX, WBPER7Y);
							DrawImage("Switch/50percent", WBPERLX, WBPER5Y);
							DrawImage("Switch/40percent", WBPERRX, WBPER5Y);
							DrawImage("Switch/30percent", WBPERLX, WBPER3Y);
							DrawImage("Switch/20percent", WBPERRX, WBPER3Y);
							DrawImage("Switch/10percent", WBPERLX, WBPER1Y);
							DrawImage("Switch/0percent", WBPERRX, WBPER1Y);
							drawNumber(WBUNTILX+18, WBUNTILY, readRscp(PosWbUntil), PERCENT, BLACK);
							put_string(WBUNTILX-1,WBUNTILY+33,"Entladegrenze",GREY);
						}
						else
							DrawImage("Switch/Off", WBBTCX, WBBTCY);
						put_string(WBBTCX-19,WBBTCY+33,"Batterie zu Auto",GREY);
						if (readRscpWb(PosWbBbC)==1)
							DrawImage("Switch/On", WBBBCX, WBBBCY);
						else
							DrawImage("Switch/Off", WBBBCX, WBBBCY);
						put_string(WBBBCX-23,WBBBCY+33,"Batterie vor Auto",GREY);
						if (readRscpWb(PosWbUsePhases)==3)
							DrawImage("Switch/3Ph", WBPHX, WBPHY);
						else
							DrawImage("Switch/1Ph", WBPHX, WBPHY);
						put_string(WBPHX-23,WBPHY+33,"Erwartete Phasen",GREY);
						if (readRscpWb(PosWbLED_BAT)==1)
							DrawImage("Switch/Stop", WBSTOPX, WBSTOPY);
						else
							DrawImage("Switch/StopOff", WBSTOPX, WBSTOPY);
						if (readRscp(PosWbBaM)==1)
							DrawImage("Switch/Off", WBMODEX+58+58-3, WBMODEY);
						else
							DrawImage("Switch/On", WBMODEX+58+58-3, WBMODEY);
						put_string(WBMODEX+58+58-3-8, WBMODEY+33,"Bat. im Mix-Mode",GREY);
					}
					int x;
					if(TAG_WbSolar > 200 && WbGrid < 200){
						x = drawNumber(470, 270, TAG_WbAll, WATT, GREEN);
						DrawImage("Wallbox/Autogr", x, 270);
					}
					else if(TAG_WbSolar < 200 && WbGrid > 200){
						x = drawNumber(470, 270, TAG_WbAll, WATT, ORANGE);
						DrawImage("Wallbox/Autoor", x, 270);
					}
					else{
						x = drawNumber(470, 270, TAG_WbAll, WATT, BLACK);
						DrawImage("Wallbox/Autosw", x, 270);
					}

					x = drawNumber(470, 220, WbGrid, WATT, BLUE);
					DrawImage("Wallbox/Netzbl", x, 220);
					x = drawNumber(470, 245, TAG_WbSolar, WATT, ORANGE);
					DrawImage("Wallbox/Solaror", x, 245);
					wbCheckSumOld = readRscpWb(PosWbCheckSum);
				}
				break;
			}
//####################################################
			//Funktion Grafik erstellen
			case ScreenFunktion:{
				GuiTime = RscpTime;
				if(counter == 0 ){
					writeScreen(ScreenCounter, 60);
					if(screenState == ScreenOn){
						drawMainScreen();
						// Grafik für Batterie-Limits
						drawSquare(BL,RP1-20,335,340,GREY);
						drawCorner(BL,RP1-20,335,340,WHITE);
						drawSquare(BL+3,RP1+40,329,277,WHITE);
						drawCorner(BL+3,RP1+40,329,277,GREY);
						DrawImage("Batterie/BatterieLimits", BL+60, RP1-5);
						// Grafik für EP_Reserve
						drawSquare(EP,RP1-20,335,340,GREY);
						drawCorner(EP,RP1-20,335,340,WHITE);
						drawSquare(EP+3,RP1+40,329,277,WHITE);
						drawCorner(EP+3,RP1+40,329,277,GREY);
						DrawImage("EpReserve/NotRes", EP+60, RP1-5);
					}
				}
				// Battery-Limits
				DrawImage("Batterie/Auto", BLS1, BLR1);
				DrawImage("Batterie/Manuell", BLS3+30, BLR1);
				DrawImage("Batterie/Charge", BLS1, BLR2);
				if(readTo(PosToBlCharge) - 500 >= 0 ) DrawImage("EpReserve/Minus500", BLS1, BLR3);
				else DrawImage("EpReserve/Minus500Off", BLS1, BLR3);
				if(readTo(PosToBlCharge) + 500 <= 65355 ) DrawImage("EpReserve/Plus500", BLS2, BLR3);
				else DrawImage("EpReserve/Plus500Off", BLS2, BLR3);
				if(readTo(PosToBlCharge) - 2000 >= 0 ) DrawImage("EpReserve/Minus2000", BLS1, BLR4);
				else DrawImage("EpReserve/Minus2000Off", BLS1, BLR4);
				if(readTo(PosToBlCharge) + 2000 <= 65355 ) DrawImage("EpReserve/Plus2000", BLS2, BLR4);
				else DrawImage("EpReserve/Plus2000Off", BLS2, BLR4);
				DrawImage("Batterie/Discharge", BLS1, BLR5);
				if(readTo(PosToBlDischarge) - 500 >= 0 ) DrawImage("EpReserve/Minus500", BLS1, BLR6);
				else DrawImage("EpReserve/Minus500Off", BLS1, BLR6);
				if(readTo(PosToBlDischarge) + 500 <= 65355 ) DrawImage("EpReserve/Plus500", BLS2, BLR6);
				else DrawImage("EpReserve/Plus500Off", BLS2, BLR6);
				if(readTo(PosToBlDischarge) - 2000 >= 0 ) DrawImage("EpReserve/Minus2000", BLS1, BLR7);
				else DrawImage("EpReserve/Minus2000Off", BLS1, BLR7);
				if(readTo(PosToBlDischarge) + 2000 <= 65355 ) DrawImage("EpReserve/Plus2000", BLS2, BLR7);
				else DrawImage("EpReserve/Plus2000Off", BLS2, BLR7);

				//EP_Reserve
				int x;
				int maxW = readRscp(PosEpReservMaxW)*0.8;
				int maxC = maxW / 500;
				int maxSet = maxC * 500;
				writeTo(PosToEpMax,maxSet);
				drawNumber(EPPERX -10+24, EPPERY, readRscp(PosEpReserv), PERCENT, BLACK);
				x = drawNumber(EPWX -10, EPWY, readRscp(PosEpReservW), WATTH, BLACK);
				DrawImage("EpReserve/reservesw", x, EPWY);
				DrawImage("EpReserve/reservesw", x, EPPERY);
				x = drawNumber(EPWMAXX -10, EPWMAXY, maxSet, WATTH, BLACK);
				DrawImage("EpReserve/maxsw", x, EPWMAXY);
				if (readRscp(PosEpReservW) > 0)
					DrawImage("Switch/On", EPSWX, EPSWY);
				else
					DrawImage("Switch/Off", EPSWX, EPSWY);
				x = drawNumber(EPSETX -10, EPSETY, readTo(PosToEpSet), WATTH, BLACK);
				DrawImage("EpReserve/setsw", x, EPSETY);
				DrawImage("EpReserve/Set", EPSETS2, EPSETR1);
				if(readTo(PosToEpSet) - 500 >= 0 ) DrawImage("EpReserve/Minus500", EPSETS1, EPSETR1);
				else DrawImage("EpReserve/Minus500Off", EPSETS1, EPSETR1);
				if(readTo(PosToEpSet) + 500 <= maxSet ) DrawImage("EpReserve/Plus500", EPSETS3, EPSETR1);
				else DrawImage("EpReserve/Plus500Off", EPSETS3, EPSETR1);
				if(readTo(PosToEpSet) - 2000 >= 0 ) DrawImage("EpReserve/Minus2000", EPSETS1, EPSETR2);
				else DrawImage("EpReserve/Minus2000Off", EPSETS1, EPSETR2);
				if(readTo(PosToEpSet) + 2000 <= maxSet ) DrawImage("EpReserve/Plus2000", EPSETS3, EPSETR2);
				else DrawImage("EpReserve/Plus2000Off", EPSETS3, EPSETR2);
				if(readTo(PosToEpSet) - 10000 >= 0 ) DrawImage("EpReserve/Minus10000", EPSETS1, EPSETR3);
				else DrawImage("EpReserve/Minus10000Off", EPSETS1, EPSETR3);
				if(readTo(PosToEpSet) + 10000 <= maxSet ) DrawImage("EpReserve/Plus10000", EPSETS3, EPSETR3);
				else DrawImage("EpReserve/Plus10000Off", EPSETS3, EPSETR3);

				//BL Aktuell
				if (readRscp(PosBlUsed) == 1)
					DrawImage("Switch/On", BLS2, BLR1);
				else
					DrawImage("Switch/Off", BLS2, BLR1);
				DrawImage("EpReserve/Set", BLS3+30, BLR4);
				drawNumber(BLS3+18, BLR2, readRscp(PosBlCharge), WATT, BLACK);
				drawNumber(BLS3+18, BLR3, readTo(PosToBlCharge), WATT, BLACK);
				DrawImage("EpReserve/Set", BLS3+30, BLR7);
				drawNumber(BLS3+18, BLR5, readRscp(PosBlDischarge), WATT, BLACK);
				drawNumber(BLS3+18, BLR6, readTo(PosToBlDischarge), WATT, BLACK);

				break;
			}
//####################################################
			//Monitor Grafik erstellen
			case ScreenIdlePeriods:{
				GuiTime = RscpTime;
				if(counter == 0 ){
					writeScreen(ScreenCounter, 60);
					if(screenState == ScreenOn){
						writeTo(PosToIdlePeriod,1);
						sleep(1);
						drawMainScreen();
						// Grafik für Ladesperrzeit
						drawSquare(IX,IY,IW,IH,GREY);
						drawCorner(IX,IY,IW,IH,WHITE);
						drawSquare(IX+3,IY+32,IW-6,IH-32-3,WHITE);
						drawCorner(IX+3,IY+32,IW-6,IH-32-3,GREY);
						DrawImage("IdleCharge", 80,IY+2);
						// Grafik für Entladesperrzeit
						drawSquare(IX,IY+IH+6,IW,IH,GREY);
						drawCorner(IX,IY+IH+6,IW,IH,WHITE);
						drawSquare(IX+3,IY+IH+6+32,IW-6,IH-32-3,WHITE);
						drawCorner(IX+3,IY+IH+6+32,IW-6,IH-32-3,GREY);
						DrawImage("IdleDischarge", 80,IY+2+IH+6);
						// Day
						drawIdlePeriodsDay("Montag",ISP1,IRC,PosMonCharge);
						drawIdlePeriodsDay("Dienstag",ISP2,IRC,PosTueCharge);
						drawIdlePeriodsDay("Mitwoch",ISP3,IRC,PosWedCharge);
						drawIdlePeriodsDay("Donnerstag",ISP4,IRC,PosThuCharge);
						drawIdlePeriodsDay("Freitag",ISP5,IRC,PosFriCharge);
						drawIdlePeriodsDay("Samstag",ISP6,IRC,PosSatCharge);
						drawIdlePeriodsDay("Sonntag",ISP7,IRC,PosSunCharge);

						drawIdlePeriodsDay("Montag",ISP1,IRD,PosMonDischarge);
						drawIdlePeriodsDay("Dienstag",ISP2,IRD,PosTueDischarge);
						drawIdlePeriodsDay("Mitwoch",ISP3,IRD,PosWedDischarge);
						drawIdlePeriodsDay("Donnerstag",ISP4,IRD,PosThuDischarge);
						drawIdlePeriodsDay("Freitag",ISP5,IRD,PosFriDischarge);
						drawIdlePeriodsDay("Samstag",ISP6,IRD,PosSatDischarge);
						drawIdlePeriodsDay("Sonntag",ISP7,IRD,PosSunDischarge);
					}
				}

				break;
			}
//####################################################
			//Monitor Grafik erstellen
			case ScreenMonitor:{
				GuiTime = RscpTime;
				if(counter == 0 ){
					writeScreen(ScreenCounter, 60);
					if(screenState == ScreenOn){
						drawMainScreen();
						// Grafik für Tracker1
						drawSquare(T1,RP1-20,160,340,GREY);
						drawCorner(T1,RP1-20,160,340,WHITE);
						drawSquare(T1+3,RP1+40,154,277,WHITE);
						drawCorner(T1+3,RP1+40,154,277,GREY);
						DrawImage("EpReserve/Tracker1", T1+20, RP1-5);
						DrawImage("PV_Modul_aktiv", T1+20, 200);
						// Grafik für Tracker2
						drawSquare(T2,RP1-20,160,340,GREY);
						drawCorner(T2,RP1-20,160,340,WHITE);
						drawSquare(T2+3,RP1+40,154,277,WHITE);
						drawCorner(T2+3,RP1+40,154,277,GREY);
						DrawImage("EpReserve/Tracker2", T2+20, RP1-5);
						if (PVI_TRACKER == 2)
							DrawImage("PV_Modul_aktiv", T2+20, 200);
						else
							DrawImage("PV_Modul_deaktiv", T2+20, 200);
						// Grafik für System Info
						drawSquare(SI,RP1-20,335,340,GREY);
						drawCorner(SI,RP1-20,335,340,WHITE);
						drawSquare(SI+3,RP1+40,329,277,WHITE);
						drawCorner(SI+3,RP1+40,329,277,GREY);
						DrawImage("SystemInfo", SI+60, RP1-5);
					}
				}
				if(counter == 0 || screenState == ScreenOn){
					//PVI DC Power1
					int TAG_PVIState = readRscp(PosPVIState);
					if(TAG_PVIState >= 1){
						int TAG_PVIDCP1 = readRscp(PosPVIDCP1);
						drawNumber(T1+30, 320, TAG_PVIDCP1, WATT, BLACK);
						drawNumber(T1+30, 345, readRscp(PosPVIDCU1), VOLT, BLACK);
						if(TAG_PVIDCP1 > 0){
							double TAG_PVIDCI1 = readRscp(PosPVIDCI1);
							int pviA = TAG_PVIDCI1/100;
							int pviDotA = (TAG_PVIDCI1/100 - pviA) * 100;
							int x = drawNumber(T1+30-5, 370, pviA, DOT, BLACK);
							drawNumber(x, 370, pviDotA, AMP, BLACK);
						}
					}
					else{
						DrawImage("Zahlen/pvOff", T1+40, 320);
					}
					//PVI Tracker 2
					if (PVI_TRACKER == 2){
						//PVI DC Power2
						if(TAG_PVIState >= 1){
							int TAG_PVIDCP2 = readRscp(PosPVIDCP2);
							drawNumber(T2+30, 320, TAG_PVIDCP2, WATT, BLACK);
							drawNumber(T2+30, 345, readRscp(PosPVIDCU2), VOLT, BLACK);
							if(TAG_PVIDCP2 > 0){
								double TAG_PVIDCI2 = readRscp(PosPVIDCI2);
								int pviA = TAG_PVIDCI2/100;
								int pviDotA = (TAG_PVIDCI2/100 - pviA) * 100;
								int x = drawNumber(T2+30-5, 370, pviA, DOT, BLACK);
								drawNumber(x, 370, pviDotA, AMP, BLACK);
							}
						}
						else
						DrawImage("Zahlen/pvOff", T2+40, 320);
					}
					// System_Info
					DrawImage("Info/Seriennummer", SIS1, SIR1);
					drawOutput(SIS3,SIR1+8,120,12, serialnumber, GREY);
					DrawImage("Info/SwRelease", SIS1, SIR2);
					drawOutput(SIS3,SIR2+8,120,12, swrelease, GREY);
					DrawImage("Info/InstPower", SIS1, SIR3);
					snprintf (OUT, (size_t)100, "%i Wp", readRscp(PosInstalledPeak));
					drawOutput(SIS3,SIR3+8,120,12, OUT, GREY);
					DrawImage("Info/DeratePercent", SIS1, SIR4);
					snprintf (OUT, (size_t)100, "%i %%", readRscp(PosDerateAtPercent));
					drawOutput(SIS3,SIR4+8,120,12, OUT, GREY);
					DrawImage("Info/DeratePower", SIS1, SIR5);
					snprintf (OUT, (size_t)100, "%i W", readRscp(PosDerateAtPower));
					drawOutput(SIS3,SIR5+8,120,12, OUT, GREY);
					DrawImage("Info/EmsState", SIS1, SIR6);
					snprintf (OUT, (size_t)100, "0x0%x", readRscp(PosEmsState));
					drawOutput(SIS3,SIR6+8,120,12, OUT, GREY);
					DrawImage("Info/EpState", SIS1, SIR7);
					snprintf (OUT, (size_t)100, "0x0%x", readRscp(PosEpState));
					drawOutput(SIS3,SIR7+8,120,12, OUT, GREY);
				}
				break;
			}
//####################################################
			//Homematic Grafik erstellen
			case ScreenHM:{
				if(Homematic_GUI == 1){
					GuiTime = make_HM_Gui(GuiTime, counter);    //Ausgelagert in die HMGui.h Parameter sind in der HMparameter.h zu difinieren
				}
				break;
			}
//####################################################
			//Setup Grafik erstellen
			case ScreenSetup:{
				GuiTime = SetupTime;
				Screen[ScreenShutdown] = readScreen(ScreenShutdown);
				int screenShutdown = Screen[ScreenShutdown];

				if(counter == 0){
					writeScreen(ScreenCounter, 60);
					drawMainScreen();
					switch(screenShutdown){
						case ShutdownSD:{
							drawSquare(SP1,RP2-20,180,30,GREY);
							drawCorner(SP1,RP2-20,180,30, WHITE);
							put_string(SP1+20,RP2-20+8, "3 Sekunden", LIGHT_RED);
							break;
						}
						case ShutdownSRS:{
							drawSquare(SP1,RP3-20,180,30,GREY);
							drawCorner(SP1,RP3-20,180,30, WHITE);
							put_string(SP1+20,RP3-20+8, "Software Neustart", GREEN);
							break;
						}
						case ShutdownHRS:{
							drawSquare(SP1,RP4-20,180,30,RED);
							drawCorner(SP1,RP4-20,180,30, WHITE);
							put_string(SP1+20,RP4-20+8, "Hardware Neustart", WHITE);
							break;
						}
						case ShutdownSStop:{
							drawSquare(SP1,RP5-20,180,30,RED);
							drawCorner(SP1,RP5-20,180,30, WHITE);
							put_string(SP1+20,RP5-20+8, "Software Stoppen", WHITE);
							break;
						}
						case ShutdownSDN:{
							drawSquare(SP1,RP2-20,180,30,RED);
							drawCorner(SP1,RP2-20,180,30, WHITE);
							put_string(SP1+20,RP2-20+8, "SHUTDOWN", WHITE);
							break;
						}
						case ShutdownWD:{
							drawSquare(300,200,200,100,RED);
							drawCorner(300,200,200,100, WHITE);
							drawSquare(304,204,192,92,LIGHT_RED);
							drawCorner(304,204,192,92, RED);
							put_string(365,235, "Watchdog", WHITE);
							put_string(365,250, " Reboot", WHITE);
							break;
						}
						case ShutdownRun:{
							drawSquare(SP1,RP2-20,180,30,GREY);
							drawCorner(SP1,RP2-20,180,30, WHITE);
							put_string(SP1+20,RP2-20+8, "Ausschalten", WHITE);
							drawSquare(SP1,RP3-20,180,30,GREY);
							drawCorner(SP1,RP3-20,180,30, WHITE);
							put_string(SP1+20,RP3-20+8, "Software Neustart", WHITE);
							drawSquare(SP1,RP4-20,180,30,GREY);
							drawCorner(SP1,RP4-20,180,30, WHITE);
							put_string(SP1+20,RP4-20+8, "Hardware Neustart", WHITE);
							drawSquare(SP1,RP5-20,180,30,GREY);
							drawCorner(SP1,RP5-20,180,30, WHITE);
							put_string(SP1+20,RP5-20+8, "Software Stoppen", WHITE);
							break;
						}
						default:{
							writeScreen(ScreenShutdown, ShutdownRun);
							break;
						}
					}
					//Version anzeigen
					readVersion(OUT, Value);
					drawSquare(SP1,RP1-20,180,30,GREY);
					drawCorner(SP1,RP1-20,180,30, WHITE);
					put_string(SP1+20,RP1-20, OUT, WHITE);
					put_string(SP1+20,RP1-8, Value, WHITE);
					//Daten für PI Informationen laden
					char PiTemp[20], PiUptime[40], PiCPU[20], PiIPeth0[24], PiIPwlan0[24], Pieth0[24], Piwlan0[24], Pihost[24];
					//Pi Temp
					double T = piTemp(PiTemp);
					//Pi Uptime
					piUptime(PiUptime);
					//Pi IP
					snprintf(Pihost, (size_t)24, "eth0");
					piIP(Pihost, Pieth0);
					snprintf(PiIPeth0, (size_t)24, "ETH  : %s", Pieth0);
					snprintf(Pihost, (size_t)24, "wlan0");
					piIP(Pihost, Piwlan0);
					snprintf(PiIPwlan0, (size_t)24, "WLAN : %s", Piwlan0);
					//Pi CPU
					int PiCPUint = piCPU(PiCPU);
					//Helligkeit
					int brightness = readBrightness();
					//PIR
					pirUse = readPirUse();
					//Grafiken für Pi Informationen erstellen
					// Grafik für Helligkeit
					drawSquare(SP4,RP1-20,328,60,GREY);
					drawCorner(SP4,RP1-20,328,60,WHITE);
					drawSquare(SP4+100,RP1-17,225,54,WHITE);
					drawCorner(SP4+100,RP1-17,225,54,GREY);
					put_string(SP4+6, RP1+4, "Helligkeit", WHITE);
					// Grafik für Uptime
					drawSquare(SP4,RP2-20,328,60,GREY);
					drawCorner(SP4,RP2-20,328,60,WHITE);
					drawSquare(SP4+100,RP2-17,225,54,WHITE);
					drawCorner(SP4+100,RP2-17,225,54,GREY);
					put_string(SP4+6, RP2+4, "PIR", WHITE);
					// Grafik für Temp
					drawSquare(SP4,RP3-20,328,60,GREY);
					drawCorner(SP4,RP3-20,328,60,WHITE);
					drawSquare(SP4+100,RP3-17,225,54,WHITE);
					drawCorner(SP4+100,RP3-17,225,54,GREY);
					put_string(SP4+6, RP3+4, "Uptime", WHITE);
					// Grafik für CPU
					drawSquare(SP4,RP4-20,328,60,GREY);
					drawCorner(SP4,RP4-20,328,60,WHITE);
					drawSquare(SP4+100,RP4-17,225,54,WHITE);
					drawCorner(SP4+100,RP4-17,225,54,GREY);
					put_string(SP4+6, RP4+4, "CPU / Temp", WHITE);
					// Grafik für IP
					drawSquare(SP4,RP5-20,328,60,GREY);
					drawCorner(SP4,RP5-20,328,60,WHITE);
					drawSquare(SP4+100,RP5-17,225,54,WHITE);
					drawCorner(SP4+100,RP5-17,225,54,GREY);
					put_string(SP4+6, RP5+4, "IP", WHITE);
					// Helligkeit
					drawSquare(SP4+105, RP1, Fw, 21, LTGREY);
					createData(SP4+110, RP1, "10%");
					drawSquare(SP4+160, RP1, Fw, 21, LTGREY);
					createData(SP4+165, RP1, "30%");
					drawSquare(SP4+215, RP1, Fw, 21, LTGREY);
					createData(SP4+220, RP1, "60%");
					drawSquare(SP4+270, RP1, Fw, 21, LTGREY);
					createData(SP4+270, RP1, "100%");
					if (brightness == 25)
					drawSquare(SP4+105, RP1, Fw, 21, GREEN);
					if (brightness == 76)
					drawSquare(SP4+160, RP1, Fw, 21, GREEN);
					if (brightness == 150)
					drawSquare(SP4+215, RP1, Fw, 21, GREEN);
					if (brightness == 255)
					drawSquare(SP4+270, RP1, Fw, 21, GREEN);
					// PIR
					if (pirUse == true){
						drawSquare(SP6-20, RP2, Fw, 21, LIGHT_GREEN);
						createData(SP6-25, RP2, "PIR On");
					}
					else{
						drawSquare(SP6-20, RP2, Fw, 21, LIGHT_RED);
						createData(SP6-25, RP2, "PIR Off");
					}
					// Uptime
					put_string(SP6-50, RP3+4, PiUptime, GREY);
					// Temp
					if (T > 20){
						drawSquare(SP7-20, RP4, Fw, 21, LIGHT_GREEN);
						createData(SP7-25, RP4, PiTemp);
					}
					else if (T > 40){
						drawSquare(SP7-20, RP4, Fw, 21, LIGHT_RED);
						createData(SP7-25, RP4, PiTemp);
					}
					else if (T > 60){
						drawSquare(SP7-20, RP4, Fw, 21, RED);
						createData(SP7-25, RP4, PiTemp);
					}
					else{
						drawSquare(SP7-20, RP4, Fw, 21, GREEN);
						createData(SP7-25, RP4, PiTemp);
					}
					// CPU
					if (PiCPUint > 5){
						drawSquare(SP6-20, RP4, Fw, 21, LIGHT_GREEN);
						createData(SP6-15, RP4, PiCPU);
					}
					else if (PiCPUint > 10){
						drawSquare(SP6-20, RP4, Fw, 21, LIGHT_RED);
						createData(SP6-15, RP4, PiCPU);
					}
					else if (PiCPUint > 20){
						drawSquare(SP6-20, RP4, Fw, 21, RED);
						createData(SP6-15, RP4, PiCPU);
					}
					else{
						drawSquare(SP6-20, RP4, Fw, 21, GREEN);
						createData(SP6-15, RP4, PiCPU);
					}
					// IP
					createData(SP6-50, RP5-26, PiIPeth0);
					createData(SP6-50, RP5-10, PiIPwlan0);
					writeScreen(ScreenCounter, 60);
				}
				break;
			}
//####################################################
			//SmartHome Grafik erstellen
			case ScreenSmart:{
				GuiTime = PiTime;
				if(counter == 0 ){
					writeScreen(ScreenCounter, 60);
					SmartCounter = 0;
					if(screenState == ScreenOn){
						drawMainScreen();
						if(E3DC_S10 ==1)
							makeAktorFrame();                      // External/Aktor.h
						makeDHTFrame();                         // External/dht11.h
					}
				}
				if(E3DC_S10 ==1)
					makeAktorState();                        // External/Aktor.h
				if (SmartCounter == 0){
					makeDHTState();                          // External/dht11.h
					SmartCounter = 10;
				}
				SmartCounter--;
				break;
			}
//####################################################
			//Wetter Grafik erstellen
			case ScreenWetter:{
				if (wetterGui == 1)
					GuiTime = makeWetterGui(GuiTime, counter, weatherTime);    //Ausgelagert in die Datei WetterGui.h
				break;
			}
//####################################################
			//Grünbeck Grafik erstellen
			case ScreenGB:{
				if (Gruenbeck == 1)
					GuiTime = makeGruenbeck(GuiTime, counter, gruenTime);
				break;
			}
//####################################################
			//Müllkalender Grafik erstellen
			case ScreenMuell:{
				if (Abfuhrkalender == 1)
					GuiTime = makeMuellGui(GuiTime, counter);
				break;
			}
//####################################################
			default:{
				if(E3DC_S10 ==1)
					writeScreen(ScreenChange, ScreenAktuell);
				else if(Homematic_GUI == 1)
					writeScreen(ScreenChange, ScreenHM);
				else
					writeScreen(ScreenChange, ScreenSetup);   //writeScreen(ScreenChange, ScreenWetter);
					break;
			}
		} //switch(screenChange)

//####################################################
	//Schaltaktoren
		checkAktor();
//####################################################
	//Debug.txt um 0:00Uhr speichern
		checkDEBUG();
//####################################################
	//WatchdogHM Daten für WD schreiben
		int Unixtime[UnixtimeMAX];
		if((counter == 0 || counter == 30) && Homematic_GUI == 1){
			char Value[20];
			read_HM(ISE_UnixTime, 10, Value);
			UnixTime = atoi(Value);
			read_HM(ISE_TimestampHM, 16,TimestampHM);
			writeUnixtime(UnixtimeHM, UnixTime);
		}
	//Watchdog GuiMain
		if(counter == 0 || counter == 20 || counter == 40){
			int AktuallTime = time(NULL);
			writeUnixtime(UnixtimeGui, AktuallTime);
		}
//####################################################
	//Time
		if(GuiTime == RscpTime && E3DC_S10 == 1 && screenState == ScreenOn){
			put_string(330, 454, "Letzter Zeitstempel: ", GREY);
			int AktuallTime = time(NULL);
			int TAG_UnixTime = readUnixtime(UnixtimeE3dc);
			int DiffTime = AktuallTime - TAG_UnixTime;
			snprintf (OUT, (size_t)100, "%s %s", TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME);
			if(DiffTime > 180){
				drawOutput(500,456,170,12, OUT, RED);
				changeStop = 1;
			}
			else{
				drawOutput(500,456,170,12, OUT, GREEN);
				changeStop = 0;
			}
			if(E3DC_S10 == 1 && counter == 0 && Screen[ScreenHistory] > 0 && screenState == ScreenOn && screenChange ==	ScreenAktuell){
				float HistoryUnix = readHistory(dataTime, Screen[ScreenHistory]);
				time_t timeStamp;
				timeStamp = (time_t)HistoryUnix -60;
				struct tm *now;
				now = gmtime( &timeStamp );
				if (Screen[ScreenHistory] == today)
					strftime (OUT,100,"Today %d.%m. %H:%M",now);
				else if (Screen[ScreenHistory] == yesterday)
					strftime (OUT,100,"Yesterday %d.%m. %H:%M",now);
				else{
					snprintf (OUT, (size_t)100, "");
					writeScreen(ScreenHistory, historyOff);
				}
				put_string(330, 435, OUT, GREY);
			}
		}
		else if(GuiTime == HomematicTime && Homematic_GUI == 1 && screenState == ScreenOn){
			put_string(20, 454, "Letzter Zeitstempel der Homematic: ", GREY);
			int AktuallTime = time(NULL);
			int DiffTime = AktuallTime - UnixTime;
			if(DiffTime > 180){
				drawOutput(300,456,170,12, TimestampHM, RED);
			}
			else{
				drawOutput(300,456,170,12, TimestampHM, GREEN);
			}
		}
		else if(GuiTime == WeatherTime && screenState == ScreenOn){
			putAktuell(WetterS1, 456);
			int AktuallTime = time(NULL);
			put_string(400, 454, "OpenWeatherMap Datensatz: ", GREY);
			drawOutput(610,456,170,12, weatherTime, GREEN);
		}
		else if(GuiTime == GruenTime && screenState == ScreenOn){
			putAktuell(WetterS1, 456);
			put_string(400, 454, "Gr\201nbeck Datensatz: ", GREY);
			drawOutput(560,456,170,12, gruenTime, GREEN);
		}
		else if(GuiTime == PiTime && screenState == ScreenOn){
			putAktuell(WetterS1, 456);
		}
		else if(GuiTime == SetupTime && screenState == ScreenOn){
			putAktuell(20, 456);
			madeBy(OUT);
			put_string(325,454, OUT, BLUE);
		}
//####################################################
	//Wallbox HM read and senden
	if(WALLBOX_ACTION == 1 || EP_RESERVE_ACTION == 1 || BATTERYLIMIT_ACTION == 1 || POWERSAVE_ACTION == 1 || WEATHER_REG_ACTION == 1){
		if( time(NULL) - actionSendTime >= HM_Intervall){
			actionSendTime = time(NULL);
			actionCheckAll();
		}
	}
//####################################################
	//Bildschirmschoner
		Screen[ScreenSaver] = readScreen(ScreenSaver);      //Zählerdatei für den Bildschirmschoner auslesen
		int ScreenSaverCounter = Screen[ScreenSaver] +1;
		if (ScreenSaverCounter == ScreenSaverTime){
			if (pirUse == true){
				if (digitalRead( PirPin ) == false){
					screenOff();
				}
			}
			else if (pirUse == false){
				screenOff();
			}
			ScreenSaverCounter = 0;
			if(E3DC_S10 ==1){
				writeScreen(ScreenChange, ScreenAktuell);
				writeScreen(ScreenHistory, historyOff);
			}
			else if(Homematic_GUI == 1)
				writeScreen(ScreenChange, ScreenHM);
			else
				writeScreen(ScreenChange, ScreenSetup);    //writeScreen(ScreenChange, ScreenWetter);
		}
		writeScreen(ScreenSaver, ScreenSaverCounter);
//####################################################
	//HistoryValues vom S10 mit "S10history" abfragen.
		if (historyAktiv == 1){
			if(HistoryCounter == 0){
				system("/home/pi/E3dcGui/S10history/S10history -T &");// > /dev/null 2>&1");
				HistoryCounter = historyDelay;
			}
			if(HistoryCounter == (historyDelay-15)){
				system("/home/pi/E3dcGui/S10history/S10history -Y &");//  > /dev/null 2>&1");
			}
			HistoryCounter --;
		}
//####################################################
	//Abfrageintervall
		DEBUG("ENDE\n");
		sleep(1);
	}
}

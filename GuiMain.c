/*
gcc -g -o GuiMain  GuiMain.c
*/
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/ioctl.h>
#include <sys/sysinfo.h>
#include "parameter.h"
#include "parameterHM.h"
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
#include "HMGui.h"
#include "WetterGui.h"


//####################################
int main(){
	signal(SIGINT, INThandler);

	int  xres,yres;

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	int GuiTime, change = 0, changeStop = 0;
	char batch[256], OUT [100],Path[100],Value[20],writeTxt[20],TimestampHM[20],RscpTimestamp[40],weatherTime[64];
  char TAG_EMS_OUT_DATE[20], TAG_EMS_OUT_TIME[20], serialnumber[17];
	int counter, ScreenSaverCounter, HistoryCounter = 15;
	int UnixTime;

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
//####################################
	while(1){
		readRscpChar(TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME, serialnumber);
		GuiTime = PiTime;

		Screen[ScreenCounter] = readScreen(ScreenCounter);
		counter = Screen[ScreenCounter] -1;
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
				int screenState = readScreen(ScreenState);
				if(counter == 0 ){
					writeScreen(ScreenCounter, 60);
					if(screenState == ScreenOn){
						drawMainScreen();                         // Hintergrundbild mit Bildern für den oberen Auswahlbereich erzeugen
						DrawImage("PviImage", 40, 50);
						int pmAktivPhases = readRscp(PosPMPhases);
						DrawNetImage(pmAktivPhases);              //Einfügen der Grafik für NetImage oder NetImageOff je nach Status der Phasen am LM in der Frame/DrawNetImage.h
						DrawImage("BatImage", 50, 310);
						DrawImage("HomeImage", 640, 310);
						DrawImage("S10Image", 270, 110);
						DrawImage("ExtImage", 40, 190);
						DrawImage("WallboxImage", 650, 190);
						if (E3DC_S10 ==1 && historyAktiv == true){
							if (Screen[ScreenHistory] == today)
								DrawImage("Yesterday", 370, 405);
							else if (Screen[ScreenHistory] == yesterday)
								DrawImage("HistoryOff", 370, 405);
							else
								DrawImage("Today", 370, 405);
						}
					}
				}
				if(screenState == ScreenOn){
					//Bildwechsel für > > >
					if(changeStop == 0){
						if(change == 1){
							change = 0;
						}
						else{
							change = 1;
						}
					}
					//Programm-Start
					drawOutput(340,100,140,12, serialnumber, GREY);
					//PVI
					int TAG_PVI = readRscp(PosPVI);
					int TAG_PVIState = readRscp(PosPVIState);
					snprintf (OUT, (size_t)100, "%i W", TAG_PVI);
					if(TAG_PVIState >= 1){
						if(TAG_PVI > 50)
							drawOutputRGB(190,120,80,12, OUT, 225,122,34);
						else
							drawOutput(190,120,80,12, OUT, GREY);
						if(TAG_PVI > 0){
							if(change == 1)
								drawOutput(190,100,80,12, "> > >",GREY);
							else
								drawOutput(190,100,80,12, " > > ",GREY);
						}
					}
					else
						drawOutput(190,120,80,12, "PVI-DOWN", RED);
					//Grid
					int TAG_Grid = readRscp(PosGrid);
					int TAG_PMState = readRscp(PosPMState);
					if(TAG_PMState >= 1){
						if(TAG_Grid < 0){
							TAG_Grid = TAG_Grid * -1;
							snprintf (OUT, (size_t)100, "%i W", TAG_Grid);
							drawOutput(570,120,80,12, OUT, CYAN);
							if (TAG_Grid > 15){
								if(change == 1)
									drawOutput(570,100,80,12, "> > >",GREY);
								else
									drawOutput(570,100,80,12, " > > ",GREY);
							}
							else
								drawOutput(570,120,80,12, OUT, LIGHT_BLUE);
						}
						else{
							snprintf (OUT, (size_t)100, "%i W", TAG_Grid);
							drawOutput(570,120,80,12, OUT, BLUE);
							if (TAG_Grid > 5){
								if(change == 1)
									drawOutput(570,100,80,12, "< < <",GREY);
								else
									drawOutput(570,100,80,12, " < < ",GREY);
							}
							else
								drawOutput(570,120,80,12, OUT, GREEN);
						}
					}
					else
						drawOutput(570,120,80,12, "LM-DOWN", RED);
					//Home
					int TAG_Home = readRscp(PosHome);
					snprintf (OUT, (size_t)100, "%i W", TAG_Home);
					drawOutput(570,360,80,12, OUT, GREY);
					if(change == 1)
						drawOutput(570,380,80,12,"> > >", GREY);
					else
						drawOutput(570,380,80,12," > > ", GREY);
					//Battery
					int TAG_Bat = readRscp(PosBat);
					if(TAG_Bat < 0){
						TAG_Bat = TAG_Bat * -1;
						snprintf (OUT, (size_t)100, "%i W", TAG_Bat);
						drawOutput(190,360,80,12, OUT, GREY);
						if(change == 1)
							drawOutput(190,380,80,12,"> > >",GREY);
						else
							drawOutput(190,380,80,12," > > ",GREY);
					}
					else{
						snprintf (OUT, (size_t)100, "%i W", TAG_Bat);
						drawOutput(190,360,80,12, OUT, GREY);
						if(TAG_Bat > 0){
							if(change == 1)
								drawOutput(190,380,80,12,"< < <", GREY);
							else
								drawOutput(190,380,80,12," < < ", GREY);
						}
					}
					//Additional
					if(Additional == 1){
						int TAG_ADD = readRscp(PosADD);
						snprintf (OUT, (size_t)100, "%i W", TAG_ADD);
						drawOutput(190,248,60,12, OUT, GREY);
						if(TAG_ADD > 0){
							if(change == 1)
								drawOutput(190,236,60,12,"> > >",GREY);
							else
								drawOutput(190,236,60,12," > > ",GREY);
						}
					}
					else{
						drawSquare(40,190,200,120,WHITE);
					}
					//Wallbox
					if(Wallbox == 1){
						int TAG_WbAll = readRscp(PosWbAll);
						int TAG_WbSolar = readRscp(PosWbSolar);
						if(TAG_WbAll > 1){
							if(change == 1)
								drawOutput(570,236,60,12,"> > >",GREY);
							else
								drawOutput(570,236,60,12," > > ",GREY);
						}
						if(TAG_WbSolar > 0){
						 snprintf (OUT, (size_t)100, "%i W", TAG_WbAll);
						 drawOutput(570,248,60,12, OUT, GREY);
						 put_string(570,258,"All",GREY);
						 snprintf (OUT, (size_t)100, "%i W", TAG_WbSolar);
						 drawOutputRGB(570,224,60,12, OUT, 225, 122, 34);
						 put_stringRGB(570,214,"Solar", 225, 122, 34);
						}
						else{
							snprintf (OUT, (size_t)100, "%i W", TAG_WbAll);
							drawOutput(570,248,60,12, OUT, GREY);
							put_string(570,258,"All",GREY);
						}
					}
					else{
						drawSquare(570,190,200,120,WHITE);
					}
					//SOC
					int TAG_SOC = readRscp(PosSOC);
					snprintf (OUT, (size_t)100, "%i %%", TAG_SOC);
					int TAG_BatState = readRscp(PosBatState);
					if(TAG_BatState >= 1){
						int SOCx = TAG_SOC * 0.82;
						drawSquare(74,340,48,82,WHITE);
						drawSquare(74,340+82-SOCx,48,SOCx,BLUE);
						drawOutput(80,432,50,12, OUT, GREY);
					}
					else{
						drawOutput(80,432,50,12,"OFF",RED);
					}
					//Autarky
					int TAG_Autarky = readRscp(PosAutarky);
					int Autarkyx = 2 * TAG_Autarky;
					drawSquare(405,366-200,60,200-Autarkyx,WHITE);
					drawSquare(405,366-Autarkyx,60,Autarkyx,GREEN);
					snprintf (OUT, (size_t)100, "%i %%", TAG_Autarky);
					drawOutput(420,380,45,12, OUT, GREY);
					snprintf (OUT, (size_t)100, "Autarkie");
					put_string(420,392,OUT,GREY);
					//SelfConsumption
					int TAG_SelfCon = readRscp(PosSelfCon);
					int SelfConx = 2 * TAG_SelfCon;
					drawSquare(340,366-200,60,200-SelfConx,WHITE);
					drawSquare(340,366-SelfConx,60,SelfConx,LTGREY);
					snprintf (OUT, (size_t)100, "%i %%", TAG_SelfCon);
					drawOutput(350,380,45,12, OUT, GREY);
					snprintf (OUT, (size_t)100, "Eigenstrom");
					put_string(310,392,OUT,GREY);
					//HistoryValues
					if(counter == 0 && Screen[ScreenHistory] > 0 && historyAktiv == true){
						float historyPV = readHistory(dataPV, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "%.1f kWh", historyPV/1000);
						put_stringRGB(180, 140, OUT, 225, 122, 34);
						float historyGridIn = readHistory(dataGridIn, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "IN  %.1f kWh", historyGridIn/1000);
						put_stringRGB(560, 140, OUT, 0, 172, 255);
						float historyGridOut = readHistory(dataGridOut, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "OUT %.1f kWh", historyGridOut/1000);
						put_stringRGB(560, 160, OUT, 0, 0, 172);
						float historyHome = readHistory(dataHome, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "%.1f kWh", historyHome/1000);
						put_stringRGB(560, 400, OUT, 225, 30, 30);
						float historyBatIn = readHistory(dataBatIn, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "IN  %.1f kWh", historyBatIn/1000);
						put_stringRGB(180, 400, OUT, 0, 225, 64);
						float historyBatOut = readHistory(dataBatOut, Screen[ScreenHistory]);
						snprintf (OUT, (size_t)100, "OUT %.1f kWh", historyBatOut/1000);
						put_stringRGB(180, 420, OUT, 0, 172, 0);
					}
				}
				break;
			}
//####################################################
			//Langzeit Grafik erstellen
			case ScreenLangzeit:{
				GuiTime = RscpTime;
	 		  //Legende
				int Legende [LegendeMAX];

				Legende[SOC] = readLegende(SOC);
				if (Legende[SOC] == 0)
					drawSquare(364,443,50,20, LTGREY);
				else
		 			drawSquareRGB(364,443,50,20, 64, 134, 64);
				put_string(366,449," SOC", WHITE);

				Legende[Solar] = readLegende(Solar);
	 			if (Legende[Solar] == 0)
	 				drawSquare(417,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(417,443,50,20, 225, 122, 34);
	 			put_string(419,449,"Solar", WHITE);

				Legende[Home] = readLegende(Home);
	 			if (Legende[Home] == 0)
	 				drawSquare(470,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(470,443,50,20, 225, 30, 30);
				put_string(471,449," Home", WHITE);

				Legende[NetIn] = readLegende(NetIn);
	 			if (Legende[NetIn] == 0)
	 				drawSquare(523,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(523,443,50,20, 0, 172, 255);
	 			put_string(526,449,"NetIn", WHITE);

				Legende[NetOut] = readLegende(NetOut);
	 			if (Legende[NetOut] == 0)
	 				drawSquare(576,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(576,443,50,20, 0, 0, 172);
	 			put_string(577,449,"NetOut", WHITE);

				Legende[BatIn] = readLegende(BatIn);
	 			if (Legende[BatIn] == 0)
	 				drawSquare(629,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(629,443,50,20, 0, 225, 64);
	 			put_string(632,449,"BatIn", WHITE);

				Legende[BatOut] = readLegende(BatOut);
	 			if (Legende[BatOut] == 0)
	 				drawSquare(682,443,50,20, LTGREY);
	 			else
	 				drawSquareRGB(682,443,50,20, 0, 172, 0);
	 			put_string(683,449,"BatOut", WHITE);

				if(Additional == 1 ){
					Legende[ADD] = readLegende(ADD);
		 			if (Legende[ADD] == 0)
		 				drawSquare(735,443,50,20, LTGREY);
		 			else
		 				drawSquareRGB(735,443,50,20, 172, 0, 172);
		 			put_string(737,449," ADD", WHITE);
				}
				else {
					Legende[ADD] = 0;
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
	 				drawSquareRGB(783-x24h, 430-330, 1, 330, 255, 172, 64);								//Ziechnen der 0:00 Uhr Linie in den Langzeitwerten
					writeScreen(ScreenCounter, 60);
				}
				break;
			}
//####################################################
			//Monitor Grafik erstellen
			case ScreenMonitor:{
				GuiTime = RscpTime;
				int screenState = readScreen(ScreenState);
				if(counter == 0 ){
					writeScreen(ScreenCounter, 60);
					if(screenState == ScreenOn){
						drawMainScreen();
						DrawImage("PV_Modul_aktiv", 240, 150);
						if (PVI_TRACKER == 2)
							DrawImage("PV_Modul_aktiv", 440, 150);
						else
							DrawImage("PV_Modul_deaktiv", 440, 150);
					}
				}
				if(counter == 0 || screenState == ScreenOn){
					//PVI DC Power1
					int TAG_PVIState = readRscp(PosPVIState);
					if(TAG_PVIState >= 1){
						int TAG_PVIDCP1 = readRscp(PosPVIDCP1);
						snprintf (OUT, (size_t)100, "%i W", TAG_PVIDCP1);
						drawOutput(270,270,80,12, OUT, GREY);
						int TAG_PVIDCU1 = readRscp(PosPVIDCU1);
						snprintf (OUT, (size_t)100, "%i V", TAG_PVIDCU1);
						drawOutput(270,290,80,12,OUT, GREY);
						if(TAG_PVIDCP1 > 0){
							double TAG_PVIDCI1 = readRscp(PosPVIDCI1);
							snprintf (OUT, (size_t)100, "%2.2f A", TAG_PVIDCI1/100);
							drawOutput(270,310,80,12, OUT, GREY);
						}
					}
					else{
						drawOutput(250, 250,80,12,"PVI-DOWN", RED);
					}
					//PVI Tracker 2
					if (PVI_TRACKER == 2){
						//PVI DC Power2
						if(TAG_PVIState >= 1){
							int TAG_PVIDCP2 = readRscp(PosPVIDCP2);
							snprintf (OUT, (size_t)100, "%i W", TAG_PVIDCP2);
							drawOutput(470, 270,80,12, OUT, GREY);
							int TAG_PVIDCU2 = readRscp(PosPVIDCU2);
							snprintf (OUT, (size_t)100, "%i V", TAG_PVIDCU2);
							drawOutput(470, 290,80,12, OUT, GREY);
							if(TAG_PVIDCP2 > 0){
								double TAG_PVIDCI2 = readRscp(PosPVIDCI2);
								snprintf (OUT, (size_t)100, "%2.2f A", TAG_PVIDCI2/100);
								drawOutput(470, 310,80,12, OUT, GREY);
							}
						}
						else
							drawOutput(450, 250,80,12, "PVI-DOWN", RED);
					}
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
				GuiTime = PiTime;
				Screen[ScreenShutdown] = readScreen(ScreenShutdown);
				int screenShutdown = Screen[ScreenShutdown];

				if(counter == 0){
					writeScreen(ScreenCounter, 60);
					drawMainScreen();
					switch(screenShutdown){
						case ShutdownSD:{
							drawSquare(S1,R2-20,180,30,GREY);
							drawCorner(S1,R2-20,180,30, WHITE);
							put_string(S1+20,R2-20+8, "3 Sekunden", LIGHT_RED);
							break;
						}
						case ShutdownSRS:{
							drawSquare(S1,R3-20,180,30,GREY);
							drawCorner(S1,R3-20,180,30, WHITE);
							put_string(S1+20,R3-20+8, "Software Neustart", GREEN);
							break;
						}
						case ShutdownHRS:{
							drawSquare(S1,R4-20,180,30,RED);
							drawCorner(S1,R4-20,180,30, WHITE);
							put_string(S1+20,R4-20+8, "Hardware Neustart", WHITE);
							break;
						}
						case ShutdownSDN:{
							drawSquare(S1,R2-20,180,30,RED);
							drawCorner(S1,R2-20,180,30, WHITE);
							put_string(S1+20,R2-20+8, "SHUTDOWN", WHITE);
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
							drawSquare(S1,R2-20,180,30,GREY);
							drawCorner(S1,R2-20,180,30, WHITE);
							put_string(S1+20,R2-20+8, "Ausschalten", WHITE);
							drawSquare(S1,R3-20,180,30,GREY);
							drawCorner(S1,R3-20,180,30, WHITE);
							put_string(S1+20,R3-20+8, "Software Neustart", WHITE);
							drawSquare(S1,R4-20,180,30,GREY);
							drawCorner(S1,R4-20,180,30, WHITE);
							put_string(S1+20,R4-20+8, "Hardware Neustart", WHITE);
							break;
						}
						default:{
							writeScreen(ScreenShutdown, ShutdownRun);
							break;
						}
					}
					//Version anzeigen
					readVersion(OUT);
					put_string(20,20, OUT, GREY);
					//Daten für PI Informationen laden
					char PiTemp[20], PiUptime[40], PiCPU[20], PiIP1[16], PiIP2[16], PiIP3[16];
					//Pi Temp
					double T = piTemp(PiTemp);
					//Pi Uptime
					piUptime(PiUptime);
					//Pi CPU
					int PiCPUint = piCPU(PiCPU);
					//Pi IP
					piIP(PiIP1, PiIP2, PiIP3);
					//Helligkeit
					int brightness = readBrightness();
					//Grafiken für Pi Informationen erstellen
					// Grafik für Helligkeit
					drawSquare(S4,R1-20,328,60,GREY);
					drawCorner(S4,R1-20,328,60,WHITE);
					drawSquare(S4+100,R1-17,225,54,WHITE);
					drawCorner(S4+100,R1-17,225,54,GREY);
					put_string(S4+6, R1+4, "Helligkeit", WHITE);
					// Grafik für Uptime
					drawSquare(S4,R2-20,328,60,GREY);
					drawCorner(S4,R2-20,328,60,WHITE);
					drawSquare(S4+100,R2-17,225,54,WHITE);
					drawCorner(S4+100,R2-17,225,54,GREY);
					put_string(S4+6, R2+4, "Uptime", WHITE);
					// Grafik für Temp
					drawSquare(S4,R3-20,328,60,GREY);
					drawCorner(S4,R3-20,328,60,WHITE);
					drawSquare(S4+100,R3-17,225,54,WHITE);
					drawCorner(S4+100,R3-17,225,54,GREY);
					put_string(S4+6, R3+4, "Temp", WHITE);
					// Grafik für CPU
					drawSquare(S4,R4-20,328,60,GREY);
					drawCorner(S4,R4-20,328,60,WHITE);
					drawSquare(S4+100,R4-17,225,54,WHITE);
					drawCorner(S4+100,R4-17,225,54,GREY);
					put_string(S4+6, R4+4, "CPU", WHITE);
					// Grafik für IP
					drawSquare(S4,R5-20,328,60,GREY);
					drawCorner(S4,R5-20,328,60,WHITE);
					drawSquare(S4+100,R5-17,225,54,WHITE);
					drawCorner(S4+100,R5-17,225,54,GREY);
					put_string(S4+6, R5+4, "IP", WHITE);
					// Helligkeit
					drawSquare(S4+105, R1, Fw, 21, LTGREY);
					createData(S4+110, R1, "10%");
					drawSquare(S4+160, R1, Fw, 21, LTGREY);
					createData(S4+165, R1, "30%");
					drawSquare(S4+215, R1, Fw, 21, LTGREY);
					createData(S4+220, R1, "60%");
					drawSquare(S4+270, R1, Fw, 21, LTGREY);
					createData(S4+270, R1, "100%");
					if (brightness == 25)
					drawSquare(S4+105, R1, Fw, 21, GREEN);
					if (brightness == 76)
					drawSquare(S4+160, R1, Fw, 21, GREEN);
					if (brightness == 150)
					drawSquare(S4+215, R1, Fw, 21, GREEN);
					if (brightness == 255)
					drawSquare(S4+270, R1, Fw, 21, GREEN);
					// Uptime
					put_string(S6-50, R2+4, PiUptime, GREY);
					// Temp
					if (T > 20){
						drawSquare(S6-20, R3, Fw, 21, LIGHT_GREEN);
						createData(S6-25, R3, PiTemp);
					}
					else if (T > 40){
						drawSquare(S6-20, R3, Fw, 21, LIGHT_RED);
						createData(S6-25, R3, PiTemp);
					}
					else if (T > 60){
						drawSquare(S6-20, R3, Fw, 21, RED);
						createData(S6-25, R3, PiTemp);
					}
					else{
						drawSquare(S6-20, R3, Fw, 21, GREEN);
						createData(S6-25, R3, PiTemp);
					}
					// CPU
					if (PiCPUint > 5){
						drawSquare(S6-20, R4, Fw, 21, LIGHT_GREEN);
						createData(S6-10, R4, PiCPU);
					}
					else if (PiCPUint > 10){
						drawSquare(S6-20, R4, Fw, 21, LIGHT_RED);
						createData(S6-10, R4, PiCPU);
					}
					else if (PiCPUint > 20){
						drawSquare(S6-20, R4, Fw, 21, RED);
						createData(S6-10, R4, PiCPU);
					}
					else{
						drawSquare(S6-20, R4, Fw, 21, GREEN);
						createData(S6-10, R4, PiCPU);
					}
					// IP
					createData(S6-50, R5-32, PiIP1);
					if (strcmp (PiIP1,PiIP2) != 0)
						createData(S6-50, R5-16, PiIP2);
					if (strcmp (PiIP2,PiIP3) != 0)
						createData(S6-50, R5, PiIP3);
					writeScreen(ScreenCounter, 60);
				}
				break;
			}
//####################################################
			//Wetter Grafik erstellen
			case ScreenWetter:{
				GuiTime = makeWetterGui(GuiTime, counter, weatherTime);    //Ausgelagert in die Datei WetterGui.h
				break;
			}
//####################################################
			default:{
				if(E3DC_S10 ==1)
					writeScreen(ScreenChange, ScreenAktuell);
				else if(Homematic_GUI == 1)
					writeScreen(ScreenChange, ScreenHM);
				else
					writeScreen(ScreenChange, ScreenMonitor);
					break;
			}
		} //switch(screenChange)

//####################################################
	//WatchdogHM Daten für WD schreiben
		int Unixtime[UnixtimeMAX];
		if(counter == 0 && Homematic_GUI == 1){
			char Value[20];
			read_HM(ISE_UnixTime, 10, Value);
			UnixTime = atoi(Value);
			read_HM(ISE_TimestampHM, 16,TimestampHM);
			writeUnixtime(UnixtimeHM, UnixTime);
		}
	//Watchdog GuiMain
		if(counter == 0){
			int AktuallTime = time(NULL);
			writeUnixtime(UnixtimeGui, AktuallTime);
		}
	//Time
		int screenState = readScreen(ScreenState);
		if(GuiTime == RscpTime && E3DC_S10 == 1 && screenState == ScreenOn){
			put_string(20, 458, "Letzter Zeitstempel: ", GREY);
			int AktuallTime = time(NULL);
			int TAG_UnixTime = readUnixtime(UnixtimeE3dc);
			int DiffTime = AktuallTime - TAG_UnixTime;
			snprintf (OUT, (size_t)100, "%s %s", TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME);
			if(DiffTime > 180){
				drawOutput(190,458,170,12, OUT, RED);
				changeStop = 1;
			}
			else{
				drawOutput(190,458,170,12, OUT, GREEN);
				changeStop = 0;
			}
			if(E3DC_S10 == 1 && counter == 0 && Screen[ScreenHistory] > 0){
				float HistoryUnix = readHistory(dataTime, Screen[ScreenHistory]);
				time_t timeStamp;
				timeStamp = (time_t)HistoryUnix;
				struct tm *now;
				now = localtime( &timeStamp );
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
			put_string(20, 458, "Letzter Zeitstempel der Homematic: ", GREY);
			//Time
			int AktuallTime = time(NULL);
			int DiffTime = AktuallTime - UnixTime;
			if(DiffTime > 180){
				drawOutput(300,458,170,12, TimestampHM, RED);
			}
			else{
				drawOutput(300,458,170,12, TimestampHM, GREEN);
			}
		}
		else if(GuiTime == WeatherTime && screenState == ScreenOn){
			put_string(WetterS1, 458, "Aktuelle Zeit: ", GREY);
			time_t timeStamp;
			struct tm *now;
			time( &timeStamp );
			now = localtime( &timeStamp );
			strftime (OUT,100,"%d.%m.%Y %H:%M:%S",now);
			drawOutput(170,458,170,12, OUT, GREEN);
			put_string(400, 458, "Yahoo Datensatz: ", GREY);
			drawOutput(550,458,170,12, weatherTime, GREEN);
		}
		else if(screenState == ScreenOn){
			put_string(20, 458, "Aktuelle Zeit: ", GREY);
			time_t timeStamp;
			struct tm *now;
			time( &timeStamp );
			now = localtime( &timeStamp );
			strftime (OUT,100,"%d.%m.%Y %H:%M:%S",now);
			drawOutput(150,458,170,12, OUT, GREEN);
			madeBy(OUT);
			put_string(325,458, OUT, BLUE);
		}
	//Bildschirmschoner
		Screen[ScreenSaver] = readScreen(ScreenSaver);      //Zählerdatei für den Bildschirmschoner auslesen
		int ScreenSaverCounter = Screen[ScreenSaver] +1;
		if(ScreenSaverCounter == ScreenSaverTime){
			screenOff();
			ScreenSaverCounter = 0;
			if(E3DC_S10 ==1){
				writeScreen(ScreenChange, ScreenAktuell);
				writeScreen(ScreenHistory, historyOff);
			}
			else if(Homematic_GUI == 1)
				writeScreen(ScreenChange, ScreenHM);
			else
				writeScreen(ScreenChange, ScreenWetter);
		}
		writeScreen(ScreenSaver, ScreenSaverCounter);

	//HistoryValues vom S10 mit "S10history" abfragen.
		if (historyAktiv == true){
			if(HistoryCounter == 0){
				system("/home/pi/E3dcGui/S10history/S10history -T &");// > /dev/null 2>&1");
				HistoryCounter = historyDelay;
			}
			if(HistoryCounter == (historyDelay-15))
				system("/home/pi/E3dcGui/S10history/S10history -Y &");//  > /dev/null 2>&1");
			HistoryCounter --;
		}
	//Abfrageintervall
		sleep(1);
	}
}

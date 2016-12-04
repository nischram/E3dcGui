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


//####################################
int main(){
	signal(SIGINT, INThandler);

	int  xres,yres;

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	int GuiTime, change = 0, changeStop = 0;
	char batch[256], OUT [100],Path[100],Value[20],writeTxt[20],TimestampHM[20],RscpTimestamp[40];
  char TAG_EMS_OUT_DATE[20], TAG_EMS_OUT_TIME[20], serialnumber[17];
	char PVI_Current_1[10], PVI_Current_2[10];           //PVI Current wird als char gespeichert und gelesen damit die Nachkommastellen angezeigt werden können
	int counter, ScreenSaverCounter, read;
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
		readRscpChar(TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME, serialnumber, PVI_Current_1, PVI_Current_2);
		GuiTime = PiTime;

		Screen[ScreenCounter] = readScreen(ScreenCounter);
		counter = Screen[ScreenCounter] -1;
		if(counter < 0)
			counter = 0;
		writeScreen(ScreenCounter, counter);

		Screen[ScreenChange] = readScreen(ScreenChange);
		int screenChange = Screen[ScreenChange];

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
						drawSquare(2,2,800,480,LTGREY);																				//drawSquare ist eine Funktion im frameBuffer.c zum erstellen eines farbigen Viereck
						drawCorner(2, 2, 800, 480, BLACK);																		//drawCorner ist eine eigene Funktion in Frame/DrawCorner.c zum erstellen von abgerundetetn Ecken
		        drawSquare(12,12,778,458,WHITE);
						drawCorner(12, 12, 778, 458, LTGREY);
		        DrawImage("EinstImage", 180, 12);					//DrawImage ist eine Funktion um ein Bild auf dem Display zu Zeichnen
						if(E3DC_S10 ==1){
							DrawImage("AktuellImage", 270, 12);
							DrawImage("LangzeitImage", 360, 12);
							DrawImage("MonitorImage", 450, 12);
						}
						if(Homematic_GUI ==1){
							DrawImage("HMImage", 540, 12);
						}
						DrawImage("PviImage", 40, 50);
						int pmAktivPhases = readRscp(PosPMPhases);
						DrawNetImage(pmAktivPhases);              //Einfügen der Grafik für NetImage oder NetImageOff je nach Status der Phasen am LM in der Frame/DrawNetImage.h
						DrawImage("BatImage", 50, 310);
						DrawImage("HomeImage", 640, 310);
						DrawImage("S10Image", 270, 110);
						DrawImage("ExtImage", 40, 190);
						DrawImage("WallboxImage", 650, 190);
					}
				}
				//Bildwechsel für > > >
				if(changeStop == 0){
					if(change == 1){
						change = 0;
					}
					else{
						change = 1;
					}
				}
				if(counter == 0 || screenState == ScreenOn){
					//Programm-Start
					drawSquare(340,100,140,12,WHITE);
					put_string(340,100,serialnumber,GREY);																						//put_string ist eine Funktion im frameBuffer.c für eine Textausgabe
					//PVI
					int TAG_PVI = readRscp(PosPVI);
					int TAG_PVIState = readRscp(PosPVIState);
					snprintf (OUT, (size_t)100, "%i W", TAG_PVI);
					if(TAG_PVIState >= 1){
						if(TAG_PVI > 50){
							drawSquare(190, 120,80,12,WHITE);
							put_stringRGB(190, 120, OUT, 225, 122, 34);
						}
						else{
							drawSquare(190, 120,80,12,WHITE);
							put_string(190, 120, OUT, GREY);
						}
						drawSquare(190,100,80,12,WHITE);
						if(TAG_PVI > 0){
							if(change == 1){
								put_string(190,100,"> > >",GREY);
							}
							else{
								put_string(190,100," > > ",GREY);
							}
						}
					}
					else{
						drawSquare(190, 120,80,12,WHITE);
						put_string(190, 120, "PVI-DOWN", RED);
					}
					//Grid
					int TAG_Grid = readRscp(PosGrid);
					int TAG_PMState = readRscp(PosPMState);
					if(TAG_PMState >= 1){
						if(TAG_Grid < 0){
							TAG_Grid = TAG_Grid * -1;
							snprintf (OUT, (size_t)100, "%i W", TAG_Grid);
							drawSquare(570, 120,80,12,WHITE);
							put_string(570, 120, OUT, CYAN);
							drawSquare(570,100,80,12,WHITE);
							if (TAG_Grid > 15){
								if(change == 1){
									put_string(570,100,"> > >", GREY);
								}
								else{
									put_string(570,100," > > ", GREY);
								}
							}
							else{
								drawSquare(570, 120,80,12,WHITE);
								put_string(570, 120, OUT, LIGHT_BLUE);
							}
						}
						else{
							snprintf (OUT, (size_t)100, "%i W", TAG_Grid);
							drawSquare(570, 120,80,12,WHITE);
							put_string(570, 120, OUT, BLUE);
							drawSquare(570,100,80,12,WHITE);
							if (TAG_Grid > 5){
								if(change == 1){
									put_string(570,100,"< < <",GREY);
								}
								else{
									put_string(570,100," < < ",GREY);
								}
							}
							else{
								drawSquare(570, 120,80,12,WHITE);
								put_string(570, 120, OUT, GREEN);
							}
						}
					}
					else{
						drawSquare(570, 120,80,12,WHITE);
						put_string(570, 120, "LM-DOWN", RED);
					}
					//Home
					int TAG_Home = readRscp(PosHome);
					snprintf (OUT, (size_t)100, "%i W", TAG_Home);
					drawSquare(570, 360,80,12,WHITE);
					put_string(570, 360, OUT, GREY);
					drawSquare(570,380,80,12,WHITE);
					if(change == 1){
						put_string(570,380,"> > >", GREY);
					}
					else{
						put_string(570,380," > > ", GREY);
					}
					//Battery
					int TAG_Bat = readRscp(PosBat);
					if(TAG_Bat < 0){
						TAG_Bat = TAG_Bat * -1;
						snprintf (OUT, (size_t)100, "%i W", TAG_Bat);
						drawSquare(190, 360,80,12,WHITE);
						put_string(190, 360, OUT, GREY);
						drawSquare(190,380,80,12,WHITE);
						if(change == 1){
							put_string(190,380,"> > >",GREY);
						}
						else{
							put_string(190,380," > > ",GREY);
						}
					}
					else{
						snprintf (OUT, (size_t)100, "%i W", TAG_Bat);
						drawSquare(190, 360,80,12,WHITE);
						put_string(190, 360, OUT, GREY);
						drawSquare(190,380,80,12,WHITE);
						if(TAG_Bat > 0){
							if(change == 1){
								put_string(190,380,"< < <", GREY);
							}
							else{
								put_string(190,380," < < ", GREY);
							}
						}
					}
					//Additional
					if(Additional == 1){
						int TAG_ADD = readRscp(PosADD);
						snprintf (OUT, (size_t)100, "%i W", TAG_ADD);
						drawSquare(190,248,60,12,WHITE);
						put_string(190,248,OUT,GREY);
						drawSquare(190,236,60,12,WHITE);
						if(TAG_ADD > 0){
							if(change == 1){
								put_string(190,236,"> > >",GREY);
							}
							else{
								put_string(190,236," > > ",GREY);
							}
						}
					}
					else{
						drawSquare(40,190,200,120,WHITE);
					}
					//Wallbox
					if(Wallbox == 1){
						int TAG_WbAll = readRscp(PosWbAll);
						int TAG_WbSolar = readRscp(PosWbSolar);
						drawSquare(570,236,60,12,WHITE);
						if(TAG_WbAll > 1){
							if(change == 1){
								put_string(570,236,"> > >",GREY);
							}
							else{
								put_string(570,236," > > ",GREY);
							}
						}
						if(TAG_WbSolar > 0){
						 snprintf (OUT, (size_t)100, "%i W", TAG_WbAll);
						 drawSquare(570,248,60,12,WHITE);
						 put_string(570,248,OUT,GREY);
						 put_string(570,258,"All",GREY);
						 snprintf (OUT, (size_t)100, "%i W", TAG_WbSolar);
						 drawSquare(570,224,60,12,WHITE);
						 put_stringRGB(570,224,OUT, 225, 122, 34);
						 put_stringRGB(570,214,"Solar", 225, 122, 34);
						}
						else{
							snprintf (OUT, (size_t)100, "%i W", TAG_WbAll);
							drawSquare(570,248,60,12,WHITE);
							put_string(570,248,OUT,GREY);
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
					drawSquare(80,430,50,12,WHITE);
					if(TAG_BatState >= 1){
						int SOCx = TAG_SOC * 0.82;
						drawSquare(74,340,48,82,WHITE);
						drawSquare(74,340+82-SOCx,48,SOCx,BLUE);
						put_string(80,432,OUT,GREY);
					}
					else{
						put_string(80,432,"OFF",RED);
					}
					//Autarky
					int TAG_Autarky = readRscp(PosAutarky);
					int Autarkyx = 2 * TAG_Autarky;
					drawSquare(405,366-200,60,200-Autarkyx,WHITE);
					drawSquare(405,366-Autarkyx,60,Autarkyx,GREEN);
					drawSquare(420,380,45,12,WHITE);
					snprintf (OUT, (size_t)100, "%i %%", TAG_Autarky);
					put_string(420,380,OUT,GREY);
					snprintf (OUT, (size_t)100, "Autarkie");
					put_string(420,392,OUT,GREY);
					//SelfConsumption
					int TAG_SelfCon = readRscp(PosSelfCon);
					int SelfConx = 2 * TAG_SelfCon;
					drawSquare(340,366-200,60,200-SelfConx,WHITE);
					drawSquare(340,366-SelfConx,60,SelfConx,LTGREY);
					drawSquare(350,380,45,12,WHITE);
					snprintf (OUT, (size_t)100, "%i %%", TAG_SelfCon);
					put_string(350,380,OUT,GREY);
					snprintf (OUT, (size_t)100, "Eigenstrom");
					put_string(310,392,OUT,GREY);
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
	 				drawSquare(2,2,800,480,LTGREY);
	 				drawCorner(2, 2, 800, 480, BLACK);
	 				drawSquare(12,12,778,458,WHITE);
	 				drawCorner(12, 12, 778, 458, LTGREY);
	 				DrawImage("EinstImage", 180, 12);
	 				if(E3DC_S10 ==1){
	 					DrawImage("AktuellImage", 270, 12);
	 					DrawImage("LangzeitImage", 360, 12);
						DrawImage("MonitorImage", 450, 12);
	 				}
	 				if(Homematic_GUI ==1){
	 					DrawImage("HMImage", 540, 12);
	 				}
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
						drawSquare(2,2,800,480,LTGREY);																				//drawSquare ist eine Funktion im frameBuffer.c zum erstellen eines farbigen Viereck
						drawCorner(2, 2, 800, 480, BLACK);																		//drawCorner ist eine eigene Funktion in Frame/DrawCorner.c zum erstellen von abgerundetetn Ecken
		        drawSquare(12,12,778,458,WHITE);
						drawCorner(12, 12, 778, 458, LTGREY);
		        DrawImage("EinstImage", 180, 12);					//DrawImage ist eine Funktion um ein Bild auf dem Display zu Zeichnen
						if(E3DC_S10 ==1){
							DrawImage("AktuellImage", 270, 12);
							DrawImage("LangzeitImage", 360, 12);
							DrawImage("MonitorImage", 450, 12);
						}
						if(Homematic_GUI ==1){
							DrawImage("HMImage", 540, 12);
						}
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
						drawSquare(270, 270,80,12,WHITE);
						put_string(270, 270, OUT, GREY);
						int TAG_PVIDCU1 = readRscp(PosPVIDCU1);
						snprintf (OUT, (size_t)100, "%i V", TAG_PVIDCU1);
						drawSquare(270, 290,80,12,WHITE);
						put_string(270, 290, OUT, GREY);
						if(TAG_PVIDCP1 > 0){
							double TAG_PVIDCI1 = atof(PVI_Current_1);
							snprintf (OUT, (size_t)100, "%2.2f A", TAG_PVIDCI1);
							drawSquare(270, 310,80,12,WHITE);
							put_string(270, 310, OUT, GREY);
						}
					}
					else{
						drawSquare(250, 250,80,12,WHITE);
						put_string(250, 250, "PVI-DOWN", RED);
					}
					//PVI Tracker 2
					if (PVI_TRACKER == 2){
						//PVI DC Power2
						if(TAG_PVIState >= 1){
							int TAG_PVIDCP2 = readRscp(PosPVIDCP2);
							snprintf (OUT, (size_t)100, "%i W", TAG_PVIDCP2);
							drawSquare(470, 270,80,12,WHITE);
							put_string(470, 270, OUT, GREY);
							int TAG_PVIDCU2 = readRscp(PosPVIDCU2);
							snprintf (OUT, (size_t)100, "%i V", TAG_PVIDCU2);
							drawSquare(470, 290,80,12,WHITE);
							put_string(470, 290, OUT, GREY);
							if(TAG_PVIDCP2 > 0){
								double TAG_PVIDCI2 = atof(PVI_Current_2);
								snprintf (OUT, (size_t)100, "%2.2f A", TAG_PVIDCI2);
								drawSquare(470, 310,80,12,WHITE);
								put_string(470, 310, OUT, GREY);
							}
						}
						else{
							drawSquare(450, 250,80,12,WHITE);
							put_string(450, 250, "PVI-DOWN", RED);
						}
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
			case ScreenSetup:{																												//Im ScreenSetup werden nur die Rückgaben per Datei aus dem "screenSave" Programm dargestellt
				GuiTime = PiTime;
				Screen[ScreenShutdown] = readScreen(ScreenShutdown);
				int screenShutdown = Screen[ScreenShutdown];

				if(counter == 0){
					drawSquare(2,2,800,480,LTGREY);
					drawCorner(2, 2, 800, 480, BLACK);
					drawSquare(12,12,778,458,WHITE);
					drawCorner(12, 12, 778, 458, LTGREY);
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
					char file_Path [100],file_read [100];
					FILE *fp;
					snprintf (file_Path, (size_t)100, "/home/pi/E3dcGui/Data/Timezone.txt");
					fp = fopen(file_Path, "r");
					if(fp == NULL) {
						printf("Datei konnte NICHT geoeffnet werden.\n");
						snprintf (file_read, (size_t)20, "Summertime");
					}
					else {
						fgets(file_read,20,fp);
						strtok(file_read, "\n");
						fclose(fp);
					}
					if (strcmp ("Wintertime",file_read) == 0){
						drawSquare(S1,R5-20,180,30,GREY);
						drawCorner(S1,R5-20,180,30, WHITE);
						put_string(S1+20,R5-20+8, "Winterzeit", WHITE);
					}
					else{
						drawSquare(S1,R5-20,180,30,GREY);
						drawCorner(S1,R5-20,180,30, WHITE);
						put_string(S1+20,R5-20+8, "Sommerzeit", WHITE);
					}
					DrawImage("EinstImage", 180, 12);
					if(E3DC_S10 ==1){
						DrawImage("AktuellImage", 270, 12);
						DrawImage("LangzeitImage", 360, 12);
						DrawImage("MonitorImage", 450, 12);
					}
					if(Homematic_GUI ==1){
						DrawImage("HMImage", 540, 12);
					}
					writeScreen(ScreenCounter, 60);
					//Daten für PI Informationen laden
					char PiTemp[20], PiUptime[20], PiCPU[20], PiIP1[20], PiIP2[20], PiIP3[20];
					//Pi Temp
					FILE *temperatureFile;
					double T;
					temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
					fscanf (temperatureFile, "%lf", &T);
					T /= 1000;
					snprintf (PiTemp, (size_t)20, "%3.1f 'C", T);
					fclose (temperatureFile);
					//Pi Uptime
					struct sysinfo info;
					sysinfo(&info);
					snprintf (Value, (size_t)20, "%ld\n", info.uptime);
					double PiUptimeint = atoi(Value);
					PiUptimeint = (double)PiUptimeint /60;
					snprintf (PiUptime, (size_t)20, "%8.1f Minuten", PiUptimeint);
					//CPU
					FILE *cpuFile;
					snprintf(batch, (size_t)256, "vmstat| head -3l | tail -1l  | cut -b 73-75");
					cpuFile = popen (batch, "r");
					fgets(OUT,sizeof(OUT),cpuFile);
					int PiCPUint = atoi(OUT);
					PiCPUint = 100 - PiCPUint;
					snprintf (PiCPU, (size_t)20, "%i %%", PiCPUint);
					pclose (cpuFile);
					//CPU
					FILE *ipFile;
					snprintf(batch, (size_t)256, "ifconfig | grep \" inet Adresse\" | cut -d: -f2 | cut -d\" \" -f1");
					ipFile = popen (batch, "r");
					fgets(OUT,100,ipFile);
					sscanf(OUT, "%s\n", PiIP1);
					fgets(OUT,100,ipFile);
					sscanf(OUT, "%s\n", PiIP2);
					fgets(OUT,100,ipFile);
					sscanf(OUT, "%s\n", PiIP3);
					pclose (ipFile);
					//Grafiken für Pi nformationen erstellen
					// Grafik für Uptime
					drawSquare(S4,R2-20,320,60,GREY);
					drawCorner(S4,R2-20,320,60,WHITE);
					drawSquare(S4+60,R2-17,257,54,WHITE);
					drawCorner(S4+60,R2-17,257,54,GREY);
					put_string(S4+6, R2+4, "Uptime", WHITE);
					// Grafik für Temp
					drawSquare(S4,R3-20,320,60,GREY);
					drawCorner(S4,R3-20,320,60,WHITE);
					drawSquare(S4+60,R3-17,257,54,WHITE);
					drawCorner(S4+60,R3-17,257,54,GREY);
					put_string(S4+6, R3+4, "Temp", WHITE);
					// Grafik für CPU
					drawSquare(S4,R4-20,320,60,GREY);
					drawCorner(S4,R4-20,320,60,WHITE);
					drawSquare(S4+60,R4-17,257,54,WHITE);
					drawCorner(S4+60,R4-17,257,54,GREY);
					put_string(S4+6, R4+4, "CPU", WHITE);
					// Grafik für IP
					drawSquare(S4,R5-20,320,60,GREY);
					drawCorner(S4,R5-20,320,60,WHITE);
					drawSquare(S4+60,R5-17,257,54,WHITE);
					drawCorner(S4+60,R5-17,257,54,GREY);
					put_string(S4+6, R5+4, "IP", WHITE);
					// Uptime
					drawSquare(S6-30, R2+4, 140, 12, WHITE);
					put_string(S6-30, R2+4, PiUptime, GREY);
					// Temp
					if (T > 20){
						drawSquare(S6, R3, Fw, 21, LIGHT_GREEN);
						createData(S6, R3, PiTemp);
					}
					else if (T > 40){
						drawSquare(S6, R3, Fw, 21, LIGHT_RED);
						createData(S6, R3, PiTemp);
					}
					else if (T > 60){
						drawSquare(S6, R3, Fw, 21, RED);
						createData(S6, R3, PiTemp);
					}
					else{
						drawSquare(S6, R3, Fw, 21, GREEN);
						createData(S6, R3, PiTemp);
					}
					// CPU
					if (PiCPUint > 5){
						drawSquare(S6, R4, Fw, 21, LIGHT_GREEN);
						createData(S6, R4, PiCPU);
					}
					else if (PiCPUint > 10){
						drawSquare(S6, R4, Fw, 21, LIGHT_RED);
						createData(S6, R4, PiCPU);
					}
					else if (PiCPUint > 20){
						drawSquare(S6, R4, Fw, 21, RED);
						createData(S6, R4, PiCPU);
					}
					else{
						drawSquare(S6, R4, Fw, 21, GREEN);
						createData(S6, R4, PiCPU);
					}
					// IP
					createData(S6-40, R5-32, PiIP1);
					createData(S6-40, R5-16, PiIP2);
					if (strcmp (PiIP2,PiIP3) != 0)
						createData(S6-40, R5, PiIP3);
					writeScreen(ScreenCounter, 60);
				}
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
		//!!!! Nico !!!!! Astro Watchdog kontrolle
		if (AstroUse == 1){
			int stateAstro = readScreen(ScreenAstro);
			if (stateAstro == 1){
				drawSquare(760,20,20,20,GREEN);
				drawCorner(760,20,20,20,WHITE);
			}
			else{
				drawSquare(760,20,20,20,LIGHT_RED);
				drawCorner(760,20,20,20,WHITE);
			}
			int countAstro = countAstro + 1;
			if (countAstro == 4){
				writeScreen(ScreenAstro, false);
				countAstro = 0;
			}
		}
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
				drawSquare(190,458,170,12,WHITE);
				put_string(190, 458, OUT, RED);
				changeStop = 1;
			}
			else{
				drawSquare(190,458,170,12,WHITE);
				put_string(190, 458, OUT, GREEN);
				changeStop = 0;
			}
		}
		else if(GuiTime == HomematicTime && Homematic_GUI == 1){
			put_string(20, 458, "Letzter Zeitstempel der Homematic: ", GREY);
			//Time
			int AktuallTime = time(NULL);
			int DiffTime = AktuallTime - UnixTime;
			if(DiffTime > 180){
				drawSquare(300,458,170,12,WHITE);
				put_string(300, 458, TimestampHM, RED);
			}
			else{
				drawSquare(300,458,170,12,WHITE);
				put_string(300, 458, TimestampHM, GREEN);
			}
		}
		else if(screenState == ScreenOn){
			put_string(20, 458, "Aktuelle Zeit: ", GREY);
			time_t timeStamp;
			struct tm *now;
			time( &timeStamp );
			now = localtime( &timeStamp );
			strftime (OUT,100,"%d.%m.%Y %H:%M:%S",now);
			drawSquare(150,458,170,12,WHITE);
			put_string(150,458, OUT, GREEN);
			madeBy(OUT);
			put_string(325,458, OUT, BLUE);
		}
	//Bildschirmschoner
		Screen[ScreenSaver] = readScreen(ScreenSaver);      //Zählerdatei für den Bildschirmschoner auslesen
		int ScreenSaverCounter = Screen[ScreenSaver] +1;
		if(ScreenSaverCounter == ScreenSaverTime){
			screenOff();
			ScreenSaverCounter = 0;
			if(E3DC_S10 ==1)
				writeScreen(ScreenChange, ScreenAktuell);
			else if(Homematic_GUI == 1)
				writeScreen(ScreenChange, ScreenHM);
			else
				writeScreen(ScreenChange, ScreenMonitor);
		}
		writeScreen(ScreenSaver, ScreenSaverCounter);
	//Abfrageintervall
		sleep(1);
	}
}

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
#include "Frame/touch.h"
#include "Frame/touch.c"
#include "Frame/framebuffer.c"
#include "Frame/DrawSkala.c"
#include "Frame/DrawLine.c"
#include "Frame/DrawSOC.c"
#include "Frame/DrawImage.h"
#include "Frame/DrawCorner.c"
#include "funktion.h"
#include "parameter.h"

//####################################
int main(){
	signal(SIGINT, INThandler);

	int  xres,yres;

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	char TAG_Time[20],TAG_Date[20];
	char TAG_PVI[10],TAG_Bat[10],TAG_Home[10],TAG_Grid[10],TAG_SOC[10];
	char TAG_BatState[10],TAG_Autarky[20],TAG_SelfCon[20],TAG_SerialNr[20];
	char TAG_UnixTime[20],TAG_ADD[10],TAG_WbAll[10],TAG_WbSolar[10],TAG_PVIState[10],TAG_PMState[10];

	int GuiTime, change = 0, changeStop = 0;
	char batch[256], OUT [100],Path[100],Value[20],delValue[20],writeTxt[20],screenState[30],TimestampHM[20];
	int counter, ScreenSaverCounter;
	int readOK, UnixTime;

  screenOn();
  writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
	if(E3DC_S10 ==1){
		writeData("/mnt/RAMDisk/ScreenChange.txt", "1\n");
	}
	else{
		writeData("/mnt/RAMDisk/ScreenChange.txt", "12\n");
	}
	writeData("/mnt/RAMDisk/ScreenShutdown.txt", "5\n");
  writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");

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
		GuiTime = PiTime;
		//Daten aus RAMDisk Datei einlesen
		readRscpGui(TAG_Time,TAG_Date,TAG_PVI,TAG_Bat,TAG_Home,TAG_Grid,TAG_SOC,TAG_BatState,TAG_Autarky,TAG_SelfCon,TAG_SerialNr,TAG_UnixTime,TAG_ADD,TAG_WbAll,TAG_WbSolar,TAG_PVIState,TAG_PMState);
		readData("ScreenCounter", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
		counter = atoi(Value) -1;
		if(counter < 0){
			counter = 0;
		}
		snprintf (OUT, (size_t)100, "%i", counter);
		writeData("/mnt/RAMDisk/ScreenCounter.txt", OUT);
		readData("ScreenChange", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
		int screenChange = atoi(Value);
		readData("ScreenShutdown", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
		int screenShutdown = atoi(Value);
		readData("ScreenState", 1, screenState,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
//####################################
		switch(screenChange){
//####################################
//Aktuell Grafik erstellen
			case ScreenAktuell:{
				GuiTime = RscpTime;
				if(counter == 0){
					drawSquare(2,2,800,480,LTGREY);																				//drawSquare ist eine Funktion im frameBuffer.c zum erstellen eines farbigen Viereck
					drawCorner(2, 2, 800, 480, BLACK);																		//drawCorner ist eine eigene Funktion in Frame/DrawCorner.c zum erstellen von abgerundetetn Ecken
	        drawSquare(12,12,778,458,WHITE);
					drawCorner(12, 12, 778, 458, LTGREY);
	        DrawImage("/home/pi/E3dcGui/Image/EinstImage.ppm", 180, 12);					//DrawImage ist eine Funktion um ein Bild auf dem Display zu Zeichnen
					if(E3DC_S10 ==1){
						DrawImage("/home/pi/E3dcGui/Image/AktuellImage.ppm", 270, 12);
						DrawImage("/home/pi/E3dcGui/Image/LangzeitImage.ppm", 360, 12);
					}
					DrawImage("/home/pi/E3dcGui/Image/MonitorImage.ppm", 450, 12);
					if(Homematic_GUI ==1){
						DrawImage("/home/pi/E3dcGui/Image/HMImage.ppm", 540, 12);
					}
					DrawImage("/home/pi/E3dcGui/Image/PviImage.ppm", 40, 50);
					DrawImage("/home/pi/E3dcGui/Image/NetImage.ppm", 640, 50);
					DrawImage("/home/pi/E3dcGui/Image/BatImage.ppm", 50, 310);
					DrawImage("/home/pi/E3dcGui/Image/HomeImage.ppm", 640, 310);
					DrawImage("/home/pi/E3dcGui/Image/S10Image.ppm", 270, 110);
					DrawImage("/home/pi/E3dcGui/Image/ExtImage.ppm", 40, 190);
					DrawImage("/home/pi/E3dcGui/Image/WallboxImage.ppm", 650, 190);
					writeData("/mnt/RAMDisk/ScreenCounter.txt", "120");
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
				//Programm-Start
				snprintf (OUT, (size_t)100, "%s", TAG_SerialNr);
				drawSquare(340,100,140,12,WHITE);
				put_string(340,100,OUT,GREY);																						//put_string ist eine Funktion im frameBuffer.c für eine Textausgabe
				//PVI
				snprintf (OUT, (size_t)100, "%s W", TAG_PVI);
				int TAG_PVIint = atoi(TAG_PVI);
				int TAG_PVIStateint = atoi(TAG_PVIState);
				if(TAG_PVIStateint >= 1){
					if(TAG_PVIint > 50){
						drawSquare(190, 120,80,12,WHITE);
						put_stringRGB(190, 120, OUT, 225, 122, 34);
					}
					else{
  					 drawSquare(190, 120,80,12,WHITE);
  					 put_string(190, 120, OUT, GREY);
  				 }
					 drawSquare(190,100,80,12,WHITE);
	 				 if(TAG_PVIint > 0){
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
				 int TAG_Gridint = atoi(TAG_Grid);
				 int TAG_PMStateint = atoi(TAG_PMState);
				 if(TAG_PMStateint >= 1){
					if(TAG_Gridint < 0){
						TAG_Gridint = TAG_Gridint * -1;
						snprintf (OUT, (size_t)100, "%i W", TAG_Gridint);
						drawSquare(570, 120,80,12,WHITE);
						put_string(570, 120, OUT, CYAN);
						drawSquare(570,100,80,12,WHITE);
						if (TAG_Gridint > 15){
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
						//printf("Grid-IN %s; ", OUT);
					}
					else{
						snprintf (OUT, (size_t)100, "%s W", TAG_Grid);
						drawSquare(570, 120,80,12,WHITE);
						put_string(570, 120, OUT, BLUE);
						drawSquare(570,100,80,12,WHITE);
						if (TAG_Gridint > 5){
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
				snprintf (OUT, (size_t)100, "%s W", TAG_Home);
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
				int TAG_Batint = atoi(TAG_Bat);
				if(TAG_Batint < 0){
					TAG_Batint = TAG_Batint * -1;
					snprintf (OUT, (size_t)100, "%i W", TAG_Batint);
					drawSquare(190, 360,80,12,WHITE);
					put_string(190, 360, OUT, GREY);
					drawSquare(190,380,80,12,WHITE);
					if(change == 1){
						put_string(190,380,"> > >",GREY);
					}
					else{
						put_string(190,380," > > ",GREY);
					}
					//printf("Bat OUT %s; ", OUT);
				}
				else{
					snprintf (OUT, (size_t)100, "%s W", TAG_Bat);
					drawSquare(190, 360,80,12,WHITE);
					put_string(190, 360, OUT, GREY);
					drawSquare(190,380,80,12,WHITE);
					if(TAG_Batint > 0){
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
					snprintf (OUT, (size_t)100, "%s W", TAG_ADD);
					drawSquare(190,248,60,12,WHITE);
					put_string(190,248,OUT,GREY);
					int TAG_ADDint = atoi(TAG_ADD);
					drawSquare(190,236,60,12,WHITE);
					if(TAG_ADDint > 0){
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
					int TAG_WbAllint = atoi(TAG_WbAll);
					int TAG_WbSolarint = atoi(TAG_WbSolar);
					drawSquare(570,236,60,12,WHITE);
					if(TAG_WbAllint > 1){
						if(change == 1){
							put_string(570,236,"> > >",GREY);
						}
						else{
							put_string(570,236," > > ",GREY);
						}
					}
					if(TAG_WbSolarint > 0){
					 snprintf (OUT, (size_t)100, "%s W", TAG_WbAll);
					 drawSquare(570,248,60,12,WHITE);
					 put_string(570,248,OUT,GREY);
					 put_string(570,258,"All",GREY);
					 snprintf (OUT, (size_t)100, "%s W", TAG_WbSolar);
					 drawSquare(570,224,60,12,WHITE);
					 put_stringRGB(570,224,OUT, 225, 122, 34);
					 put_stringRGB(570,214,"Solar", 225, 122, 34);
					}
					else{
						snprintf (OUT, (size_t)100, "%s W", TAG_WbAll);
						drawSquare(570,248,60,12,WHITE);
						put_string(570,248,OUT,GREY);
						put_string(570,258,"All",GREY);
					}
				}
				else{
					drawSquare(570,190,200,120,WHITE);
				}
				//SOC
				snprintf (OUT, (size_t)100, "%s %%", TAG_SOC);
				int TAG_BatStateint = atoi(TAG_BatState);
				drawSquare(80,430,50,12,WHITE);
				if(TAG_BatStateint >= 1){
					int TAG_SOCint = atoi(TAG_SOC);
					int SOCx = TAG_SOCint * 0.82;
					drawSquare(74,340,48,82,WHITE);
					drawSquare(74,340+82-SOCx,48,SOCx,BLUE);
					put_string(80,432,OUT,GREY);
				}
				else{
					put_string(80,432,"OFF",RED);
				}
				//Autarky
				int TAG_Autarkyint = atoi(TAG_Autarky);
				int Autarkyx = 2 * TAG_Autarkyint;
				drawSquare(405,366-200,60,200-Autarkyx,WHITE);
				drawSquare(405,366-Autarkyx,60,Autarkyx,GREEN);
				drawSquare(420,380,45,12,WHITE);
				snprintf (OUT, (size_t)100, "%s %%", TAG_Autarky);
				put_string(420,380,OUT,GREY);
				snprintf (OUT, (size_t)100, "Autarkie");
				put_string(420,392,OUT,GREY);
				//SelfConsumption
				int TAG_SelfConint = atoi(TAG_SelfCon);
				int SelfConx = 2 * TAG_SelfConint;
				drawSquare(340,366-200,60,200-SelfConx,WHITE);
				drawSquare(340,366-SelfConx,60,SelfConx,LTGREY);
				drawSquare(350,380,45,12,WHITE);
				snprintf (OUT, (size_t)100, "%s %%", TAG_SelfCon);
				put_string(350,380,OUT,GREY);
				snprintf (OUT, (size_t)100, "Eigenstrom");
				put_string(310,392,OUT,GREY);
				break;
			}
//####################################################
			//Langzeit Grafik erstellen
			case ScreenLangzeit:{
				GuiTime = RscpTime;
	 		  //Legende
	 			int LegendeSOC, LegendeSolar, LegendeADD, LegendeHome, LegendeNetIn, LegendeNetOut, LegendeBatIn, LegendeBatOut;
	 			readData("LegendeSOC", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
	 			if (strcmp ("false",Value) == 0){
	 				drawSquare(364,443,50,20, LTGREY);
	 				LegendeSOC = 0;
	 			}
	 			else {
	 				drawSquareRGB(364,443,50,20, 64, 134, 64);
	 				LegendeSOC = 1;
	 			}
	 			put_string(366,449," SOC", WHITE);
	 			readData("LegendeSolar", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
	 			if (strcmp ("false",Value) == 0){
	 				drawSquare(417,443,50,20, LTGREY);
	 				LegendeSolar = 0;
	 			}
	 			else {
	 				drawSquareRGB(417,443,50,20, 225, 122, 34);
	 				LegendeSolar = 1;
	 			}
	 			put_string(419,449,"Solar", WHITE);
	 			readData("LegendeHome", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
	 			if (strcmp ("false",Value) == 0){
	 				drawSquare(470,443,50,20, LTGREY);
	 				LegendeHome = 0;
	 			}
	 			else {
	 				drawSquareRGB(470,443,50,20, 225, 30, 30);
	 				LegendeHome = 1;
	 			}
	 			put_string(471,449," Home", WHITE);
	 			readData("LegendeNetIn", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
	 			if (strcmp ("false",Value) == 0){
	 				drawSquare(523,443,50,20, LTGREY);
	 				LegendeNetIn = 0;
	 			}
	 			else {
	 				drawSquareRGB(523,443,50,20, 0, 172, 255);
	 				LegendeNetIn = 1;
	 			}
	 			put_string(526,449,"NetIn", WHITE);
	 			readData("LegendeNetOut", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
	 			if (strcmp ("false",Value) == 0){
	 				drawSquare(576,443,50,20, LTGREY);
	 				LegendeNetOut = 0;
	 			}
	 			else {
	 				drawSquareRGB(576,443,50,20, 0, 0, 172);
	 				LegendeNetOut = 1;
	 			}
	 			put_string(577,449,"NetOut", WHITE);
	 			readData("LegendeBatIn", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
	 			if (strcmp ("false",Value) == 0){
	 				drawSquare(629,443,50,20, LTGREY);
	 				LegendeBatIn = 0;
	 			}
	 			else {
	 				drawSquareRGB(629,443,50,20, 0, 225, 64);
	 				LegendeBatIn = 1;
	 			}
	 			put_string(632,449,"BatIn", WHITE);
	 			readData("LegendeBatOut", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
	 			if (strcmp ("false",Value) == 0){
	 				drawSquare(682,443,50,20, LTGREY);
	 				LegendeBatOut = 0;
	 			}
	 			else {
	 				drawSquareRGB(682,443,50,20, 0, 172, 0);
	 				LegendeBatOut = 1;
	 			}
	 			put_string(683,449,"BatOut", WHITE);
				if(Additional == 1 ){
					readData("LegendeADD", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
		 			if (strcmp ("false",Value) == 0){
		 				drawSquare(735,443,50,20, LTGREY);
		 				LegendeADD = 0;
		 			}
		 			else {
		 				drawSquareRGB(735,443,50,20, 172, 0, 172);
		 				LegendeADD = 1;
		 			}
		 			put_string(737,449," ADD", WHITE);
				}

	 			if(counter == 0){
	 				drawSquare(2,2,800,480,LTGREY);
	 				drawCorner(2, 2, 800, 480, BLACK);
	 				drawSquare(12,12,778,458,WHITE);
	 				drawCorner(12, 12, 778, 458, LTGREY);
	 				DrawImage("/home/pi/E3dcGui/Image/EinstImage.ppm", 180, 12);
	 				if(E3DC_S10 ==1){
	 					DrawImage("/home/pi/E3dcGui/Image/AktuellImage.ppm", 270, 12);
	 					DrawImage("/home/pi/E3dcGui/Image/LangzeitImage.ppm", 360, 12);
	 				}
	 				DrawImage("/home/pi/E3dcGui/Image/MonitorImage.ppm", 450, 12);
	 				if(Homematic_GUI ==1){
	 					DrawImage("/home/pi/E3dcGui/Image/HMImage.ppm", 540, 12);
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
	 				if(LegendeSOC == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/SOC900.txt");
	 					drawSOC(Path, 64, 134, 64);
	 				}
	 				if(LegendeHome == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/Home900.txt");
	 					drawLine(Path, 225, 30, 30, PowerMax);
	 				}
	 				if(LegendeADD == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/Add900.txt");
	 					drawLine(Path, 225, 30, 30, PowerMax);
	 				}
	 				if(LegendeNetIn == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/NetIn900.txt");
	 					drawLine(Path, 0, 172, 255, PowerMax);
	 				}
	 				if(LegendeNetOut == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/NetOut900.txt");
	 					drawLine(Path, 0, 0, 172, PowerMax);
	 				}
	 				if(LegendeBatIn == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/BatIn900.txt");
	 					drawLine(Path, 0, 225, 64, PowerMax);
	 				}
	 				if(LegendeBatOut == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/BatOut900.txt");
	 					drawLine(Path, 0, 172, 0, PowerMax);
	 				}
	 				if(LegendeSolar == 1){
	 					snprintf (Path, (size_t)100, "/home/pi/E3dcGui/Data/Solar900.txt");
	 					drawLine(Path, 225, 122, 34, PowerMax);
	 				}
	 				drawSquareRGB(783-x24h, 430-330, 1, 330, 255, 172, 64);								//Ziechnen der 0:00 Uhr Linie in den Langzeitwerten
	 				writeData("/mnt/RAMDisk/ScreenCounter.txt", "60");
				}
				break;
			}
//####################################################
			//Monitor Grafik erstellen
			case ScreenMonitor:{
				GuiTime = PiTime;
				char PiTemp[20], PiUptime[20], PiCPU[20];
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

				if(counter == 0){
					drawSquare(2,2,800,480,LTGREY);
					drawCorner(2, 2, 800, 480, BLACK);
					drawSquare(12,12,778,458,WHITE);
					drawCorner(12, 12, 778, 458, LTGREY);
					DrawImage("/home/pi/E3dcGui/Image/EinstImage.ppm", 180, 12);
					if(E3DC_S10 ==1){
						DrawImage("/home/pi/E3dcGui/Image/AktuellImage.ppm", 270, 12);
						DrawImage("/home/pi/E3dcGui/Image/LangzeitImage.ppm", 360, 12);
					}
					DrawImage("/home/pi/E3dcGui/Image/MonitorImage.ppm", 450, 12);
					if(Homematic_GUI ==1){
						DrawImage("/home/pi/E3dcGui/Image/HMImage.ppm", 540, 12);
					}
					writeData("/mnt/RAMDisk/ScreenCounter.txt", "60");
					// Grafik für Uptime
					drawSquare(S2,R1-20,320,60,GREY);
					drawCorner(S2,R1-20,320,60,WHITE);
					drawSquare(S2+60,R1-17,257,54,WHITE);
					drawCorner(S2+60,R1-17,257,54,GREY);
					put_string(S2+6, R1+4, "Uptime", WHITE);
					// Grafik für Temp
					drawSquare(S2,R2-20,320,60,GREY);
					drawCorner(S2,R2-20,320,60,WHITE);
					drawSquare(S2+60,R2-17,257,54,WHITE);
					drawCorner(S2+60,R2-17,257,54,GREY);
					put_string(S2+6, R2+4, "Temp", WHITE);
					// Grafik für CPU
					drawSquare(S2,R3-20,320,60,GREY);
					drawCorner(S2,R3-20,320,60,WHITE);
					drawSquare(S2+60,R3-17,257,54,WHITE);
					drawCorner(S2+60,R3-17,257,54,GREY);
					put_string(S2+6, R3+4, "CPU", WHITE);
				}
				// Uptime
				drawSquare(S4-30, R1+4, 140, 12, WHITE);
				put_string(S4-30, R1+4, PiUptime, GREY);
				// Temp
				if (T > 20){
					drawSquare(S4, R2, Fw, 21, LIGHT_GREEN);
					createData(S4, R2, PiTemp);
				}
				else if (T > 40){
					drawSquare(S4, R2, Fw, 21, LIGHT_RED);
					createData(S4, R2, PiTemp);
				}
				else if (T > 60){
					drawSquare(S4, R2, Fw, 21, RED);
					createData(S4, R2, PiTemp);
				}
				else{
					drawSquare(S4, R2, Fw, 21, GREEN);
					createData(S4, R2, PiTemp);
				}
				// CPU
				if (PiCPUint > 5){
					drawSquare(S4, R3, Fw, 21, LIGHT_GREEN);
					createData(S4, R3, PiCPU);
				}
				else if (PiCPUint > 10){
					drawSquare(S4, R3, Fw, 21, LIGHT_RED);
					createData(S4, R3, PiCPU);
				}
				else if (PiCPUint > 20){
					drawSquare(S4, R3, Fw, 21, RED);
					createData(S4, R3, PiCPU);
				}
				else{
					drawSquare(S4, R3, Fw, 21, GREEN);
					createData(S4, R3, PiCPU);
				}
				break;
			}
//####################################################
			//Homematic Grafik erstellen
			case ScreenHM:{
				if(Homematic_GUI == 1){
					char VerschlussEG[20], VerschlussOG[20], VerschlussDG[20], VerschlussGa[20], VerschlussKel[20];
					char DGSFli[20], DGSFre[20], DGTF[20], DGSTemp[20];
					char OGKFli[20], OGKFre[20], OGWSt[20], OGWBt[20], OGBF[20], OGKTemp[20], OGWTemp[20], OGBTemp[20], OGKSpul[20], OGParty[20];
					char OGKJalli[20], OGKJalre[20], OGWJalSt[20], OGWJalBt[20], OGBJal[20], OGEJal[20];
					char EGKFli[20], EGKFre[20], EGWFli[20], EGWFre[20], EGSSt[20], EGWCF[20], EGWTemp[20];
					char Markise[20], EGKJalli[20], EGKJalre[20], EGWJal[20], EGSJal[20], EGWCJal[20];
					char Kellertuer[20], Heizung[20], pumpensumpf[20], HeizungState[20];
					char Garagentor[20], Garagentuerh[20], Garagentuerv[20], GarageTemp[20];
					char AussenTemp[20], AussenHum[20], AussenRegen[20], Brunnen[20],MacSchrankValue[20];
					char Party[20], Sumpf[20], Heiz[20], Spuel[20], HMCPU[20], HMupTage[20];
					char Sonnenaufgang[20], Sonnenuntergang[20], Vollmond[20];
					int HMcounter = HMcounter +1;
					GuiTime = HomematicTime;
					//Daten von der HM lesen
					if(counter == 0 || ((strcmp ("screenOn",screenState) == 0) && HMcounter == HM_Intervall)){
						//Read Date Beginn
						drawSquare(760,440,20,20,LIGHT_RED);
						drawCorner(760,440,20,20,WHITE);
						//WatchdogHM
						read_HM(ISE_UnixTime, 10, Value);																		//Auslesen der Werte von Geräten oder Variablen aus der Homematic mit Funtion aus funktion.h
						UnixTime = atoi(Value);
						read_HM(ISE_TimestampHM, 16,TimestampHM);
						snprintf (writeTxt, (size_t)100, "%s \n%i", TimestampHM, UnixTime);
						writeData("/mnt/RAMDisk/UnixtimeHM.txt", writeTxt);									//Schreiben einer Datei mit Funtion aus funktion.h
						//Verschluss
						read_HM(ISE_VerschlussDG, 4, VerschlussDG);
						read_HM(ISE_VerschlussOG, 4, VerschlussOG);
						read_HM(ISE_VerschlussEG, 4, VerschlussEG);
						read_HM(ISE_VerschlussKel, 4, VerschlussKel);
						read_HM(ISE_VerschlussGa, 4, VerschlussGa);
						//DG
						read_HM(ISE_DGSFli, 4, DGSFli);
						read_HM(ISE_DGSFre, 4, DGSFre);
						read_HM(ISE_DGSTemp, 4, DGSTemp);
						read_HM(ISE_DGTF, 4, DGTF);
						//OG
						read_HM(ISE_OGWSt, 4, OGWSt);
						readJalou_HM(ISE_OGWJalSt, OGWJalSt);																//Jalousie Wert aus der Homematic Abfrage
						read_HM(ISE_OGWBt, 4, OGWBt);
						readJalou_HM(ISE_OGWJalBt, OGWJalBt);
						//OGWJalBt = atoi(Value);
						read_HM(ISE_OGWTemp, 4, OGWTemp);
						read_HM(ISE_OGKFli, 4, OGKFli);
						readJalou_HM(ISE_OGKJalli, OGKJalli);
						read_HM(ISE_OGKFre, 4, OGKFre);
						readJalou_HM(ISE_OGKJalre, OGKJalre);
						read_HM(ISE_OGKTemp, 4, OGKTemp);
						read_HM(ISE_OGBF, 4, OGBF);
						readJalou_HM(ISE_OGBJal, OGBJal);
						read_HM(ISE_OGBTemp, 4, OGBTemp);
						readJalou_HM(ISE_OGEJal, OGEJal);
						read_HM(ISE_OGKSpul, 4, OGKSpul);
						read_HM(ISE_OGParty, 4, OGParty);
						read_HM(ISE_MacSchrankValue, 4, MacSchrankValue);
						//EG
						read_HM(ISE_EGWFli, 4, EGWFli);
						read_HM(ISE_EGWFre, 4, EGWFre);
						readJalou_HM(ISE_EGWJal, EGWJal);
						read_HM(ISE_EGWTemp, 4, EGWTemp);
						read_HM(ISE_EGKFli, 4, EGKFli);
						read_HM(ISE_EGKFre, 4, EGKFre);
						readJalou_HM(ISE_EGKJalli, EGKJalli);
						readJalou_HM(ISE_EGKJalre, EGKJalre);
						read_HM(ISE_EGSSt, 4, EGSSt);
						readJalou_HM(ISE_EGSJal, EGSJal);
						read_HM(ISE_EGWCF, 4, EGWCF);
						readJalou_HM(ISE_EGWCJal, EGWCJal);
						//Keller
						read_HM(ISE_Kellertuer, 4, Kellertuer);
						read_HM(ISE_Heizung, 4, Heizung);
						read_HM(ISE_HeizungState, 4, HeizungState);
						read_HM(ISE_pumpensumpf, 4, pumpensumpf);
						//Garage
						read_HM(ISE_GarageTemp, 4, GarageTemp);
						read_HM(ISE_Garagentor, 4, Garagentor);
						read_HM(ISE_Garagentuerv, 4, Garagentuerv);
						read_HM(ISE_Garagentuerh, 4, Garagentuerh);
						//Aussen
						read_HM(ISE_AussenTemp, 4, AussenTemp);
						readJalou_HM(ISE_Markise, Markise);
						read_HM(ISE_AussenHum, 2, AussenHum);
						read_HM(ISE_AussenRegen, 1, AussenRegen);
						read_HM(ISE_Brunnen, 4, Brunnen);
						//Homematic
						read_HM(ISE_HMCPU, 4, HMCPU);
						read_HM(ISE_HMupTage, 8, HMupTage);
						//Read Date Ende
						drawSquare(760,440,20,20,GREEN);
						drawCorner(760,440,20,20,WHITE);
						HMcounter = 0;
						readOK = 1;
					}
					//Hintergrund-Grafik HM erstellen
					if(counter == 0){
						drawSquare(2,2,800,480,LTGREY);
						drawCorner(2, 2, 800, 480, BLACK);
						drawSquare(12,12,778,458,WHITE);
						drawCorner(12, 12, 778, 458, LTGREY);
						DrawImage("/home/pi/E3dcGui/Image/EinstImage.ppm", 180, 12);
						if(E3DC_S10 ==1){
							DrawImage("/home/pi/E3dcGui/Image/AktuellImage.ppm", 270, 12);
							DrawImage("/home/pi/E3dcGui/Image/LangzeitImage.ppm", 360, 12);
						}
						DrawImage("/home/pi/E3dcGui/Image/MonitorImage.ppm", 450, 12);
						DrawImage("/home/pi/E3dcGui/Image/HMImage.ppm", 540, 12);
						// Grafik für Dachgeschoss
						drawSquare(S0,R1-20,400,60,GREY);
						drawCorner(S0,R1-20,400,60,WHITE);
						if (strcmp ("true",VerschlussDG) == 0){
							drawSquare(S0+60,R1-17,337,54,LIGHT_RED);
							drawSquare(S0+63,R1-14,331,48,WHITE);
							drawCorner(S0+63,R1-14,331,48,LIGHT_RED);
				    }
				    else if (strcmp ("fals",VerschlussDG) == 0){
							drawSquare(S0+60,R1-17,337,54,GREEN);
							drawSquare(S0+63,R1-14,331,48,WHITE);
							drawCorner(S0+63,R1-14,331,48,GREEN);
				    }
						drawCorner(S0+60,R1-17,337,54,GREY);
						put_string(S0+6, R1+4, "DG", WHITE);
						// Grafik für Obergeschoss
						drawSquare(S0,R2-20,730,60,GREY);
						drawCorner(S0,R2-20,730,60,WHITE);
						if (strcmp ("true",VerschlussOG) == 0){
							drawSquare(S0+60,R2-17,667,54,LIGHT_RED);
							drawSquare(S0+63,R2-14,661,48,WHITE);
							drawCorner(S0+63,R2-14,661,48,LIGHT_RED);
				    }
				    else if (strcmp ("fals",VerschlussOG) == 0){
							drawSquare(S0+60,R2-17,667,54,GREEN);
							drawSquare(S0+63,R2-14,661,48,WHITE);
							drawCorner(S0+63,R2-14,661,48,GREEN);
				    }
						drawCorner(S0+60,R2-17,667,54,GREY);
						put_string(S0+6, R2+4, "OG", WHITE);
						// Grafik für Erdgeschoss
						drawSquare(S0,R3-20,400,60,GREY);
						drawCorner(S0,R3-20,400,60,WHITE);
						if (strcmp ("true",VerschlussEG) == 0){
							drawSquare(S0+60,R3-17,337,54,LIGHT_RED);
							drawSquare(S0+63,R3-14,331,48,WHITE);
							drawCorner(S0+63,R3-14,331,48,LIGHT_RED);
				    }
				    else if (strcmp ("fals",VerschlussEG) == 0){
							drawSquare(S0+60,R3-17,337,54,GREEN);
							drawSquare(S0+63,R3-14,331,48,WHITE);
							drawCorner(S0+63,R3-14,331,48,GREEN);
				    }
						drawCorner(S0+60,R3-17,337,54,GREY);
						put_string(S0+6, R3+4, "EG", WHITE);
						// Grafik für Keller
						drawSquare(S0,R4-20,400,60,GREY);
						drawCorner(S0,R4-20,400,60,WHITE);
						if (strcmp ("true",VerschlussKel) == 0){
							drawSquare(S0+60,R4-17,337,54,LIGHT_RED);
							drawSquare(S0+63,R4-14,331,48,WHITE);
							drawCorner(S0+63,R4-14,331,48,LIGHT_RED);
				    }

				    else if (strcmp ("fals",VerschlussKel) == 0){
							drawSquare(S0+60,R4-17,337,54,GREEN);
							drawSquare(S0+63,R4-14,331,48,WHITE);
							drawCorner(S0+63,R4-14,331,48,GREEN);
				    }
						drawCorner(S0+60,R4-17,337,54,GREY);
						put_string(S0+6, R4+4, "Keller", WHITE);
						// Grafik für Aussen
						drawSquare(S5,R1-20,320,60,GREY);
						drawCorner(S5,R1-20,320,60,WHITE);
						drawSquare(S5+60,R1-17,257,54,WHITE);
						drawCorner(S5+60,R1-17,257,54,GREY);
						put_string(S5+6, R1+4, "HM", WHITE);
						// Grafik für Garage
						drawSquare(S5,R3-20,320,60,GREY);
						drawCorner(S5,R3-20,320,60,WHITE);
						if (strcmp ("true",VerschlussGa) == 0){
							drawSquare(S5+60,R3-17,257,54,LIGHT_RED);
							drawSquare(S5+63,R3-14,251,48,WHITE);
							drawCorner(S5+63,R3-14,251,48,LIGHT_RED);
				    }
				    else if (strcmp ("fals",VerschlussGa) == 0){
							drawSquare(S5+60,R3-17,257,54,GREEN);
							drawSquare(S5+63,R3-14,251,48,WHITE);
							drawCorner(S5+63,R3-14,251,48,GREEN);
				    }
						drawCorner(S5+60,R3-17,257,54,GREY);
						put_string(S5+6, R3+4, "Garage", WHITE);
						// Grafik für Aussen
						drawSquare(S5,R4-20,320,60,GREY);
						drawCorner(S5,R4-20,320,60,WHITE);
						drawSquare(S5+60,R4-17,257,54,WHITE);
						drawCorner(S5+60,R4-17,257,54,GREY);
						put_string(S5+6, R4+4, "Aussen", WHITE);
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "60");
					}
					//Data_Grafik erstellen
					if(readOK == 1){
						readOK = 0;
						//Read Date neu schreiben
						drawSquare(760,440,20,20,GREEN);
						drawCorner(760,440,20,20,WHITE);
						//DG
						createWindow(S1, R1, Fhw, "Schlaf", DGSFli);												//Grafik für Fenster erstellen offen=rot, geschlossen=grün, nicht vorhanden=grau
						createWindow(S1+Fhw, R1, Fhw, "", DGSFre);
						createData(S1, R1, DGSTemp);																				//Daten unter die Fenstergrafik für z.B. Temperatur
						createWindow(S2, R1, Fw, "Treppe", DGTF);
						//OG
						createWindow(S1, R2, Fhw, "Wohnz.", OGWSt);
						createJalousie(S1, R2, Fhw, OGWJalSt);															//Jalousie über die Fenstergrafik legen
						createWindow((S1+Fhw), R2, Fhw, "", OGWBt);													//Teilweise mit Fhw nur die halbe Fensterbreite für zwei Fenster in einem Raum
						createJalousie((S1+Fhw), R2, Fhw, OGWJalBt);
						createData(S1, R2, OGWTemp);
						createWindow(S2, R2, Fhw, "Kueche", OGKFli);
						createJalousie(S2, R2, Fhw, OGKJalli);
						createWindow((S2+Fhw), R2, Fhw, "", OGKFre);
						createJalousie((S2+Fhw), R2, Fhw, OGKJalre);
						createData(S2, R2, OGKTemp);
						createWindow(S3, R2, Fw, " Bad", OGBF);
						createJalousie(S3, R2, Fw, OGBJal);
						createData(S3, R2, OGBTemp);
						createWindow(S4, R2, Fw, "Eisenb.", "");
						createJalousie(S4, R2, Fw, OGEJal);
				    if (strcmp ("fals",OGKSpul) == 0){
				      snprintf (Spuel, (size_t)5, "-");
				    }
				    else if (strcmp ("true",OGKSpul) == 0){
				      snprintf (Spuel, (size_t)5, "fals");
				    }
				    createWindow(S6, R2, Fw, "OG-Spuel", Spuel);
				    if (strcmp ("fals",OGParty) == 0){
				      snprintf (Party, (size_t)5, "-");
				    }
				    else if (strcmp ("true",OGParty) == 0){
				      snprintf (Party, (size_t)5, "fals");
				    }
				    createWindow(S7, R2, Fw, "OG-Party", Party);
						createWindow(S8, R2, Fw, "OG-Mac", MacSchrankValue);
						//EG
						createWindow(S1, R3, Fhw, "Wohnz.", EGWFli);
						createWindow((S1+Fhw), R3, Fhw, "", EGWFre);
						createJalousie(S1, R3, Fw, EGWJal);
						createData(S1, R3, EGWTemp);
						createWindow(S2, R3, Fhw, "Kueche", EGKFli);
						createJalousie(S2, R3, Fhw, EGKJalli);
						createWindow((S2+Fhw), R3, Fhw, "", EGKFre);
						createJalousie((S2+Fhw), R3, Fhw, EGKJalre);
						createWindow(S3, R3, Fw, "Schlaf", EGSSt);
						createJalousie(S3, R3, Fw, EGSJal);
						createWindow(S4, R3, Fw, "  WC", EGWCF);
						createJalousie(S4, R3, Fw, EGWCJal);
						//Keller
				    createWindow(S1, R4, Fw, " Tuer", Kellertuer);
				    if (strcmp ("fals",Heizung) == 0){																	//mit der IF Funktin wird ggf. die Ausgabe geändert fals
				      snprintf (Heiz, (size_t)5, "true");																//ein Gerät nicht entsprechend für rot und grün "true" oder "false" ausgibt
				    }
				    else if (strcmp ("true",Heizung) == 0){															//Achtung da ich immer nur vier Zeichen einlese und auswerte muss der
				      snprintf (Heiz, (size_t)5, "fals");																//neue Wert nicht auf "false" sondern nur auf "fals" gesetzt werden
				    }
						createWindow(S2, R4, Fw, "Heizung", Heiz);
				    if (strcmp ("fals",HeizungState) == 0){
				      createData(S2, R4, " ON");
				    }
				    else if (strcmp ("true",HeizungState) == 0){
				      createData(S2, R4, " OFF");
				    }
				    if (strcmp ("fals",pumpensumpf) == 0){
				      snprintf (Sumpf, (size_t)5, "true");
				    }
				    else if (strcmp ("true",pumpensumpf) == 0){
				      snprintf (Sumpf, (size_t)5, "fals");
				    }
						createWindow(S3, R4, Fw, " Sumpf", Sumpf);
						//Garage
						drawSquare(S5+6,R3+20,40,12,GREY);
						put_string(S5+6, (R3+20), GarageTemp,WHITE);
						createWindow(S6, R3, Fw, " Tor", Garagentor);
						createWindow(S7, R3, Fw, "Vorne", Garagentuerv);
						createWindow(S8, R3, Fw, "Hinten", Garagentuerh);
						//Aussen
						createWindow(S6, R4, Fw, "Aussen", "");
						createData(S6, R4, AussenTemp);
						createJalousie(S6, R4, Fw, Markise);
						createWindow(S7, R4, Fw, "Regen", "no");
						createData(S7, R4, AussenHum);
						if (strcmp ("0",AussenRegen) == 0){
							drawSquare(S7, R4, Fw, 21, ORANGE);
						}
						else if (strcmp ("1",AussenRegen) == 0){
							drawSquare(S7, R4, Fw, 21, CYAN);
						}
						createWindow(S8, R4, Fw, "Brunnen", "no");
						if (strcmp ("true",Brunnen) == 0){
							drawSquare(S8, R4, Fw, 21, BLUE);
						}
						else if (strcmp ("fals",Brunnen) == 0){
							drawSquare(S8, R4, Fw, 21, LTGREY);
						}
						//Homematic
						createWindow(S6, R1, Fw, "HM-CPU", "no");
						createData(S6, R1-15, HMCPU);
						createData(S6-5, R1, HMupTage);
						//Read Date Ende
						drawSquare(760,440,20,20,GREEN);
						drawCorner(760,440,20,20,WHITE);
						if(counter == 0){
							read_HM(ISE_Sonnenaufgang, 5, Sonnenaufgang);
							read_HM(ISE_Sonnenuntergang, 5, Sonnenuntergang);
							read_HM(ISE_Vollmond, 10, Vollmond);
							snprintf (OUT, (size_t)100, "Sonnenaufgang:        %s", Sonnenaufgang);
							put_string(40, 408, OUT, GREY);
							snprintf (OUT, (size_t)100, "Sonnenuntergang:      %s", Sonnenuntergang);
							put_string(40, 420, OUT, GREY);
							snprintf (OUT, (size_t)100, "Vollmond:        %s", Vollmond);
							put_string(40, 432, OUT, GREY);
						}
						HMcounter = 0;
					}
				}
				break;
			}
//####################################################
			//Setup Grafik erstellen
			case ScreenSetup:{																												//Im ScreenSetup werden nur die Rückgaben per Datei aus dem "screenSave" Programm dargestellt
				GuiTime = PiTime;
				readData("ScreenShutdown", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);
				screenShutdown = atoi(Value);
				if(counter == 0){
					drawSquare(2,2,800,480,LTGREY);
					drawCorner(2, 2, 800, 480, BLACK);
					drawSquare(12,12,778,458,WHITE);
					drawCorner(12, 12, 778, 458, LTGREY);
					switch(screenShutdown){
						case ShutdownSD:{
							drawSquare(100,200,180,30,GREY);
							drawCorner(100,200,180,30, WHITE);
							put_string(120,208, "3 Sekunden", LIGHT_RED);
							break;
						}
						case ShutdownSRS:{
							drawSquare(100,250,180,30,GREY);
							drawCorner(100,250,180,30, WHITE);
							put_string(120,258, "Software Neustart", GREEN);
							break;
						}
						case ShutdownHRS:{
							drawSquare(100,300,180,30,RED);
							drawCorner(100,300,180,30, WHITE);
							put_string(120,308, "Hardware Neustart", WHITE);
							break;
						}
						case ShutdownSDN:{
							drawSquare(100,200,180,30,RED);
							drawCorner(100,200,180,30, WHITE);
							put_string(120,308, "SHUTDOWN", WHITE);
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
							drawSquare(100,200,180,30,GREY);
							drawCorner(100,200,180,30, WHITE);
							put_string(120,208, "Ausschalten", WHITE);
							drawSquare(100,250,180,30,GREY);
							drawCorner(100,250,180,30, WHITE);
							put_string(120,258, "Software Neustart", WHITE);
							drawSquare(100,300,180,30,GREY);
							drawCorner(100,300,180,30, WHITE);
							put_string(120,308, "Hardware Neustart", WHITE);
							break;
						}
						default:{
							screenShutdown = ShutdownRun;
						}
					}
					DrawImage("/home/pi/E3dcGui/Image/EinstImage.ppm", 180, 12);
					if(E3DC_S10 ==1){
						DrawImage("/home/pi/E3dcGui/Image/AktuellImage.ppm", 270, 12);
						DrawImage("/home/pi/E3dcGui/Image/LangzeitImage.ppm", 360, 12);
					}
					DrawImage("/home/pi/E3dcGui/Image/MonitorImage.ppm", 450, 12);
					if(Homematic_GUI ==1){
						DrawImage("/home/pi/E3dcGui/Image/HMImage.ppm", 540, 12);
					}
					writeData("/mnt/RAMDisk/ScreenCounter.txt", "120");
				}
				break;
			}
			default:{
				if(E3DC_S10 ==0){
					screenChange = ScreenHM;
				}
				else{
					screenChange = ScreenAktuell;
				}
			}
		}
//####################################################
	//WatchdogHM Daten für WD schreiben
		if(counter == 0){
			char Value[20];
			read_HM(ISE_UnixTime, 10, Value);
			UnixTime = atoi(Value);
			read_HM(ISE_TimestampHM, 16,TimestampHM);
			snprintf (writeTxt, (size_t)100, "%s \n%i", TimestampHM, UnixTime);
			writeData("/mnt/RAMDisk/UnixtimeHM.txt", writeTxt);
		}
	//Time
		if(GuiTime == RscpTime && E3DC_S10 == 1){
			put_string(20, 458, "Letzter Zeitstempel: ", GREY);
			int AktuallTime = time(NULL);
			int TAG_UnixTimeint = atoi(TAG_UnixTime);
			int DiffTime = AktuallTime - TAG_UnixTimeint;
			snprintf (OUT, (size_t)100, "%s %s", TAG_Date, TAG_Time);
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
		else{
			put_string(20, 458, "Aktuelle Zeit: ", GREY);
			time_t timeStamp;
			struct tm *now;
			time( &timeStamp );
			now = localtime( &timeStamp );
			strftime (OUT,100,"%d.%m.%Y %H:%M:%S",now);
			drawSquare(150,458,170,12,WHITE);
			put_string(150,458, OUT, GREEN);
		}
		//Bildschirmschoner
		readData("ScreenSaver", 1, Value,delValue,delValue,delValue,delValue,delValue,delValue,delValue);  //Zählerdatei für den Bildschirmschoner auslesen
		ScreenSaverCounter = atoi(Value);
		ScreenSaverCounter = ScreenSaverCounter +1;
		if(ScreenSaverCounter == ScreenSaverTime){
			screenOff();
			ScreenSaverCounter = 0;
			if(E3DC_S10 ==0){
				writeData("/mnt/RAMDisk/ScreenChange.txt", "12\n");
			}
			else{
				writeData("/mnt/RAMDisk/ScreenChange.txt", "1\n");
			}
		}
		snprintf (OUT, (size_t)100, "%i", ScreenSaverCounter);
		writeData("/mnt/RAMDisk/ScreenSaver.txt", OUT);
		//Abfrageintervall
		sleep(1);
	}
}

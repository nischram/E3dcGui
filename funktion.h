#ifndef __FUNKTION_H_
#define __FUNKTION_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/ioctl.h>
#include <signal.h>
#include "parameter.h"
#include "parameterHM.h"

// Zusatzfunktion für die Jalousie-Funktin "readJalou_HM"
unsigned replace_character(char* string, char from, char to){
   unsigned result = 0;
   if (!string) return 0;
   while (*string != '\0')
   {
     if (*string == from){
       *string = to;
       result++;
     }
     string++;
   }
   return result;
}
//Homematic Abfrage von Geräten und Variablen
int read_HM(int id, int cutnumber, char* value){
  if (id == 0){
    snprintf(value, (size_t)256, "NoISE");
    return -1;
  }
  int cutlast;
  cutlast = 8 + cutnumber - 1;
  char batch[256];
  memset(&batch, 0, sizeof(batch));
  snprintf(batch, (size_t)256, "curl http://%s/config/xmlapi/state.cgi?datapoint_id=%i |cut -d \" \" -f 6 | cut -b 8-%i", HM_IP, id, cutlast);
  FILE *out = NULL;
  out = popen( batch, "r" );
  if( out != NULL ){
    fscanf(out, "%s", value);
    pclose(out);
    return 1;
  }
  pclose(out);
  snprintf(value, (size_t)256, "no");
  return 0;
}
//Homematic Abfrage von Jalousien
int readJalou_HM(int id, char* value){  //id= ISE_ID, Hm_IP= IP der Homematic
  if (id == 0){
    snprintf(value, (size_t)256, "NoISE");
    return 0;
  }
  char batch[256], res_Jal[20];
  memset(&batch, 0, sizeof(batch));
  memset(&res_Jal, 0, sizeof(res_Jal));
  snprintf(batch, (size_t)256, "curl http://%s/config/xmlapi/state.cgi?datapoint_id=%i |cut -d \" \" -f 6 | cut -b 8-11", HM_IP, id);
  FILE *out = NULL;
  out = popen( batch, "r" );
  if( out != NULL ){
    fscanf(out, "%s", res_Jal);
    pclose(out);
    replace_character(res_Jal, '.', '0');    //if(Jalousie == 0){
    int Jalousie = atoi(res_Jal);
    if(Jalousie==1000){
      Jalousie= Jalousie/10;
    }
    snprintf(value, (size_t)20, "%i", Jalousie);
    return 0;
  }
  else{
    snprintf(res_Jal, (size_t)20, "100");
    snprintf(value, (size_t)20, "%s", res_Jal);
    return 1;
  }
}
//Homematic senden von Daten zur HM
void printsendHM(int id, char value[20]){
    char batch[128];
    memset(batch, 0x00, sizeof(batch));
    snprintf(batch, sizeof(batch), "curl \"http://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=%s\" &",HM_IP , id, value);
    printf("send :[%s]\n",batch);
    system(batch);
}
//Dateien schreiben mit übergebenem Pfad (z.B. Sommer/Winterzeit)
void writeData(char filePath[100], char write[100]){
	FILE *fp;
	fp = fopen(filePath, "w");
	if(fp == NULL) {
		printf("%s konnte NICHT geoeffnet werden. (writeData)\n", filePath);
	}
	else {
		fprintf(fp, "%s", write);
		fclose(fp);
	}
}
//Homematic Fenster ziechnen mit Name
void createWindow(int Wx, int Wy, int Ww, char room[20], char var[20]){
	put_string((Wx-3),(Wy-11),room,GREY);
	if (strcmp ("fals",var) == 0){
		drawSquare(Wx,Wy,Ww,21,GREEN);
	}
	else if (strcmp ("true",var) == 0){
		drawSquare(Wx,Wy,Ww,21,LIGHT_RED);
	}
  else if (strcmp ("no",var) == 0){
    drawSquare(Wx,Wy,Ww,21,WHITE);
	}
  else if (strcmp ("NoISE",var) == 0){
    drawSquare(Wx,Wy,Ww,21,LIGHT_BLUE);
	}
	else{
		drawSquare(Wx,Wy,Ww,21,LTGREY);
	}
}
//Homematic Jalousie ziechnen
void createJalousie(int Jx, int Jy, int Jw, char Value[20])
{
  int percent = atoi(Value);
	if(percent < 95){
		drawSquare(Jx,Jy,Jw,1,WHITE);
	}
	if(percent < 80){
		drawSquare(Jx,(Jy+4),Jw,1,WHITE);
	}
	if(percent < 65){
		drawSquare(Jx,(Jy+8),Jw,1,WHITE);
	}
	if(percent < 50){
		drawSquare(Jx,(Jy+12),Jw,1,WHITE);
	}
	if(percent < 35){
		drawSquare(Jx,(Jy+16),Jw,1,WHITE);
	}
	if(percent < 20){
		drawSquare(Jx,(Jy+20),Jw,1,WHITE);
	}
}
// Homematic Daten einfügen z.B. Temperatur
void createData(int x, int y, char *c){
  drawSquare(x-5,y+21,60,12,WHITE);
	put_string(x+5, (y+22), c,GREY);
}
// Hintergrundbild mit Bildern erzeugen
int drawMainScreen()
{
  drawSquare(2,2,800,480,LTGREY);
  drawCorner(2, 2, 800, 480, BLACK);
  drawSquare(12,12,778,458,WHITE);
  drawCorner(12, 12, 778, 458, LTGREY);
  DrawImage("EinstImage", Picture1, PictureLine1);
  DrawImage("WetterImage", Picture2, PictureLine1);
  if(Abfuhrkalender ==1)
    DrawImage("MuellImage", Picture7, PictureLine1);
  if(E3DC_S10 ==1){
    DrawImage("AktuellImage", Picture3, PictureLine1);
    DrawImage("LangzeitImage", Picture4, PictureLine1);
    DrawImage("MonitorImage", Picture5, PictureLine1);
  }
  if(Homematic_GUI ==1){
    DrawImage("HMImage", Picture6, PictureLine1);
  }
}
// Bit aus einer Datei lesen, ändern und schreiben
int BitChange(char filePath[128], int Position, int max)
{
  int c = max;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  FILE *fpr;
  fpr = fopen(filePath, "r");
  if(fpr == NULL) {
    printf("%s konnte NICHT geoeffnet werden. (BitChange read)\n", filePath);
    snprintf (read, (size_t)128, "-");
  }
  else {
    for( c = 0; c < max; ++c )
    {
      fgets(read,128,fpr);
      out[c] = atoi(read);
    }
    fclose(fpr);
  }

  //Ändere Bit an Position
  if (out[Position] == 1)
    out[Position] = 0;
  else
    out[Position] = 1;

  //Schreibe geändertes Byte in Datei
  FILE *fp;
  fp = fopen(filePath, "w");
  if(fp == NULL) {
    printf("%s konnte NICHT geoeffnet werden. (BitChange write)\n", filePath);
  }
  else {
    for( c = 0; c < max; ++c )
     fprintf(fp, "%d\n", out[c]);
    fclose(fp);
  }
 return 0;
}

// Bit in eine Datei schreiben
int BitWrite(char filePath[128], int Position, int NewValue, int max)
{
  int c = max;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  FILE *fpr;
  fpr = fopen(filePath, "r");
  if(fpr == NULL) {
    printf("%s konnte NICHT geoeffnet werden. (BitWrite)\n", filePath);
    snprintf (read, (size_t)128, "-");
  }
  else {
    for( c = 0; c < max; ++c )
    {
      fgets(read,128,fpr);
      out[c] = atoi(read);
    }
    fclose(fpr);
  }

  //Ändere Bit an Position
  out[Position] = NewValue;

  //Schreibe geändertes Byte in Datei
  FILE *fp;
  fp = fopen(filePath, "w");
  if(fp == NULL) {
    printf("%s konnte NICHT geoeffnet werden. (BitWrite)\n", filePath);
  }
  else {
    for( c = 0; c < max; ++c )
     fprintf(fp, "%d\n", out[c]);
    fclose(fp);
  }
 return 0;
}

// Bit aus einer Datei lesen
int BitRead(char filePath[128], int Position, int max)
{
  int c = max;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  FILE *fp;
  fp = fopen(filePath, "r");
  if(fp == NULL) {
    printf("%s konnte NICHT geoeffnet werden. (BitRead)\n", filePath);
    snprintf (read, (size_t)128, "-");
  }
  else {
    for( c = 0; c < max; ++c )
    {
      fgets(read,128,fp);
      strtok(read, "\n");
      out[c] = atoi(read);
    }
    fclose(fp);
  }
  return out[Position];
}
int readScreen(int ScreenPosition)
{
  char PathScreen [128];
	snprintf (PathScreen, (size_t)128, "/mnt/RAMDisk/Screen.txt");
	int ret = BitRead(PathScreen, ScreenPosition, ScreenMAX);
  return ret;
}
int writeScreen(int ScreenPosition, int NewValue)
{
  char PathScreen [128];
	snprintf (PathScreen, (size_t)128, "/mnt/RAMDisk/Screen.txt");
  BitWrite(PathScreen, ScreenPosition, NewValue, ScreenMAX);
  return 1;
}
int readLegende(int LegendePosition)
{
  char PathLegende [128];
  snprintf (PathLegende, (size_t)128, "/mnt/RAMDisk/Legende.txt");
	int ret = BitRead(PathLegende, LegendePosition, LegendeMAX);
  return ret;
}
int writeLegende(int LegendePosition, int NewValue)
{
  char PathLegende [128];
  snprintf (PathLegende, (size_t)128, "/mnt/RAMDisk/Legende.txt");
  BitWrite(PathLegende, LegendePosition, NewValue, LegendeMAX);
  return 1;
}
int changeLegende(int LegendePosition)
{
  char PathLegende [128];
  snprintf (PathLegende, (size_t)128, "/mnt/RAMDisk/Legende.txt");
  BitChange(PathLegende, LegendePosition, LegendeMAX);
  return 1;
}
int writeUnixtime(int UnixtimePosition, int NewValue)
{
  char PathUnixtime [128];
  snprintf (PathUnixtime, (size_t)128, "/mnt/RAMDisk/Unixtime.txt");
  BitWrite(PathUnixtime, UnixtimePosition, NewValue, UnixtimeMAX);
  return 1;
}
int readUnixtime(int UnixtimePosition)
{
  char PathUnixtime [128];
  snprintf (PathUnixtime, (size_t)128, "/mnt/RAMDisk/Unixtime.txt");
	int ret = BitRead(PathUnixtime, UnixtimePosition, UnixtimeMAX);
  return ret;
}

//Daten aus Datei RscpGuiX.txt auslesen.
int readRscp(int RscpPosition)
{
  char PathRscp [128];
  snprintf (PathRscp, (size_t)128, "/mnt/RAMDisk/E3dcGuiData.txt");
  int ret = BitRead(PathRscp, RscpPosition,24);
  return ret;
}
//Lesen der RSCP Daten aus dem RAMDisk
void readRscpChar(char* TAG_Date, char* TAG_Time, char* TAG_SerialNr)
{
  FILE *fp;
  fp = fopen("/mnt/RAMDisk/E3dcGuiChar.txt", "r");
  if(fp == NULL) {
    printf("Datei konnte NICHT geoeffnet werden.\n");
  }
  else {
    fgets(TAG_Date,20,fp);
    strtok(TAG_Date, "\n");
    fgets(TAG_Time,20,fp);
    strtok(TAG_Time, "\n");
    fgets(TAG_SerialNr,20,fp);
    strtok(TAG_SerialNr, "\n");
    fclose(fp);
  }
}
// History in Datei schreiben
int writeHistory(int Position, int NewValue, int writedata, int max)
{
	if (writedata == today || writedata == yesterday){
		char filePath[128];
		if (writedata == today){
			snprintf (filePath, (size_t)128, "%s", today_path);
		}
		else if (writedata == yesterday){
			snprintf (filePath, (size_t)128, "%s", yesterday_path);
		}
    BitWrite(filePath, Position, NewValue, max);
    return 1;
  }
}
//HistoryValues aus Datei lesen.
int readHistory(int HistoryPosition, int writedata)
{
  char PathHistory [128];
  if (writedata == yesterday){
    snprintf (PathHistory, (size_t)128, "%s", yesterday_path);  // /mnt/RAMDisk/yesterday.txt
  }
  else {
    snprintf (PathHistory, (size_t)128, "%s", today_path); //  RAMDisk/today.txt
  }
  int ret = BitRead(PathHistory, HistoryPosition, dataMax);
  return ret;
}
int makeHistory()
{
  int data;
  int writedata;
  for( writedata = 1; writedata < 3; ++writedata ){
    for( data = 0; data < 7; ++data )
      writeHistory(data, 0, writedata, 7);
  }
  return 1;
}

// Debug Funktion zum Ausführen, im Code  -- DEBUG("Text"); --  einbauen. (-- nicht übernehmen!)
//Der Text wird in Datei DEBUG.txt gespeichert.
//Das letzte eingefügten von  -- DEBUG("Text\n"); --  muss ein " \n " enthalten sein.
int DEBUG(char write[128]){
	FILE *fp;
	fp = fopen("/mnt/RAMDisk/DEBUG.txt", "a");
	if(fp == NULL) {
		printf("Datei konnte NICHT geoeffnet werden.\n");
	}
	else {
		fprintf(fp, "%s", write);
		fclose(fp);
	}
	return 0;
}

// Touchfunktion
void  INThandler(int sig)
{

        signal(sig, SIG_IGN);
	closeFramebuffer();
        exit(0);
}
int mymillis()
{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

//Made By Nischram
int madeBy(char *OUT){
  snprintf (OUT, (size_t)100, "Made by Nischram  https://github.com/nischram/E3dcGui.git");
  return 1;
}

//Killen der Applikationen
int killall()
{
	system("killall -9 GuiMain RscpMain watchdog");
}

//Hintergrundbeleuhtung ausschalten
int screenOff()
{
  writeScreen(ScreenState, ScreenOff);
	system("sudo chmod 777 /sys/class/backlight/rpi_backlight/bl_power");
	system("echo 1 > /sys/class/backlight/rpi_backlight/bl_power");
  drawSquare(2,2,800,480,LTGREY);
  drawCorner(2, 2, 800, 480, BLACK);
  drawSquare(12,12,778,458,WHITE);
  drawCorner(12, 12, 778, 458, LTGREY);
}

//Hintergrundbeleuhtung einschalten
int screenOn()
{
  writeScreen(ScreenState, ScreenOn);
	system("sudo chmod 777 /sys/class/backlight/rpi_backlight/bl_power");
	system("echo 0 > /sys/class/backlight/rpi_backlight/bl_power");
}

//Bildschirmhelligkeit lesen
int readBrightness()
{
  char PathScreen [128];
	snprintf (PathScreen, (size_t)128, "/mnt/RAMDisk/Screen.txt");
	int ret = BitRead(PathScreen, ScreenBrightness, ScreenMAX);
  return ret;
}

//Bildschirmhelligkeit setzen und speichern
int setBrightness(int NewValue)
{
  char batch[128];
  memset(batch, 0x00, sizeof(batch));
  snprintf(batch, sizeof(batch), "sudo bash -c \"echo %i > /sys/class/backlight/rpi_backlight/brightness\" &", NewValue);
  system(batch);
  char PathScreen [128];
	snprintf (PathScreen, (size_t)128, "/mnt/RAMDisk/Screen.txt");
  BitWrite(PathScreen, ScreenBrightness, NewValue, ScreenMAX);
  int ret = readBrightness();
  return ret;
}

//Version lesen
int readVersion (char* version, char* datum)
{
  char file_Path [100], read[100], off[100];
  FILE *fp;
  snprintf (file_Path, (size_t)100, "/home/pi/E3dcGui/README.markdown");
  fp = fopen(file_Path, "r");
  if(fp == NULL) {
    printf("Datei konnte NICHT geoeffnet werden.\n");
    snprintf (version, (size_t)20, "V0.00");
    snprintf (datum, (size_t)20, "01.01.1970");
    fclose(fp);
    return 0;
  }
  else {
    fgets(off,100,fp);
    fgets(read,100,fp);
    sscanf(read, "[Stand: %s %s", version, datum);
    strtok(datum, "]");
    fclose(fp);
  }
  return 1;
}

//Pi Temperatur
int piTemp (char* PiTemp)
{
  FILE *temperatureFile;
  double T;
  temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
  fscanf (temperatureFile, "%lf", &T);
  fclose (temperatureFile);
  T /= 1000;
  snprintf (PiTemp, (size_t)20, "%3.1f 'C", T);
  return T;
}

//Pi Uptime
int piUptime (char* PiUptime)
{
  char Value[128];
  struct sysinfo info;
  sysinfo(&info);
  snprintf (Value, (size_t)20, "%ld\n", info.uptime);
  double PiUptimeint = atoi(Value);
  PiUptimeint = (double)PiUptimeint /60;
  int PiUpHour = PiUptimeint /60;
  int PiUpMin = PiUptimeint - (PiUpHour * 60);
  int PiUpDay = PiUpHour / 24;
  PiUpHour = PiUpHour - (PiUpDay *24);
  if (PiUpDay == 0)
    snprintf (PiUptime, (size_t)40, "       %i Std. %i Min.", PiUpHour, PiUpMin);
  else if (PiUpDay == 1)
    snprintf (PiUptime, (size_t)40, "%i Tag %i Std. %i Min.", PiUpDay, PiUpHour, PiUpMin);
  else
    snprintf (PiUptime, (size_t)40, "%i Tage %i Std. %i Min.", PiUpDay, PiUpHour, PiUpMin);
    return 1;
}

// Pi CPU
int piCPU (char* PiCPU)
{
  char OUT[128], batch[256];
  FILE *cpuFile;
  snprintf(batch, (size_t)256, "vmstat| head -3l | tail -1l  | cut -b 73-75");
  cpuFile = popen (batch, "r");
  fgets(OUT,sizeof(OUT),cpuFile);
  pclose (cpuFile);
  int PiCPUint = atoi(OUT);
  PiCPUint = 100 - PiCPUint;
  snprintf (PiCPU, (size_t)20, "%i %%", PiCPUint);
  return PiCPUint;
}

//IP
int piIP (char* PiIP1, char* PiIP2, char* PiIP3)
{
  char OUT[128], batch[256];
  FILE *ipFile;
  snprintf(batch, (size_t)256, "hostname --all-ip-addresses");
  ipFile = popen (batch, "r");
  fgets(PiIP1,(size_t)20,ipFile);
  fgets(PiIP2,(size_t)20,ipFile);
  fgets(PiIP3,(size_t)20,ipFile);
  pclose (ipFile);
  strtok(PiIP1, "\n");
  strtok(PiIP2, "\n");
  strtok(PiIP3, "\n");
  printf("\n### IP ###\n%s\n%s\n%s\n##########\n",PiIP1,PiIP2,PiIP3);
  snprintf (OUT, (size_t)4, "%s", PiIP2);
  int compare = atoi(OUT);
  if (compare > 0)
    printf("");
  else
    snprintf (PiIP2, (size_t)16, "%s", PiIP1);
  snprintf (OUT, (size_t)4, "%s", PiIP3);
  compare = atoi(OUT);
  if (compare > 0)
    printf("");
  else
    snprintf (PiIP3, (size_t)16, "%s", PiIP2);
    return 1;
}
//Autuelle Zeit
int putAktuell(int x, int y)
{
  char OUT[128];
  put_string(x, y, "Aktuelle Zeit: ", GREY);
  time_t timeStamp;
  struct tm *now;
  time( &timeStamp );
  now = localtime( &timeStamp );
  strftime (OUT,100,"%d.%m.%Y %H:%M:%S",now);
  drawOutput(x+120,y,170,12, OUT, GREEN);
  return 1;
}

#endif // __FUNKTION_H_

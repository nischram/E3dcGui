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
    return 0;
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
    return 0;
  }
  pclose(out);
  snprintf(value, (size_t)256, "no");
  return 1;
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
		printf("%s konnte NICHT geoeffnet werden.\n", filePath);
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
    printf("%s konnte NICHT geoeffnet werden.\n", filePath);
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
    printf("%s konnte NICHT geoeffnet werden.\n", filePath);
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
    printf("%s konnte NICHT geoeffnet werden.\n", filePath);
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
    printf("%s konnte NICHT geoeffnet werden.\n", filePath);
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
    printf("%s konnte NICHT geoeffnet werden.\n", filePath);
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
  return;
}

//Killen der Applikationen
int pkill()
{
	system("pkill GuiMain");
	system("pkill RscpMain");
	system("pkill watchdog");
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

#endif // __FUNKTION_H_

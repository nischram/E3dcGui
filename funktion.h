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
void printsend(int id, char value[20]){
    char batch[128];
    memset(batch, 0x00, sizeof(batch));
    snprintf(batch, sizeof(batch), "curl \"http://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=%s\" &",HM_IP , id, value);
    printf("send :[%s]\n",batch);
    system(batch);
}
//Dateien schreiben mit übergebenem Pfad
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
//Dateien lese Funktion für diverse Dateien aus dem RAMDisk
int readData(char fileName[100], int fileNr, char* read){
	char filePath [100];
	FILE *fp;
	snprintf (filePath, (size_t)100, "/mnt/RAMDisk/%s.txt", fileName);
	fp = fopen(filePath, "r");
	if(fp == NULL) {
		printf("Datei konnte NICHT geoeffnet werden.\n");
		snprintf (read, (size_t)20, "-");
	}
	else {
		fgets(read,20,fp);
		strtok(read, "\n");
		fclose(fp);
	}
  return fileNr;
}
//Datei lesen und Status von "true" auf "false" oder umgekehrt
int ChangeState(char fileName[100])
{
  char read [100];
  char filePath [100];
  snprintf (filePath, (size_t)100, "/mnt/RAMDisk/%s.txt", fileName);
  readData(fileName, 1, read);
	if (strcmp ("true",read) == 0){
		writeData(filePath, "false");
	}
	else {
		writeData(filePath, "true");
	}
}

//Lesen der RSCP Daten aus dem RAMDisk
void readRscpGui(char* TAG_Time,char* TAG_Date,char* TAG_PVI,char* TAG_Bat,char* TAG_Home,char* TAG_Grid,char* TAG_SOC,char* TAG_BatState,char* TAG_Autarky,char* TAG_SelfCon,char* TAG_SerialNr,char* TAG_UnixTime,char* TAG_ADD,char* TAG_WbAll,char* TAG_WbSolar,char* TAG_PVIState,char* TAG_PMState)
{
  FILE *fp;
  fp = fopen("/mnt/RAMDisk/E3dcGui.txt", "r");
  if(fp == NULL) {
    printf("Datei konnte NICHT geoeffnet werden.\n");
  }
  else {
    fgets(TAG_Date,20,fp);
    strtok(TAG_Date, "\n");
    fgets(TAG_Time,20,fp);
    strtok(TAG_Time, "\n");
    fgets(TAG_PVI,20,fp);
    strtok(TAG_PVI, "\n");
    fgets(TAG_Bat,20,fp);
    strtok(TAG_Bat, "\n");
    fgets(TAG_Home,20,fp);
    strtok(TAG_Home, "\n");
    fgets(TAG_Grid,20,fp);
    strtok(TAG_Grid, "\n");
    fgets(TAG_SOC,20,fp);
    strtok(TAG_SOC, "\n");
    fgets(TAG_BatState,20,fp);
    strtok(TAG_BatState, "\n");
    fgets(TAG_Autarky,20,fp);
    strtok(TAG_Autarky, "\n");
    fgets(TAG_SelfCon,20,fp);
    strtok(TAG_SelfCon, "\n");
    fgets(TAG_SerialNr,20,fp);
    strtok(TAG_SerialNr, "\n");
    fgets(TAG_UnixTime,20,fp);
    strtok(TAG_UnixTime, "\n");
    fgets(TAG_ADD,20,fp);
    strtok(TAG_ADD, "\n");
    fgets(TAG_WbAll,20,fp);
    strtok(TAG_WbAll, "\n");
    fgets(TAG_WbSolar,20,fp);
    strtok(TAG_WbSolar, "\n");
    fgets(TAG_PVIState,20,fp);
    strtok(TAG_PVIState, "\n");
    fgets(TAG_PMState,20,fp);
    strtok(TAG_PMState, "\n");
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
int pkill()
{
	system("pkill GuiMain");
	system("pkill RscpMain");
	system("pkill watchdog");
}
//Hintergrundbeleuhtung ausschalten
int screenOff()
{
	system("sudo chmod 777 /sys/class/backlight/rpi_backlight/bl_power");
	system("echo 1 > /sys/class/backlight/rpi_backlight/bl_power");
  writeData("/mnt/RAMDisk/ScreenState.txt", "screenOff");
}
//Hintergrundbeleuhtung einschalten
int screenOn()
{
	system("sudo chmod 777 /sys/class/backlight/rpi_backlight/bl_power");
	system("echo 0 > /sys/class/backlight/rpi_backlight/bl_power");
  writeData("/mnt/RAMDisk/ScreenState.txt", "screenOn");
}

#endif // __FUNKTION_H_

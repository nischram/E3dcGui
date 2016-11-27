#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <string>
#include <limits>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "../parameter.h"

using namespace std;

void printsendHM(int CounterHM, int id, int value){
  if(Homematic_E3DC == 1){
    if(CounterHM == HM_Intervall){
      char batch[128];
      memset(batch, 0x00, sizeof(batch));
      snprintf(batch, sizeof(batch), "curl \"http://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=%i\" > /dev/null 2>&1",HM_IP , id, value);
      printf("send to Homematic ISE_ID %i new Value = %i\n",id, value);
      system(batch);
    }
  }
}

void printsendCharHM(int CounterHM, int id, char value[32]){
  if(Homematic_E3DC == 1){
    if(CounterHM == HM_Intervall){
      char batch[128];
      memset(batch, 0x00, sizeof(batch));
      snprintf(batch, sizeof(batch), "curl \"http://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=%s\" > /dev/null 2>&1",HM_IP , id, value);
      printf("send to Homematic ISE_ID %i new Value = %i\n",id, value);
      system(batch);
    }
  }
}

int readTimeZone()
{
  int time_zone;
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
    time_zone = 3600;
  }
  else{
    time_zone = 7200;
  }
  printf("Time offset is %i second\n",time_zone);
  return time_zone;
}

void readWriteData(char *fileName, int NewValue){
  int c;
  int line[100];
  char read[20];
  char Path [128];
  snprintf (Path, (size_t)128, "/home/pi/E3dcGui/Data/%s.txt", fileName);
  fstream datei(Path);
  if (datei.is_open()) {
    for( c = 0; c < 97; ++c ){
      datei.getline(read	,20, '\n');
      line[c] = atoi(read);
    }
    datei.close();
  }
  ofstream fout(Path);
  if (fout.is_open()) {
    for( c = 1; c < 96; ++c ){
      fout << line[c] <<"\n";
    }
    fout << NewValue <<"\n";
    fout.close();
    }
  else cerr << "Konnte Datei nicht erstellen!";
}

int writeData(char Path[128], int Position, int NewValue, int max)
{
  int c = max;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  fstream datei(Path);
  if (datei.is_open()) {
    for( c = 0; c < max; ++c ){
      datei.getline(read ,128, '\n');
      out[c] = atoi(read);
    }
    datei.close();
  }
  else cerr << "Konnte Datei nicht oeffnen!\n";

  //Ändere Bit an Position
  out[Position] = NewValue;

  //Schreibe geändertes Byte in Datei
  ofstream fout(Path);
  if (fout.is_open()) {
    for( c = 0; c < max; ++c )
      fout << out[c] << endl;
    fout.close();
  }
  else cerr << "Konnte Datei nicht erstellen!\n";

 return 0;
}

int makeData(char Path[128], int Position, int NewValue, int max)
{
  int c = max;
  int out [c];
  //Ändere Bit an Position
  out[Position] = NewValue;

  //Schreibe geändertes Byte in Datei
  ofstream fout(Path);
  if (fout.is_open()) {
    for( c = 0; c < max; ++c )
      fout << out[c] << endl;
    fout.close();
  }
  else cerr << "Konnte Datei nicht erstellen!\n";

 return 0;
}

int writeCharData(char Path[128], char *TAG_EMS_OUT_DATE, char *TAG_EMS_OUT_TIME, char *TAG_EMS_OUT_SERIAL_NUMBER)
{
  ofstream fout(Path);
  if (fout.is_open()) {
    fout << TAG_EMS_OUT_DATE << "\n" << TAG_EMS_OUT_TIME << "\n" << TAG_EMS_OUT_SERIAL_NUMBER << endl;
    fout.close();
  }
  else cerr << "Konnte Datei nicht erstellen!\n";

 return 0;
}

int readData(char Path[128], int Position, int max)
{
  int c = max;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  fstream datei(Path);
  if (datei.is_open()) {
    for( c = 0; c < max; ++c ){
      datei.getline(read ,128, '\n');
      out[c] = atoi(read);
    }
    datei.close();
  }
  else cerr << "Konnte Datei nicht oeffnen!\n";

 return out[Position];
}

int writeRscp(int Position, int NewValue)
{
  char PathRscp [128];
  snprintf (PathRscp, (size_t)128, "/mnt/RAMDisk/E3dcGuiData.txt");
  writeData(PathRscp, Position, NewValue, PosMAX);
  return 1;
}
int makeRscp(int Position, int NewValue)
{
  char PathRscp [128];
  snprintf (PathRscp, (size_t)128, "/mnt/RAMDisk/E3dcGuiData.txt");
  makeData(PathRscp, Position, NewValue, PosMAX);
  return 1;
}
int writeCharRscp(char *TAG_EMS_OUT_DATE, char *TAG_EMS_OUT_TIME, char *TAG_EMS_OUT_SERIAL_NUMBER)
{
  char PathRscp [128];
  snprintf (PathRscp, (size_t)128, "/mnt/RAMDisk/E3dcGuiChar.txt");
  writeCharData(PathRscp, TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME, TAG_EMS_OUT_SERIAL_NUMBER);
  return 1;
}
int readRscp(int Position)
{
  char PathRscp [128];
  snprintf (PathRscp, (size_t)128, "/mnt/RAMDisk/E3dcGuiData.txt");
  int ret = readData(PathRscp, Position, PosMAX);
  return ret;
}
int makeCharRscp()
{
  char TAG_Date [128], TAG_Time [128], serialnumber [128];
  snprintf (TAG_Date, (size_t)128, "01.01.2016");
  snprintf (TAG_Time, (size_t)128, "00:00:00");
  snprintf (serialnumber, (size_t)128, "S10-XXXXXXXXXXXX");
  writeCharRscp(TAG_Date, TAG_Time, serialnumber);
}
int writeUnixtime(int Position, int NewTime)
{
  char PathUnixtime [128];
  snprintf (PathUnixtime, (size_t)128, "/mnt/RAMDisk/Unixtime.txt");
  writeData(PathUnixtime, Position, NewTime, 4);
  return 1;
}
int write900(int Position, char *fileName, int NewValue, int Counter900)
{
  if ( GUI == 1 && E3DC_S10 == 1){
    char Path900 [128];
    snprintf (Path900, (size_t)128, "/mnt/RAMDisk/Rscp900.txt");
    int read900 = readData(Path900, Position, PosMAX900);
    NewValue = read900 + NewValue;
    if (Counter900 == 900){
      NewValue = NewValue / 900;
      readWriteData(fileName, NewValue);
      writeData(Path900, Position, 1, PosMAX900);
    }
    else {
      writeData(Path900, Position, NewValue, PosMAX900);
    }
  }
  return 0;
}
int make900(int Position, int NewValue)
{
  if ( GUI == 1 && E3DC_S10 == 1){
    char Path900 [128];
    snprintf (Path900, (size_t)128, "/mnt/RAMDisk/Rscp900.txt");
    makeData(Path900, Position, NewValue, PosMAX900);
  }
  return 1;
}
int readScreen(int Position)
{
  char PathScreen [128];
  snprintf (PathScreen, (size_t)128, "/mnt/RAMDisk/Screen.txt");
  int ret = readData(PathScreen, Position, ScreenMAX);
  return ret;
}

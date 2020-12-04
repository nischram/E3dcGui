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

void printsendHM(int CounterHM, int id, float value)
{
  if(Homematic_E3DC == 1){
    if(CounterHM == HM_Intervall){
      char batch[128];
      memset(batch, 0x00, sizeof(batch));
      snprintf(batch, sizeof(batch), "curl -s \"http://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=%.3f\" > /dev/null 2>&1",HM_IP , id, value);
      printf("send to Homematic ISE_ID %i new Value = %.3f\n",id, value);
      system(batch);
    }
  }
}
void printsendCharHM(int CounterHM, int id, char value[32])
{
  if(Homematic_E3DC == 1){
    if(CounterHM == HM_Intervall){
      char batch[128];
      memset(batch, 0x00, sizeof(batch));
      snprintf(batch, sizeof(batch), "curl -s \"http://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=%s\" > /dev/null 2>&1",HM_IP , id, value);
      printf("send to Homematic ISE_ID %i new Value = %s\n",id, value);
      system(batch);
    }
  }
}
void readWrite900(char *fileName, int NewValue)
{
  int c;
  int line[100];
  char read[20];
  char Path [128];
  snprintf (Path, (size_t)128, "/home/pi/E3dcGui/Data/%s.txt", fileName);
  fstream datei;
  datei.open(Path, ios::in | ios::out);
  if (datei.fail()){
    datei.clear();
    datei.open(Path, ios::out);
    datei.close();
    cout << "Datei Data/" << fileName << ".txt wurde erstellt!\n";
    datei.clear();
    datei.open(Path, ios::in | ios::out);
  }
  if (datei.good()){
    for( c = 0; c < 97; ++c ){
      datei.getline(read	,20, '\n');
      if (read == NULL)
        snprintf (read, (size_t)20, "0");
      line[c] = atoi(read);
    }
    datei.clear();
    datei.seekg (0, ios::beg);
    for( c = 1; c < 96; ++c ){
      datei << line[c] <<"\n";
    }
    datei << NewValue <<"\n";
    datei.close();
  }
  else {
    cout << "Konnte Datei Data/" << fileName << ".txt nicht erstellen!\n";
  }
  return;
}
int writeData(char *Path, int Position, int NewValue, int max)
{
  int c = max;
  int out [c];
  char read[128];
  //Lese Wert aus Datei ein
  fstream datei;
  datei.open(Path, ios::in | ios::out);
  if (datei.fail()){
    datei.clear();
    datei.open(Path, ios::out);
    datei.close();
    cout << "Datei " << Path << " wurde erstellt!\n";
    datei.clear();
    datei.open(Path, ios::in | ios::out);
  }
  if (datei.good()){
    for( c = 0; c < max; ++c ){
      datei.getline(read ,128, '\n');
      if (read == NULL)
        snprintf (read, (size_t)20, "0");
      out[c] = atoi(read);
    }
    out[Position] = NewValue;
    datei.clear();
    datei.seekg (0, ios::beg);
    for( c = 0; c < max; ++c ){
      datei << out[c] << endl;
    }
    datei.close();
  }
  else {
    cout << "Konnte Datei " << Path << " nicht erstellen!\n";
  }
  return 0;
}
int makeData(char *Path, int NewValue, int max)
{
  int c = max;
  ofstream fout;
  fout.open(Path);
  if (fout.fail()){
    fout.clear();
    fout.open(Path);
    fout.close();
    cout << "Datei " << Path << " wurde erstellt!\n";
    fout.clear();
    fout.open(Path);
  }
  if (fout.good()){
    for( c = 0; c < max; ++c )
      fout << NewValue << endl;
    fout.close();
  }
  else {
    cout << "Konnte Datei " << Path << " nicht erstellen!\n";
  }
 return 0;
}
int writeCharData(char *Path, char *CHAR1, char *CHAR2, char *CHAR3)
{
  ofstream fout(Path);
  if (fout.is_open()) {
    fout << CHAR1 << "\n" << CHAR2 << "\n" << CHAR3 << endl;
    fout.close();
  }
  else cerr << "Konnte Datei nicht erstellen!\n";

 return 0;
}
int readData(char *Path, int Position, int max)
{
  int c = max;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  fstream datei;
  datei.open(Path, ios::in);
  if (datei.fail()){
    datei.clear();
    datei.open(Path, ios::out);
    datei.close();
    cout << "Datei " << Path << " wurde erstellt!\n";
    datei.clear();
    datei.open(Path, ios::in);
  }
  if (datei.good()){
    for( c = 0; c < max; ++c ){
      datei.getline(read ,128, '\n');
      if (read == NULL)
        snprintf (read, (size_t)20, "0");
      out[c] = atoi(read);
    }
    datei.close();
  }
  else {
    cout << "Konnte Datei " << Path << " nicht erstellen!\n";
  }
 return out[Position];
}
int writeRscp(int Position, int NewValue)
{
  char PathRscp [128];
  snprintf (PathRscp, (size_t)128, "/mnt/RAMDisk/E3dcRscpCache.txt");
  writeData(PathRscp, Position, NewValue, PosMAX);
  return 1;
}
int writeCharRscp(char *TAG_EMS_OUT_DATE, char *TAG_EMS_OUT_TIME, char *TAG_EMS_OUT_SERIAL_NUMBER)
{
  char PathRscp [128];
  snprintf (PathRscp, (size_t)128, "/mnt/RAMDisk/E3dcGuiChar.txt");
  writeCharData(PathRscp, TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME, TAG_EMS_OUT_SERIAL_NUMBER);
  return 1;
}
int makeCharRscp()
{
  char TAG_Date [128], TAG_Time [128], serialnumber [128];
  snprintf (TAG_Date, (size_t)128, "01.01.2016");
  snprintf (TAG_Time, (size_t)128, "00:00:00");
  snprintf (serialnumber, (size_t)128, "S10-XXXXXXXXXXXX");
  writeCharRscp(TAG_Date, TAG_Time, serialnumber);
  return 1;
}
int writeUnixtime(int Position, int NewTime)
{
  char PathUnixtime [128];
  snprintf (PathUnixtime, (size_t)128, "/mnt/RAMDisk/Unixtime.txt");
  writeData(PathUnixtime, Position, NewTime, UnixtimeMAX);
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
      readWrite900(fileName, NewValue);
      writeData(Path900, Position, 1, PosMAX900);
    }
    else {
      writeData(Path900, Position, NewValue, PosMAX900);
    }
  }
  return 0;
}
int make900()
{
  if ( GUI == 1 && E3DC_S10 == 1){
    char Path [128];
    snprintf (Path, (size_t)128, "/mnt/RAMDisk/Rscp900.txt");
    makeData(Path, 1, PosMAX900);
  }
  return 1;
}
int writeHistory(int Position, int NewValue, int writedata)
{
	if (writedata == today || writedata == yesterday){
		char filePath[128];
		if (writedata == today){
			snprintf (filePath, (size_t)128, "%s", today_path);
		}
		else if (writedata == yesterday){
			snprintf (filePath, (size_t)128, "%s", yesterday_path);
		}
    writeData(filePath, Position, NewValue, dataMax);
    return 1;
	}
	else return 0;
}
int writeHistoryTime(int Position,int AktuellTime, int NewValue, int writedata)
{
  int ret;
  if(writedata == today)
    ret = writeHistory(Position, AktuellTime, writedata);
  else if(writedata == yesterday)
    ret = writeHistory(Position, NewValue, writedata);
  return ret;
}

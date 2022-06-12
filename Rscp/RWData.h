#ifndef __RWData_H_
#define __RWData_H_

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

using namespace std;
// In future there should be an implementation for converting string to UTF8. Otherwise strings, e.g. with blanks wouldn't be written to Homematic.
string replaceinString(std::string str, std::string tofind, std::string toreplace)
{
        size_t position = 0;
        for ( position = str.find(tofind); position != std::string::npos; position = str.find(tofind,position) )
        {
                str.replace(position ,1, toreplace);
        }
        return(str);
}

void printsendHM(int CounterHM, int id, float value)
{
  if(Homematic_E3DC == 1){
    if(CounterHM == HM_Intervall && id != 0){
      char batch[128];
      memset(batch, 0x00, sizeof(batch));
      snprintf(batch, sizeof(batch), "curl -k -s \"https://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=%.1f\" > /dev/null 2>&1",HM_IP , id, value);
      printf("send to Homematic ISE_ID %i new Value = %.3f\n",id, value);
      system(batch);
    }
  }
}
void printsendCharHM(int CounterHM, int id, char value[32])
{
  if(Homematic_E3DC == 1){
    if(CounterHM == HM_Intervall && id != 0){
      char batch[128];
      memset(batch, 0x00, sizeof(batch));
      snprintf(batch, sizeof(batch), "curl -k -s \"https://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=%s\" > /dev/null 2>&1",HM_IP , id, value);
      printf("send to Homematic ISE_ID %i new Value = %s\n",id, value);
      system(batch);
    }
  }
}
void printsendBitHM(int CounterHM, int id, int Value, int Bit)
{
  if(Homematic_E3DC == 1){
    if(CounterHM == HM_Intervall && id != 0){
      char batch[128];
      memset(batch, 0x00, sizeof(batch));
      bool response = Value&Bit;
      if(response){
        snprintf(batch, sizeof(batch), "curl -k -s \"https://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=true\" > /dev/null 2>&1",HM_IP , id);
        printf("send to Homematic ISE_ID %i new Value = true\n",id);
      }
      else {
        snprintf(batch, sizeof(batch), "curl -k -s \"https://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=false\" > /dev/null 2>&1",HM_IP , id);
        printf("send to Homematic ISE_ID %i new Value = false\n",id);
      }
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
int writeCharData(char *Path, char *CHAR1, char *CHAR2, char *CHAR3, char *CHAR4)
{
  ofstream fout(Path);
  if (fout.is_open()) {
    fout << CHAR1 << "\n" << CHAR2 << "\n" << CHAR3 << "\n" << CHAR4 << endl;
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
  snprintf (PathRscp, (size_t)128, "/mnt/RAMDisk/E3dcRscpData.txt");
  writeData(PathRscp, Position, NewValue, PosMAX);
  return 1;
}
int writeCharRscp(char *TAG_EMS_OUT_DATE, char *TAG_EMS_OUT_TIME, char *TAG_EMS_OUT_SERIAL_NUMBER, char *TAG_INFO_OUT_SW_RELEASE)
{
  char PathRscp [128];
  snprintf (PathRscp, (size_t)128, "/mnt/RAMDisk/E3dcGuiChar.txt");
  writeCharData(PathRscp, TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME, TAG_EMS_OUT_SERIAL_NUMBER, TAG_INFO_OUT_SW_RELEASE);
  return 1;
}
int makeCharRscp()
{
  char TAG_Date [128], TAG_Time [128], serialnumber [128], release [128];
  snprintf (TAG_Date, (size_t)128, "01.01.2016");
  snprintf (TAG_Time, (size_t)128, "00:00:00");
  snprintf (serialnumber, (size_t)128, "S10-XXXXXXXXXXXX");
  snprintf (release, (size_t)128, "S10_20XX_XX");
  writeCharRscp(TAG_Date, TAG_Time, serialnumber, release);
  return 1;
}
int writeRscpWb(int Position, int NewValue)
{
  char PathRscpWb [128];
  snprintf (PathRscpWb, (size_t)128, "/mnt/RAMDisk/E3dcRscpWbData.txt");
  writeData(PathRscpWb, Position, NewValue, PosWbMAX);
  return 1;
}
int checkBit(int Value, int Bit)
{
  bool response = Value&Bit;
  if(response)return 1;
  else return 0;
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
int readTo(int Position)
{
  char Path [128];
  snprintf (Path, (size_t)128, "/mnt/RAMDisk/ValueTo.txt");
  return readData(Path, Position, PosToMAX);
}
int writeTo(int Position, int NewValue)
{
  char Path [128];
  snprintf (Path, (size_t)128, "/mnt/RAMDisk/ValueTo.txt");
  writeData(Path, Position, NewValue, PosToMAX);
  return 1;
}
int writeIdle(int Position, int State, int startH, int startM, int stopH, int stopM)
{
  char Path [128];
  snprintf (Path, (size_t)128, "/mnt/RAMDisk/IdlePeriods.txt");
  writeData(Path, Position, State, PosIdleMax);
  writeData(Path, Position+1, startH, PosIdleMax);
  writeData(Path, Position+2, startM, PosIdleMax);
  writeData(Path, Position+3, stopH, PosIdleMax);
  writeData(Path, Position+4, stopM, PosIdleMax);
  return 1;
}
int makeIdle()
{
  char Path [128];
  snprintf (Path, (size_t)128, "/mnt/RAMDisk/IdlePeriods.txt");
  makeData(Path, 0, PosIdleMax);
  return 1;
}

typedef struct {
    uint8_t hour;
    uint8_t minute;
}idle_time_t;

typedef struct {
    uint8_t type;
    uint8_t day;
    uint8_t active;
    idle_time_t start;
    idle_time_t stop;
}idle_period_t;

int handleResponseEMSGetIdlePeriods(RscpProtocol * protocol,SRscpValue * emsData,idle_period_t *periods)
{
  // check each idle periods sub tag
  switch (emsData->tag) {
    case TAG_EMS_IDLE_PERIOD:{
	    std::vector < SRscpValue > idleData = protocol->getValueAsContainer(emsData);
	    // check each idle period sub tag
      for (size_t j = 0; j < idleData.size(); ++j) {
        if (idleData[j].dataType == RSCP::eTypeError) {
          // handle error for example access denied errors
          uint32_t uiErrorCode = protocol->getValueAsUInt32(&idleData[j]);
          printf("Tag 0x%08X received error code %u.\n",idleData[j].tag, uiErrorCode);
          return -1;
        }
        switch (idleData[j].tag) {
          case TAG_EMS_IDLE_PERIOD_TYPE:{
            periods->type = protocol->getValueAsUChar8(&idleData[j]);
            break;
          }
          case TAG_EMS_IDLE_PERIOD_DAY:{
            periods->day = protocol->getValueAsUChar8(&idleData[j]);
            break;
          }
          case TAG_EMS_IDLE_PERIOD_ACTIVE:{
            periods->active = protocol->getValueAsUChar8(&idleData[j]);
            break;
          }
          case TAG_EMS_IDLE_PERIOD_START:{
            std::vector < SRscpValue > periodData = protocol->getValueAsContainer(&idleData[j]);
            // check each idle period start sub tag
            for (size_t k = 0; k < periodData.size(); ++k) {
              if (periodData[k].dataType == RSCP::eTypeError) {
                // handle error for example access denied errors
                uint32_t uiErrorCode = protocol->getValueAsUInt32(&periodData[k]);
                printf("Tag 0x%08X received error code %u.\n",periodData[k].tag, uiErrorCode);
                return -1;
              }
              switch (periodData[k].tag) {
                case TAG_EMS_IDLE_PERIOD_HOUR:{
                  periods->start.hour = protocol->getValueAsUChar8(&periodData[k]);
                  break;
                }
                case TAG_EMS_IDLE_PERIOD_MINUTE:{
                  periods->start.minute = protocol->getValueAsUChar8(&periodData[k]);
                  break;
                }
                default:
                // default behaviour
                uint8_t unknown = protocol->getValueAsUChar8(&periodData[k]);
                printf("Unknown period tag %08X -> %i.\n",periodData[k].tag, unknown);
                break;
              }
            }
            break;
          }
          case TAG_EMS_IDLE_PERIOD_END:{
            std::vector < SRscpValue > periodData = protocol->getValueAsContainer(&idleData[j]);
            // check each idle period stop sub tag
            for (size_t k = 0; k < periodData.size(); ++k) {
              if (periodData[k].dataType == RSCP::eTypeError) {
                // handle error for example access denied errors
                uint32_t uiErrorCode = protocol->getValueAsUInt32(&periodData[k]);
                printf("Tag 0x%08X received error code %u.\n",periodData[k].tag, uiErrorCode);
                return -1;
              }
              switch (periodData[k].tag) {
                case TAG_EMS_IDLE_PERIOD_HOUR:{
                  periods->stop.hour = protocol->getValueAsUChar8(&periodData[k]);
                  break;
                }
                case TAG_EMS_IDLE_PERIOD_MINUTE:{
                  periods->stop.minute = protocol->getValueAsUChar8(&periodData[k]);
                  break;
                }
                default:
                // default behaviour
                uint8_t unknown = protocol->getValueAsUChar8(&periodData[k]);
                printf("Unknown period tag %08X -> %i.\n",periodData[k].tag, unknown);
                break;
              }
            }
            break;
          }
        }
	    }
	    // print idle periods summary
	    if (periods->day == MON){
		    printf("Monday:     \t");
        if (periods->type == CHARGE)writeIdle(PosMonCharge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
        else if (periods->type == DISCHARGE)writeIdle(PosMonDischarge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
      }
	    else if (periods->day == TUE){
		    printf("Tuesday:    \t");
        if (periods->type == CHARGE)writeIdle(PosTueCharge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
        else if (periods->type == DISCHARGE)writeIdle(PosTueDischarge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
      }
	    else if (periods->day == WED){
		    printf("Wednesday:  \t");
        if (periods->type == CHARGE)writeIdle(PosWedCharge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
        else if (periods->type == DISCHARGE)writeIdle(PosWedDischarge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
      }
	    else if (periods->day == THU){
		    printf("Thursday:   \t");
        if (periods->type == CHARGE)writeIdle(PosThuCharge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
        else if (periods->type == DISCHARGE)writeIdle(PosThuDischarge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
      }
	    else if (periods->day == FRI){
		    printf("Friday:     \t");
        if (periods->type == CHARGE)writeIdle(PosFriCharge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
        else if (periods->type == DISCHARGE)writeIdle(PosFriDischarge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
      }
	    else if (periods->day == SAT){
		    printf("Saturday:   \t");
        if (periods->type == CHARGE)writeIdle(PosSatCharge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
        else if (periods->type == DISCHARGE)writeIdle(PosSatDischarge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
      }
	    else if (periods->day == SUN){
		    printf("Sunday:     \t");
        if (periods->type == CHARGE)writeIdle(PosSunCharge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
        else if (periods->type == DISCHARGE)writeIdle(PosSunDischarge, periods->active, periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
      }
	    else
		    printf("Unknown day:\t");

	    if (periods->type == CHARGE)
		    printf("Ladesperre    ");
	    else if (periods->type == DISCHARGE)
		    printf("Entladesperre ");
	    else
		    printf("Unknown type!");

	    if (periods->active)
		    printf("aktiv von %02i:%02i - %02i:%02i",periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
	    else if (!periods->active)
        printf("inaktiv  (%02i:%02i - %02i:%02i)",periods->start.hour, periods->start.minute, periods->stop.hour, periods->stop.minute);
	    else
		    printf("Activity unknown! ");
	    printf("\n");
	    break;
    }
    default:
    // default behaviour
    uint8_t unknown = protocol->getValueAsUChar8(emsData);
    printf("Unknown ems tag %08X -> %i.\n", emsData->tag, unknown);
    break;
  }
  protocol->destroyValueData(emsData);
}

#endif // __RWData_H_

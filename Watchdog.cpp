/*
g++ -O1 Watchdog.cpp -o watchdog
 */
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string.h>
#include "checkPara.h"

#define WDfail                    0
#define WDwait                    1
#define WDaktiv                   2

using namespace std;

void WriteWatchdog(int value)
{
  ofstream fout("/home/pi/E3dcGui/Data/WatchdogAktiv.txt");
  if (fout.is_open()) {
    fout << value << endl;
    fout.close();
  }
}
int ReadWatchdog()
{
  int value = 0;
  char read[128];
  fstream datei("/home/pi/E3dcGui/Data/WatchdogAktiv.txt");
  if (datei.is_open()) {
    datei.getline(read ,128, '\n');
    value = atoi(read);
    datei.close();
    return value;
  }
  else cerr << "Konnte Datei WatchdogAktiv nicht öffnen!\n";
  return 0;
}
void writeReleaseLast(char* value)
{
  ofstream fout("/home/pi/E3dcGui/Data/S10_Release.txt");
  if (fout.is_open()) {
    fout << value << endl;
    fout.close();
  }
}
void readReleaseLast(char* value)
{
  fstream datei("/home/pi/E3dcGui/Data/S10_Release.txt");
  if (datei.is_open()) {
    datei.getline(value ,128, '\n');
    datei.close();
  }
  else cerr << "Konnte Datei S10_Release.txt nicht öffnen!\n";
}
void readReleaseRscp(char* value)
{
  char del[128];
  fstream datei("/mnt/RAMDisk/E3dcGuiChar.txt");
  if (datei.is_open()) {
    datei.getline(del ,20, '\n');
    datei.getline(del ,20, '\n');
    datei.getline(del ,20, '\n');
    datei.getline(value ,20, '\n');
    datei.close();
  }
  else cerr << "Konnte Datei S10_Release.txt nicht öffnen!\n";
}

void WriteDataWDcsv(char DATE[40],char TIME[40], int AktuallTime, int UnixTime, int resetCounter, char OUT[100]){
  ofstream fout("/home/pi/E3dcGui/Data/Watchdog.csv", ios::app);
  if (fout.is_open()) {
    fout << DATE << ";" << TIME << ";" << AktuallTime << ";" << UnixTime << ";" << resetCounter << ";" << OUT << endl;
    fout.close();
  }
}
void WDcsvKontrolle(char DATE[40],char TIME[40], char pingOUT[40], int AktuallTime, int UnixTimeE3dc, int UnixTimeHM, int UnixTimeGUI, char OUT[100]){
  if (WDkontrolle == 1){
    ofstream fout("/home/pi/E3dcGui/Data/WatchdogKontrolle.csv", ios::app);
    if (fout.is_open()) {
      fout << DATE << " ; " << TIME << " ; Ping " << pingOUT << " ; PI " << AktuallTime << " ; RSCP " << UnixTimeE3dc << " ; HM "<< UnixTimeHM << " ; GUI "<< UnixTimeGUI <<" >>> "<< OUT <<  endl ;
      fout.close();
    }
  }
}

void sendEmail(char EmailAdress[128], char Betreff[128], char Text[512])
{
  char batch[512];
  snprintf(batch, (size_t)768, "sudo sendEmail -o tls=%s -f %s -t %s -s %s:%s -xu %s -xp %s -u \"%s\" -m \"%s\"", smtpTLS, FromEmailAdress, EmailAdress, smtpServer, smtpPort, smtpBenutzer, smtpPass, Betreff, Text);
  printf("sendEmail To: %s\n      Betreff:%s\n      Text:%s\n",EmailAdress, Betreff, Text);
  system(batch);
}

int ReadUnixtime(int Position, int max)
{
  int c = max;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  fstream datei("/mnt/RAMDisk/Unixtime.txt");
  if (datei.is_open()) {
    for( c = 0; c < max; ++c ){
      datei.getline(read ,128, '\n');
      out[c] = atoi(read);
    }
    datei.close();
    return out[Position];
  }
  else cerr << "Konnte Datei nicht erstellen!\n";
  return -1;
}
int WriteScreen(int Position, int NewValue, int max)
{
  int c = max;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  fstream datei("/mnt/RAMDisk/Screen.txt");
  if (datei.is_open()) {
    for( c = 0; c < max; ++c ){
      datei.getline(read ,128, '\n');
      out[c] = atoi(read);
    }
    datei.close();
  }
  else {
    cerr << "Konnte Screen Datei nicht oeffnen!\n";
    return 0;
  }

  //Ändere Bit an Position
  out[Position] = NewValue;

  //Schreibe geändertes Byte in Datei
  ofstream fout("/mnt/RAMDisk/Screen.txt");
  if (fout.is_open()) {
    for( c = 0; c < 8; ++c )
      fout << out[c] << endl;
    fout.close();
  }
  else {
    cerr << "Konnte Screen Datei nicht oeffnen!\n";
    return 0;
  }
 return 1;
}

int ping (char* respons)
{
  char batch[256], OUT[100];
  FILE *pingOUT;
  snprintf(batch, (size_t)256, "sudo ping -c 1 %s | head -2l | tail -1l | cut -d: -f2 | cut -d\" \" -f4  | cut -b 6-10", RouterIP);
  pingOUT = popen (batch, "r");
  fgets(OUT,sizeof(OUT),pingOUT);
  float pingTime = atof(OUT);
  pclose (pingOUT);
  if (pingTime == 0 || pingTime > 300)
    snprintf(respons, (size_t)128, "NOK");
  else
    snprintf(respons, (size_t)128, "OK");
  return pingTime *100;
}


int main()
{
    //Parameter einbinden, checken oder default setzen
  	checkDefinePara(0);

    int counterReboot = 0,counterRebootHM = 0,counterRebootGUI = 0,resetCounter = 0;
    int resetTime = resetMin *60 / sleepTimeWD;
    int jump = 0;
    char EmailAdress[128], EmailBetreff[128], EmailText[512];
    int Unixtime[4];
    char DATE[40], TIME[40], OUT[100], batch[256], pingOUT[128];
    char releaseSwLast[40], releaseSwRscp[40];

    int readWD = ReadWatchdog();
    if (readWD == WDfail){
      WriteWatchdog(WDwait);
    }
    else if (readWD == WDaktiv){
      sleep (3600);
      WriteWatchdog(WDwait);
    }
    while(1){
      sleep(sleepTimeWD);
      int AktuallTime = time(NULL);
      time_t timestamp;
      tm *sys;
      timestamp = time(NULL);
      sys = localtime(&timestamp);
      strftime (DATE,40,"%d.%m.%Y",sys);
      strftime (TIME,40,"%H:%M:%S",sys);

      float pingTime = ping (pingOUT);
      pingTime = pingTime /100;

      int UnixTimeE3dc = ReadUnixtime(UnixtimeE3dc, UnixtimeMAX);
      int DiffTimeE3dc = AktuallTime - UnixTimeE3dc;

      int UnixTimeHM = ReadUnixtime(UnixtimeHM, UnixtimeMAX);
      int DiffTimeHM = AktuallTime - UnixTimeHM;

      int UnixTimeGUI = ReadUnixtime(UnixtimeGui, UnixtimeMAX);
      int DiffTimeGUI = AktuallTime - UnixTimeGUI;

      if ((strcmp ("NOK",pingOUT) == 0) && (PingWD == 1 || resetWLAN == 1)){
        counterReboot ++;
        if (counterReboot == rebootCounter+2 && PingWD == 1){
          snprintf (OUT, (size_t)100, "PING-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, AktuallTime, resetCounter, OUT);
          WriteScreen(ScreenShutdown, ShutdownWD, ScreenMAX);
          WriteScreen(ScreenCounter, 0, ScreenMAX);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nPing Test > 500ms \nPI: %i", AktuallTime);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
          WriteWatchdog(WDaktiv);
          sleep (5);
          system("sudo reboot");
          return(0);
        }
        if (WDsendEmailKill == 1){
          snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
          snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
          snprintf (EmailText, (size_t)512, "Watchdog >>> WLAN-reset\nPing Test > 500ms \nPI: %i", AktuallTime);
          sendEmail(EmailAdress, EmailBetreff, EmailText);
        }
        if (resetWLAN == 1){
          system("sudo ifconfig wlan0 down");
          sleep(1);
          system("sudo ifconfig wlan0 up");
          snprintf (OUT, (size_t)100, "WLAN-reset");
          WriteDataWDcsv(DATE, TIME, AktuallTime, AktuallTime, resetCounter, OUT);
        }
        jump ++;
      }
      else if(DiffTimeE3dc > WDdiff && E3DC_S10 == 1 && jump == 0){
        counterReboot ++;
        if (counterReboot == rebootCounter){
          snprintf (OUT, (size_t)100, "RSCP-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeE3dc, resetCounter, OUT);
          WriteScreen(ScreenShutdown, ShutdownWD, ScreenMAX);
          WriteScreen(ScreenCounter, 0, ScreenMAX);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nRSCP-Time > %i Sek. \nPI: %i / RSCP: %i", WDdiff, AktuallTime, UnixTimeE3dc);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
          WriteWatchdog(WDaktiv);
					sleep (5);
          system("sudo reboot");
          return(0);
        }
        if (WDsendEmailKill == 1){
          snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
          snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
          snprintf (EmailText, (size_t)512, "Watchdog >>> Kill\nRSCP-Time > %i Sek. \nPI: %i / RSCP: %i", WDdiff, AktuallTime, UnixTimeE3dc);
          sendEmail(EmailAdress, EmailBetreff, EmailText);
        }
        system("killall -9 RscpMain");
        sleep(2);
        system("/home/pi/E3dcGui/RscpMain &");
        snprintf (OUT, (size_t)100, "RSCP-kill");
        WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeE3dc, resetCounter, OUT);
        jump ++;
      }
      else if(DiffTimeHM > WDdiff && jump == 0 && Homematic_GUI == 1 && WDuseHM_Gui == 1){
        counterRebootHM ++;
        if (counterRebootHM == rebootCounter){
          snprintf (OUT, (size_t)100, "HM_GUI-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeHM, resetCounter, OUT);
          WriteScreen(ScreenShutdown, ShutdownWD, ScreenMAX);
          WriteScreen(ScreenCounter, 0, ScreenMAX);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nHM-Time > %i Sek. \nPI: %i / HM: %i", WDdiff, AktuallTime, UnixTimeHM);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
          WriteWatchdog(WDaktiv);
					sleep (5);
          system("sudo reboot");
          return(0);
        }
        if (WDsendEmailKill == 1){
          snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
          snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
          snprintf (EmailText, (size_t)512, "Watchdog >>> Kill\nHM-Time > %i Sek. \nPI: %i / HM: %i", WDdiff, AktuallTime, UnixTimeHM);
          sendEmail(EmailAdress, EmailBetreff, EmailText);
        }
        system("killall -9 GuiMain screenSave screenSaveHM");
        sleep(2);
        system("/home/pi/E3dcGui/GuiMain &");
        system("/home/pi/E3dcGui/screenSave &");
        system("/home/pi/E3dcGui/screenSaveHM &");
        snprintf (OUT, (size_t)100, "HM_GUI-kill");
        WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeHM, resetCounter, OUT);
        jump ++;
      }
      else if(DiffTimeGUI > WDdiff && jump == 0 && (GUI == 1 || Homematic_GUI == 1)){
        counterRebootGUI ++;
        if (counterRebootGUI == rebootCounter){
          snprintf (OUT, (size_t)100, "GuiMain-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeGUI, resetCounter, OUT);
          WriteScreen(ScreenShutdown, ShutdownWD, ScreenMAX);
          WriteScreen(ScreenCounter, 0, ScreenMAX);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nGUI-Time > %i Sek. \nPI: %i / GUI: %i", WDdiff, AktuallTime, UnixTimeGUI);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
          WriteWatchdog(WDaktiv);
          sleep (5);
          system("sudo reboot");
          return(0);
        }
        if (WDsendEmailKill == 1){
          snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
          snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
          snprintf (EmailText, (size_t)512, "Watchdog >>> Kill\nGUI-Time > %i Sek. \nPI: %i / GUI: %i", WDdiff, AktuallTime, UnixTimeGUI);
          sendEmail(EmailAdress, EmailBetreff, EmailText);
        }
        system("killall -9 GuiMain screenSave screenSaveHM");
        sleep(2);
        system("/home/pi/E3dcGui/GuiMain &");
        system("/home/pi/E3dcGui/screenSave &");
        system("/home/pi/E3dcGui/screenSaveHM &");
        snprintf (OUT, (size_t)100, "GuiMain-kill");
        WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeGUI, resetCounter, OUT);
      }
      else {
        resetCounter ++;
        snprintf (OUT, (size_t)100, "---");
        if(resetCounter == resetTime){
          counterReboot = 0;
          counterRebootHM = 0;
          counterRebootGUI = 0;
          resetCounter = 0;
        }
      }
      jump = 0;

      cout << "Watchdog: " << DATE << " ; " << TIME << "\n";
      cout << "   PI " << AktuallTime << " ; RSCP " << UnixTimeE3dc << " ; HM "<< UnixTimeHM << " ; GUI "<< UnixTimeGUI << "\n";
      cout << "   Reset Zähler: " << resetCounter << " ; RSCP Zähler bis Reboot: " << counterReboot << " ; HM Zähler bis Reboot: " << counterRebootHM << "\n";
      cout << "   GUI Zähler bis Reboot: " << counterRebootGUI << " ; Ping: " << pingOUT << " ; Pingzeit: " << pingTime << " ms\n";

      readReleaseLast(releaseSwLast);
      readReleaseRscp(releaseSwRscp);
      if (strcmp (releaseSwLast,releaseSwRscp) == 0){
        cout << "   S10 Release Check Ready > Last: " << releaseSwLast << " Read: " << releaseSwRscp << "\n";
      }
      else{
        cout << "   S10 Release Check Faild > Last: " << releaseSwLast << " Last: " << releaseSwRscp << "\n";
        if (WDsendEmailRelease == 1){
          snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
          snprintf (EmailBetreff, (size_t)128, "Neue S10 Release Version erkannt");
          snprintf (EmailText, (size_t)512, "E3dcGui Watchdog >>> Software-Release Kontrolle\n \nAltes Software-Release: %s\n Neues Software-Release: %s\n", releaseSwLast, releaseSwRscp);
          sendEmail(EmailAdress, EmailBetreff, EmailText);
        }
        writeReleaseLast(releaseSwRscp);
      }

      if (WDkontrolle == 1)
        WDcsvKontrolle( DATE, TIME ,pingOUT ,AktuallTime, UnixTimeE3dc, UnixTimeHM, UnixTimeGUI, OUT);
    }
}

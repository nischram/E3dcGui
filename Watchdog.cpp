#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include "parameter.h"

using namespace std;

void WriteDataWDcsv(char DATE[40],char TIME[40], int AktuallTime, int UnixTime, int resetCounter, char OUT[100]){
  ofstream fout("/home/pi/E3dcGui/Watchdog.csv", ios::app);
  if (fout.is_open()) {
    fout << DATE << ";" << TIME << ";" << AktuallTime << ";" << UnixTime << ";" << resetCounter << ";" << OUT << endl;
    fout.close();
  }
}
void WDcsvKontrolle(char DATE[40],char TIME[40], int AktuallTime, int UnixTimeE3dc, int UnixTimeHM, int UnixTimeGUI, char OUT[100]){
  if (WDkontrolle == 1){
    ofstream fout("/home/pi/E3dcGui/WatchdogKontrolle.csv", ios::app);
    if (fout.is_open()) {
      fout << DATE << " ; " << TIME << "; PI " << AktuallTime << " ; RSCP " << UnixTimeE3dc << " ; HM "<< UnixTimeHM << " ; GUI "<< UnixTimeGUI <<" >>> "<< OUT <<  endl ;
      fout.close();
    }
  }
}

int sendEmail(char EmailAdress[128], char Betreff[128], char Text[512])
{
  char batch[512];
  snprintf(batch, (size_t)768, "sudo sendEmail -o tls=%s -f %s -t %s -s %s:%s -xu %s -xp %s -u \"%s\" -m \"%s\"", smtpTLS, FromEmailAdress, EmailAdress, smtpServer, smtpPort, smtpBenutzer, smtpPass, Betreff, Text);
  printf("sendEmail To: %s\n      Betreff:%s\n      Text:%s\n",EmailAdress, Betreff, Text);
  system(batch);
}

int ReadUnixtime(int Position)
{
  int c = 4;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  fstream datei("/mnt/RAMDisk/Unixtime.txt");
  if (datei.is_open()) {
    for( c = 0; c < 4; ++c ){
      datei.getline(read ,128, '\n');
      out[c] = atoi(read);
    }
    datei.close();
    return out[Position];
  }
  else cerr << "Konnte Datei nicht erstellen!\n";
}
int WriteScreen(int Position, int NewValue)
{
  int c = 8;
  int out [c];
  char read[128];
  //Lese Byte aus Datei ein
  fstream datei("/mnt/RAMDisk/Screen.txt");
  if (datei.is_open()) {
    for( c = 0; c < 8; ++c ){
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

char Path[100], DATE[40], TIME[40], OUT[100];

int main()
{
    int counterReboot = 0,counterRebootHM = 0,counterRebootGUI = 0,resetCounter = 0;
    int resetTime = resetMin *60 / sleepTimeWD;
    int jump = 0;
    char EmailAdress[128], EmailBetreff[128], EmailText[512];
    int Unixtime[4];

    while(1){
      sleep(sleepTimeWD);
      int AktuallTime = time(NULL);
      time_t timestamp;
      tm *sys;
      timestamp = time(NULL);
      sys = localtime(&timestamp);
      strftime (DATE,40,"%d.%m.%Y",sys);
      strftime (TIME,40,"%H:%M:%S",sys);

      int UnixTimeE3dc = ReadUnixtime(UnixtimeE3dc);
      int DiffTimeE3dc = AktuallTime - UnixTimeE3dc;

      int UnixTimeHM = ReadUnixtime(UnixtimeHM);
      int DiffTimeHM = AktuallTime - UnixTimeHM;

      int UnixTimeGUI = ReadUnixtime(UnixtimeGui);
      int DiffTimeGUI = AktuallTime - UnixTimeGUI;

      if(DiffTimeE3dc > WDdiff && E3DC_S10 == 1){
        counterReboot ++;
        if (counterReboot == rebootCounter){
          snprintf (OUT, (size_t)100, "RSCP-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeE3dc, resetCounter, OUT);
          WDcsvKontrolle( DATE, TIME ,AktuallTime, UnixTimeE3dc, UnixTimeHM, UnixTimeGUI, OUT);
          WriteScreen(ScreenShutdown, ShutdownWD);
          WriteScreen(ScreenCounter, 0);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nRSCP-Time > %i Sek. \nPI: %i / RSCP: %i", WDdiff, AktuallTime, UnixTimeE3dc);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
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
        system("pkill RscpMain");
        sleep(2);
        system("/home/pi/E3dcGui/RscpMain &");
        snprintf (OUT, (size_t)100, "RSCP-pkill");
        WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeE3dc, resetCounter, OUT);
        WDcsvKontrolle( DATE, TIME ,AktuallTime, UnixTimeE3dc, UnixTimeHM, UnixTimeGUI, OUT);
        jump ++;
      }
      else if(DiffTimeHM > WDdiff && jump == 0 && WDuseHM_Gui == 1){
        counterRebootHM ++;
        if (counterRebootHM == rebootCounter){
          snprintf (OUT, (size_t)100, "HM_GUI-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeHM, resetCounter, OUT);
          WDcsvKontrolle( DATE, TIME ,AktuallTime, UnixTimeE3dc, UnixTimeHM, UnixTimeGUI, OUT);
          WriteScreen(ScreenShutdown, ShutdownWD);
          WriteScreen(ScreenCounter, 0);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nHM-Time > %i Sek. \nPI: %i / HM: %i", WDdiff, AktuallTime, UnixTimeHM);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
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
        system("pkill GuiMain");
        system("pkill screenSave");
        sleep(2);
        system("/home/pi/E3dcGui/GuiMain &");
        system("/home/pi/E3dcGui/screenSave &");
        snprintf (OUT, (size_t)100, "HM_GUI-pkill");
        WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeHM, resetCounter, OUT);
        WDcsvKontrolle( DATE, TIME ,AktuallTime, UnixTimeE3dc, UnixTimeHM, UnixTimeGUI, OUT);
        jump ++;
      }
      else if(DiffTimeGUI > WDdiff && jump == 0 && (GUI == 1 || Homematic_GUI == 1)){
        counterRebootGUI ++;
        if (counterRebootGUI == rebootCounter){
          snprintf (OUT, (size_t)100, "HM_GUI-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeGUI, resetCounter, OUT);
          WDcsvKontrolle( DATE, TIME ,AktuallTime, UnixTimeE3dc, UnixTimeHM, UnixTimeGUI, OUT);
          WriteScreen(ScreenShutdown, ShutdownWD);
          WriteScreen(ScreenCounter, 0);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nGUI-Time > %i Sek. \nPI: %i / GUI: %i", WDdiff, AktuallTime, UnixTimeGUI);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
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
        system("pkill GuiMain");
        system("pkill screenSave");
        sleep(2);
        system("/home/pi/E3dcGui/GuiMain &");
        system("/home/pi/E3dcGui/screenSave &");
        snprintf (OUT, (size_t)100, "GuiMain-pkill");
        WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeHM, resetCounter, OUT);
        WDcsvKontrolle( DATE, TIME ,AktuallTime, UnixTimeE3dc, UnixTimeHM, UnixTimeGUI, OUT);
      }
      else {
        resetCounter ++;
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
      cout << "   Reset Zähler: " << resetCounter << " ; RSCP Zähler bis Reboot: " << counterReboot << " ; HM Zähler bis Reboot: " << counterRebootHM << " ; GUI Zähler bis Reboot: " << counterRebootGUI << " \n" ;
      snprintf (OUT, (size_t)100, "--");
      WDcsvKontrolle( DATE, TIME ,AktuallTime, UnixTimeE3dc, UnixTimeHM, UnixTimeGUI, OUT);
    }
}

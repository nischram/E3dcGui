#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include "parameter.h"

using namespace std;

char response[20], delete_res[20];

void ReadData(char filePath[100]){
  fstream datei(filePath);
  if (datei.is_open()) {
    datei.getline(delete_res ,20, '\n');
    datei.getline(response ,20, '\n');
    datei.close();
  }
  else cerr << "Konnte Datei nicht erstellen!\n";
}
void WriteData(char filePath[100],char OUT[100]){
  ofstream fout(filePath);
  if (fout.is_open()) {
    fout << OUT << endl;
    fout.close();
  }
}

void WriteDataWDcsv(char DATE[40],char TIME[40], int AktuallTime, int UnixTime, int resetCounter, char OUT[100]){
  ofstream fout("/home/pi/E3dcGui/Watchdog.csv", ios::app);
  if (fout.is_open()) {
    fout << DATE << ";" << TIME << ";" << AktuallTime << ";" << UnixTime << ";" << resetCounter << ";" << OUT << endl;
    fout.close();
  }
}

int sendEmail(char EmailAdress[128], char Betreff[128], char Text[512])
{
  char batch[512];
  snprintf(batch, (size_t)768, "sudo sendEmail -o tls=%s -f %s -t %s -s %s:%s -xu %s -xp %s -u \"%s\" -m \"%s\"", smtpTLS, FromEmailAdress, EmailAdress, smtpServer, smtpPort, smtpBenutzer, smtpPass, Betreff, Text);
  printf("sendEmail To: %s\n      Betreff:%s\n      Text:%s\n",EmailAdress, Betreff, Text);
  system(batch);
}

char Path[100], DATE[40], TIME[40], OUT[100];

int main()
{
    int counterReboot = 0,counterRebootHM = 0,counterRebootGUI = 0,resetCounter = 0;
    int resetTime = resetMin *60 / sleepTimeWD;
    int jump = 0;
    char EmailAdress[128], EmailBetreff[128], EmailText[512];

    while(1){
      sleep(sleepTimeWD);
      int AktuallTime = time(NULL);
      time_t timestamp;
      tm *sys;
      timestamp = time(NULL);
      sys = localtime(&timestamp);
      strftime (DATE,40,"%d.%m.%Y",sys);
      strftime (TIME,40,"%H:%M:%S",sys);
      snprintf (Path, (size_t)100, "/mnt/RAMDisk/UnixtimeE3dc.txt");
      ReadData(Path);
      int UnixTimeE3dc = atoi(response);
      int DiffTimeE3dc = AktuallTime - UnixTimeE3dc;
      snprintf (Path, (size_t)100, "/mnt/RAMDisk/UnixtimeHM.txt");
      ReadData(Path);
      int UnixTimeHM = atoi(response);
      int DiffTimeHM = AktuallTime - UnixTimeHM;
      snprintf (Path, (size_t)100, "/mnt/RAMDisk/UnixtimeGUI.txt");
      ReadData(Path);
      int UnixTimeGUI = atoi(response);
      int DiffTimeGUI = AktuallTime - UnixTimeGUI;

      if(DiffTimeE3dc > WDdiff && E3DC_S10 == 1){
        counterReboot ++;
        if (counterReboot == rebootCounter){
          snprintf (OUT, (size_t)100, "RSCP-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeE3dc, resetCounter, OUT);
          snprintf (Path, (size_t)100, "/mnt/RAMDisk/ScreenShutdown.txt");
          snprintf (OUT, (size_t)100, "10");
          WriteData(Path, OUT);
          snprintf (Path, (size_t)100, "/mnt/RAMDisk/ScreenCounter.txt");
          snprintf (OUT, (size_t)100, "0");
          WriteData(Path, OUT);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nRSCP-Time > %i Sek. ", WDdiff);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
					sleep (5);
          system("sudo reboot");
          return(0);
        }
        if (WDsendEmailKill == 1){
          snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
          snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
          snprintf (EmailText, (size_t)512, "Watchdog >>> Kill\nRSCP-Time > %i Sek. ", WDdiff);
          sendEmail(EmailAdress, EmailBetreff, EmailText);
        }
        system("pkill RscpMain");
        sleep(2);
        system("/home/pi/E3dcGui/RscpMain &");
        snprintf (OUT, (size_t)100, "RSCP-pkill");
        WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeE3dc, resetCounter, OUT);
        jump ++;
      }
      else if(DiffTimeHM > WDdiff && jump == 0 && WDuseHM_Gui == 1){
        counterRebootHM ++;
        if (counterRebootHM == rebootCounter){
          snprintf (OUT, (size_t)100, "HM_GUI-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeHM, resetCounter, OUT);
          snprintf (Path, (size_t)100, "/mnt/RAMDisk/ScreenShutdown.txt");
          snprintf (OUT, (size_t)100, "10");
          WriteData(Path, OUT);
          snprintf (Path, (size_t)100, "/mnt/RAMDisk/ScreenCounter.txt");
          snprintf (OUT, (size_t)100, "0");
          WriteData(Path, OUT);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nHM-Time > %i Sek. ", WDdiff);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
					sleep (5);
          system("sudo reboot");
          return(0);
        }
        if (WDsendEmailKill == 1){
          snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
          snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
          snprintf (EmailText, (size_t)512, "Watchdog >>> Kill\nHM-Time > %i Sek. ", WDdiff);
          sendEmail(EmailAdress, EmailBetreff, EmailText);
        }
        system("pkill GuiMain");
        system("pkill screenSave");
        sleep(2);
        system("/home/pi/E3dcGui/GuiMain &");
        system("/home/pi/E3dcGui/screenSave &");
        snprintf (OUT, (size_t)100, "HM_GUI-pkill");
        WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeHM, resetCounter, OUT);
        jump ++;
      }
      else if(DiffTimeGUI > WDdiff && jump == 0 && (GUI == 1 || Homematic_GUI == 1)){
        counterRebootGUI ++;
        if (counterRebootGUI == rebootCounter){
          snprintf (OUT, (size_t)100, "HM_GUI-reboot");
          WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeGUI, resetCounter, OUT);
          snprintf (Path, (size_t)100, "/mnt/RAMDisk/ScreenShutdown.txt");
          snprintf (OUT, (size_t)100, "10");
          WriteData(Path, OUT);
          snprintf (Path, (size_t)100, "/mnt/RAMDisk/ScreenCounter.txt");
          snprintf (OUT, (size_t)100, "0");
          WriteData(Path, OUT);
          if (WDsendEmailReboot == 1){
            snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
            snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
            snprintf (EmailText, (size_t)512, "Watchdog >>> Reboot\nGUI-Time > %i Sek. ", WDdiff);
            sendEmail(EmailAdress, EmailBetreff, EmailText);
          }
          sleep (5);
          system("sudo reboot");
          return(0);
        }
        if (WDsendEmailKill == 1){
          snprintf (EmailAdress, (size_t)128, "%s", WDtoEmailAdress);
          snprintf (EmailBetreff, (size_t)128, "E3dcGui Watchdog");
          snprintf (EmailText, (size_t)512, "Watchdog >>> Kill\nGUI-Time > %i Sek. ", WDdiff);
          sendEmail(EmailAdress, EmailBetreff, EmailText);
        }
        system("pkill GuiMain");
        system("pkill screenSave");
        sleep(2);
        system("/home/pi/E3dcGui/GuiMain &");
        system("/home/pi/E3dcGui/screenSave &");
        snprintf (OUT, (size_t)100, "GuiMain-pkill");
        WriteDataWDcsv(DATE, TIME, AktuallTime, UnixTimeHM, resetCounter, OUT);
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
    }
}

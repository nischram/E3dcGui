#ifndef __PARAMETER_H_
#define __PARAMETER_H_

//#######################################
//### Raspberry 7" Display ###
#define GUI                         1         //Wichtig!    // Wenn die RSCP-Applikation ohne Display genutzt werden soll 0, wenn das Display genutzt wird 1
#define framebuffer_File            "/dev/fb0"              //Wichtig wenn GUI=1!    // Deitei in der die Parameter zum Display gespeichert werden Speicherort abfragen "ls /dev/" meistens "fb0"
#define touch_File                  "/dev/input/event0"     //Wichtig wenn GUI=1!    // Deitei in der die Parameter zur Touchfunktion gespeichert werden Speicherort abfragen "ls /dev/input/" meistens "event0" kann aber variieren wenn Tastatur / Maus angeschschlossen sind. Auslesen der Datei mit: "udevadm info --name=/dev/input/eventX --attribute-walk" X ersetzen mit 0...7 etc.

//#######################################
//### E3DC S10 Einstellungen ###
#define E3DC_S10                    1         //Wichtig!    // Ist ein E3DC Speicher vorhanden dann 1, soll das display nur für Homematic gentzt werden dann 0
#define SleepTime                   1                       // Abfrageintervall für die RSCP-Applikation in Sekunden, wenn "GUI = 1 & E3DC_S10 = 1" ist der Intervall automatisch = 1
// S10 Einstellungen wie IP-Adresse, Benutzername, Passwort und RSCP-Schlüssel
#define SERVER_IP                   "192.168.178.xxx"       // IP-Adresse vom S10
#define E3DC_USER                   "Max.Mustermann"        // Benutzername vom E3DC Portal
#define E3DC_PASS                   "password"              // Passwort vom E3DC Portal
#define AES_PASS                    "1234567890"            // RSCP-Schlüssel wie am Display vom S10 eingestellt
//#######################################
//### Homematic Einstellungen für RSCP-Applikation ###
#define Homematic_E3DC              1         //Wichtig!    // Sollen die Daten zur Homematic gesendet werden? Daten senden 1 eintragen sonst 0
#define HM_IP                       "192.168.178.yyy"       // IP-Adresse von der Homematic
#define HM_Intervall                30                      // Sendeintervall der E3DC Daten zur Homematic und Abfrageintevall für Display
// ISE-ID der diversen Systemvariablen
#define TAG_EMS_ISE_POWER_PV        38872
#define TAG_EMS_ISE_POWER_BAT       38873
#define TAG_EMS_ISE_POWER_HOME      38874
#define TAG_EMS_ISE_POWER_GRID      38947
#define TAG_EMS_ISE_POWER_NET_IN    38875                   // Die Homematic kann keine negativen Werte auswerten somit wird NetIn und NetOut benötigt
#define TAG_EMS_ISE_POWER_NET_OUT   38876
#define TAG_BAT_ISE_SOC             38878
#define TAG_EMS_ISE_UNIXTIME        38946
//Wenn vorhandene Bitte für Additional (Externe-Quelle) und
//Wallbox je ein 1, sonst 0 eintragen.
#define Additional                  0
#define TAG_EMS_ISE_POWER_ADD       38877
#define Wallbox                     0
#define TAG_EMS_ISE_POWER_WB_ALL    38879
#define TAG_EMS_ISE_POWER_WB_SOLAR  38880
//Soll Seriennummer, Autarky und Eigenstrom auch zur Homematic gesendet werden
// Ja = 1, Nien = 0, Wenn Ja bitte ISE_ID eingeben
#define Seriennummer                0
#define Autarky                     0
#define Eigenstrom                  0
#define TAG_EMS_ISE_SERIAL_NUMBER   38881
#define TAG_EMS_ISE_AUTARKY         38882
#define TAG_EMS_ISE_SELFCON         38883
//Wieviele Tracker am Wechselrichter sind belegt, 1 oder 2
#define PVI_TRACKER                 2
//Weclche Werte sollen vom Tracker/ von den Trackern abgefragt werden
#define TRACKER_POWER               1
#define TRACKER_VOLTAGE             1
#define TRACKER_CURRENT             1
//Abfragen der History Values
#define historyAktiv                0                       //History Values Abfragen und Anzeigen aktiv=1 deaktiv=0
#define historyDelay                900                     //Abfrageintervall in Sekunden mindestens 60, Empfehlung 900

//#######################################
// Homematic Parameter
#define Homematic_GUI               1         //Wichtig!    //Wenn das Display für Homematic genutzt werden soll 1 sonst 0
/*
Wenn das Display für HM Daten genutzt wird
aber die ISE_ID noch nicht angepasst sind,
wird es zu langen wartezeiten beim lesen der
Daten (read_HM) kommen. Damit diese Wartezeiten
unterbunden werden, alle ISE_ID au "0" setzen!
Es kann aber zu darstellungsfehler kommen.
*/
// !!! Wichtig >>> Weitere Parameter zur Homematic bitte in der "parameterHM.h" definieren

//#######################################
//### GuiMain Parameter ###
#define ScreenSaverTime             180                     // Zeit bis sich der Bilschirmschoner aktiviert (in Sekunden)
//Einstellungen Langzeitanzeige (15 Minuten Mittelwerte in duchrlaufender 24 Stundenanzeige)
#define PowerMax                    5400                    // maximal zu erwartende Leistung als 15Min Mittelwert für Solarproduktion oder Hausverbrauch
#define skalaLine                   1000                    // Einteilung der Scala

//#######################################
//### Watchdog Einstellungen ###
//Zeitdifferenz zur aktuellen Zeit bis zur Watchdog aktivierung (in Sekunden)
#define WDdiff                      300                     // Minimalwert 240
//Zeitinterval für die Abfragen des Watchdog (in Sekunden)
#define sleepTimeWD                 120
//Anzahl Programm Neustarts bis zum Reboot
#define rebootCounter               4
//Nach dieser Zeit wird der rebootCounter zurückgesetzt wenn die Daten aktuell sind (in Minuten)
#define resetMin                    60
//WD für HM-Gui (nur wenn Unixtime und Timestamp als Variablen in der Homematic angelegt sind und abgefragt werden)
#define WDuseHM_Gui                 0
//Kontrolldatei aktivieren damit häufige Aktivitäten erkannt werden können
#define WDkontrolle                 0                       //1=aktiv / 0=deaktiv Empfehlung: 0=deaktiv
//Email senden bei aktivität
#define WDsendEmailReboot           1                       // 1= senden erlaubt (sendEmail installiert) 0=deaktiv
#define WDsendEmailKill             0                       // 1= senden erlaubt (sendEmail installiert) 0=deaktiv
#define WDtoEmailAdress             "max.mustermann@web.de"                     // Email Adresse für Watchdog aktivitäten
//Ping zum Router füre einen LAN/WLAN test
#define RouterIP                    "192.168.178.1"         // IP-Adresse vom Router
#define PingOn                      1                       // 1= Ping zu Router aktiv
#define resetWLAN                   1                       // 1= Reset für WLAN bei Pingfehler
#define PingWD                      1                       // 1= Reboot nach dem 3. Pingfehler

//#######################################
// sendEmail Parameter
#define FromEmailAdress             "max.mustermann@web.de"                     //eMail-Adresse von der gesendet wird
#define smtpServer                  "smtp.web.de"                               //eMail-Server zum senden
#define smtpPort                    "587"                                       //Port für den smtpServer
#define smtpTLS                     "yes"                                       //TSL-Verschlüsselung "yes" oder "no"
#define smtpBenutzer                "max.mustermann@web.de"                     //Benutzername für den eMail Account
#define smtpPass                    "1234abc"                                   //Passwort für den eMail Account


//##############################################################################
//########### Ab hier die Parameter nicht verändern! ###########################
//##############################################################################

//#######################################
// Systemintern GuiMain
#define ScreenChange                0    //!!
#define ScreenCounter               1    //!!
#define ScreenSaver                 2    //!!
#define ScreenShutdown              3    //!!
#define ScreenState                 4    //!!
#define ScreenHistory               5    //!!
#define ScreenBrightness            6    //!!
#define ScreenMAX                   7    //!!

#define ScreenAktuell               0    //!!
#define ScreenLangzeit              1    //!!
#define ScreenSetup                 2    //!!
#define ScreenMonitor               3    //!!
#define ScreenHM                    4    //!!

#define ShutdownRun                 0    //!!
#define ShutdownSD                  1    //!!
#define ShutdownSRS                 2    //!!
#define ShutdownHRS                 3    //!!
#define ShutdownSDN                 4    //!!
#define ShutdownWD                  5    //!!

#define RscpTime                    1    //!!
#define HomematicTime               2    //!!
#define PiTime                      3    //!!

#define SOC                         0    //!!
#define Solar                       1    //!!
#define Home                        2    //!!
#define NetIn                       3    //!!
#define NetOut                      4    //!!
#define BatIn                       5    //!!
#define BatOut                      6    //!!
#define ADD                         7    //!!
#define LegendeMAX                  8    //!!

#define UnixtimeE3dc                0    //!!
#define UnixtimeHM                  1    //!!
#define UnixtimeGui                 2    //!!
#define UnixtimeMAX                 4    //!!

#define true                        1    //!!
#define false                       0    //!!
#define ScreenOn                    1    //!!
#define ScreenOff                   0    //!!

// Allgemeines für die RSCP-Applikation
#define AES_KEY_SIZE                32    //!!
#define AES_BLOCK_SIZE              32    //!!
#define SERVER_PORT                 5033  //!!

//Beschreibung für ReadRscp
#define PosPVI                      0    //!!
#define PosBat                      1    //!!
#define PosHome                     2    //!!
#define PosGrid                     3    //!!
#define PosSOC                      4    //!!
#define PosBatState                 5    //!!
#define PosAutarky                  6    //!!
#define PosSelfCon                  7    //!!
#define PosADD                      8    //!!
#define PosWbAll                    9    //!!
#define PosWbSolar                  10   //!!
#define PosPVIState                 11   //!!
#define PosPVIDCP1                  12   //!!
#define PosPVIDCP2                  13   //!!
#define PosPVIDCU1                  14   //!!
#define PosPVIDCU2                  15   //!!
#define PosPVIDCI1                  16   //!!
#define PosPVIDCI2                  17   //!!
#define PosPMState                  18   //!!
#define PosPMPhases                 19   //!!
#define PosTimeZone                 20   //!!
#define PosMAX                      21   //!!

#define PosPVI900                   0    //!!
#define PosSOC900                   1    //!!
#define PosHome900                  2    //!!
#define PosNetIn900                 3    //!!
#define PosNetOut900                4    //!!
#define PosBatIn900                 5    //!!
#define PosBatOut900                6    //!!
#define PosAdd900                   7    //!!
#define PosWBAll900                 8    //!!
#define PosWBSolar900               9    //!!
#define PosMAX900                   10   //!!

// Beschreibung für S10history
#define dataPV                      0    //!!
#define dataHome                    1    //!!
#define dataGridIn                  2    //!!
#define dataGridOut                 3    //!!
#define dataBatIn                   4    //!!
#define dataBatOut                  5    //!!
#define dataTime                    6    //!!
#define dataMax                     7    //!!

#define historyOff                  0    //!!
#define today                       1    //!!
#define yesterday                   2    //!!
#define today_path                  "/mnt/RAMDisk/today.txt"         //!!
#define yesterday_path              "/mnt/RAMDisk/yesterday.txt"     //!!

// Touch Parameter
#define BUTTON_ON                   1   //!!
#define BUTTON_OFF                  0   //!!

#define X                           0   //!!
#define Y                           1   //!!
#define W                           2   //!!
#define H                           3   //!!

//#######################################

#endif // __PARAMETER_H_

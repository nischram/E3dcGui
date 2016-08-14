#ifndef __PARAMETER_H_
#define __PARAMETER_H_

//#######################################
//### Raspberry 7" Display ###
#define GUI                         1         //Wichtig!    // Wenn die RSCP-Applikation ohne Display genutzt werden soll 0, wenn das Display genutzt wird 1

//#######################################
//### E3DC S10 Einstellungen ###
#define E3DC_S10                    1         //Wichtig!    // Ist ein E3DC Speicher vorhanden dann 1, soll das display nur für Homematic gentzt werden dann 0
#define SleepTime                   1                       // Abfrageintervall für die RSCP-Applikation
// S10 Einstellungen wie IP-Adresse, Benutzername, Passwort und RSCP-Schlüssel
#define SERVER_IP                   "192.168.178.xxx"       // IP-Adresse vom S10
#define E3DC_USER                   "Max.Mustermann"        // Benutzername vom E3DC Portal
#define E3DC_PASS                   "password"              // Passwort vom E3DC Portal
#define AES_PASS                    "1234567890"            // RSCP-Schlüssel wie am Display vom S10 eingestellt
//#######################################
//### Homematic Einstellungen für RSCP-Applikation ###
#define Homematic_E3DC              1         //Wichtig!    // Sollen die Daten zur Homematic gesendet werden? Daten senden 1 eintragen sonst 0
#define HM_IP                       "192.168.178.yyy"       // IP-Adresse von der Homematic
#define HM_Intervall                15                      // Sendeintervall der E3DC Daten zur Homematic und Abfrageintevall für Display
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
// Allgemeines für die RSCP-Applikation
// !!!!! nicht verändern !!!!!
#define AES_KEY_SIZE                32    //!!
#define AES_BLOCK_SIZE              32    //!!
#define SERVER_PORT                 5033  //!!

//#######################################
//### GuiMain Parameter ###
#define ScreenSaverTime             180                     // Zeit bis sich der Bilschirmschoner aktiviert (in Sekunden)
//Einstellungen Langzeitanzeige (15 Minuten Mittelwerte in duchrlaufender 24 Stundenanzeige)
#define PowerMax                    5400                    // maximal zu erwartende Leistung als 15Min Mittelwert für Solarproduktion oder Hausverbrauch
#define skalaLine                   1000                    // Einteilung der Scala
//Homematic Position der Spalten für die Elemente auf dem Display
#define  S0                         40
#define  S1                         120
#define  S2                         200
#define  S3                         280
#define  S4                         360
#define  S5                         450
#define  S6                         530
#define  S7                         610
#define  S8                         690
//Homematic Position der Reihen für die Elemente auf dem Display
#define  R1                         120
#define  R2                         200
#define  R3                         280
#define  R4                         360
//Homematic Fensterbreite /halbe Breite
#define  Fw                         50
#define  Fhw                        25
// !!! nicht verändern !!!
#define ScreenAktuell               1    //!!
#define ScreenLangzeit              2    //!!
#define ScreenSetup                 3    //!!
#define ScreenShutdown              4    //!!
#define ScreenMonitor               11   //!!
#define ScreenHM                    12   //!!

#define ShutdownRun                 5    //!!
#define ShutdownSD                  6    //!!
#define ShutdownSRS                 7    //!!
#define ShutdownHRS                 8    //!!
#define ShutdownSDN                 9    //!!
#define ShutdownWD                  10   //!!

#define RscpTime                    1    //!!
#define HomematicTime               2    //!!
#define PiTime                      3    //!!

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

//#######################################
// Homematic Parameter
#define Homematic_GUI               0         //Wichtig!    //Wenn das Display für Homematic genutzt werden soll 1 sonst 0
//http://192.168.178.yyy/addons/hq/ui/#tabVariables         //Link um mit de HQ WebUi die Variable auszulesen
//Allgemein
#define ISE_UnixTime                43674  //10 Zeichen
#define ISE_TimestampHM             43756  //16 Zeichen
#define ISE_HMCPU                   43784  //Alle ohne Info mit 4 Zeichen
#define ISE_HMupTage                44564
#define ISE_HMupStd                 44565
#define ISE_AussenTemp              2122
#define ISE_AussenHum               2121   //2 Zeichen
#define ISE_AussenRegen             23898  //1 Zeichen
#define ISE_VerschlussGe            9892
#define ISE_VerschlussEG            9889
#define ISE_VerschlussOG            9888
#define ISE_VerschlussDG            9787
#define ISE_VerschlussGa            9891
#define ISE_VerschlussKel           9890
#define ISE_Kellertuer              27971
#define ISE_Heizung                 7771
#define ISE_HeizungState            27671
#define ISE_pumpensumpf             3958
#define ISE_Markise                 8372
#define ISE_Brunnen                 16019
#define ISE_MacSchrank              44826
#define ISE_MacSchrankValue         45187
#define ISE_Sonnenaufgang           45308  //5 Zeichen
#define ISE_Sonnenuntergang         45309  //5 Zeichen
#define ISE_Vollmond                17683  //10 Zeichen
//Erdgeschoss
#define ISE_EGWTemp                 7932
#define ISE_EGKFli                  10236
#define ISE_EGKFre                  20883
#define ISE_EGWFli                  12948
#define ISE_EGWFre                  12893
#define ISE_EGSSt                   8449
#define ISE_EGWCF                   20938
#define ISE_EGKJalli                8316
#define ISE_EGKJalre                8344
#define ISE_EGWJal                  8288
#define ISE_EGSJal                  20111
#define ISE_EGWCJal                 20301
//Obergeschoss
#define ISE_OGParty                 26807
#define ISE_Send_OGPARTY            26807
#define ISE_OGKTemp                 12981
#define ISE_OGWTemp                 1277
#define ISE_OGBTemp                 13042
#define ISE_OGKSpul                 43938
#define ISE_Send_OGKSpul            43961
#define ISE_OGKFli                  22114
#define ISE_OGKFre                  22169
#define ISE_OGWSt                   22216
#define ISE_OGWBt                   36512
#define ISE_OGBF                    20828
#define ISE_OGKJalli                20771
#define ISE_OGKJalre                20735
#define ISE_OGWJalSt                21659
#define ISE_OGWJalBt                21695
#define ISE_OGBJal                  20179
#define ISE_OGEJal                  23163
//Dachgeschoss
#define ISE_DGSTemp                 2166
#define ISE_DGSFli                  1946
#define ISE_DGSFre                  1946
#define ISE_DGTF                    36567
//Garage
#define ISE_GarageTemp              19538
#define ISE_Garagentor              27968
#define ISE_Garagentuerh            27970
#define ISE_Garagentuerv            27969

//#######################################
// Touch Parameter
// !!! nicht verändern !!!
#define BUTTON_ON                   1   //!!
#define BUTTON_OFF                  0   //!!

#define X                           0   //!!
#define Y                           1   //!!
#define W                           2   //!!
#define H                           3   //!!


#endif // __PARAMETER_H_

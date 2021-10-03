#ifndef __DEFINE_H_
#define __DEFINE_H_

//#######################################
// Systemintern GuiMain
#define ScreenChange                0    //!!
#define ScreenCounter               1    //!!
#define ScreenSaver                 2    //!!
#define ScreenShutdown              3    //!!
#define ScreenState                 4    //!!
#define ScreenHistory               5    //!!
#define ScreenMAX                   6    //!!

#define ScreenAktuell               0    //!!
#define ScreenLangzeit              1    //!!
#define ScreenSetup                 2    //!!
#define ScreenMonitor               3    //!!
#define ScreenHM                    4    //!!
#define ScreenWetter                5    //!!
#define ScreenMuell                 6    //!!
#define ScreenSmart                 7    //!!
#define ScreenGB                    8    //!!
#define ScreenWallbox               9    //!!
#define ScreenFunktion              10   //!!

#define ShutdownRun                 0    //!!
#define ShutdownSD                  1    //!!
#define ShutdownSRS                 2    //!!
#define ShutdownHRS                 3    //!!
#define ShutdownSDN                 4    //!!
#define ShutdownWD                  5    //!!
#define ShutdownSStop               6    //!!

#define RscpTime                    1    //!!
#define HomematicTime               2    //!!
#define PiTime                      3    //!!
#define WeatherTime                 4    //!!
#define SetupTime                   5    //!!
#define GruenTime                   6    //!!

#define SOC                         0    //!!
#define Solar                       1    //!!
#define Home                        2    //!!
#define NetIn                       3    //!!
#define NetOut                      4    //!!
#define BatIn                       5    //!!
#define BatOut                      6    //!!
#define ADD                         7    //!!
#define WbSolar                     8    //!!
#define WbAll                       9    //!!
#define LegendeMAX                  10   //!!

#define LF01                        258  //!!
#define LF02                        311  //!!
#define LF03                        364  //!!
#define LF04                        417  //!!
#define LF05                        470  //!!
#define LF06                        523  //!!
#define LF07                        576  //!!
#define LF08                        629  //!!
#define LF09                        682  //!!
#define LF10                        735  //!!

#define UnixtimeE3dc                0    //!!
#define UnixtimeHM                  1    //!!
#define UnixtimeGui                 2    //!!
#define UnixtimeMAX                 4    //!!

#define true                        1    //!!
#define false                       0    //!!
#define ScreenOn                    1    //!!
#define ScreenOff                   0    //!!
#define NO                          0    //!!
#define WATT                        1    //!!
#define WATTH                       2    //!!
#define PERCENT                     3    //!!
#define AMP                         4    //!!
#define VOLT                        5    //!!
#define DOT                         6    //!!

#define PictureLine1                16   //!!
#define PictureW                    75   //!!
#define PictureH                    75   //!!

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
#define PosEpReserv                 21   //!!
#define PosEpReservW                22   //!!
#define PosEpReservMaxW             23   //!!
#define PosInstalledPeak            24   //!!
#define PosDerateAtPercent          25   //!!
#define PosDerateAtPower            26   //!!
#define PosEmsState                 27   //!!
#define PosEpState                  28   //!!
#define PosBlUsed                   29   //!!
#define PosBlCharge                 30   //!!
#define PosBlDischarge              31   //!!
#define PosBlStart                  32   //!!
#define PosPowerSave                33   //!!
#define PosWeatherReg               34   //!!
#define PosMAX                      35   //!!

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

#define PosWbState                  0    //!!
#define PosWbActPhases              1    //!!
#define PosWbMode                   2    //!!
#define PosWbReady                  3    //!!
#define PosWbCharge                 4    //!!
#define PosWbLocked                 5    //!!
#define PosWbConnect                6    //!!
#define PosWbSOn                    7    //!!
#define PosWbSConnect               8    //!!
#define PosWbSLocked                9     //!!
#define PosWbLED_ERR                10    //!!
#define PosWbLED_SON                11    //!!
#define PosWbLED_BAT                12    //!!
#define PosWbSRelais                13    //!!
#define PosWbRelais16               14    //!!
#define PosWbRelais32               15    //!!
#define PosWbUsePhases              16    //!!
#define PosWbCurrent                17    //!!
#define PosWbDebug                  18    //!!
#define PosWbCheckSum               19    //!!
#define PosWbBtC                    20    //!!
#define PosWbBbC                    21    //!!
#define PosWbMAX                    22    //!!

// Berschreibung Übergaben im Programm
#define PosToEpSet                  0    //!!
#define PosToEpMax                  1    //!!
#define PosToBlUsed                 2   //!!
#define PosToBlCharge               3   //!!
#define PosToBlDischarge            4   //!!
#define PosToBlStart                5   //!!
#define PosToPowerSave              6   //!!
#define PosToWeatherReg             7   //!!
#define PosToMAX                    8    //!!

// Berschreibung Grünbeck
#define PosGB_CONSUMPTION           0    //!!
#define PosGB_CURRENT_REGENERATION  1    //!!
#define PosGB_PERCENT_REGENERATION  2    //!!
#define PosGB_CURRENT_FLOW          3    //!!
#define PosGB_R_CAPACITY            4    //!!
#define PosGB_LAST_REGENERATION     5    //!!
#define PosGB_STATUS_EMAIL          6    //!!
#define PosGB_Max                   7    //!!

#define PosSaveMonth                0    //!!
#define PosSaveYear                 1    //!!
#define PosSaveAll                  2    //!!
#define PosSaveMAX                  3    //!!

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

//Wetter
#define WetterS1                    55  //!!
#define WetterS2                    295 //!!
#define WetterS3                    540 //!!
#define WetterZ1                    100 //!!
#define WetterZ2                    125 //!!
#define WetterZ3                    150 //!!
#define WetterZ4                    175 //!!
#define WetterZ5                    200 //!!
#define WetterZ6                    225 //!!
#define WetterZ7                    250 //!!
#define WetterUZ1                   300 //!!
#define WetterUZ2                   325 //!!
#define WetterUZ3                   350 //!!
#define WetterUZ4                   375 //!!
#define WetterUZ5                   400 //!!
#define WetterUZ6                   425 //!!

//Müll
#define MuellS0                     65  //!!
#define MuellS1                     150 //!!
#define MuellS2                     240 //!!
#define MuellS3                     330 //!!
#define MuellS4                     420 //!!
#define MuellS5                     510 //!!
#define MuellS6                     600 //!!
#define MuellS7                     690 //!!
#define MuellZ0                     100 //!!
#define MuellZ1                     130 //!!
#define MuellZ2                     230 //!!
#define MuellZ3                     340 //!!

//Screen Funktion
#define BL                          50  //!!
#define BLR1                        168          //!!
#define BLR2                        BLR1 + 36    //!!
#define BLR3                        BLR2 + 36    //!!
#define BLR4                        BLR3 + 36    //!!
#define BLR5                        BLR4 + 36    //!!
#define BLR6                        BLR5 + 36    //!!
#define BLR7                        BLR6 + 36    //!!
#define BLS2                        BL + 100     //!!
#define BLS1                        BLS2 - 80    //!!
#define BLS3                        BLS2 + 80    //!!

#define EP                          420 //!!
#define EPPERX                      EP + 120     //!!
#define EPPERY                      160          //!!
#define EPWX                        EPPERX       //!!
#define EPWY                        EPPERY + 25  //!!
#define EPWMAXX                     EPPERX       //!!
#define EPWMAXY                     EPWY + 25    //!!
#define EPSWX                       EPPERX       //!!
#define EPSWY                       EPWMAXY + 30 //!!
#define EPSETX                      EPPERX       //!!
#define EPSETY                      EPSWY + 36   //!!
#define EPSETS1                     EPPERX - 90  //!!
#define EPSETS2                     EPPERX       //!!
#define EPSETS3                     EPPERX + 90  //!!
#define EPSETR1                     EPSETY + 36  //!!
#define EPSETR2                     EPSETR1 + 36 //!!
#define EPSETR3                     EPSETR2 + 36 //!!

//Screen Monitor
#define T1                          50  //!!
#define T2                          225 //!!
#define SI                          420 //!!
#define SIR1                        168          //!!
#define SIR2                        SIR1 + 36    //!!
#define SIR3                        SIR2 + 36    //!!
#define SIR4                        SIR3 + 36    //!!
#define SIR5                        SIR4 + 36    //!!
#define SIR6                        SIR5 + 36    //!!
#define SIR7                        SIR6 + 36    //!!
#define SIS1                        SI + 10      //!!
#define SIS2                        SI + 120     //!!
#define SIS3                        SI + 200     //!!

//Screen Wallbox
#define WB1                         30  //!!
#define WBCARX                      375 //!!
#define WBCARY                      199 //!!
#define WBCURX                      228 //!!
#define WBCURY                      160 //!!
#define WBMODEX                     WBCURX       //!!
#define WBMODEY                     WBCURY  + 55 //!!
#define WBBTCX                      WBCURX       //!!
#define WBBTCY                      WBMODEY + 55 //!!
#define WBBBCX                      WBCURX       //!!
#define WBBBCY                      WBBTCY  + 55 //!!
#define WBPHX                       WBCURX       //!!
#define WBPHY                       WBBBCY  + 55 //!!
#define WBSTOPX                     655          //!!
#define WBSTOPY                     WBCURY       //!!

//wiringPi
#define OK                          1   //!!
#define ERROR                       -1  //!!
#define Bewegung                    1   //!!
#define Aktiv                       1   //!!
#define Deaktiv                     0   //!!

//Schaltaktoren
#define Aktor1Position              0    //!!
#define Aktor2Position              5    //!!
#define Aktor3Position              10   //!!
#define Aktor4Position              15   //!!
#define Aktor5Position              20   //!!
#define AktorPrioPosition           25   //!!
#define AktorMAX                    26   //!!

#define counterOn                   0    //!!
#define counterOff                  1    //!!
#define MinOn                       2    //!!
#define MinOff                      3    //!!
#define stateTimer                  4    //!!

//DHT11
#define DHT1Position                0    //!!
#define DHT2Position                3    //!!
#define DHT3Position                6    //!!
#define DHT4Position                9    //!!
#define DHT5Position                12   //!!
#define DHT11MAX                    15   //!!

//Spalten
#define  SP0                        40   //!!
#define  SP1                        120  //!!
#define  SP2                        200  //!!
#define  SP3                        280  //!!
#define  SP4                        360  //!!
#define  SP5                        450  //!!
#define  SP6                        530  //!!
#define  SP7                        610  //!!
#define  SP8                        690  //!!
//Reihen
#define  RP1                        110  //!!
#define  RP2                        182  //!!
#define  RP3                        254  //!!
#define  RP4                        326  //!!
#define  RP5                        398  //!!
//Größe
#define  FPw                        50   //!!
#define  FPhw                       25   //!!

//#######################################

#endif // __DEFINE_H_

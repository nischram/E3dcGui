#ifndef __PARAMETERHM_H_
#define __PARAMETERHM_H_
/*
Wenn das Display für HM Daten genutzt wird
aber die ISE_ID noch nicht angepasst sind,
wird es zu langen wartezeiten beim lesen der
Daten (read_HM) kommen. Damit diese Wartezeiten
unterbunden werden, alle ISE_ID au "0" setzen!
Es kann aber zu darstellungsfehler kommen.
*/
// Weitere Parameter zur Homematic bitte in der HMparameter.h definieren
#define HM_Sonderdarstellung        0                       //z.B. für Sonnenaufgang etc.

//http://192.168.178.yyy/addons/hq/ui/#tabVariables         //Link um mit de HQ WebUi die Variable auszulesen
//Definition der ISE_ID von der Variablen und Geräten der Homematic
/* Beispiel:
#define ISE_UnixTime                43674
*/
#define ISE_UnixTime                0  //10 Zeichen
#define ISE_TimestampHM             0  //16 Zeichen
#define ISE_HMCPU                   0  //Alle ohne Info mit 4 Zeichen
#define ISE_HMupTage                0
#define ISE_HMupStd                 0
#define ISE_AussenTemp              0
#define ISE_AussenHum               0  //2 Zeichen
#define ISE_AussenRegen             0  //1 Zeichen
#define ISE_VerschlussGe            0
#define ISE_VerschlussEG            0
#define ISE_VerschlussOG            0
#define ISE_VerschlussDG            0
#define ISE_VerschlussGa            0
#define ISE_VerschlussKel           0
#define ISE_Kellertuer              0
#define ISE_Heizung                 0
#define ISE_HeizungState            0
#define ISE_pumpensumpf             0
#define ISE_Markise                 0
#define ISE_Brunnen                 0
#define ISE_MacSchrank              0
#define ISE_MacSchrankValue         0
#define ISE_Sonnenaufgang           0  //5 Zeichen
#define ISE_Sonnenuntergang         0  //5 Zeichen
#define ISE_Vollmond                0  //10 Zeichen
//Erdgeschoss
#define ISE_EGWTemp                 0
#define ISE_EGKFli                  0
#define ISE_EGKFre                  0
#define ISE_EGWFli                  0
#define ISE_EGWFre                  0
#define ISE_EGSSt                   0
#define ISE_EGWCF                   0
#define ISE_EGKJalli                0
#define ISE_EGKJalre                0
#define ISE_EGWJal                  0
#define ISE_EGSJal                  0
#define ISE_EGWCJal                 0
//Obergeschoss
#define ISE_OGParty                 0
#define ISE_Send_OGPARTY            0
#define ISE_OGKTemp                 0
#define ISE_OGWTemp                 0
#define ISE_OGBTemp                 0
#define ISE_OGKSpul                 0
#define ISE_Send_OGKSpul            0
#define ISE_OGKFli                  0
#define ISE_OGKFre                  0
#define ISE_OGWSt                   0
#define ISE_OGWBt                   0
#define ISE_OGBF                    0
#define ISE_OGKJalli                0
#define ISE_OGKJalre                0
#define ISE_OGWJalSt                0
#define ISE_OGWJalBt                0
#define ISE_OGBJal                  0
#define ISE_OGEJal                  0
//Dachgeschoss
#define ISE_DGSTemp                 0
#define ISE_DGSFli                  0
#define ISE_DGSFre                  0
#define ISE_DGTF                    0
//Garage
#define ISE_GarageTemp              0
#define ISE_Garagentor              0
#define ISE_Garagentuerh            0
#define ISE_Garagentuerv            0
#define ISE_OGWLicht                0
//Ende ISE_ID's

//Homematic Position der Spalten für die HM-Elemente auf dem Display
#define  S0                         40
#define  S1                         120
#define  S2                         200
#define  S3                         280
#define  S4                         360
#define  S5                         450
#define  S6                         530
#define  S7                         610
#define  S8                         690
//Homematic Position der Reihen für die HM-Elemente auf dem Display
#define  R1                         110
#define  R2                         180
#define  R3                         250
#define  R4                         320
#define  R5                         390
//Homematic Fensterbreite /halbe Breite
#define  Fw                         50
#define  Fhw                        25

//#######################################
#endif // __PARAMETERHM_H_

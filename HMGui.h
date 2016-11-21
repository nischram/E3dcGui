#ifndef __HMGUI_H_
#define __HMGUI_H_

//Homematic Grafik erstellen
make_HM_Gui(int GuiTime, int counter)
{
	//Difiniton der Variablen die auf dem Display angezeigt werden sollen.
	char VerschlussEG[20], VerschlussOG[20], VerschlussDG[20], VerschlussGa[20], VerschlussKel[20];
	char DGSFli[20], DGSFre[20], DGTF[20], DGSTemp[20];
	char OGKFli[20], OGKFre[20], OGWSt[20], OGWBt[20], OGBF[20], OGKTemp[20], OGWTemp[20], OGBTemp[20], OGKSpul[20], OGParty[20];
	char OGKJalli[20], OGKJalre[20], OGWJalSt[20], OGWJalBt[20], OGBJal[20], OGEJal[20];
	char EGKFli[20], EGKFre[20], EGWFli[20], EGWFre[20], EGSSt[20], EGWCF[20], EGWTemp[20];
	char Markise[20], EGKJalli[20], EGKJalre[20], EGWJal[20], EGSJal[20], EGWCJal[20];
	char Kellertuer[20], Heizung[20], pumpensumpf[20], HeizungState[20];
	char Garagentor[20], Garagentuerh[20], Garagentuerv[20], GarageTemp[20];
	char AussenTemp[20], AussenHum[20], AussenRegen[20], Brunnen[20],MacSchrankValue[20];
	char Party[20], Sumpf[20], Heiz[20], Spuel[20], HMCPU[20], HMupTage[20];
	char Sonnenaufgang[20], Sonnenuntergang[20], Vollmond[20];
	char OUT [100],Value[20],writeTxt[20],TimestampHM[20];
	int readOK = 0;
	//Zähler für die Aktualisierung des Displays
	int HMcounter = HMcounter +1;

	char PathScreen [128];
	snprintf (PathScreen, (size_t)128, "/mnt/RAMDisk/Screen.txt");
	int screenState = BitRead(PathScreen, ScreenState);

	//Zeit Darstellung im Display
	GuiTime = HomematicTime;

	//Daten von der HM lesen
	if(counter == 0 || (screenState == ScreenOn && HMcounter == HM_Intervall)){
		int Unixtime[4];
		char PathUnixtime [128];
		snprintf (PathUnixtime, (size_t)128, "/mnt/RAMDisk/Unixtime.txt");
		//Read Daten Beginn (roter Punkt unten rechts)
		drawSquare(760,440,20,20,LIGHT_RED);
		drawCorner(760,440,20,20,WHITE);
		//WatchdogHM
		read_HM(ISE_UnixTime, 10, Value);																		//Auslesen HM Variable für die Unixtime (muss in der HM angelegt sein und mit einem Programm permanent aktuallisiert werden)
		int UnixTime = atoi(Value);
		read_HM(ISE_TimestampHM, 16,TimestampHM);														//Auslesen HM Variable für die den Zeitstempel (muss in der HM angelegt sein und mit einem Programm permanent aktuallisiert werden)
		BitWrite(PathUnixtime, UnixtimeHM, UnixTime);						      			//Schreiben der Unixtime in Datei für den Watchdog
		//Verschluss (Variablen müssen per Programm in der HM bei aktivitäten an Fenstern etc. aktualisiert werden)
		read_HM(ISE_VerschlussDG, 4, VerschlussDG);
		read_HM(ISE_VerschlussOG, 4, VerschlussOG);
		read_HM(ISE_VerschlussEG, 4, VerschlussEG);
		read_HM(ISE_VerschlussKel, 4, VerschlussKel);
		read_HM(ISE_VerschlussGa, 4, VerschlussGa);
		//DG (Geräte)
		read_HM(ISE_DGSFli, 4, DGSFli);
		read_HM(ISE_DGSFre, 4, DGSFre);
		read_HM(ISE_DGSTemp, 4, DGSTemp);
		read_HM(ISE_DGTF, 4, DGTF);
		//OG (Geräte)
		read_HM(ISE_OGWSt, 4, OGWSt);                                       //Fenster oder Türkontakte abfragen
		readJalou_HM(ISE_OGWJalSt, OGWJalSt);																//Jalousie Werte abfrage
		read_HM(ISE_OGWBt, 4, OGWBt);
		readJalou_HM(ISE_OGWJalBt, OGWJalBt);
		read_HM(ISE_OGWTemp, 4, OGWTemp);
		read_HM(ISE_OGKFli, 4, OGKFli);
		readJalou_HM(ISE_OGKJalli, OGKJalli);
		read_HM(ISE_OGKFre, 4, OGKFre);
		readJalou_HM(ISE_OGKJalre, OGKJalre);
		read_HM(ISE_OGKTemp, 4, OGKTemp);
		read_HM(ISE_OGBF, 4, OGBF);
		readJalou_HM(ISE_OGBJal, OGBJal);
		read_HM(ISE_OGBTemp, 4, OGBTemp);
		readJalou_HM(ISE_OGEJal, OGEJal);
		read_HM(ISE_OGKSpul, 4, OGKSpul);
		read_HM(ISE_OGParty, 4, OGParty);
		read_HM(ISE_MacSchrankValue, 4, MacSchrankValue);
		//EG
		read_HM(ISE_EGWFli, 4, EGWFli);
		read_HM(ISE_EGWFre, 4, EGWFre);
		readJalou_HM(ISE_EGWJal, EGWJal);
		read_HM(ISE_EGWTemp, 4, EGWTemp);
		read_HM(ISE_EGKFli, 4, EGKFli);
		read_HM(ISE_EGKFre, 4, EGKFre);
		readJalou_HM(ISE_EGKJalli, EGKJalli);
		readJalou_HM(ISE_EGKJalre, EGKJalre);
		read_HM(ISE_EGSSt, 4, EGSSt);
		readJalou_HM(ISE_EGSJal, EGSJal);
		read_HM(ISE_EGWCF, 4, EGWCF);
		readJalou_HM(ISE_EGWCJal, EGWCJal);
		//Keller
		read_HM(ISE_Kellertuer, 4, Kellertuer);
		read_HM(ISE_Heizung, 4, Heizung);
		read_HM(ISE_HeizungState, 4, HeizungState);
		read_HM(ISE_pumpensumpf, 4, pumpensumpf);
		//Garage
		read_HM(ISE_GarageTemp, 4, GarageTemp);
		read_HM(ISE_Garagentor, 4, Garagentor);
		read_HM(ISE_Garagentuerv, 4, Garagentuerv);
		read_HM(ISE_Garagentuerh, 4, Garagentuerh);
		//Aussen
		read_HM(ISE_AussenTemp, 4, AussenTemp);
		readJalou_HM(ISE_Markise, Markise);
		read_HM(ISE_AussenHum, 2, AussenHum);
		read_HM(ISE_AussenRegen, 1, AussenRegen);
		read_HM(ISE_Brunnen, 4, Brunnen);
		//Homematic
		read_HM(ISE_HMCPU, 4, HMCPU);
		read_HM(ISE_HMupTage, 8, HMupTage);
		//Read Daten Ende (grüner Punkt unten rechts)
		drawSquare(760,440,20,20,GREEN);
		drawCorner(760,440,20,20,WHITE);
		HMcounter = 0;
		readOK = 1;
	}
	//Hintergrund-Grafik für HM erstellen
	if(counter == 0){
		drawSquare(2,2,800,480,LTGREY);
		drawCorner(2, 2, 800, 480, BLACK);
		drawSquare(12,12,778,458,WHITE);
		drawCorner(12, 12, 778, 458, LTGREY);
		DrawImage("EinstImage", 180, 12);
		if(E3DC_S10 ==1){
			DrawImage("AktuellImage", 270, 12);
			DrawImage("LangzeitImage", 360, 12);
		}
		DrawImage("MonitorImage", 450, 12);
		DrawImage("HMImage", 540, 12);
		// Grafik für Dachgeschoss
		drawSquare(S0,R1-20,400,60,GREY);
		drawCorner(S0,R1-20,400,60,WHITE);
		if (strcmp ("true",VerschlussDG) == 0){
			drawSquare(S0+60,R1-17,337,54,LIGHT_RED);
			drawSquare(S0+63,R1-14,331,48,WHITE);
			drawCorner(S0+63,R1-14,331,48,LIGHT_RED);
		}
		else if (strcmp ("fals",VerschlussDG) == 0){
			drawSquare(S0+60,R1-17,337,54,GREEN);
			drawSquare(S0+63,R1-14,331,48,WHITE);
			drawCorner(S0+63,R1-14,331,48,GREEN);
		}
		drawCorner(S0+60,R1-17,337,54,GREY);
		put_string(S0+6, R1+4, "DG", WHITE);
		// Grafik für Obergeschoss
		drawSquare(S0,R2-20,730,60,GREY);
		drawCorner(S0,R2-20,730,60,WHITE);
		if (strcmp ("true",VerschlussOG) == 0){
			drawSquare(S0+60,R2-17,667,54,LIGHT_RED);
			drawSquare(S0+63,R2-14,661,48,WHITE);
			drawCorner(S0+63,R2-14,661,48,LIGHT_RED);
		}
		else if (strcmp ("fals",VerschlussOG) == 0){
			drawSquare(S0+60,R2-17,667,54,GREEN);
			drawSquare(S0+63,R2-14,661,48,WHITE);
			drawCorner(S0+63,R2-14,661,48,GREEN);
		}
		drawCorner(S0+60,R2-17,667,54,GREY);
		put_string(S0+6, R2+4, "OG", WHITE);
		// Grafik für Erdgeschoss
		drawSquare(S0,R3-20,400,60,GREY);
		drawCorner(S0,R3-20,400,60,WHITE);
		if (strcmp ("true",VerschlussEG) == 0){
			drawSquare(S0+60,R3-17,337,54,LIGHT_RED);
			drawSquare(S0+63,R3-14,331,48,WHITE);
			drawCorner(S0+63,R3-14,331,48,LIGHT_RED);
		}
		else if (strcmp ("fals",VerschlussEG) == 0){
			drawSquare(S0+60,R3-17,337,54,GREEN);
			drawSquare(S0+63,R3-14,331,48,WHITE);
			drawCorner(S0+63,R3-14,331,48,GREEN);
		}
		drawCorner(S0+60,R3-17,337,54,GREY);
		put_string(S0+6, R3+4, "EG", WHITE);
		// Grafik für Keller
		drawSquare(S0,R4-20,400,60,GREY);
		drawCorner(S0,R4-20,400,60,WHITE);
		if (strcmp ("true",VerschlussKel) == 0){
			drawSquare(S0+60,R4-17,337,54,LIGHT_RED);
			drawSquare(S0+63,R4-14,331,48,WHITE);
			drawCorner(S0+63,R4-14,331,48,LIGHT_RED);
		}

		else if (strcmp ("fals",VerschlussKel) == 0){
			drawSquare(S0+60,R4-17,337,54,GREEN);
			drawSquare(S0+63,R4-14,331,48,WHITE);
			drawCorner(S0+63,R4-14,331,48,GREEN);
		}
		drawCorner(S0+60,R4-17,337,54,GREY);
		put_string(S0+6, R4+4, "Keller", WHITE);
		// Grafik für Aussen
		drawSquare(S5,R1-20,320,60,GREY);
		drawCorner(S5,R1-20,320,60,WHITE);
		drawSquare(S5+60,R1-17,257,54,WHITE);
		drawCorner(S5+60,R1-17,257,54,GREY);
		put_string(S5+6, R1+4, "HM", WHITE);
		// Grafik für Garage
		drawSquare(S5,R3-20,320,60,GREY);
		drawCorner(S5,R3-20,320,60,WHITE);
		if (strcmp ("true",VerschlussGa) == 0){
			drawSquare(S5+60,R3-17,257,54,LIGHT_RED);
			drawSquare(S5+63,R3-14,251,48,WHITE);
			drawCorner(S5+63,R3-14,251,48,LIGHT_RED);
		}
		else if (strcmp ("fals",VerschlussGa) == 0){
			drawSquare(S5+60,R3-17,257,54,GREEN);
			drawSquare(S5+63,R3-14,251,48,WHITE);
			drawCorner(S5+63,R3-14,251,48,GREEN);
		}
		drawCorner(S5+60,R3-17,257,54,GREY);
		put_string(S5+6, R3+4, "Garage", WHITE);
		// Grafik für Aussen
		drawSquare(S5,R4-20,320,60,GREY);
		drawCorner(S5,R4-20,320,60,WHITE);
		drawSquare(S5+60,R4-17,257,54,WHITE);
		drawCorner(S5+60,R4-17,257,54,GREY);
		put_string(S5+6, R4+4, "Aussen", WHITE);

		//Aktualisierungszähler hochsetzen auf 60 Sekunden
		BitWrite(PathScreen, ScreenCounter, 60);
	}
	//Daten Grafik erstellen
	if(readOK == 1){
		readOK = 0;
		//Read Daten neu schreiben
		drawSquare(760,440,20,20,GREEN);
		drawCorner(760,440,20,20,WHITE);

		//createWindow = Grafik für Fenster erstellen offen=rot, geschlossen=grün, nicht vorhanden=grau
		//createData = Daten unter die Fenstergrafik für z.B. Temperatur
		//createJalousie = Jalousie über die Fenstergrafik legen
		//S1 = Spalte 1; R1 = Reihe 1; Fhw = halbes Fenster/Jalousie S ud R positioenen sind in der
		//parameter.h definiert
		//Teilweise mit Fhw nur die halbe Fensterbreite für zwei Fenster in einem Raum
		//DG
		createWindow(S1, R1, Fhw, "Schlaf", DGSFli);
		createWindow(S1+Fhw, R1, Fhw, "", DGSFre);
		createData(S1, R1, DGSTemp);
		createWindow(S2, R1, Fw, "Treppe", DGTF);
		//OG
		createWindow(S1, R2, Fhw, "Wohnz.", OGWSt);
		createJalousie(S1, R2, Fhw, OGWJalSt);
		createWindow((S1+Fhw), R2, Fhw, "", OGWBt);
		createJalousie((S1+Fhw), R2, Fhw, OGWJalBt);
		createData(S1, R2, OGWTemp);
		createWindow(S2, R2, Fhw, "Kueche", OGKFli);
		createJalousie(S2, R2, Fhw, OGKJalli);
		createWindow((S2+Fhw), R2, Fhw, "", OGKFre);
		createJalousie((S2+Fhw), R2, Fhw, OGKJalre);
		createData(S2, R2, OGKTemp);
		createWindow(S3, R2, Fw, " Bad", OGBF);
		createJalousie(S3, R2, Fw, OGBJal);
		createData(S3, R2, OGBTemp);
		createWindow(S4, R2, Fw, "Eisenb.", "");
		createJalousie(S4, R2, Fw, OGEJal);
		if (strcmp ("fals",OGKSpul) == 0){
			snprintf (Spuel, (size_t)5, "-");
		}
		else if (strcmp ("true",OGKSpul) == 0){
			snprintf (Spuel, (size_t)5, "fals");
		}
		createWindow(S6, R2, Fw, "OG-Spuel", Spuel);
		if (strcmp ("fals",OGParty) == 0){
			snprintf (Party, (size_t)5, "-");
		}
		else if (strcmp ("true",OGParty) == 0){
			snprintf (Party, (size_t)5, "fals");
		}
		createWindow(S7, R2, Fw, "OG-Party", Party);
		createWindow(S8, R2, Fw, "OG-Mac", MacSchrankValue);
		createWindow(S5, R2, Fw, "Jal.50%", "");
		//EG
		createWindow(S1, R3, Fhw, "Wohnz.", EGWFli);
		createWindow((S1+Fhw), R3, Fhw, "", EGWFre);
		createJalousie(S1, R3, Fw, EGWJal);
		createData(S1, R3, EGWTemp);
		createWindow(S2, R3, Fhw, "Kueche", EGKFli);
		createJalousie(S2, R3, Fhw, EGKJalli);
		createWindow((S2+Fhw), R3, Fhw, "", EGKFre);
		createJalousie((S2+Fhw), R3, Fhw, EGKJalre);
		createWindow(S3, R3, Fw, "Schlaf", EGSSt);
		createJalousie(S3, R3, Fw, EGSJal);
		createWindow(S4, R3, Fw, "  WC", EGWCF);
		createJalousie(S4, R3, Fw, EGWCJal);
		//Keller
		createWindow(S1, R4, Fw, " Tuer", Kellertuer);
		if (strcmp ("fals",Heizung) == 0){																	//mit der IF Funktin wird ggf. die Ausgabe geändert fals
			snprintf (Heiz, (size_t)5, "true");																//ein Gerät nicht entsprechend für rot und grün "true" oder "false" ausgibt
		}
		else if (strcmp ("true",Heizung) == 0){															//Achtung da ich immer nur vier Zeichen einlese und auswerte muss der
			snprintf (Heiz, (size_t)5, "fals");																//neue Wert nicht auf "false" sondern nur auf "fals" gesetzt werden
		}
		createWindow(S2, R4, Fw, "Heizung", Heiz);
		if (strcmp ("fals",HeizungState) == 0){
			createData(S2, R4, " ON");
		}
		else if (strcmp ("true",HeizungState) == 0){
			createData(S2, R4, " OFF");
		}
		if (strcmp ("fals",pumpensumpf) == 0){
			snprintf (Sumpf, (size_t)5, "true");
		}
		else if (strcmp ("true",pumpensumpf) == 0){
			snprintf (Sumpf, (size_t)5, "fals");
		}
		createWindow(S3, R4, Fw, " Sumpf", Sumpf);
		//Garage
		drawSquare(S5+6,R3+20,40,12,GREY);
		put_string(S5+6, (R3+20), GarageTemp,WHITE);
		createWindow(S6, R3, Fw, " Tor", Garagentor);
		createWindow(S7, R3, Fw, "Vorne", Garagentuerv);
		createWindow(S8, R3, Fw, "Hinten", Garagentuerh);
		//Aussen
		createWindow(S6, R4, Fw, "Aussen", "");
		createData(S6, R4, AussenTemp);
		createJalousie(S6, R4, Fw, Markise);
		createWindow(S7, R4, Fw, "Regen", "no");
		createData(S7, R4, AussenHum);
		if (strcmp ("0",AussenRegen) == 0){
			drawSquare(S7, R4, Fw, 21, ORANGE);																				//Fenster zeichnen in geänderter Farbe, hier "ORANGE"
		}                                                                           //die Farben sind in der "framebuffer.c" definiert
		else if (strcmp ("1",AussenRegen) == 0){
			drawSquare(S7, R4, Fw, 21, CYAN);
		}
		createWindow(S8, R4, Fw, "Brunnen", "no");
		if (strcmp ("true",Brunnen) == 0){
			drawSquare(S8, R4, Fw, 21, BLUE);
		}
		else if (strcmp ("fals",Brunnen) == 0){
			drawSquare(S8, R4, Fw, 21, LTGREY);
		}
		//Homematic
		createWindow(S6, R1, Fw, "HM-CPU", "no");
		createData(S6, R1-15, HMCPU);
		createData(S6-5, R1, HMupTage);
		//HM_Sonderdarstellung
		if(counter == 0 && HM_Sonderdarstellung == 1){
			read_HM(ISE_Sonnenaufgang, 5, Sonnenaufgang);
			read_HM(ISE_Sonnenuntergang, 5, Sonnenuntergang);
			read_HM(ISE_Vollmond, 10, Vollmond);
			snprintf (OUT, (size_t)100, "Sonnenaufgang:        %s", Sonnenaufgang);
			put_string(40, 408, OUT, GREY);
			snprintf (OUT, (size_t)100, "Sonnenuntergang:      %s", Sonnenuntergang);
			put_string(40, 420, OUT, GREY);
			snprintf (OUT, (size_t)100, "Vollmond:        %s", Vollmond);
			put_string(40, 432, OUT, GREY);
		}
		HMcounter = 0;
	}
	return 1;
}
#endif // __HMGUI_H_

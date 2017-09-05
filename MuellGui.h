#ifndef __MUELLGUI_H_
#define __MUELLGUI_H_

//KW aus Mülldatei einlesen
int readMuellKW(int kw, char* jahr, char* kwLine, char* kwLine2)
{
  char Path[128], read[256];
	time_t timeStamp;
  struct tm *now;
  time( &timeStamp );
  now = localtime( &timeStamp );
  strftime (jahr,20,"%Y",now);
	snprintf(Path, (size_t)256, "/home/pi/E3dcGui/Data/Entsorgung_%s.txt", jahr);
  int line = kw + 12;
  FILE *fp;
  fp = fopen(Path, "r");
  if(fp == NULL) {
    printf("Datei %s konnte NICHT geoeffnet werden.\n", Path);
    return -1;
  }
  else {
    for( int c = 1; c < 56; ++c ){
      fgets(read,256,fp);
      if ( c == line){
        snprintf (kwLine, (size_t)256, "%s", read);
      }
      if ( c == line+1){
        snprintf (kwLine2, (size_t)256, "%s", read);
				return 1;
      }
    }
    fclose(fp);
  }
  return 1;
}
//Müllsymbole erstellen
int drawMuell(int x, int y, char day1[64], char day2[64])
{
  char OUT[128], batch[256];
  snprintf(OUT, (size_t)64, "Muell/%s", day1);
  DrawImage(OUT, x, y);
  if (strcmp ("frei",day2) == 0)
    return 1;
  else {
    snprintf(OUT, (size_t)64, "Muell/%s", day2);
    DrawImage(OUT, x-12, y+17);
  }
  return 1;
}

//Müll Grafik erstellen
int makeMuellGui(int GuiTime, int counter)
{
	char OUT[128], off[128], kwLine[256], kwLine2[256], jahrread[64], jahr[64];
	char Mon1kw1[64], Tue1kw1[64], Wed1kw1[64], Thu1kw1[64], Fri1kw1[64], Sat1kw1[64], Sun1kw1[64];
	char Mon2kw1[64], Tue2kw1[64], Wed2kw1[64], Thu2kw1[64], Fri2kw1[64], Sat2kw1[64], Sun2kw1[64];
	char Mon1kw2[64], Tue1kw2[64], Wed1kw2[64], Thu1kw2[64], Fri1kw2[64], Sat1kw2[64], Sun1kw2[64];
	char Mon2kw2[64], Tue2kw2[64], Wed2kw2[64], Thu2kw2[64], Fri2kw2[64], Sat2kw2[64], Sun2kw2[64];
	//Read Daten Beginn (roter Punkt unten rechts)
	GuiTime = PiTime;
	if (counter == 0){
		writeScreen(ScreenCounter, 60);
		drawMainScreen();
		drawSquare(760,440,20,20,LIGHT_RED);
		drawCorner(760,440,20,20,WHITE);
		//Mülldatei einlesen für aktuelle KW
		time_t timeStamp;
		struct tm *now;
		time( &timeStamp );
		now = localtime( &timeStamp );
		strftime (OUT,100,"%V",now);
		int kw = atoi(OUT);
		readMuellKW(kw, jahr, kwLine, kwLine2);
		sscanf(kwLine, "%s Mon= %s %s Tue= %s %s Wed= %s %s Thu= %s %s Fri= %s %s Sat= %s %s Sun= %s %s", off, Mon1kw1, Mon2kw1, Tue1kw1, Tue2kw1, Wed1kw1, Wed2kw1, Thu1kw1, Thu2kw1, Fri1kw1, Fri2kw1, Sat1kw1, Sat2kw1, Sun1kw1, Sun2kw1);
		sscanf(kwLine2, "%s Mon= %s %s Tue= %s %s Wed= %s %s Thu= %s %s Fri= %s %s Sat= %s %s Sun= %s %s", off, Mon1kw2, Mon2kw2, Tue1kw2, Tue2kw2, Wed1kw2, Wed2kw2, Thu1kw2, Thu2kw2, Fri1kw2, Fri2kw2, Sat1kw2, Sat2kw2, Sun1kw2, Sun2kw2);
		int back = readMuellKW(-11, off, kwLine, off);
		sscanf(kwLine, "%s", jahrread);
		// Grafik für Current
		drawSquare(MuellS0-25,MuellZ0-10,720,220,GREY);
		drawCorner(MuellS0-25,MuellZ0-10,720,220,WHITE);
		drawSquare(MuellS0-22,MuellZ0-7,714,214,WHITE);
		drawCorner(MuellS0-22,MuellZ0-7,714,214,GREY);

		//Grafiken erstellen
		if (back == -1){
			put_string(MuellS0, MuellZ0, "keine", RED);
      snprintf(OUT, (size_t)64, "Entsorgung_%s.txt", jahr);
			put_string(MuellS0, MuellZ0+16, OUT, RED);
		}
		else
			put_string(MuellS0, MuellZ0, jahr, GREEN);
		put_string(MuellS1-10, MuellZ0, "Montag", GREY);
		put_string(MuellS2-10, MuellZ0, "Dienstag", GREY);
		put_string(MuellS3-10, MuellZ0, "Mittwoch", GREY);
		put_string(MuellS4-10, MuellZ0, "Donnerstag", GREY);
		put_string(MuellS5-10, MuellZ0, "Freitag", GREY);
		put_string(MuellS6-10, MuellZ0, "Samstag", GREY);
		put_string(MuellS7-10, MuellZ0, "Sonntag", GREY);

		snprintf(OUT, (size_t)64, "KW %i", kw);
		put_string(MuellS0, MuellZ1+20, OUT, GREY);
		snprintf(OUT, (size_t)64, "KW %i", kw+1);
		put_string(MuellS0, MuellZ2+20, OUT, GREY);

		if (back == 1){
			drawMuell(MuellS1, MuellZ1, Mon1kw1, Mon2kw1);
			drawMuell(MuellS2, MuellZ1, Tue1kw1, Tue2kw1);
			drawMuell(MuellS3, MuellZ1, Wed1kw1, Wed2kw1);
			drawMuell(MuellS4, MuellZ1, Thu1kw1, Thu2kw1);
			drawMuell(MuellS5, MuellZ1, Fri1kw1, Fri2kw1);
			drawMuell(MuellS6, MuellZ1, Sat1kw1, Sat2kw1);
			drawMuell(MuellS7, MuellZ1, Sun1kw1, Sun2kw1);

			drawMuell(MuellS1, MuellZ2, Mon1kw2, Mon2kw2);
			drawMuell(MuellS2, MuellZ2, Tue1kw2, Tue2kw2);
			drawMuell(MuellS3, MuellZ2, Wed1kw2, Wed2kw2);
			drawMuell(MuellS4, MuellZ2, Thu1kw2, Thu2kw2);
			drawMuell(MuellS5, MuellZ2, Fri1kw2, Fri2kw2);
			drawMuell(MuellS6, MuellZ2, Sat1kw2, Sat2kw2);
			drawMuell(MuellS7, MuellZ2, Sun1kw2, Sun2kw2);
			//ReadDaten grün
			drawSquare(760,440,20,20,GREEN);
			drawCorner(760,440,20,20,WHITE);
		}
		else {
			//ReadDaten orange
			drawSquareRGB(760,440,20,20,225,122,34);
			drawCorner(760,440,20,20,WHITE);
		}
		if (MuellLegende == 1){
			drawMuell(40, MuellZ3, "bio", "frei");
			put_string(40-10, MuellZ3+60, "Biom\201ll", GREY);
			drawMuell(115, MuellZ3, "papier", "frei");
			put_string(115-6, MuellZ3+60, "Papier", GREY);
			put_string(115-6, MuellZ3+72, "Tonne", GREY);
			drawMuell(190, MuellZ3, "gelb", "frei");
			put_string(190-6, MuellZ3+60, "Gelber", GREY);
			put_string(190-6, MuellZ3+72, " Sack", GREY);
			drawMuell(265, MuellZ3, "rest", "frei");
			put_string(265-10, MuellZ3+60, "Restm\201ll", GREY);
			drawMuell(340, MuellZ3, "glas", "frei");
			put_string(340, MuellZ3+60, "Glas", GREY);
			drawMuell(415, MuellZ3, "metall", "frei");
			put_string(415-10, MuellZ3+60, "Metall", GREY);
			drawMuell(490, MuellZ3, "schadstoff", "frei");
			put_string(490-20, MuellZ3+60, "Schadstoff", GREY);
			put_string(490-10, MuellZ3+72, " Mobil", GREY);
			drawMuell(565, MuellZ3, "frei", "frei");
			put_string(565, MuellZ3+60, "keine", GREY);
			put_string(565, MuellZ3+72, "Tonne", GREY);
			drawMuell(640, MuellZ3, "feiertag", "frei");
			put_string(640-14, MuellZ3+60, "Feiertag", GREY);
			drawMuell(715, MuellZ3-20, "bio", "gelb");
			put_string(715-10, MuellZ3+60, "Beispiel", GREY);
			put_string(715-10, MuellZ3+72, "2 Tonnen", GREY);
		}
	}
	return GuiTime;

}
#endif // __MuellGui_H_

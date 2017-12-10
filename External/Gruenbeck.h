#ifndef __GRUENBECK_H_
#define __GRUENBECK_H_

#define GB_TIME                     "D_C_4_2"           // Uhrzeit
#define GB_CONSUMPTION              "D_Y_1"             // Wasserverbrauch gestern
#define GB_CURRENT_REGENERATION     "D_Y_5"             // Aktueller Regenerationsschritt
#define GB_PERCENT_REGENERATION     "D_Y_10_1"          // Aktuelle Restkapazität Austauscher
#define GB_CURRENT_FLOW             "D_A_1_1"           // Aktueller Durchfluss
#define GB_R_CAPACITY               "D_A_1_2"           // Restkapazität
#define GB_LAST_REGENERATION        "D_A_3_1"           // Letzte Regeneration
#define GB_STATUS_EMAIL             "D_Y_8_11"          // Ergebnis letzter E-Mail Versand

#define ISE_GB_CONSUMPTION          60750
#define ISE_GB_CURRENT_REGENERATION 60749
#define ISE_GB_PERCENT_REGENERATION 60756
#define ISE_GB_CURRENT_FLOW         60747
#define ISE_GB_R_CAPACITY           60748
#define ISE_GB_LAST_REGENERATION    60754
#define ISE_GB_STATUS_EMAIL         60755
#define ISE_GB_PI_SEND_OK           60757

#define OK                          1

int readGB(char parameter[8], char *value)
{
  int repeat = 0;
  char batch[256];
  while (repeat <= 2) {
    memset(batch, 0x00, sizeof(batch));
    memset(value, 0x00, sizeof(value));
    snprintf(batch, sizeof(batch), "curl http://%s/mux_http --data \"id=625&show=%s~\" --silent --output /dev/stdout | xmllint --xpath \"string(//%s)\" --format -", GRUEN_IP, parameter, parameter);
    FILE *out = NULL;
    out = popen( batch, "r" );
    if( out != NULL ){
      fscanf(out, "%s", value);
      pclose(out);
      if( value != NULL ){
        return 1;
      }
      else
        repeat++;
    }
    pclose(out);
  }
  snprintf(value, (size_t)256, "0");
  return 0;
}

float readGruenRAM(int GruenbeckPosition)
{
	float ret = BitRead("/mnt/RAMDisk/Gruenbeck.txt", GruenbeckPosition, PosGB_Max);
  ret = ret / 100;
  return ret;
}
int writeGruenRAM(int GruenbeckPosition, float NewValue)
{
  NewValue = NewValue * 100;
  BitWrite("/mnt/RAMDisk/Gruenbeck.txt", GruenbeckPosition, NewValue, PosGB_Max);
  return 1;
}
int makeGruenRAM()
{
	BitMake("/mnt/RAMDisk/Gruenbeck.txt", PosGB_Max);
  return 0;
}
float readGruenSave(int GruenSavePosition)
{
	float ret = BitRead("/home/pi/E3dcGui/Data/GBSave.txt", GruenSavePosition, PosSaveMAX);
  ret = ret / 100;
  return ret;
}
int writeGruenSave(int GruenSavePosition, float NewValue)
{
  NewValue = NewValue * 100;
  BitWrite("/home/pi/E3dcGui/Data/GBSave.txt", GruenSavePosition, NewValue, PosSaveMAX);
  return 1;
}
int makeGruenSave()
{
	BitMake("/home/pi/E3dcGui/Data/GBSave.txt", PosSaveMAX);
  return 0;
}
/*
char Path[40];
snprintf(Path, (size_t)256, "/home/pi/E3dcGui/Data/Gruenbeck_%s.txt", fileYear);
*/
void drawGBpercent(int x, int y, int percentRegeneration){
  char OUT[128];
  DrawImage("GB_Prozent", x, y);
  if (percentRegeneration >= 80)
    drawSquare(x +40,y +24,35,10,WHITE);
  if (percentRegeneration >= 60)
    drawSquare(x +40,y +38,35,10,WHITE);
  if (percentRegeneration >= 40)
    drawSquare(x +40,y +52,35,10,WHITE);
  if (percentRegeneration >= 20)
    drawSquare(x +40,y +66,35,10,WHITE);
  if (percentRegeneration >= 10)
    drawSquare(x +40,y +80,35,10,WHITE);
    put_string(x -10, y +120, "Anlagenkapazit\204t", GREY);
  snprintf(OUT, (size_t)64, "%i %%", percentRegeneration);
  put_string(x +40, y +132, OUT, GREEN);
}
void putGBRegState(int x, int y, int currentRegeneration)
{
  char OUT[128];
  put_string(x, y, "Regenegationsschritt", GREY);
  if (currentRegeneration == 0)
    snprintf(OUT, (size_t)64, "keine Regeneration");
  else if (currentRegeneration == 1)
    snprintf(OUT, (size_t)64, "Soletank f\201llen");
  else if (currentRegeneration == 2)
    snprintf(OUT, (size_t)64, "Besalzen");
  else if (currentRegeneration == 3)
    snprintf(OUT, (size_t)64, "Verdr\204ngen");
  else if (currentRegeneration == 4)
    snprintf(OUT, (size_t)64, "Rücksp\201len");
  else if (currentRegeneration == 4)
    snprintf(OUT, (size_t)64, "Erstfiltrat");
  put_string(x, y +12, OUT, GREEN);
}
void putGBeMailState(int x, int y, int statusEmail)
{
  char OUT[128];
  put_string(x, y, "Status letzte eMail", GREY);
  if (statusEmail == 0)
    snprintf(OUT, (size_t)64, "keine Mail versandt");
  else if (statusEmail == 1)
    snprintf(OUT, (size_t)64, "Mail erfolgreich versandt");
  else if (statusEmail == 2)
    snprintf(OUT, (size_t)64, "Benutzerdaten fehlerhaft");
  else if (statusEmail == 3)
    snprintf(OUT, (size_t)64, "kein Internetzugang/Server nicht bereit");
  put_string(x, y +12, OUT, GREEN);
}
void readGruenbeck(char *gruenTime)
{
  char value[256], date[24];

  time_t timeStamp = time(NULL);
  struct tm *now;
  time( &timeStamp );
  now = localtime( &timeStamp );
  strftime (date,24,"%d.%m.%Y",now);

  readGB(GB_TIME, value);
  snprintf(gruenTime, (size_t)24, "%s %s", date, value);
  printf("Gruenbeck Uhr: %s\n", gruenTime);
  system("sudo /home/pi/E3dcGui/External/gruenSave &");
}
int saveGruenbeck(char *gruenTime)
{
  char vMinute[20], day[20], month[20], year[20], stamp[20], file[40], path[40], OUT[128];
  float consumption, GBMonth, GBYear, GBAll;

  time_t timeStamp = time(NULL);
  struct tm *now;
  time( &timeStamp );
  now = localtime( &timeStamp );
  strftime (vMinute,20,"%M:%S",now);
  strftime (day,20,"%H:%M:%S",now);
  strftime (month,20,"%d_%H:%M:%S",now);
  strftime (year,20,"%d.%m_%H:%M:%S",now);
  strftime (stamp,20,"%d.%m.%Y_%H:%M",now);
  strftime (file,20,"%Y",now);

  if ((strcmp ("00:00",vMinute) == 0) || (strcmp ("15:00",vMinute) == 0) || (strcmp ("30:00",vMinute) == 0) || (strcmp ("45:00",vMinute) == 0)){
    readGruenbeck(gruenTime);
  }

  consumption = readGruenRAM(PosGB_CONSUMPTION);

  if (strcmp ("23:59:00",day) == 0){
    GBMonth = readGruenSave(PosSaveMonth) /100;
    GBMonth = GBMonth + (consumption /1000);
    writeGruenSave(PosSaveMonth, GBMonth *100);
    GBYear = readGruenSave(PosSaveYear) /100;
    GBYear = GBYear + (consumption /1000);
    writeGruenSave(PosSaveYear, GBYear *100);
    GBAll = readGruenSave(PosSaveAll) /100;
    GBAll = GBAll + (consumption /1000);
    writeGruenSave(PosSaveAll, GBAll *100);
    snprintf(OUT, (size_t)128, "%s;%.0f;%.2f;%.2f;%.2f", stamp, consumption, GBMonth, GBYear, GBAll);
    snprintf(path, (size_t)128, "/home/pi/E3dcGui/Data/GB_%s.csv", file);
    appeendedData(path, OUT);
  }
  if (strcmp ("01_00:00:00",month) == 0){
    writeGruenSave(PosSaveMonth, 0);
  }
  if (strcmp ("01.01_00:00:00",year) == 0){
    writeGruenSave(PosSaveYear, 0);
  }
  return 1;
}

//Gruenbeck Grafik erstellen
int makeGruenbeck(int GuiTime, int counter, char *gruenTime)
{
  char OUT[128], off[128], value[256], date[24];
  float consumption, currentRegeneration, percentRegeneration, currentFlow, rCapasity, lastRegeneration, statusEmail;
  int GBDay;
  float GBConMonth, GBConYear, GBConAll;

	//Read Daten Beginn (roter Punkt unten rechts)
	GuiTime = GruenTime;
	if (counter == 0){
		writeScreen(ScreenCounter, 60);
		drawMainScreen();
		drawSquare(760,440,20,20,LIGHT_RED);
		drawCorner(760,440,20,20,WHITE);
    consumption = readGruenRAM(PosGB_CONSUMPTION);
    currentRegeneration = readGruenRAM(PosGB_CURRENT_REGENERATION);
    percentRegeneration = readGruenRAM(PosGB_PERCENT_REGENERATION);
    currentFlow = readGruenRAM(PosGB_CURRENT_FLOW);
    rCapasity = readGruenRAM(PosGB_R_CAPACITY);
    lastRegeneration = readGruenRAM(PosGB_LAST_REGENERATION);
    statusEmail = readGruenRAM(PosGB_STATUS_EMAIL);
    GBConMonth = readGruenSave(PosSaveMonth) /100;
    GBConYear = readGruenSave(PosSaveYear) /100;
    GBConAll = readGruenSave(PosSaveAll) /100;
    drawSquare(760,440,20,20,LIGHT_GREEN);
		drawCorner(760,440,20,20,WHITE);

		// GB Grafik
    drawSquare(40,100,267,266,GREY);
		drawCorner(40,100,267,266,WHITE);
    DrawImage("GB_Logo", 41, 101);
    drawCorner(43,103,261,260,GREY);
    // GB GUI
    drawSquare(320,100,440,266,GREY);
		drawCorner(320,100,440,266,WHITE);
		drawSquare(323,103,434,260,WHITE);
		drawCorner(323,103,434,260,GREY);
    // GB Werte
    drawGBpercent(340, 120, percentRegeneration);
    put_string(500, 120, "Verbrauch gestern", GREY);
    snprintf(OUT, (size_t)64, "%.0f l", consumption);
    put_string(550, 132, OUT, GREEN);
    put_string(500, 160, "Restkapazit\204t", GREY);
    snprintf(OUT, (size_t)64, "%.2f qm", rCapasity);
    put_string(550, 172, OUT, GREEN);
    put_string(500, 200, "Aktueller Durchfluss", GREY);
    snprintf(OUT, (size_t)64, "%.2f qm", currentFlow);
    put_string(550, 212, OUT, GREEN);
    put_string(500, 240, "Letzte Regeneration", GREY);
    snprintf(OUT, (size_t)64, "%.0f Std", lastRegeneration);
    put_string(550, 252, OUT, GREEN);
    putGBRegState(330, 280, currentRegeneration);
    putGBeMailState(500, 280, statusEmail);
    put_string(330, 320, "Verbrauch", GREY);
    put_string(450, 320, "Monat", GREY);
    snprintf(OUT, (size_t)64, "%.2f qm", GBConMonth);
    put_string(450, 332, OUT, GREEN);
    put_string(550, 320, "Jahr", GREY);
    snprintf(OUT, (size_t)64, "%.2f qm", GBConYear);
    put_string(550, 332, OUT, GREEN);
    put_string(650, 320, "Gesamt", GREY);
    snprintf(OUT, (size_t)64, "%.2f qm", GBConAll);
    put_string(650, 332, OUT, GREEN);
	}
	return GuiTime;

}
#endif // __GRUENBECK_H_

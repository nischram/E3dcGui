#ifndef __WETTERGUI_H_
#define __WETTERGUI_H_

int find(char * str, char chr, int pos) {
    char * p = str + pos;

    while(*p != chr && *p != '\0' && *p != '\n') {
      ++p;
    }
    return (p - str);
}
int extraktChar(char* str, char chr, char* output){
  int r = find(str, '>', 0);
  char * newstr  = &str[r+1];
  int re = find(newstr, chr, 0);
  newstr[re] = '\0';
  snprintf(output, (size_t)256, "%s", newstr);
}
double extraktDouble(char* str, char chr){
  char value[64];
  extraktChar(str, chr, value);
  return atof(value);
}
int extraktInt(char* str, char chr){
  char value[64];
  extraktChar(str, chr, value);
  return atoi(value);
}
int extraktDate(char* str, char chr, char* output){
  char read[64], read1[64], read2[64], read3[64];
  extraktChar(str, '<', read);
  strtok(read, "-");
  sscanf(read, "%s", read1);
  char * new1 = &read[5];
  strtok(new1, "-");
  sscanf(new1, "%s", read2);
  char * new2 = &new1[3];
  strtok(new2, "-");
  sscanf(new2, "%s", read3);
  snprintf(output, (size_t)64, "%s.%s.%s", read3, read2, read1);
}
int extraktTime(char* str, char chr, char* output){
  char value[64];
  extraktChar(str, chr, value);
  int r = find(value, ':', 0);
  char * newstr  = &value[r-2];
  snprintf(output, (size_t)256, "%s", newstr);
}
int extraktDay(char* str, char chr, char* output){
  char value[64];
  extraktChar(str, chr, value);
  int r = find(value, ':', 0);
  value[r-2] = '\0';
  snprintf(output, (size_t)256, "%s", value);
}

//Übersetzung  Umlaute \204=ä \216=Ä \224=ö \231=Ö \201=ü \232=Ü \341=ß
int changeToText(int code, char * output){
  if (code == 	200	) snprintf(output, (size_t)64, "Gewitter mit leichtem Regen");
  else if (code == 	201	) snprintf(output, (size_t)64, "Gewitter mit Regen");
  else if (code == 	202	) snprintf(output, (size_t)64, "Gewitter mit starkem Regen");
  else if (code == 	210	) snprintf(output, (size_t)64, "leichtes Gewitter");
  else if (code == 	211	) snprintf(output, (size_t)64, "Gewitter");
  else if (code == 	212	) snprintf(output, (size_t)64, "schweres Gewitter");
  else if (code == 	221	) snprintf(output, (size_t)64, "zerrissenes Gewitter");
  else if (code == 	230	) snprintf(output, (size_t)64, "Gewitter mit leichtem Nieselregen");
  else if (code == 	231	) snprintf(output, (size_t)64, "Gewitter mit Nieselregen");
  else if (code == 	232	) snprintf(output, (size_t)64, "Gewitter mit starkem Nieselregen");
  else if (code == 	300	) snprintf(output, (size_t)64, "leichter Niesel");
  else if (code == 	301	) snprintf(output, (size_t)64, "Niesel");
  else if (code == 	302	) snprintf(output, (size_t)64, "starker Niesel");
  else if (code == 	310	) snprintf(output, (size_t)64, "leichter Nieselregen");
  else if (code == 	311	) snprintf(output, (size_t)64, "Nieselregen");
  else if (code == 	312	) snprintf(output, (size_t)64, "starker Nieselregen");
  else if (code == 	313	) snprintf(output, (size_t)64, "Scheeregen und Niesel");
  else if (code == 	314	) snprintf(output, (size_t)64, "starker Scheeregen und Niesel");
  else if (code == 	321	) snprintf(output, (size_t)64, "Nieselregenschauer");
  else if (code == 	500	) snprintf(output, (size_t)64, "leichter Regen");
  else if (code == 	501	) snprintf(output, (size_t)64, "moderater Regen");
  else if (code == 	502	) snprintf(output, (size_t)64, "Regen");
  else if (code == 	503	) snprintf(output, (size_t)64, "sehr starker Regen");
  else if (code == 	504	) snprintf(output, (size_t)64, "extremer Regen");
  else if (code == 	511	) snprintf(output, (size_t)64, "gefrierender Regen");
  else if (code == 	520	) snprintf(output, (size_t)64, "leichte Regenschauer");
  else if (code == 	521	) snprintf(output, (size_t)64, "Regenschauer");
  else if (code == 	522	) snprintf(output, (size_t)64, "starke Regenschauer");
  else if (code == 	531	) snprintf(output, (size_t)64, "unregelm\204ßige Regenschauer");
  else if (code == 	600	) snprintf(output, (size_t)64, "leichter Schnee");
  else if (code == 	601	) snprintf(output, (size_t)64, "Schnee");
  else if (code == 	602	) snprintf(output, (size_t)64, "starker Schnee");
  else if (code == 	611	) snprintf(output, (size_t)64, "Schneeregen");
  else if (code == 	612	) snprintf(output, (size_t)64, "Schneeregenschauen");
  else if (code == 	615	) snprintf(output, (size_t)64, "leichter Regen und Schnee");
  else if (code == 	616	) snprintf(output, (size_t)64, "Regen und Schnee");
  else if (code == 	620	) snprintf(output, (size_t)64, "leichte Schneeschauer");
  else if (code == 	621	) snprintf(output, (size_t)64, "Schneeschauer");
  else if (code == 	622	) snprintf(output, (size_t)64, "starke Schneeschauer");
  else if (code == 	701	) snprintf(output, (size_t)64, "Nebel");
  else if (code == 	711	) snprintf(output, (size_t)64, "Rauch");
  else if (code == 	721	) snprintf(output, (size_t)64, "Dunst");
  else if (code == 	731	) snprintf(output, (size_t)64, "Sand, Staub wirbelt");
  else if (code == 	741	) snprintf(output, (size_t)64, "Nebel");
  else if (code == 	751	) snprintf(output, (size_t)64, "Sand");
  else if (code == 	761	) snprintf(output, (size_t)64, "Staub");
  else if (code == 	762	) snprintf(output, (size_t)64, "Vulkan Asche");
  else if (code == 	771	) snprintf(output, (size_t)64, "Stürme");
  else if (code == 	781	) snprintf(output, (size_t)64, "Tornado");
  else if (code == 	800	) snprintf(output, (size_t)64, "klarer Himmel");
  else if (code == 	801	) snprintf(output, (size_t)64, "ein paar Wolken");
  else if (code == 	802	) snprintf(output, (size_t)64, "aufgelockerte Bew\224=lkung");
  else if (code == 	803	) snprintf(output, (size_t)64, "aufgelockert bew\224=lkt");
  else if (code == 	804	) snprintf(output, (size_t)64, "wolkig");
  else snprintf(output, (size_t)64, "Unbest\204ndig");
}

//Wetter Grafik erstellen
int makeWetterGui(int GuiTime, int counter, char* weatherTime)
{
	//Difiniton der Variablen die auf dem Display angezeigt werden sollen.
  char country[64], region[64], city[64], current_date[64], current_text[64], direction[64], sunrise[64], sunset[64];
  char forecast0_date[64], forecast0_text[64], forecast1_date[64], forecast1_text[64], forecast2_date[64], forecast2_text[64];
  char current_image[64], forecast0_image[64], forecast1_image[64], forecast2_image[64];
  double longitude, latitude, visibility, current_temp, current_temp_low, current_temp_high, speed;
  double forecast0_temp_low, forecast0_temp_high, forecast1_temp_low, forecast1_temp_high, forecast2_temp_low, forecast2_temp_high;
  int humidity, pressure, current_code, forecast0_code, forecast1_code, forecast2_code;
  char OUT[128], value[128], read[128], read1[64], read2[64], read3[64], read4[64], read5[64], read6[64], off[128], batch[256];
	//Read Daten Beginn (roter Punkt unten rechts)
	GuiTime = WeatherTime;
	if (counter == 0){
		writeScreen(ScreenCounter, 60);
		drawMainScreen();
		drawSquare(760,440,20,20,LIGHT_RED);
		drawCorner(760,440,20,20,WHITE);
		//Wetterdaten einlesen
		memset(&batch, 0, sizeof(batch));
		snprintf(batch, (size_t)256, "curl \"https://weather.tuxnet24.de/?apikey=%s&id=%i&unit=metric&mode=xml\"", weatherKey, weatherID);
		FILE *out = NULL;
		out = popen( batch, "r" );
		if( out != NULL ){
      fgets(off, 128, out);                           //xml version
      fgets(off, 128, out);                           //leer
      fgets(off, 128, out);                           //info
      fgets(off, 128, out);                           //id
      fgets(read, 128, out);                          //longitude
      longitude = extraktDouble(read, '<');
      fgets(read, 128, out);                          //latitude
      latitude = extraktDouble(read, '<');
      fgets(read, 128, out);                          //country
      extraktChar(read, '<', country);
      fgets(read, 128, out);                          //region
      extraktChar(read, '<', region);
      fgets(read, 128, out);                          //city
      extraktChar(read, '<', city);
      fgets(read, 128, out);                          //humidity
      humidity = extraktInt(read, '%');
      fgets(read, 128, out);                          //visibility
      visibility = extraktDouble(read, '<');
      fgets(read, 128, out);                          //pressure
      pressure = extraktDouble(read, 'h');
      fgets(off, 128, out);                           //rising
      fgets(read, 128, out);                          //sunrise
      extraktChar(read, '<', sunrise);
      fgets(read, 128, out);                          //sunset
      extraktChar(read, '<', sunset);
      fgets(read, 128, out);                          //current_code
      current_code = extraktInt(read, '<');
      changeToText(current_code, current_text);
      fgets(off, 128, out);                           //current_day
      fgets(read, 128, out);                          //current_date
      extraktDay(read, '<', current_date);
      extraktTime(read, '<', weatherTime);
      fgets(read, 128, out);                          //current_temp
      current_temp = extraktDouble(read, '&');
      fgets(read, 128, out);                          //current_temp_low
      current_temp_low = extraktDouble(read, '&');
      fgets(read, 128, out);                          //current_temp_high
      current_temp_high = extraktDouble(read, '&');
      fgets(off, 128, out);                           //current_text
      fgets(off, 128, out);                           //current_description
      fgets(read, 128, out);                          //current_image
      extraktChar(read, '.', current_image);
      fgets(off, 128, out);                           //current_icon
      fgets(read, 128, out);                          //speed
      speed = extraktDouble(read, 'm');
      fgets(read, 128, out);                          //direction
      extraktChar(read, '&', direction);
      fgets(off, 128, out);                           //chill
      fgets(read, 128, out);                          //forecast0_day
      extraktChar(read, '<', read1);
      fgets(read, 128, out);                          //forecast0_date
      extraktDate(read, '<', read2);
      snprintf(forecast0_date, (size_t)64, "%s %s", read1, read2);
      fgets(read, 128, out);                          //forecast0_temp_low
      sscanf(read, "%s", value);
      forecast0_temp_low = extraktDouble(read, '&');
      fgets(read, 128, out);                          //forecast0_temp_high
      forecast0_temp_high = extraktDouble(read, '&');
      fgets(off, 128, out);                           //forecast0_text
      fgets(off, 128, out);                           //forecast0_description
      fgets(read, 128, out);                          //forecast0_code
      forecast0_code = extraktInt(read, '<');
      changeToText(forecast0_code, forecast0_text);
      fgets(off, 128, out);                           //forecast0_icon
      fgets(read, 128, out);                          //forecast0_image
      extraktChar(read, '.', forecast0_image);
      fgets(read, 128, out);                          //forecast1_day
      extraktChar(read, '<', read1);
      fgets(read, 128, out);                          //forecast1_date
      extraktDate(read, '<', read2);
      snprintf(forecast1_date, (size_t)64, "%s %s", read1, read2);
      fgets(read, 128, out);                          //forecast1_temp_low
      forecast1_temp_low = extraktDouble(read, '&');
      fgets(read, 128, out);                          //forecast1_temp_high
      forecast1_temp_high = extraktDouble(read, '&');
      fgets(off, 128, out);                           //forecast1_text
      fgets(off, 128, out);                           //forecast1_description
      fgets(read, 128, out);                          //forecast1_code
      forecast1_code = extraktInt(read, '<');
      changeToText(forecast1_code, forecast1_text);
      fgets(off, 128, out);                           //forecast1_icon
      fgets(read, 128, out);                          //forecast1_image
      extraktChar(read, '.', forecast1_image);
      fgets(read, 128, out);                          //forecast2_day
      extraktChar(read, '<', read1);
      fgets(read, 128, out);                          //forecast2_date
      extraktDate(read, '<', read2);
      snprintf(forecast2_date, (size_t)64, "%s %s", read1, read2);
      fgets(read, 128, out);                          //forecast2_temp_low
      forecast2_temp_low = extraktDouble(read, '&');
      fgets(read, 128, out);                          //forecast2_temp_high
      forecast2_temp_high = extraktDouble(read, '&');
      fgets(off, 128, out);                           //forecast2_text
      fgets(off, 128, out);                           //forecast2_description
      fgets(read, 128, out);                          //forecast2_code
      forecast2_code = extraktInt(read, '<');
      changeToText(forecast2_code, forecast2_text);
      fgets(off, 128, out);                           //forecast2_icon
      fgets(read, 128, out);                          //forecast2_image
      extraktChar(read, '.', forecast2_image);
      pclose(out);
    }
    printf("OpenWeatherMap Time: %s %s\n", current_date, weatherTime);
    printf(" %4.5f %4.5f %s %s %s\n", latitude, longitude, city,  region, country, humidity, pressure, speed, direction);
    printf(" %i %i %.2f %s\n", humidity, pressure, speed, direction);
    printf(" %s %.0f %.0f %.0f %s %s %i\n", current_date, current_temp, current_temp_low, current_temp_high, current_text, current_image, current_code);
    printf(" %s %.0f %.0f %s %s %i\n", forecast0_date, forecast0_temp_low, forecast0_temp_high, forecast0_text, forecast0_image, forecast0_code);
    printf(" %s %.0f %.0f %s %s %i\n", forecast1_date, forecast1_temp_low, forecast1_temp_high, forecast1_text, forecast1_image, forecast1_code);
    printf(" %s %.0f %.0f %s %s %i\n", forecast2_date, forecast2_temp_low, forecast2_temp_high, forecast2_text, forecast2_image, forecast2_code);

		//Read Daten Ende (grüner Punkt unten rechts)
		drawSquare(760,440,20,20,GREEN);
		drawCorner(760,440,20,20,WHITE);
		drawMainScreen();
		// Grafik für Current
		drawSquare(WetterS1-15,WetterZ1-10,720,190,GREY);
		drawCorner(WetterS1-15,WetterZ1-10,720,190,WHITE);
		drawSquare(WetterS1-12,WetterZ1-7,714,184,WHITE);
		drawCorner(WetterS1-12,WetterZ1-7,714,184,GREY);
		// Grafik für Forecast 1
		drawSquare(WetterS1-15,WetterUZ1-10,233,160,GREY);
		drawCorner(WetterS1-15,WetterUZ1-10,233,160,WHITE);
		drawSquare(WetterS1-12,WetterUZ1-7,227,154,WHITE);
		drawCorner(WetterS1-12,WetterUZ1-7,227,154,GREY);
		// Grafik für Forecast 2
		drawSquare(WetterS2-15,WetterUZ1-10,234,160,GREY);
		drawCorner(WetterS2-15,WetterUZ1-10,234,160,WHITE);
		drawSquare(WetterS2-12,WetterUZ1-7,228,154,WHITE);
		drawCorner(WetterS2-12,WetterUZ1-7,228,154,GREY);
		// Grafik für Forecast 3
		drawSquare(WetterS3-15,WetterUZ1-10,233,160,GREY);
		drawCorner(WetterS3-15,WetterUZ1-10,233,160,WHITE);
		drawSquare(WetterS3-12,WetterUZ1-7,227,154,WHITE);
		drawCorner(WetterS3-12,WetterUZ1-7,227,154,GREY);

		//Daten Grafik erstellen
		//ReadDaten grün
		drawSquare(760,440,20,20,GREEN);
		drawCorner(760,440,20,20,WHITE);

		//Grafiken erstellen
		snprintf(OUT, (size_t)64, "%s", current_date);
		put_string(WetterS1, WetterZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%s", current_image);
		DrawImage(OUT, WetterS1+70, WetterZ2-10);
		snprintf(OUT, (size_t)64, "Temperatur:          %.0f%cC", current_temp, 248);
		put_string(WetterS1, WetterZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Max.:     %.0f%cC", current_temp_high, 248);
		put_string(WetterS1, WetterZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Min.:     %.0f%cC", current_temp_low, 248);
		put_string(WetterS1, WetterZ6, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s ", current_text);
		put_string(WetterS1, WetterZ7, OUT, GREY);

		snprintf(OUT, (size_t)64, "%s", city);
		put_string(WetterS2, WetterZ2, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s   %s", region, country);
		put_string(WetterS2, WetterZ3, OUT, GREY);
		snprintf(OUT, (size_t)64, "Location-ID: %i", weatherID);
		put_string(WetterS2, WetterZ4, OUT, GREY);


		snprintf(OUT, (size_t)64, "Luftfeuchtigkeit:     %i %%", humidity);
		put_string(WetterS3-70, WetterZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Fernsicht:            %.0f km", visibility);
		put_string(WetterS3-70, WetterZ2, OUT, GREY);
		snprintf(OUT, (size_t)64, "Luftdruck:            %i hPa", pressure);
		put_string(WetterS3-70, WetterZ3, OUT, GREY);
		snprintf(OUT, (size_t)64, "Windgeschwindigkeit:  %.1f m/s", speed);
		put_string(WetterS3-70, WetterZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Windrichtung:         %s%c ", direction, 248);
		put_string(WetterS3-70, WetterZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "Sonnenaufgang:        %s", sunrise);
		put_string(WetterS3-70, WetterZ6, OUT, GREY);
		snprintf(OUT, (size_t)64, "Sonnenuntergang:      %s", sunset);
		put_string(WetterS3-70, WetterZ7, OUT, GREY);

		snprintf(OUT, (size_t)64, "     %s", forecast0_date);
		put_string(WetterS1, WetterUZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%s", forecast0_image);
		DrawImage(OUT, WetterS1+70, WetterUZ2-10);
		snprintf(OUT, (size_t)64, "Temperatur Max.:     %.0f%cC", forecast0_temp_high, 248);
		put_string(WetterS1, WetterUZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Min.:     %.0f%cC", forecast0_temp_low, 248);
		put_string(WetterS1, WetterUZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s ", forecast0_text);
		put_string(WetterS1, WetterUZ6, OUT, GREY);

		snprintf(OUT, (size_t)64, "     %s", forecast1_date);
		put_string(WetterS2, WetterUZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%s", forecast1_image);
		DrawImage(OUT, WetterS2+70, WetterUZ2-10);
		snprintf(OUT, (size_t)64, "Temperatur Max.:     %.0f%cC", forecast1_temp_high, 248);
		put_string(WetterS2, WetterUZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Min.:     %.0f%cC", forecast1_temp_low, 248);
		put_string(WetterS2, WetterUZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s ", forecast1_text);
		put_string(WetterS2, WetterUZ6, OUT, GREY);

		snprintf(OUT, (size_t)64, "     %s", forecast2_date);
		put_string(WetterS3, WetterUZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%s", forecast2_image);
		DrawImage(OUT, WetterS3+70, WetterUZ2-10);
		snprintf(OUT, (size_t)64, "Temperatur Max.:     %.0f%cC", forecast2_temp_high, 248);
		put_string(WetterS3, WetterUZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Min.:     %.0f%cC", forecast2_temp_low, 248);
		put_string(WetterS3, WetterUZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s ", forecast2_text);
		put_string(WetterS3, WetterUZ6, OUT, GREY);
	}
	return GuiTime;

}
#endif // __WetterGui_H_

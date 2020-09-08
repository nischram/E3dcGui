#ifndef __WETTERGUI_H_
#define __WETTERGUI_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "weather/jsmn.h"
#include "weather/get_weather_data.h"

/**
 * Print a string with the desired format from an array of chars.
 * @param char * format
 * @param char * data The array of chars with the parsed json
 * @param jsmntok_t token The token that is used to compure the string length
 */
void print_key_value(char * format, char * data, jsmntok_t token) {
    printf(format, token.end - token.start, data + token.start);
}
void key_value(char *value, char * data, jsmntok_t token) {
    snprintf(value, 256, "%.*s", token.end - token.start, data + token.start);
}
int key_value_int(char * data, jsmntok_t token) {
    char value[256];
    snprintf(value, 256, "%.*s", token.end - token.start, data + token.start);
    return atoi(value);
}
float key_value_float(char * data, jsmntok_t token) {
    char value[256];
    snprintf(value, 256, "%.*s", token.end - token.start, data + token.start);
    return atof(value);
}
void makeDayString(char *uTime, char *timeString){
  time_t read_time = atoi(uTime);
  strftime (timeString, 100, "%a %d.%m.%Y", localtime (&read_time));
}
void makeSunString(char *uTime, char *timeString){
  time_t read_time = atoi(uTime);
  strftime (timeString, 100, "%H:%M Uhr", localtime (&read_time));
}
void makeNowString(char *timeString){
  time_t read_time = time(NULL);
  strftime (timeString, 100, "%d.%m.%Y %H:%M:%S", localtime (&read_time));
}

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
  else if (code == 	802	) snprintf(output, (size_t)64, "aufgelockerte Bew\224lkung");
  else if (code == 	803	) snprintf(output, (size_t)64, "aufgelockert bew\224lkt");
  else if (code == 	804	) snprintf(output, (size_t)64, "wolkig");
  else snprintf(output, (size_t)64, "Unbest\204ndig");
}
//Difiniton der Variablen die auf dem Display angezeigt werden sollen.
char country[64], city[64];
double longitude, latitude;
int weatherTimeZone;
char current_date[64], current_text[64], sunrise[64], sunset[64];
char forecast0_date[64], forecast0_text[64], forecast1_date[64], forecast1_text[64], forecast2_date[64], forecast2_text[64];
char current_image[64], forecast0_image[64], forecast1_image[64], forecast2_image[64];
double current_temp, current_temp_night, current_temp_day, speed;
double forecast0_temp_night, forecast0_temp_day, forecast1_temp_night, forecast1_temp_day, forecast2_temp_night, forecast2_temp_day;
int current_code, forecast0_code, forecast1_code, forecast2_code;
int humidity, pressure, windDeg;
/**
 * Output the weather data to the terminal.
 * @param char * data The array of chars with the parsed json
 * @param struct Parsed_Json parsed_json contains all the parsed tokens
 * @return void
 */
void copyWeatherData(char * data, struct Parsed_Json weather) {
     int cityInt = 0,day = 0;
     char output[256], timeString[100];
     int tempDay = 0, tempNight = 0;
     for (int i = 1; i < weather.number_of_tokens; i++) {
        int length = weather.tokens[i].end - weather.tokens[i].start;
        char string[length + 1];

        strncpy(string, data + weather.tokens[i].start, length);
        string[length] = '\0';

        if (check_json_string("country", string, weather.tokens[i]) == 0 && cityInt == 0) {
            key_value(country, data,  weather.tokens[i + 1]);
        }

        if (check_json_string("name", string, weather.tokens[i]) == 0 && cityInt == 0) {
            key_value(city, data,  weather.tokens[i + 1]);
        }

        if (check_json_string("lon", string, weather.tokens[i]) == 0 && cityInt == 0) {
            longitude = key_value_float(data,  weather.tokens[i + 1]);
        }

        if (check_json_string("lat", string, weather.tokens[i]) == 0 && cityInt == 0) {
            latitude = key_value_float(data,  weather.tokens[i + 1]);
        }

        if (check_json_string("timezone", string, weather.tokens[i]) == 0 && cityInt == 0) {
            weatherTimeZone = key_value_int(data,  weather.tokens[i + 1]);
            cityInt = 1;
        }

        if (check_json_string("dt", string, weather.tokens[i]) == 0) {
            key_value(output, data,  weather.tokens[i + 1]);
            makeDayString(output, timeString);

            if(day == 0) snprintf(current_date, 256, "%s", timeString);
            else if(day == 1) snprintf(forecast0_date, 256, "%s", timeString);
            else if(day == 2) snprintf(forecast1_date, 256, "%s", timeString);
            else if(day == 3) snprintf(forecast2_date, 256, "%s", timeString);
            day = day + 1;
        }
        if (check_json_string("sunrise", string, weather.tokens[i]) == 0) {
            key_value(output, data,  weather.tokens[i + 1]);
            makeSunString(output, timeString);
            if(day - 1 == 0) snprintf(sunrise, 256, "%s", timeString);
        }

        if (check_json_string("sunset", string, weather.tokens[i]) == 0) {
            key_value(output, data,  weather.tokens[i + 1]);
            makeSunString(output, timeString);
            if(day - 1 == 0) snprintf(sunset, 256, "%s", timeString);
        }

        if (check_json_string("id", string, weather.tokens[i]) == 0  && cityInt != 0) {
            if(day - 1 == 0){
              current_code = key_value_int(data,  weather.tokens[i + 1]);
              changeToText(current_code, current_text);
            }
            else if(day - 1 == 1){
              forecast0_code = key_value_int(data,  weather.tokens[i + 1]);
              changeToText(forecast0_code, forecast0_text);
            }
            else if(day - 1 == 2){
              forecast1_code = key_value_int(data,  weather.tokens[i + 1]);
              changeToText(forecast1_code, forecast1_text);
            }
            else if(day - 1 == 3){
              forecast2_code = key_value_int(data,  weather.tokens[i + 1]);
              changeToText(forecast2_code, forecast2_text);
            }
        }

        if (check_json_string("icon", string, weather.tokens[i]) == 0 ) {
          if(day - 1 == 0) key_value(current_image, data,  weather.tokens[i + 1]);
          else if(day - 1 == 1) key_value(forecast0_image, data,  weather.tokens[i + 1]);
          else if(day - 1 == 2) key_value(forecast1_image, data,  weather.tokens[i + 1]);
          else if(day - 1 == 3) key_value(forecast2_image, data,  weather.tokens[i + 1]);
        }

        if (check_json_string("temp", string, weather.tokens[i]) == 0 && day - 1 == 0) {
          current_temp = key_value_float(data,  weather.tokens[i + 1]);
        }

        if (check_json_string("day", string, weather.tokens[i]) == 0 ) {
          if(day - 1 == 0) current_temp_day = key_value_float(data,  weather.tokens[i + 1]);
          else if(day - 1 == 1) forecast0_temp_day = key_value_float(data,  weather.tokens[i + 1]);
          else if(day - 1 == 2) forecast1_temp_day = key_value_float(data,  weather.tokens[i + 1]);
          else if(day - 1 == 3) forecast2_temp_day = key_value_float(data,  weather.tokens[i + 1]);
        }

        if (check_json_string("night", string, weather.tokens[i]) == 0 ) {
          if(day - 1 == 0) current_temp_night = key_value_float(data,  weather.tokens[i + 1]);
          else if(day - 1 == 1) forecast0_temp_night = key_value_float(data,  weather.tokens[i + 1]);
          else if(day - 1 == 2) forecast1_temp_night = key_value_float(data,  weather.tokens[i + 1]);
          else if(day - 1 == 3) forecast2_temp_night = key_value_float(data,  weather.tokens[i + 1]);
        }

        if (check_json_string("humidity", string, weather.tokens[i]) == 0 && day - 1 == 0) {
            humidity  = key_value_int(data,  weather.tokens[i + 1]);
        }

        if (check_json_string("pressure", string, weather.tokens[i]) == 0 && day - 1 == 0) {
            pressure  = key_value_int(data,  weather.tokens[i + 1]);
        }

        if (check_json_string("speed", string, weather.tokens[i]) == 0 && day - 1 == 0) {
            speed  = key_value_float(data,  weather.tokens[i + 1]);
        }

        if (check_json_string("deg", string, weather.tokens[i]) == 0 && day - 1 == 0) {
            windDeg  = key_value_int(data,  weather.tokens[i + 1]);
        }
    }

    printf("\n--------------------------------------------------------\n\n");
}
//Wetter Grafik erstellen
int makeWetterGui(int GuiTime, int counter, char* weatherTime)
{
  char OUT[128];
	//Read Daten Beginn (roter Punkt unten rechts)
	GuiTime = WeatherTime;
	if (counter == 0){
		writeScreen(ScreenCounter, 60);
		drawMainScreen();
		drawSquare(760,440,20,20,LIGHT_RED);
		drawCorner(760,440,20,20,WHITE);
		//Wetterdaten einlesen mit Teilen der Software von https://github.com/gabrieledarrigo/c-weather
    // Retrieve the data from the API
    char * weather_data = getWeatherData();

    // Parse the api json result
    struct Parsed_Json weather = parse_json(weather_data, 2048);

    // Copy the result
    copyWeatherData(weather_data, weather);
    makeNowString(weatherTime);

    // Print weather date
    printf("OpenWeatherMap Time: %s %s", current_date, weatherTime);
    printf(" %4.5f %4.5f %s %s\n", latitude, longitude, city, country);
    printf(" %.1f %i %.2f %i\n", humidity, pressure, speed, windDeg);
    printf(" %s %.0f %.0f %.0f %s %s %i\n", current_date, current_temp, current_temp_night, current_temp_day, current_text, current_image, current_code);
    printf(" %s %.0f %.0f %s %s %i\n", forecast0_date, forecast0_temp_night, forecast0_temp_day, forecast0_text, forecast0_image, forecast0_code);
    printf(" %s %.0f %.0f %s %s %i\n", forecast1_date, forecast1_temp_night, forecast1_temp_day, forecast1_text, forecast1_image, forecast1_code);
    printf(" %s %.0f %.0f %s %s %i\n", forecast2_date, forecast2_temp_night, forecast2_temp_day, forecast2_text, forecast2_image, forecast2_code);

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
		snprintf(OUT, (size_t)64, "     %s", current_date);
		put_string(WetterS1, WetterZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%s", current_image);
		DrawImage(OUT, WetterS1+70, WetterZ2-10);
    snprintf(OUT, (size_t)64, "%s ", current_text);
		put_string(WetterS1, WetterZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Tag  :    %.0f%cC", current_temp_day, 248);
		put_string(WetterS1, WetterZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Nacht:    %.0f%cC", current_temp_night, 248);
		put_string(WetterS1, WetterZ6, OUT, GREY);

		snprintf(OUT, (size_t)64, "%s", city);
		put_string(WetterS2, WetterZ2, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s", country);
		put_string(WetterS2, WetterZ3, OUT, GREY);
    snprintf(OUT, (size_t)64, "Location-ID: %i", weatherID);
		put_string(WetterS2, WetterZ4, OUT, GREY);
    snprintf(OUT, (size_t)64, "Latitude : %4.5f", latitude);
		put_string(WetterS2, WetterZ5, OUT, GREY);
    snprintf(OUT, (size_t)64, "Longitude: %4.5f", longitude);
		put_string(WetterS2, WetterZ6, OUT, GREY);


		snprintf(OUT, (size_t)64, "Luftfeuchtigkeit:     %i %%", humidity);
		put_string(WetterS3-70, WetterZ2, OUT, GREY);
		snprintf(OUT, (size_t)64, "Luftdruck:            %i hPa", pressure);
		put_string(WetterS3-70, WetterZ3, OUT, GREY);
		snprintf(OUT, (size_t)64, "Windgeschwindigkeit:  %.1f m/s", speed);
		put_string(WetterS3-70, WetterZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Windrichtung:         %i%c ", windDeg, 248);
		put_string(WetterS3-70, WetterZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "Sonnenaufgang:        %s", sunrise);
		put_string(WetterS3-70, WetterZ6, OUT, GREY);
		snprintf(OUT, (size_t)64, "Sonnenuntergang:      %s", sunset);
		put_string(WetterS3-70, WetterZ7, OUT, GREY);

		snprintf(OUT, (size_t)64, "     %s", forecast0_date);
		put_string(WetterS1, WetterUZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%s", forecast0_image);
		DrawImage(OUT, WetterS1+70, WetterUZ2-10);
    snprintf(OUT, (size_t)64, "%s ", forecast0_text);
		put_string(WetterS1, WetterUZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Tag  :    %.0f%cC", forecast0_temp_day, 248);
		put_string(WetterS1, WetterUZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Nacht:    %.0f%cC", forecast0_temp_night, 248);
		put_string(WetterS1, WetterUZ6, OUT, GREY);

		snprintf(OUT, (size_t)64, "     %s", forecast1_date);
		put_string(WetterS2, WetterUZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%s", forecast1_image);
		DrawImage(OUT, WetterS2+70, WetterUZ2-10);
    snprintf(OUT, (size_t)64, "%s ", forecast1_text);
		put_string(WetterS2, WetterUZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Tag  :    %.0f%cC", forecast1_temp_day, 248);
		put_string(WetterS2, WetterUZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Nacht:    %.0f%cC", forecast1_temp_night, 248);
		put_string(WetterS2, WetterUZ6, OUT, GREY);

		snprintf(OUT, (size_t)64, "     %s", forecast2_date);
		put_string(WetterS3, WetterUZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%s", forecast2_image);
		DrawImage(OUT, WetterS3+70, WetterUZ2-10);
    snprintf(OUT, (size_t)64, "%s ", forecast2_text);
		put_string(WetterS3, WetterUZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Tag  :    %.0f%cC", forecast2_temp_day, 248);
		put_string(WetterS3, WetterUZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Nacht:    %.0f%cC", forecast2_temp_night, 248);
		put_string(WetterS3, WetterUZ6, OUT, GREY);
	}
	return GuiTime;

}
#endif // __WetterGui_H_

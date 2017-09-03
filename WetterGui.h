#ifndef __WETTERGUI_H_
#define __WETTERGUI_H_

//Übersetzung  Umlaute \204=ä \216=Ä \224=ö \231=Ö \201=ü \232=Ü \341=ß
int interpreter(char* read)
{
	if (strcmp ("AM Clouds/PM Sun ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags bew\224lkt/nachmittags sonnig");
	else if (strcmp ("AM Drizzle  ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags Nieselregen");
	else if (strcmp ("AM Drizzle/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "vorm. Nieselregen/Wind");
	else if (strcmp ("AM Fog/PM Clouds ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags Nebel/nachmittags bew\224lkt");
	else if (strcmp ("AM Fog/PM Sun ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags Nebel, nachmittags sonnig");
	else if (strcmp ("AM Ice  ",read) == 0)
	  snprintf(read, (size_t)64, "vorm. Eis");
	else if (strcmp ("AM Light Rain ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags leichter Regen");
	else if (strcmp ("AM Light Rain/Wind ",read) == 0)
	  snprintf(read, (size_t)64, "vorm. leichter Regen/Wind");
	else if (strcmp ("AM Light Snow ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags leichter Schneefall");
	else if (strcmp ("AM Rain  ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags Regen");
	else if (strcmp ("AM Rain/Snow Showers ",read) == 0)
	  snprintf(read, (size_t)64, "vorm. Regen-/Schneeschauer");
	else if (strcmp ("AM Rain/Snow  ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags Regen/Schnee");
	else if (strcmp ("AM Rain/Snow/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "vorm. Regen/Schnee/Wind");
	else if (strcmp ("AM Rain/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "vorm. Regen/Wind");
	else if (strcmp ("AM Showers  ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags Schauer");
	else if (strcmp ("AM Showers/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags Schauer/Wind");
	else if (strcmp ("AM Snow Showers ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags Schneeschauer");
	else if (strcmp ("AM Snow  ",read) == 0)
	  snprintf(read, (size_t)64, "vormittags Schnee");
	else if (strcmp ("AM Thundershowers  ",read) == 0)
	  snprintf(read, (size_t)64, "vorm. Gewitterschauer");
	else if (strcmp ("Blowing Snow  ",read) == 0)
	  snprintf(read, (size_t)64, "Schneetreiben");
	else if (strcmp ("Clear   ",read) == 0)
	  snprintf(read, (size_t)64, "Klar");
	else if (strcmp ("Clear/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Klar/Windig");
	else if (strcmp ("Clouds Early/Clearing Late ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Wolken/sp\204ter klar");
	else if (strcmp ("Cloudy   ",read) == 0)
	  snprintf(read, (size_t)64, "Bew\224lkt");
	else if (strcmp ("Cloudy/Wind   ",read) == 0)
	  snprintf(read, (size_t)64, "Bew\224lkt/Wind");
	else if (strcmp ("Cloudy/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Wolkig/Windig");
	else if (strcmp ("Drifting Snow  ",read) == 0)
	  snprintf(read, (size_t)64, "Schneetreiben");
	else if (strcmp ("Drifting Snow/Windy  ",read) == 0)
	  snprintf(read, (size_t)64, "Schneetreiben/Windig");
	else if (strcmp ("Drizzle Early  ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Nieselregen");
	else if (strcmp ("Drizzle Late  ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Nieselregen");
	else if (strcmp ("Drizzle   ",read) == 0)
	  snprintf(read, (size_t)64, "Nieselregen");
	else if (strcmp ("Drizzle/Fog   ",read) == 0)
	  snprintf(read, (size_t)64, "Nieselregen/Nebel");
	else if (strcmp ("Drizzle/Wind   ",read) == 0)
	  snprintf(read, (size_t)64, "Nieselregen/Wind");
	else if (strcmp ("Drizzle/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Nieselregen/Windig");
	else if (strcmp ("Fair   ",read) == 0)
	  snprintf(read, (size_t)64, "Heiter");
	else if (strcmp ("Fair/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Heiter/Windig");
	else if (strcmp ("Few Showers  ",read) == 0)
	  snprintf(read, (size_t)64, "vereinzelte Schauer");
	else if (strcmp ("Few Showers/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "vereinzelte Schauer/Wind");
	else if (strcmp ("Few Snow Showers ",read) == 0)
	  snprintf(read, (size_t)64, "vereinzelt Schneeschauer");
	else if (strcmp ("Fog Early/Clouds Late ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Nebel, später Wolken");
	else if (strcmp ("Fog Late  ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter neblig");
	else if (strcmp ("Fog   ",read) == 0)
	  snprintf(read, (size_t)64, "Nebel");
	else if (strcmp ("Fog/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Nebel/Windig");
	else if (strcmp ("Foggy   ",read) == 0)
	  snprintf(read, (size_t)64, "neblig");
	else if (strcmp ("Freezing Drizzle  ",read) == 0)
	  snprintf(read, (size_t)64, "gefrierender Nieselregen");
	else if (strcmp ("Freezing Drizzle/Windy  ",read) == 0)
	  snprintf(read, (size_t)64, "gefrierender Nieselregen/Windig");
	else if (strcmp ("Freezing Rain  ",read) == 0)
	  snprintf(read, (size_t)64, "gefrierender Regen");
	else if (strcmp ("Haze   ",read) == 0)
	  snprintf(read, (size_t)64, "Dunst");
	else if (strcmp ("Heavy Drizzle  ",read) == 0)
	  snprintf(read, (size_t)64, "starker Nieselregen");
	else if (strcmp ("Heavy Rain Shower ",read) == 0)
	  snprintf(read, (size_t)64, "Starker Regenschauer");
	else if (strcmp ("Heavy Rain  ",read) == 0)
	  snprintf(read, (size_t)64, "Starker Regen");
	else if (strcmp ("Heavy Rain/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "starker Regen/Wind");
	else if (strcmp ("Heavy Rain/Windy  ",read) == 0)
	  snprintf(read, (size_t)64, "Starker Regen/Windig");
	else if (strcmp ("Heavy Snow Shower ",read) == 0)
	  snprintf(read, (size_t)64, "Starker Schneeschauer");
	else if (strcmp ("Heavy Snow  ",read) == 0)
	  snprintf(read, (size_t)64, "Starker Schneefall");
	else if (strcmp ("Heavy Snow/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "Starker Schneefall/Wind");
	else if (strcmp ("Heavy Thunderstorm  ",read) == 0)
	  snprintf(read, (size_t)64, "Schweres Gewitter");
	else if (strcmp ("Heavy Thunderstorm/Windy  ",read) == 0)
	  snprintf(read, (size_t)64, "Schweres Gewitter/Windig");
	else if (strcmp ("Ice Crystals  ",read) == 0)
	  snprintf(read, (size_t)64, "Eiskristalle");
	else if (strcmp ("Ice Late  ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Eis");
	else if (strcmp ("Isolated T-storms  ",read) == 0)
	  snprintf(read, (size_t)64, "Vereinzelte Gewitter");
	else if (strcmp ("Isolated Thunderstorms  ",read) == 0)
	  snprintf(read, (size_t)64, "Vereinzelte Gewitter");
	else if (strcmp ("Light Drizzle  ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Nieselregen");
	else if (strcmp ("Light Freezing Drizzle ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter gefrierender Nieselregen");
	else if (strcmp ("Light Freezing Rain ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter gefrierender Regen");
	else if (strcmp ("Light Freezing Rain/Fog ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter gefrierender Regen/Nebel");
	else if (strcmp ("Light Rain Early ",read) == 0)
	  snprintf(read, (size_t)64, "anfangs leichter Regen");
	else if (strcmp ("Light Rain   ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Regen");
	else if (strcmp ("Light Rain Late ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter leichter Regen");
	else if (strcmp ("Light Rain Shower ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Regenschauer");
	else if (strcmp ("Light Rain Shower/Fog ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Regenschauer/Nebel");
	else if (strcmp ("Light Rain Shower/Windy ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Regenschauer/windig");
	else if (strcmp ("Light Rain with Thunder",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Regen mit Gewitter");
	else if (strcmp ("Light Rain/Fog  ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Regen/Nebel");
	else if (strcmp ("Light Rain/Freezing Rain ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Regen/Gefrierender Regen");
	else if (strcmp ("Light Rain/Wind Early ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h leichter Regen/Wind");
	else if (strcmp ("Light Rain/Wind Late ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter leichter Regen/Wind");
	else if (strcmp ("Light Rain/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "leichter Regen/Wind");
	else if (strcmp ("Light Rain/Windy  ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Regen/Windig");
	else if (strcmp ("Light Sleet  ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneeregen");
	else if (strcmp ("Light Snow Early ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201her leichter Schneefall");
	else if (strcmp ("Light Snow Grains ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneegriesel");
	else if (strcmp ("Light Snow Late ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter leichter Schneefall");
	else if (strcmp ("Light Snow Shower ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneeschauer");
	else if (strcmp ("Light Snow Shower/Fog ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneeschauer/Nebel");
	else if (strcmp ("Light Snow with Thunder",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneefall mit Gewitter");
	else if (strcmp ("Light Snow  ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneefall");
	else if (strcmp ("Light Snow/Fog  ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneefall/Nebel");
	else if (strcmp ("Light Snow/Freezing Rain ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneefall/Gefrierender Regen");
	else if (strcmp ("Light Snow/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneefall/Wind");
	else if (strcmp ("Light Snow/Windy  ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneeschauer/Windig");
	else if (strcmp ("Light Snow/Windy/Fog  ",read) == 0)
	  snprintf(read, (size_t)64, "Leichter Schneefall/Windig/Nebel");
	else if (strcmp ("Mist   ",read) == 0)
	  snprintf(read, (size_t)64, "Nebel");
	else if (strcmp ("Breezy   ",read) == 0)
	  snprintf(read, (size_t)64, "Windig");
	else if (strcmp ("Mostly Clear  ",read) == 0)
	  snprintf(read, (size_t)64, "\232berwiegend Klar");
	else if (strcmp ("Mostly Cloudy  ",read) == 0)
	  snprintf(read, (size_t)64, "\232berwiegend bew\224lkt");
	else if (strcmp ("Mostly Cloudy/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "meist bew\224lkt/Wind");
	else if (strcmp ("Mostly Sunny  ",read) == 0)
	  snprintf(read, (size_t)64, "\232berwiegend Sonnig");
	else if (strcmp ("Partial Fog  ",read) == 0)
	  snprintf(read, (size_t)64, "teilweise Nebel");
	else if (strcmp ("Partly Cloudy  ",read) == 0)
	  snprintf(read, (size_t)64, "Teilweise bew\224lkt");
	else if (strcmp ("Partly Cloudy/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "teilweise bew\224lkt/Wind");
	else if (strcmp ("Patches of Fog ",read) == 0)
	  snprintf(read, (size_t)64, "Nebelfelder");
	else if (strcmp ("Patches of Fog/Windy ",read) == 0)
	  snprintf(read, (size_t)64, "Nebelfelder/Windig");
	else if (strcmp ("PM Drizzle  ",read) == 0)
	  snprintf(read, (size_t)64, "nachm. Nieselregen");
	else if (strcmp ("PM Fog  ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags Nebel");
	else if (strcmp ("PM Light Snow ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags leichter Schneefall");
	else if (strcmp ("PM Light Rain ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags leichter Regen");
	else if (strcmp ("PM Light Rain/Wind ",read) == 0)
	  snprintf(read, (size_t)64, "nachm. leichter Regen/Wind");
	else if (strcmp ("PM Light Snow/Wind ",read) == 0)
	  snprintf(read, (size_t)64, "nachm. leichter Schneefall/Wind");
	else if (strcmp ("PM Rain  ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags Regen");
	else if (strcmp ("PM Rain/Snow Showers ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags Regen/Schneeschauer");
	else if (strcmp ("PM Rain/Snow  ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags Regen/Schnee");
	else if (strcmp ("PM Rain/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "nachm. Regen/Wind");
	else if (strcmp ("PM Showers  ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags Schauer");
	else if (strcmp ("PM Showers/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags Schauer/Wind");
	else if (strcmp ("PM Snow Showers ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags Schneeschauer");
	else if (strcmp ("PM Snow Showers/Wind ",read) == 0)
	  snprintf(read, (size_t)64, "nachm. Schneeschauer/Wind");
	else if (strcmp ("PM Snow  ",read) == 0)
	  snprintf(read, (size_t)64, "nachm. Schnee");
	else if (strcmp ("PM T-storms  ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags Gewitter");
	else if (strcmp ("PM Thundershowers  ",read) == 0)
	  snprintf(read, (size_t)64, "nachmittags Gewitterschauer");
	else if (strcmp ("PM Thunderstorms  ",read) == 0)
	  snprintf(read, (size_t)64, "nachm. Gewitter");
	else if (strcmp ("Rain and Snow ",read) == 0)
	  snprintf(read, (size_t)64, "Schneeregen");
	else if (strcmp ("Rain and Snow/Windy ",read) == 0)
	  snprintf(read, (size_t)64, "Regen und Schnee/Windig");
	else if (strcmp ("Rain/Snow Showers/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "Regen/Schneeschauer/Wind");
	else if (strcmp ("Rain Early  ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Regen");
	else if (strcmp ("Rain Late  ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Regen");
	else if (strcmp ("Rain Shower  ",read) == 0)
	  snprintf(read, (size_t)64, "Regenschauer");
	else if (strcmp ("Rain Shower/Windy  ",read) == 0)
	  snprintf(read, (size_t)64, "Regenschauer/Windig");
	else if (strcmp ("Rain to Snow ",read) == 0)
	  snprintf(read, (size_t)64, "Regen, in Schnee \201bergehend");
	else if (strcmp ("Rain   ",read) == 0)
	  snprintf(read, (size_t)64, "Regen");
	else if (strcmp ("Rain/Snow Early  ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Regen/Schnee");
	else if (strcmp ("Rain/Snow Late  ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Regen/Schnee");
	else if (strcmp ("Rain/Snow Showers Early ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Regen-/Schneeschauer");
	else if (strcmp ("Rain/Snow Showers Late ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Regen-/Schneeschnauer");
	else if (strcmp ("Rain/Snow Showers  ",read) == 0)
	  snprintf(read, (size_t)64, "Regen/Schneeschauer");
	else if (strcmp ("Rain/Snow  ",read) == 0)
	  snprintf(read, (size_t)64, "Regen/Schnee");
	else if (strcmp ("Rain/Snow/Wind   ",read) == 0)
	  snprintf(read, (size_t)64, "Regen/Schnee/Wind");
	else if (strcmp ("Rain/Thunder   ",read) == 0)
	  snprintf(read, (size_t)64, "Regen/Gewitter");
	else if (strcmp ("Rain/Wind Early  ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Regen/Wind");
	else if (strcmp ("Rain/Wind Late  ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Regen/Wind");
	else if (strcmp ("Rain/Wind   ",read) == 0)
	  snprintf(read, (size_t)64, "Regen/Wind");
	else if (strcmp ("Rain/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Regen/Windig");
	else if (strcmp ("Scattered Showers  ",read) == 0)
	  snprintf(read, (size_t)64, "vereinzelte Schauer");
	else if (strcmp ("Scattered Showers/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "vereinzelte Schauer/Wind");
	else if (strcmp ("Scattered Snow Showers ",read) == 0)
	  snprintf(read, (size_t)64, "vereinzelte Schneeschauer");
	else if (strcmp ("Scattered Snow Showers/Wind ",read) == 0)
	  snprintf(read, (size_t)64, "vereinzelte Schneeschauer/Wind");
	else if (strcmp ("Scattered T-storms  ",read) == 0)
	  snprintf(read, (size_t)64, "vereinzelte Gewitter");
	else if (strcmp ("Scattered Thunderstorms  ",read) == 0)
	  snprintf(read, (size_t)64, "vereinzelte Gewitter");
	else if (strcmp ("Shallow Fog  ",read) == 0)
	  snprintf(read, (size_t)64, "flacher Nebel");
	else if (strcmp ("Showers   ",read) == 0)
	  snprintf(read, (size_t)64, "Schauer");
	else if (strcmp ("Showers Early  ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Schauer");
	else if (strcmp ("Showers Late  ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Schauer");
	else if (strcmp ("Showers in the Vicinity",read) == 0)
	  snprintf(read, (size_t)64, "Regenf\204lle in der N\204he");
	else if (strcmp ("Showers/Wind   ",read) == 0)
	  snprintf(read, (size_t)64, "Schauer/Wind");
	else if (strcmp ("Sleet and Freezing Rain",read) == 0)
	  snprintf(read, (size_t)64, "Schneeregen und gefrierender Regen");
	else if (strcmp ("Sleet/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Schneeregen/Windig");
	else if (strcmp ("Sleet   ",read) == 0)
	  snprintf(read, (size_t)64, "Schneeregen");
	else if (strcmp ("Snow Grains  ",read) == 0)
	  snprintf(read, (size_t)64, "Schneegriesel");
	else if (strcmp ("Snow Late  ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Schnee");
	else if (strcmp ("Snow Shower  ",read) == 0)
	  snprintf(read, (size_t)64, "Schneeschauer");
	else if (strcmp ("Snow Showers Early ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Schneeschauer");
	else if (strcmp ("Snow Showers Late ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Schneeschauer");
	else if (strcmp ("Snow Showers  ",read) == 0)
	  snprintf(read, (size_t)64, "Schneeschauer");
	else if (strcmp ("Snow Showers/Wind  ",read) == 0)
	  snprintf(read, (size_t)64, "Schneeschauer/Wind");
	else if (strcmp ("Snow to Rain ",read) == 0)
	  snprintf(read, (size_t)64, "Schneeregen");
	else if (strcmp ("Snow   ",read) == 0)
	  snprintf(read, (size_t)64, "Schneefall");
	else if (strcmp ("Snow/Wind   ",read) == 0)
	  snprintf(read, (size_t)64, "Schneefall/Wind");
	else if (strcmp ("Snow/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Schnee/Windig");
	else if (strcmp ("Squalls   ",read) == 0)
	  snprintf(read, (size_t)64, "B\224en");
	else if (strcmp ("Sunny   ",read) == 0)
	  snprintf(read, (size_t)64, "Sonnig");
	else if (strcmp ("Sunny/Wind   ",read) == 0)
	  snprintf(read, (size_t)64, "Sonnig/Wind");
	else if (strcmp ("Sunny/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Sonnig/Windig");
	else if (strcmp ("T-showers   ",read) == 0)
	  snprintf(read, (size_t)64, "Gewitterschauer");
	else if (strcmp ("Thunder in the Vicinity",read) == 0)
	  snprintf(read, (size_t)64, "Gewitter in der Umgebung");
	else if (strcmp ("Thunder   ",read) == 0)
	  snprintf(read, (size_t)64, "Gewitter");
	else if (strcmp ("ThunderShowers Early  ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Gewitterschauer");
	else if (strcmp ("Thundershowers   ",read) == 0)
	  snprintf(read, (size_t)64, "Gewitterschauer");
	else if (strcmp ("Thunderstorm   ",read) == 0)
	  snprintf(read, (size_t)64, "Gewitter");
	else if (strcmp ("Thunderstorm/Windy   ",read) == 0)
	  snprintf(read, (size_t)64, "Gewitter/Windig");
	else if (strcmp ("Thunderstorms Early  ",read) == 0)
	  snprintf(read, (size_t)64, "fr\201h Gewitter");
	else if (strcmp ("Thunderstorms Late  ",read) == 0)
	  snprintf(read, (size_t)64, "sp\204ter Gewitter");
	else if (strcmp ("Thunderstorms   ",read) == 0)
	  snprintf(read, (size_t)64, "Gewitter");
	else if (strcmp ("Wintry Mix  ",read) == 0)
	  snprintf(read, (size_t)64, "Winterlicher Mix");
	return 1;
}

//Wetter Grafik erstellen
int makeWetterGui(int GuiTime, int counter, char* weatherTime)
{
	//Difiniton der Variablen die auf dem Display angezeigt werden sollen.
	char country[64], region[64], city[64], current_date[64], current_text[64], direction[64], sunrise[64], sunset[64];
  char forecast1_date[64], forecast1_text[64], forecast2_date[64], forecast2_text[64], forecast3_date[64], forecast3_text[64];
  double longitude, latitude, visibility, current_temp, current_temp_low, current_temp_high, speed;
  double forecast1_temp_low, forecast1_temp_high, forecast2_temp_low, forecast2_temp_high, forecast3_temp_low, forecast3_temp_high;
  int humidity, pressure, current_image, forecast1_image, forecast2_image, forecast3_image;
  char OUT[128], value[128], read[128], read1[64], read2[64], read3[64], read4[64], read5[64], read6[64], off[128], batch[256];
	//Read Daten Beginn (roter Punkt unten rechts)
	GuiTime = WeatherTime;
	if (counter == 0){
		writeScreen(ScreenCounter, 900);
		drawMainScreen();
		drawSquare(760,440,20,20,LIGHT_RED);
		drawCorner(760,440,20,20,WHITE);
		//Wetterdaten einlesen
		memset(&batch, 0, sizeof(batch));
		snprintf(batch, (size_t)256, "curl http://weather.tuxnet24.de/?id=%i", weatherID);
		FILE *out = NULL;
		out = popen( batch, "r" );
		if( out != NULL ){
			fgets(read, 128, out);                          //longitude
			sscanf(read, "longitude = %s\n", value);
			longitude = atof(value);
			fgets(read, 128, out);                          //latitude
			sscanf(read, "latitude = %s\n", value);
			latitude = atof(value);
			fgets(read, 128, out);                          //country
			sscanf(read, "country = %s", country);
			fgets(read, 128, out);                          //region
			sscanf(read, "region = %s", region);
			fgets(read, 128, out);                          //city
			sscanf(read, "city = %s", city);
			fgets(read, 128, out);                          //humidity
			sscanf(read, "humidity = %s%%", value);
			humidity = atoi(value);
			fgets(off, 128, out);                           //visibility
			sscanf(read, "visibility = %s km", value);
			visibility = atof(value);
			fgets(read, 128, out);                          //pressure
			sscanf(read, "pressure = %s mb", value);
			pressure = atoi(value);
			fgets(off, 128, out);                           //rising
			fgets(read, 128, out);                           //sunrise
			sscanf(read, "sunrise = %s %s", read1, read2);
			snprintf(sunrise, (size_t)64, "%s %s", read1, read2);
			fgets(read, 128, out);                           //sunset
			sscanf(read, "sunset = %s %s", read1, read2);
			snprintf(sunset, (size_t)64, "%s %s", read1, read2);
			fgets(read, 128, out);                          //current_code
			sscanf(read, "current_code = %s", value);
			current_image = atoi(value);
			fgets(read, 128, out);                          //current_date
			sscanf(read, "current_date = %s %s %s %s %s %s %s", off, read, read1, read2, read3, read4, read5);
			snprintf(weatherTime, (size_t)64, "%s %s %s %s %s %s", read, read1, read2, read3, read4, read5);
			fgets(read, 128, out);                          //current_temp
			sscanf(read, "current_temp = %s", value);
			current_temp = atof(value);
			fgets(read, 128, out);                           //current_temp_low
			sscanf(read, "current_temp_low = %s", value);
			current_temp_low = atof(value);
			fgets(read, 128, out);                           //current_temp_high
			sscanf(read, "current_temp_high = %s", value);
			current_temp_high = atof(value);
			fgets(read, 128, out);                          //current_text
			memset(read2, 0, sizeof read2);
			memset(read3, 0, sizeof read3);
			memset(read4, 0, sizeof read4);
			sscanf(read, "current_text = %s %s %s %s", read1, read2, read3, read4);
			snprintf(current_text, (size_t)64, "%s %s %s %s", read1, read2, read3, read4);
			interpreter(current_text);
			fgets(off, 128, out);                           //current_image
			fgets(read, 128, out);                          //speed
			sscanf(read, "speed = %s km/h", value);
			speed = atof(value);
			fgets(read, 128, out);                          //direction
			sscanf(read, "direction = %s ", direction);
			strtok(direction, "°");
			fgets(off, 128, out);                           //chill
			fgets(off, 128, out);                           //tomorrow_day
			fgets(off, 128, out);                           //tomorrow_date
			fgets(off, 128, out);                           //tomorrow_temp_low
			fgets(off, 128, out);                           //tomorrow_temp_high
			fgets(off, 128, out);                           //tomorrow_text
			fgets(off, 128, out);                           //tomorrow_code
			fgets(off, 128, out);                           //tomorrow_image
			fgets(read, 128, out);                           //forecast0_day
			sscanf(read, "forecast0_day = %s", read1);
			fgets(read, 128, out);                           //forecast0_date
			sscanf(read, "forecast0_date = %s %s %s", read2, read3, read4);
			snprintf(current_date, (size_t)64, "%s %s %s %s", read1, read2, read3, read4);
			fgets(off, 128, out);                           //forecast0_temp_low
			fgets(off, 128, out);                           //forecast0_temp_high
			fgets(off, 128, out);                           //forecast0_text
			fgets(off, 128, out);                           //forecast0_code
			fgets(off, 128, out);                           //forecast0_image
			fgets(read, 128, out);                           //forecast1_day
			sscanf(read, "forecast1_day = %s", read1);
			fgets(read, 128, out);                           //forecast1_date
			sscanf(read, "forecast1_date = %s %s %s", read2, read3, read4);
			snprintf(forecast1_date, (size_t)64, "%s %s %s %s", read1, read2, read3, read4);
			fgets(read, 128, out);                           //forecast1_temp_low
			sscanf(read, "forecast1_temp_low = %s", value);
			forecast1_temp_low = atof(value);
			fgets(read, 128, out);                           //forecast1_temp_high
			sscanf(read, "forecast1_temp_high = %s", value);
			forecast1_temp_high = atof(value);
			fgets(read, 128, out);                           //forecast1_text
			memset(read2, 0, sizeof read2);
			memset(read3, 0, sizeof read3);
			memset(read4, 0, sizeof read4);
			sscanf(read, "forecast1_text = %s %s %s %s", read1, read2, read3, read4);
			snprintf(forecast1_text, (size_t)64, "%s %s %s %s", read1, read2, read3, read4);
			interpreter(forecast1_text);
			fgets(read, 128, out);                           //forecast1_code
			sscanf(read, "forecast1_code = %s", value);
			forecast1_image = atoi(value);
			fgets(off, 128, out);                            //forecast1_image
			fgets(read, 128, out);                           //forecast2_day
			sscanf(read, "forecast2_day = %s", read1);
			fgets(read, 128, out);                           //forecast2_date
			sscanf(read, "forecast2_date = %s %s %s", read2, read3, read4);
			snprintf(forecast2_date, (size_t)64, "%s %s %s %s", read1, read2, read3, read4);
			fgets(read, 128, out);                           //forecast2_temp_low
			sscanf(read, "forecast2_temp_low = %s", value);
			forecast2_temp_low = atof(value);
			fgets(read, 128, out);                           //forecast2_temp_high
			sscanf(read, "forecast2_temp_high = %s", value);
			forecast2_temp_high = atof(value);
			fgets(read, 128, out);                           //forecast2_text
			memset(read2, 0, sizeof read2);
			memset(read3, 0, sizeof read3);
			memset(read4, 0, sizeof read4);
			sscanf(read, "forecast2_text = %s %s %s %s", read1, read2, read3, read4);
			snprintf(forecast2_text, (size_t)64, "%s %s %s %s", read1, read2, read3, read4);
			interpreter(forecast2_text);
			fgets(read, 128, out);                           //forecast2_code
			sscanf(read, "forecast2_code = %s", value);
			forecast2_image = atoi(value);
			fgets(off, 128, out);                            //forecast2_image
			fgets(read, 128, out);                           //forecast3_day
			sscanf(read, "forecast3_day = %s", read1);
			fgets(read, 128, out);                           //forecast3_date
			sscanf(read, "forecast3_date = %s %s %s", read2, read3, read4);
			snprintf(forecast3_date, (size_t)64, "%s %s %s %s", read1, read2, read3, read4);
			fgets(read, 128, out);                           //forecast3_temp_low
			sscanf(read, "forecast3_temp_low = %s", value);
			forecast3_temp_low = atof(value);
			fgets(read, 128, out);                           //forecast3_temp_high
			sscanf(read, "forecast3_temp_high = %s", value);
			forecast3_temp_high = atof(value);
			fgets(read, 128, out);                           //forecast3_text
			memset(read2, 0, sizeof read2);
			memset(read3, 0, sizeof read3);
			memset(read4, 0, sizeof read4);
			sscanf(read, "forecast3_text = %s %s %s %s", read1, read2, read3, read4);
			snprintf(forecast3_text, (size_t)64, "%s %s %s %s", read1, read2, read3, read4);
			interpreter(forecast3_text);
			fgets(read, 128, out);                           //forecast3_code
			sscanf(read, "forecast3_code = %s", value);
			forecast3_image = atoi(value);
			fgets(off, 128, out);                           //forecast3_image
			pclose(out);
		}
		printf("Yahoo Weather Time: %s\n", weatherTime);
		printf(" %4.5f\n %4.5f\n %s\n %s\n %s\n %i\n %i\n %.2f\n %s\n", longitude, latitude, country, region, city, humidity, pressure, speed, direction);
		printf(" %s\n %.0f\n %.0f\n %.0f\n %s\n %i\n", current_date, current_temp, current_temp_low, current_temp_high, current_text, current_image);
		printf(" %s\n %.0f\n %.0f\n %s\n %i\n", forecast1_date, forecast1_temp_low, forecast1_temp_high, forecast1_text, forecast1_image);
		printf(" %s\n %.0f\n %.0f\n %s\n %i\n", forecast2_date, forecast2_temp_low, forecast2_temp_high, forecast2_text, forecast2_image);
		printf(" %s\n %.0f\n %.0f\n %s\n %i\n", forecast3_date, forecast3_temp_low, forecast3_temp_high, forecast3_text, forecast3_image);

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
		snprintf(OUT, (size_t)64, "Wetter/%i", current_image);
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
		put_string(WetterS2, WetterZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s   %s", region, country);
		put_string(WetterS2, WetterZ2, OUT, GREY);


		snprintf(OUT, (size_t)64, "Luftfeuchtigkeit:     %i %%", humidity);
		put_string(WetterS3-70, WetterZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Fernsicht:            %.0f km", visibility);
		put_string(WetterS3-70, WetterZ2, OUT, GREY);
		snprintf(OUT, (size_t)64, "Luftdruck:            %i hPa", pressure);
		put_string(WetterS3-70, WetterZ3, OUT, GREY);
		snprintf(OUT, (size_t)64, "Windgeschwindigkeit:  %.1f km/h", speed);
		put_string(WetterS3-70, WetterZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Windrichtung:         %s%c ", direction, 248);
		put_string(WetterS3-70, WetterZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "Sonnenaufgang:        %s", sunrise);
		put_string(WetterS3-70, WetterZ6, OUT, GREY);
		snprintf(OUT, (size_t)64, "Sonnenuntergang:      %s", sunset);
		put_string(WetterS3-70, WetterZ7, OUT, GREY);

		snprintf(OUT, (size_t)64, "     %s", forecast1_date);
		put_string(WetterS1, WetterUZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%i", forecast1_image);
		DrawImage(OUT, WetterS1+70, WetterUZ2-10);
		snprintf(OUT, (size_t)64, "Temperatur Max.:     %.0f%cC", forecast1_temp_high, 248);
		put_string(WetterS1, WetterUZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Min.:     %.0f%cC", forecast1_temp_low, 248);
		put_string(WetterS1, WetterUZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s ", forecast1_text);
		put_string(WetterS1, WetterUZ6, OUT, GREY);

		snprintf(OUT, (size_t)64, "     %s", forecast2_date);
		put_string(WetterS2, WetterUZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%i", forecast2_image);
		DrawImage(OUT, WetterS2+70, WetterUZ2-10);
		snprintf(OUT, (size_t)64, "Temperatur Max.:     %.0f%cC", forecast2_temp_high, 248);
		put_string(WetterS2, WetterUZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Min.:     %.0f%cC", forecast2_temp_low, 248);
		put_string(WetterS2, WetterUZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s ", forecast2_text);
		put_string(WetterS2, WetterUZ6, OUT, GREY);

		snprintf(OUT, (size_t)64, "     %s", forecast3_date);
		put_string(WetterS3, WetterUZ1, OUT, GREY);
		snprintf(OUT, (size_t)64, "Wetter/%i", forecast3_image);
		DrawImage(OUT, WetterS3+70, WetterUZ2-10);
		snprintf(OUT, (size_t)64, "Temperatur Max.:     %.0f%cC", forecast3_temp_high, 248);
		put_string(WetterS3, WetterUZ4, OUT, GREY);
		snprintf(OUT, (size_t)64, "Temperatur Min.:     %.0f%cC", forecast3_temp_low, 248);
		put_string(WetterS3, WetterUZ5, OUT, GREY);
		snprintf(OUT, (size_t)64, "%s ", forecast3_text);
		put_string(WetterS3, WetterUZ6, OUT, GREY);
	}
	return GuiTime;

}
#endif // __WetterGui_H_

#ifndef __DHT11_H_
#define __DHT11_H_

/*
		Programmbasis http://www.uugear.com/portfolio/dht11-humidity-temperature-sensor-module/
 *  dht11.c is simple program read date from dht11
 */

#define MAXTIMINGS	85

int dht11_dat[5] = { 0, 0, 0, 0, 0 };

int writeDHT11(int DHT11Position, int NewValue)
{
  BitWrite("/mnt/RAMDisk/DHT11.txt", DHT11Position, NewValue, DHT11MAX);
  return 1;
}
int readDHT11(int DHT11Position)
{
	int ret = BitRead("/mnt/RAMDisk/DHT11.txt", DHT11Position, DHT11MAX);
  return ret;
}
int makeDHT11()
{
	BitMake("/mnt/RAMDisk/DHT11.txt", DHT11MAX);
  return 0;
}

int read_dht11_dat(int DHTPin, char* hum, char* temp)
{
	if ( wiringPiSetup() == -1 )
		return -1;

	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	//float	f; /* fahrenheit */
	char OUT[56];

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	/* pull pin down for 18 milliseconds */
	pinMode( DHTPin, OUTPUT );
	digitalWrite( DHTPin, LOW );
	delay( 18 );
	/* then pull it up for 40 microseconds */
	digitalWrite( DHTPin, HIGH );
	delayMicroseconds( 20 );
	/* prepare to read the pin */
	pinMode( DHTPin, INPUT );
	//pullUpDnControl ( DHTPin, PUD_UP);
	/* detect change and read data */
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead( DHTPin ) == laststate )
		{
			counter++;
			delayMicroseconds( 2 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( DHTPin );

		if ( counter == 255 )
			break;

		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			dht11_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}

	/* check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
	{
		//f = dht11_dat[2] * 9. / 5. + 32;
		//printf( "Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",
			//dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
		snprintf(hum, (size_t)56, "%d.%d", dht11_dat[0], dht11_dat[1]);
		snprintf(temp, (size_t)56, "%d.%d", dht11_dat[2], dht11_dat[3]);
		return 1;
	}else  {
		//printf( "Data not good, skip\n" );
		hum = 0;
		temp = 0;
		return 0;
	}
}

int saveDHT11(int DHTUse, int DHTPosition, int DHTPin)
{
  if (DHTUse == false)
    return 0;
	//printf( "Raspberry Pi wiringPi DHT11 Temperature Program\n" );
	char read1[56], read2[56];
	float humf = 0, tempf = 0;
	int ret;
  int i;

	for( i = 0; i < 10; ++i )
	{
		ret = read_dht11_dat( DHTPin, read1, read2);
		float hum = atof(read1);
		float temp = atof(read2);
		writeDHT11(DHTPosition,temp*10);
    writeDHT11(DHTPosition +1,hum*10);
    writeDHT11(DHTPosition +2,ret);
		if(ret == 1){
			printf( "Humidity = %.1f %% Temperature = %.1f *C \n", hum, temp);
			return ret;
		}
		else if (ret == -2)
			printf("wiringPi error\n");
		else if (ret == -1)
			printf("DHT11 error\n" );
		delay( 100 ); /* wait 1sec to refresh */
	}
	return ret;
}
int drawDHTFrame(int DHTUse, char *designation, int Line)
{
  char OUT[56];
  drawSquare(45,Line-20,348,60,GREY);
  drawCorner(45,Line-20,348,60,WHITE);
  drawSquare(45+120,Line-17,225,54,WHITE);
  drawCorner(45+120,Line-17,225,54,GREY);
  put_string(45+6, Line+4, designation, WHITE);
  return 1;
}
int drawDHTState(int DHTUse, int DHTPin, int DHTPosition, char *DHTName, int Line)
{
  char OUT[56];
  if (DHTUse > 0){
    float DHTTemp = readDHT11(DHTPosition)/10.0;
    float DHTHum = readDHT11(DHTPosition+1)/10.0;
    int DHTState = readDHT11(DHTPosition+2);
    if (DHTState == 1){
      drawSquare(45+80, Line,20,20,LIGHT_GREEN);
      drawCorner(45+80, Line,20,20,GREY);
    }
    else {
      drawSquare(45+80, Line,20,20,LIGHT_RED);
      drawCorner(45+80, Line,20,20,GREY);
    }
    drawSquare(45+120,Line-17,225,54,WHITE);
    drawCorner(45+120,Line-17,225,54,GREY);
    createData(205-30, Line-28, DHTName);
    snprintf (OUT, (size_t)100, "Temperatur        %.1f %cC", DHTTemp, 248);
    createData(205-30, Line-16, OUT);
    snprintf (OUT, (size_t)100, "Luftfeuchtigkeit  %.1f %%", DHTHum);
    createData(205-30, Line-4, OUT);
  }
  else {
    drawSquare(45+80, Line,20,20,LTGREY);
    drawCorner(45+80, Line,20,20,GREY);
  }
  return 1;
}
int makeDHTFrame()
{
  if (useDHT11 == 1){
    drawDHTFrame(DHT1Use, "Sensor 1", R1);
    drawDHTFrame(DHT2Use, "Sensor 2", R2);
    drawDHTFrame(DHT3Use, "Sensor 3", R3);
    drawDHTFrame(DHT4Use, "Sensor 4", R4);
    drawDHTFrame(DHT5Use, "Sensor 5", R5);
    return 1;
  }
  return 0;
}
int makeDHTState()
{
  if (useDHT11 == 1){
    //Read Daten Beginn (roter Punkt unten rechts)
    drawSquare(760,440,20,20,LIGHT_RED);
    drawCorner(760,440,20,20,WHITE);
    saveDHT11(DHT1Use, DHT1Position, DHT1Pin);
    drawDHTState(DHT1Use, DHT1Pin, DHT1Position, DHT1Name, R1);
    saveDHT11(DHT2Use, DHT2Position, DHT2Pin);
    drawDHTState(DHT2Use, DHT2Pin, DHT2Position, DHT2Name, R2);
    saveDHT11(DHT3Use, DHT3Position, DHT3Pin);
    drawDHTState(DHT3Use, DHT3Pin, DHT3Position, DHT3Name, R3);
    saveDHT11(DHT4Use, DHT4Position, DHT4Pin);
    drawDHTState(DHT4Use, DHT4Pin, DHT4Position, DHT4Name, R4);
    saveDHT11(DHT5Use, DHT5Position,DHT5Pin);
    drawDHTState(DHT5Use, DHT5Pin, DHT5Position, DHT5Name, R5);
    //Read Daten Ende (grüner Punkt unten rechts)
    drawSquare(760,440,20,20,GREEN);
    drawCorner(760,440,20,20,WHITE);
    return 1;
  }
  return 0;
}
#endif // __DHT11_H_

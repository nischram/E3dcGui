#ifndef __AKTOR_H_
#define __AKTOR_H_

int readAktor(int AktorPosition)
{
	int ret = BitRead("/mnt/RAMDisk/Aktor.txt", AktorPosition, AktorMAX);
  return ret;
}
int writeAktor(int AktorPosition, int NewValue)
{
  BitWrite("/mnt/RAMDisk/Aktor.txt", AktorPosition, NewValue, AktorMAX);
  return 1;
}
int makeAktor()
{
	BitMake("/mnt/RAMDisk/Aktor.txt", AktorMAX);
  return 8;
}

int netInAktor(int PowerGrid, int AktorPrio, int AktorPower, int AktorMinOn, int AktorMinOff, int AktorPin, int AktorPosition)
{
  int NetInput = PowerGrid * -1;
  int AktorState = digitalRead(AktorPin);
	int prio = readAktor(AktorPrioPosition);
	int TimerState = readAktor(AktorPosition + stateTimer);
	if (prio >= AktorPrio && TimerState == true){
		if (AktorState == Deaktiv){
	    int counterMinOff = readAktor(AktorPosition + MinOff);
	    counterMinOff--;
	    writeAktor(AktorPosition + MinOff, counterMinOff);
	    if (NetInput >=  AktorPower && counterMinOff <= 0){
	      int AktorCounterOn = readAktor(AktorPosition + counterOn);
	      AktorCounterOn++;
	      writeAktor(AktorPosition + counterOn, AktorCounterOn);
	      if(AktorCounterOn >= 5){
					if (AktorPrio > 0){
						prio++;
						writeAktor(AktorPrioPosition, prio);
					}
	        digitalWrite( AktorPin, Aktiv);
					sleep(2);
	        writeAktor(AktorPosition + counterOn, 0);
	        writeAktor(AktorPosition + counterOff, 0);
	        writeAktor(AktorPosition + MinOn, (AktorMinOn *60));
	      }
	    }
	  }
	}
  int AktorPower90 = AktorPower /10 *9;
  if (TimerState == true && NetInput <=  AktorPower90 && PowerGrid <=  100){
    writeAktor(AktorPosition + counterOn, 0);
    writeAktor(AktorPosition + counterOff, 0);
  }
  AktorState = digitalRead(AktorPin);
  if (AktorState == Aktiv){
    int counterMinOn = readAktor(AktorPosition + MinOn);
    counterMinOn--;
    writeAktor(AktorPosition + MinOn, counterMinOn);
    if ((PowerGrid >=  100 || TimerState == false)  && counterMinOn <= 0){
      int AktorCounterOff = readAktor(AktorPosition + counterOff);
      AktorCounterOff++;
      writeAktor(AktorPosition + counterOff, AktorCounterOff);
      if(AktorCounterOff >= 30){
				if (AktorPrio > 0 && prio > AktorPrio){
					prio = AktorPrio;
					writeAktor(AktorPrioPosition, prio);
				}
        digitalWrite( AktorPin, Deaktiv);
        writeAktor(AktorPosition + counterOn, 0);
        writeAktor(AktorPosition + counterOff, 0);
        writeAktor(AktorPosition + MinOff, (AktorMinOff *60));
      }
    }
  }
  return 1;
}

int solarAktor(int PowerPVI, int AktorPrio, int AktorPowerOn, int AktorPowerOff, int AktorMinOn, int AktorMinOff, int AktorPin, int AktorPosition)
{
	int AktorState = digitalRead(AktorPin);
	int prio = readAktor(AktorPrioPosition);
	int TimerState = readAktor(AktorPosition + stateTimer);
	if (prio >= AktorPrio && TimerState == true){
		if (AktorState == Deaktiv){
	    int counterMinOff = readAktor(AktorPosition + MinOff);
	    counterMinOff--;
	    writeAktor(AktorPosition + MinOff, counterMinOff);
	    if (PowerPVI >=  AktorPowerOn && counterMinOff <= 0){
	      int AktorCounterOn = readAktor(AktorPosition + counterOn);
	      AktorCounterOn++;
	      writeAktor(AktorPosition + counterOn, AktorCounterOn);
	      if(AktorCounterOn >= 30){
					if (AktorPrio > 0){
						prio++;
						writeAktor(AktorPrioPosition, prio);
					}
	        digitalWrite( AktorPin, Aktiv);
	        writeAktor(AktorPosition + counterOn, 0);
	        writeAktor(AktorPosition + counterOff, 0);
	        writeAktor(AktorPosition + MinOn, (AktorMinOn *60));
	      }
	    }
	  }
	}
  int AktorPower90 = AktorPowerOn /10 *9;
  if (TimerState == true && PowerPVI <=  AktorPower90 && PowerPVI >= AktorPowerOff){
    writeAktor(AktorPosition + counterOn, 0);
    writeAktor(AktorPosition + counterOff, 0);
  }
  AktorState = digitalRead(AktorPin);
  if (AktorState == Aktiv){
    int counterMinOn = readAktor(AktorPosition + MinOn);
    counterMinOn--;
    writeAktor(AktorPosition + MinOn, counterMinOn);
    if ((PowerPVI <= AktorPowerOff || TimerState == false) && counterMinOn <= 0){
      int AktorCounterOff = readAktor(AktorPosition + counterOff);
      AktorCounterOff++;
      writeAktor(AktorPosition + counterOff, AktorCounterOff);
      if(AktorCounterOff >= 30){
				if (AktorPrio > 0 && prio > AktorPrio){
					prio = AktorPrio;
					writeAktor(AktorPrioPosition, prio);
				}
        digitalWrite( AktorPin, Deaktiv);
        writeAktor(AktorPosition + counterOn, 0);
        writeAktor(AktorPosition + counterOff, 0);
        writeAktor(AktorPosition + MinOff, (AktorMinOff *60));
      }
    }
  }
  return 1;
}

int batteryAktor(int BatterySOC, int AktorPrio, int AktorPercentOn, int AktorPercentOff, int AktorMinOn, int AktorMinOff, int AktorPin, int AktorPosition)
{
	int AktorState = digitalRead(AktorPin);
	int prio = readAktor(AktorPrioPosition);
	int TimerState = readAktor(AktorPosition + stateTimer);
	if (prio >= AktorPrio && TimerState == true){
	  if (AktorState == Deaktiv){
	    int counterMinOff = readAktor(AktorPosition + MinOff);
	    counterMinOff--;
	    writeAktor(AktorPosition + MinOff, counterMinOff);
	    if (BatterySOC >= AktorPercentOn && counterMinOff <= 0){
	      int AktorCounterOn = readAktor(AktorPosition);
	      AktorCounterOn++;
	      writeAktor(AktorPosition, AktorCounterOn);
	      if (AktorCounterOn >= 5){
					if (AktorPrio > 0){
						prio++;
						writeAktor(AktorPrioPosition, prio);
					}
	        digitalWrite( AktorPin, Aktiv);
	        writeAktor(AktorPosition + counterOn, 0);
	        writeAktor(AktorPosition + counterOff, 0);
	        writeAktor(AktorPosition + MinOn, (AktorMinOn *60));
	      }
	    }
	  }
	}
  if (TimerState == true && BatterySOC < AktorPercentOn && BatterySOC > AktorPercentOff){
    writeAktor(AktorPosition + counterOn, 0);
    writeAktor(AktorPosition + counterOff, 0);
  }
  if (AktorState == Aktiv){
    int counterMinOn = readAktor(AktorPosition + MinOn);
    counterMinOn--;
    writeAktor(AktorPosition + MinOn, counterMinOn);
    if ((BatterySOC <= AktorPercentOff || TimerState == false) && counterMinOn <= 0){
      int AktorCounterOff = readAktor(AktorPosition + counterOff);
      AktorCounterOff++;
      writeAktor(AktorPosition + counterOff, AktorCounterOff);
      if (AktorCounterOff >= 5){
				if (AktorPrio > 0 && prio > AktorPrio){
					prio = AktorPrio;
					writeAktor(AktorPrioPosition, prio);
				}
        digitalWrite( AktorPin, Deaktiv);
        writeAktor(AktorPosition + counterOn, 0);
        writeAktor(AktorPosition + counterOff, 0);
        writeAktor(AktorPosition + MinOff, (AktorMinOff *60));
      }
    }
  }
  return 1;
}

int timerAktor(int AktorPrio, int AktorPin, int AktorPosition)
{
	int AktorState = digitalRead(AktorPin);
	int prio = readAktor(AktorPrioPosition);
	int TimerState = readAktor(AktorPosition + stateTimer);
	if (prio >= AktorPrio && TimerState == true){
	  if (AktorState == Deaktiv){
			if (AktorPrio > 0){
				prio++;
				writeAktor(AktorPrioPosition, prio);
			}
			digitalWrite( AktorPin, Aktiv);
	  }
	}
	if (TimerState == false){
		if (AktorState == Aktiv){
			if (AktorPrio > 0 && prio > AktorPrio){
				prio = AktorPrio;
				writeAktor(AktorPrioPosition, prio);
			}
			digitalWrite( AktorPin, Deaktiv);
	  }
	}
  return 1;
}

int timerState(int AktorTyp, char AktorTimerOn[20], char AktorTimerOff[20], int AktorPosition)
{
	if (AktorTyp >= 10){
		char CLOCK[20], OUT[20];
		time_t currentTime = time(NULL);
		struct tm *now;
		time( &currentTime );
		now = localtime( &currentTime );
		strftime (CLOCK,20,"%H:%M",now);
    replace_character(CLOCK, ':', '0');
    int intCurrentTime = atoi(CLOCK);
    snprintf(OUT, (size_t)20, "%s", AktorTimerOn);
    replace_character(OUT, ':', '0');
    int intTimerOn = atoi(OUT);
    snprintf(OUT, (size_t)20, "%s", AktorTimerOff);
    replace_character(OUT, ':', '0');
    int intTimerOff = atoi(OUT);

		int TimerState = readAktor(AktorPosition + stateTimer);
		if (intCurrentTime >= intTimerOn && intCurrentTime < intTimerOff){
			if (TimerState == false){
				writeAktor(AktorPosition + stateTimer, true);
			}
		}
		else {
			writeAktor(AktorPosition + stateTimer, false);
		}
	}
	else {
		writeAktor(AktorPosition + stateTimer, true);
	}
  return 1;
}

int Aktor(int AktorPrio, int AktorPowerOn, int AktorPowerOff,
              int AktorPercentOn, int AktorPercentOff,
              int AktorMinOn, int AktorMinOff,
              int AktorTyp, int AktorPin, int AktorPosition)
{
  if (AktorTyp > 0){
    if (AktorTyp == 1 || AktorTyp == 11){
			int PowerGrid = readRscp(PosGrid);
      netInAktor(PowerGrid, AktorPrio, AktorPowerOn, AktorMinOn, AktorMinOff, AktorPin, AktorPosition);
		}
    else if (AktorTyp == 2 || AktorTyp == 12){
			int PowerPVI = readRscp(PosPVI);
      solarAktor(PowerPVI, AktorPrio, AktorPowerOn, AktorPowerOff, AktorMinOn, AktorMinOff, AktorPin, AktorPosition);
		}
    else if (AktorTyp == 3 || AktorTyp == 13){
			int BatterySOC = readRscp(PosSOC);
      batteryAktor(BatterySOC, AktorPrio, AktorPercentOn, AktorPercentOff, AktorMinOn, AktorMinOff, AktorPin, AktorPosition);
		}
		else if (AktorTyp == 10)
	     timerAktor(AktorPrio, AktorPin, AktorPosition);
  }
  return 1;
}

int drawAktorFrame(int AktorTyp, int AktorPowerOn, int AktorPowerOff,
              int AktorPercentOn, int AktorPercentOff,
              char AktorTimerOn[20], char AktorTimerOff[20],
							int AktorMinOn, int AktorMinOff,
							char *designation, char *AktorName, int Line)
{
  char OUT[56];
  drawSquare(405,Line-20,363,68,GREY);
  drawCorner(405,Line-20,363,68,WHITE);
  drawSquare(405+120,Line-17,240,62,WHITE);
  drawCorner(405+120,Line-17,240,62,GREY);
  put_string(405+6, Line, designation, WHITE);
  if (AktorTyp > 0){
		put_string(565-30, Line-18, AktorName, GREY);
		if (AktorTyp >= 11){
			snprintf (OUT, (size_t)100, "On:  %s", AktorTimerOn);
			put_string(565+90, Line-3, OUT, GREY);
			snprintf (OUT, (size_t)100, "Off: %s", AktorTimerOff);
			put_string(565+90, Line+12, OUT, GREY);
		}
    if (AktorTyp == 1 || AktorTyp == 11){
			put_string(405+6, Line-16, "\232berschuss", WHITE);
      snprintf (OUT, (size_t)100, "On:  %i W", AktorPowerOn);
			put_string(565-30, Line-3, OUT, GREY);
      snprintf (OUT, (size_t)100, "Off: Bezug");
			put_string(565-30, Line+12, OUT, GREY);
      snprintf (OUT, (size_t)100, "Min On: %i Off: %i Minuten", AktorMinOn, AktorMinOff);
			put_string(565-30, Line+27, OUT, GREY);
    }
    else if (AktorTyp == 2 || AktorTyp == 12){
			put_string(405+6, Line-16, "Solar", WHITE);
      snprintf (OUT, (size_t)100, "On:  %i W", AktorPowerOn);
			put_string(565-30, Line-3, OUT, GREY);
      snprintf (OUT, (size_t)100, "Off: %i W", AktorPowerOff);
			put_string(565-30, Line+12, OUT, GREY);
      snprintf (OUT, (size_t)100, "Min On: %i Off: %i Minuten", AktorMinOn, AktorMinOff);
			put_string(565-30, Line+27, OUT, GREY);
    }
		else if (AktorTyp == 3 || AktorTyp == 13){
			put_string(405+6, Line-16, "Battery", WHITE);
      snprintf (OUT, (size_t)100, "On:  > %i %%", AktorPercentOn);
			put_string(565-30, Line-3, OUT, GREY);
      snprintf (OUT, (size_t)100, "Off: < %i %%", AktorPercentOff);
			put_string(565-30, Line+12, OUT, GREY);
      snprintf (OUT, (size_t)100, "Min On: %i Off: %i Minuten", AktorMinOn, AktorMinOff);
			put_string(565-30, Line+27, OUT, GREY);
    }
		else if (AktorTyp == 10){
			put_string(405+6, Line-16, "Zeit", WHITE);
      snprintf (OUT, (size_t)100, "On:  %s", AktorTimerOn);
			put_string(565-30, Line-3, OUT, GREY);
      snprintf (OUT, (size_t)100, "Off: %s", AktorTimerOff);
			put_string(565-30, Line+12, OUT, GREY);
    }
  }
  return 1;
}
int drawAktorState(int AktorTyp, int AktorPrio, int AktorPin, int Line, int PowerGrid, int PowerPVI, int BatterySOC)
{
  char OUT[56];
	int prio = readAktor(AktorPrioPosition);
  if (AktorTyp > 0){
    int AktorState = digitalRead(AktorPin);
    if (AktorState == Aktiv){
      drawSquare(405+80, Line,20,20,LIGHT_GREEN);
      drawCorner(405+80, Line,20,20,GREY);
    }
		else if (prio >= AktorPrio){
			drawSquare(405+80, Line,20,20,LIGHT_RED);
      drawCorner(405+80, Line,20,20,GREY);
		}
    else {
      drawSquare(405+80, Line,20,20,RED);
      drawCorner(405+80, Line,20,20,GREY);
    }
		snprintf (OUT, (size_t)100, "%i", AktorPrio);
		drawColorOutput(405+86,Line+22,20,12, OUT, WHITE, GREY);
    if (AktorTyp == 1 || AktorTyp == 11){
      snprintf (OUT, (size_t)100, "%i W", PowerGrid);
        drawColorOutput(405+6,Line+22,70,12, OUT, WHITE, GREY);
    }
    if (AktorTyp == 2 || AktorTyp == 12){
      snprintf (OUT, (size_t)100, "%i W", PowerPVI);
      drawColorOutput(405+6,Line+22,70,12, OUT, WHITE, GREY);
    }
    if (AktorTyp == 3 || AktorTyp == 13){
      snprintf (OUT, (size_t)100, "%i %%", BatterySOC);
      drawColorOutput(405+6,Line+22,70,12, OUT, WHITE, GREY);
    }
  }
  else {
    drawSquare(405+80, Line,20,20,LTGREY);
    drawCorner(405+80, Line,20,20,GREY);
  }
  return 1;
}
int makeAktorFrame()
{
  if (useAktor == 1){
    drawAktorFrame(Aktor1Typ, Aktor1PowerOn, Aktor1PowerOff, Aktor1PercentOn, Aktor1PercentOff,
              Aktor1TimerOn, Aktor1TimerOff, Aktor1MinOn, Aktor1MinOff, "Aktor 1", Aktor1Name, RP1);
    drawAktorFrame(Aktor2Typ, Aktor2PowerOn, Aktor2PowerOff, Aktor2PercentOn, Aktor2PercentOff,
              Aktor2TimerOn, Aktor2TimerOff, Aktor2MinOn, Aktor2MinOff, "Aktor 2", Aktor2Name, RP2);
    drawAktorFrame(Aktor3Typ, Aktor3PowerOn, Aktor3PowerOff, Aktor3PercentOn, Aktor3PercentOff,
              Aktor3TimerOn, Aktor3TimerOff, Aktor3MinOn, Aktor3MinOff, "Aktor 3", Aktor3Name, RP3);
    drawAktorFrame(Aktor4Typ, Aktor4PowerOn, Aktor4PowerOff, Aktor4PercentOn, Aktor4PercentOff,
              Aktor4TimerOn, Aktor4TimerOff, Aktor4MinOn, Aktor4MinOff, "Aktor 4", Aktor4Name, RP4);
    drawAktorFrame(Aktor5Typ, Aktor5PowerOn, Aktor5PowerOff, Aktor5PercentOn, Aktor5PercentOff,
              Aktor5TimerOn, Aktor5TimerOff, Aktor5MinOn, Aktor5MinOff, "Aktor 5", Aktor5Name, RP5);
    return 1;
  }
  return 0;
}
int makeAktorState()
{
  if (useAktor == 1){
    int PowerGrid = readRscp(PosGrid);
    int PowerPVI = readRscp(PosPVI);
    int BatterySOC = readRscp(PosSOC);
    drawAktorState(Aktor1Typ, Aktor1Prio, Aktor1Pin, RP1, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor2Typ, Aktor2Prio, Aktor2Pin, RP2, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor3Typ, Aktor3Prio, Aktor3Pin, RP3, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor4Typ, Aktor4Prio, Aktor4Pin, RP4, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor5Typ, Aktor5Prio, Aktor5Pin, RP5, PowerGrid, PowerPVI, BatterySOC);
    return 1;
  }
  return 0;
}
int checkAktor()
{
  if (useAktor == 1){
		timerState(Aktor1Typ, Aktor1TimerOn, Aktor1TimerOff, Aktor1Position);
		timerState(Aktor2Typ, Aktor2TimerOn, Aktor2TimerOff, Aktor2Position);
		timerState(Aktor3Typ, Aktor3TimerOn, Aktor3TimerOff, Aktor3Position);
		timerState(Aktor4Typ, Aktor4TimerOn, Aktor4TimerOff, Aktor4Position);
		timerState(Aktor5Typ, Aktor5TimerOn, Aktor5TimerOff, Aktor5Position);

    Aktor(Aktor1Prio, Aktor1PowerOn, Aktor1PowerOff, Aktor1PercentOn, Aktor1PercentOff, Aktor1MinOn, Aktor1MinOff, Aktor1Typ, Aktor1Pin, Aktor1Position);
    Aktor(Aktor2Prio, Aktor2PowerOn, Aktor2PowerOff, Aktor2PercentOn, Aktor2PercentOff, Aktor2MinOn, Aktor2MinOff, Aktor2Typ, Aktor2Pin, Aktor2Position);
    Aktor(Aktor3Prio, Aktor3PowerOn, Aktor3PowerOff, Aktor3PercentOn, Aktor3PercentOff, Aktor3MinOn, Aktor3MinOff, Aktor3Typ, Aktor3Pin, Aktor3Position);
    Aktor(Aktor4Prio, Aktor4PowerOn, Aktor4PowerOff, Aktor4PercentOn, Aktor4PercentOff, Aktor4MinOn, Aktor4MinOff, Aktor4Typ, Aktor4Pin, Aktor4Position);
    Aktor(Aktor5Prio, Aktor5PowerOn, Aktor5PowerOff, Aktor5PercentOn, Aktor5PercentOff, Aktor5MinOn, Aktor5MinOff, Aktor5Typ, Aktor5Pin, Aktor5Position);
    return 1;
  }
  return 0;
}
#endif // __AKTOR_H_

#ifndef __AKTOR_H_
#define __AKTOR_H_

#define counterOn   0
#define counterOff  1
#define MinOn       2
#define MinOff      3

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
	if (prio >= AktorPrio){
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
  if (NetInput <=  AktorPower90 && PowerGrid <=  100){
    writeAktor(AktorPosition + counterOn, 0);
    writeAktor(AktorPosition + counterOff, 0);
  }
  AktorState = digitalRead(AktorPin);
  if (AktorState == Aktiv){
    int counterMinOn = readAktor(AktorPosition + MinOn);
    counterMinOn--;
    writeAktor(AktorPosition + MinOn, counterMinOn);
    if (PowerGrid >=  100  && counterMinOn <= 0){
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
	if (prio >= AktorPrio){
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
  if (PowerPVI <=  AktorPower90 && PowerPVI >= AktorPowerOff){
    writeAktor(AktorPosition + counterOn, 0);
    writeAktor(AktorPosition + counterOff, 0);
  }
  AktorState = digitalRead(AktorPin);
  if (AktorState == Aktiv){
    int counterMinOn = readAktor(AktorPosition + MinOn);
    counterMinOn--;
    writeAktor(AktorPosition + MinOn, counterMinOn);
    if (PowerPVI <= AktorPowerOff && counterMinOn <= 0){
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
	if (prio >= AktorPrio){
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
  if (BatterySOC < AktorPercentOn && BatterySOC > AktorPercentOff){
    writeAktor(AktorPosition + counterOn, 0);
    writeAktor(AktorPosition + counterOff, 0);
  }
  if (AktorState == Aktiv){
    int counterMinOn = readAktor(AktorPosition + MinOn);
    counterMinOn--;
    writeAktor(AktorPosition + MinOn, counterMinOn);
    if (BatterySOC <= AktorPercentOff && counterMinOn <= 0){
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

int Aktor(int PowerGrid, int PowerPVI, int BatterySOC, int AktorPrio,
              int AktorPowerOn, int AktorPowerOff,
              int AktorPercentOn, int AktorPercentOff,
              int AktorMinOn, int AktorMinOff,
              int AktorTyp, int AktorPin, int AktorPosition)
{
  if (AktorTyp > 0){
    if (AktorTyp == 1)
      netInAktor(PowerGrid, AktorPrio, AktorPowerOn, AktorMinOn, AktorMinOff, AktorPin, AktorPosition);
    else if (AktorTyp == 2)
      solarAktor(PowerPVI, AktorPrio, AktorPowerOn, AktorPowerOff, AktorMinOn, AktorMinOff, AktorPin, AktorPosition);
    else if (AktorTyp == 3)
      batteryAktor(BatterySOC, AktorPrio, AktorPercentOn, AktorPercentOff, AktorMinOn, AktorMinOff, AktorPin, AktorPosition);
  }
  return 1;
}

int drawAktorFrame(int AktorTyp, int AktorPowerOn, int AktorPowerOff, int AktorPercentOn, int AktorPercentOff, int AktorMinOn, int AktorMinOff, char *designation, char *AktorName, int Line)
{
  char OUT[56];
  drawSquare(405,Line-20,363,60,GREY);
  drawCorner(405,Line-20,363,60,WHITE);
  drawSquare(405+120,Line-17,240,54,WHITE);
  drawCorner(405+120,Line-17,240,54,GREY);
  put_string(405+6, Line, designation, WHITE);
  if (AktorTyp > 0){
    createData(565-30, Line-36, AktorName);
    if (AktorTyp == 1){
      snprintf (OUT, (size_t)100, "\232berschuss On:  %i W", AktorPowerOn);
      createData(565-30, Line-22, OUT);
      snprintf (OUT, (size_t)100, "\232berschuss Off: Bezug");
      createData(565-30, Line-10, OUT);
      snprintf (OUT, (size_t)100, "Min On: %i Off: %i Minuten", AktorMinOn, AktorMinOff);
      createData(565-30, Line+2, OUT);
    }
    else if (AktorTyp == 2){
      snprintf (OUT, (size_t)100, "Solar-Power On:  %i W", AktorPowerOn);
      createData(565-30, Line-22, OUT);
      snprintf (OUT, (size_t)100, "Solar-Power Off: %i W", AktorPowerOff);
      createData(565-30, Line-10, OUT);
      snprintf (OUT, (size_t)100, "Min On: %i Off: %i Minuten", AktorMinOn, AktorMinOff);
      createData(565-30, Line+2, OUT);
    }
    else if (AktorTyp == 3){
      snprintf (OUT, (size_t)100, "SOC > On:  %i %%", AktorPercentOn);
      createData(565-30, Line-22, OUT);
      snprintf (OUT, (size_t)100, "SOC < Off: %i %%", AktorPercentOff);
      createData(565-30, Line-10, OUT);
      snprintf (OUT, (size_t)100, "Min On: %i Off: %i Minuten", AktorMinOn, AktorMinOff);
      createData(565-30, Line+2, OUT);
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
    if (AktorTyp == 1){
      //int NetInput = PowerGrid * -1;
      snprintf (OUT, (size_t)100, "%i W", PowerGrid);
      //if (NetInput > 20)
        drawColorOutput(405+6,Line+22,70,12, OUT, WHITE, GREY);
      //else
        //drawColorOutput(405+6,Line+22,70,12, "BEZUG", WHITE, GREY);
    }
    if (AktorTyp == 2){
      snprintf (OUT, (size_t)100, "%i W", PowerPVI);
      drawColorOutput(405+6,Line+22,70,12, OUT, WHITE, GREY);
    }
    if (AktorTyp == 3){
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
    drawAktorFrame(Aktor1Typ, Aktor1PowerOn, Aktor1PowerOff, Aktor1PercentOn, Aktor1PercentOff, Aktor1MinOn, Aktor1MinOff, "Aktor 1", Aktor1Name, R1);
    drawAktorFrame(Aktor2Typ, Aktor2PowerOn, Aktor2PowerOff, Aktor2PercentOn, Aktor2PercentOff, Aktor2MinOn, Aktor2MinOff, "Aktor 2", Aktor2Name, R2);
    drawAktorFrame(Aktor3Typ, Aktor3PowerOn, Aktor3PowerOff, Aktor3PercentOn, Aktor3PercentOff, Aktor3MinOn, Aktor3MinOff, "Aktor 3", Aktor3Name, R3);
    drawAktorFrame(Aktor4Typ, Aktor4PowerOn, Aktor4PowerOff, Aktor4PercentOn, Aktor4PercentOff, Aktor4MinOn, Aktor4MinOff, "Aktor 4", Aktor4Name, R4);
    drawAktorFrame(Aktor5Typ, Aktor5PowerOn, Aktor5PowerOff, Aktor5PercentOn, Aktor5PercentOff, Aktor5MinOn, Aktor5MinOff, "Aktor 5", Aktor5Name, R5);
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
    drawAktorState(Aktor1Typ, Aktor1Prio, Aktor1Pin, R1, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor2Typ, Aktor2Prio, Aktor2Pin, R2, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor3Typ, Aktor3Prio, Aktor3Pin, R3, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor4Typ, Aktor4Prio, Aktor4Pin, R4, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor5Typ, Aktor5Prio, Aktor5Pin, R5, PowerGrid, PowerPVI, BatterySOC);
    return 1;
  }
  return 0;
}
int checkAktor()
{
  if (useAktor == 1){
    int PowerGrid = readRscp(PosGrid);
    int PowerPVI = readRscp(PosPVI);
    int BatterySOC = readRscp(PosSOC);
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor1Prio, Aktor1PowerOn, Aktor1PowerOff, Aktor1PercentOn, Aktor1PercentOff, Aktor1MinOn, Aktor1MinOff, Aktor1Typ, Aktor1Pin, Aktor1Position);
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor2Prio, Aktor2PowerOn, Aktor2PowerOff, Aktor2PercentOn, Aktor2PercentOff, Aktor2MinOn, Aktor2MinOff, Aktor2Typ, Aktor2Pin, Aktor2Position);
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor3Prio, Aktor3PowerOn, Aktor3PowerOff, Aktor3PercentOn, Aktor3PercentOff, Aktor3MinOn, Aktor3MinOff, Aktor3Typ, Aktor3Pin, Aktor3Position);
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor4Prio, Aktor4PowerOn, Aktor4PowerOff, Aktor4PercentOn, Aktor4PercentOff, Aktor4MinOn, Aktor4MinOff, Aktor4Typ, Aktor4Pin, Aktor4Position);
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor5Prio, Aktor5PowerOn, Aktor5PowerOff, Aktor5PercentOn, Aktor5PercentOff, Aktor5MinOn, Aktor5MinOff, Aktor5Typ, Aktor5Pin, Aktor5Position);
    return 1;
  }
  return 0;
}
#endif // __AKTOR_H_

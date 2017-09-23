#ifndef __AKTOR_H_
#define __AKTOR_H_

int writeAktor(int AktorPosition, int NewValue)
{
  char PathAktor [128];
  snprintf (PathAktor, (size_t)128, "/mnt/RAMDisk/Aktor.txt");
  BitWrite(PathAktor, AktorPosition, NewValue, AktorMAX);
  return 1;
}

int readAktor(int AktorPosition)
{
  char PathAktor [128];
  snprintf (PathAktor, (size_t)128, "/mnt/RAMDisk/Aktor.txt");
	int ret = BitRead(PathAktor, AktorPosition, AktorMAX);
  return ret;
}

int netInAktor(int PowerGrid, int AktorPower, int AktorPin, int AktorPosition)
{
  int NetInput = PowerGrid * -1;
  int AktorState = digitalRead(AktorPin);
  if (NetInput >=  AktorPower && AktorState == Deaktiv){
    int AktorCounterOn = readAktor(AktorPosition);
    AktorCounterOn++;
    writeAktor(AktorPosition, AktorCounterOn);
    if(AktorCounterOn == 120){
      digitalWrite( AktorPin, Aktiv);
    }
  }
  int AktorPower90 = AktorPower /10 *9;
  if (NetInput <=  AktorPower90 && PowerGrid <=  100){
    writeAktor(AktorPosition, 0);
    writeAktor(AktorPosition +1, 0);
  }
  AktorState = digitalRead(AktorPin);
  if (PowerGrid >=  100 && AktorState == Aktiv){
    int AktorCounterOff = readAktor(AktorPosition +1);
    AktorCounterOff++;
    writeAktor(AktorPosition +1, AktorCounterOff);
    if(AktorCounterOff == 30){
      digitalWrite( AktorPin, Deaktiv);
      writeAktor(AktorPosition, 0);
      writeAktor(AktorPosition +1, 0);
    }
  }
  return 1;
}

int solarAktor(int PowerPVI, int AktorPowerOn, int AktorPowerOff, int AktorPin, int AktorPosition)
{
  int AktorState = digitalRead(AktorPin);
  if (PowerPVI >=  AktorPowerOn && AktorState == Deaktiv){
    int AktorCounterOn = readAktor(AktorPosition);
    AktorCounterOn++;
    writeAktor(AktorPosition, AktorCounterOn);
    if(AktorCounterOn >= 120){
      digitalWrite( AktorPin, Aktiv);
    }
  }
  int AktorPower90 = AktorPowerOn /10 *9;
  if (PowerPVI <=  AktorPower90 && PowerPVI >= AktorPowerOff){
    writeAktor(AktorPosition, 0);
    writeAktor(AktorPosition +1, 0);
  }
  AktorState = digitalRead(AktorPin);
  if (PowerPVI <= AktorPowerOff && AktorState == Aktiv){
    int AktorCounterOff = readAktor(AktorPosition +1);
    AktorCounterOff++;
    writeAktor(AktorPosition +1, AktorCounterOff);
    if(AktorCounterOff == 30){
      digitalWrite( AktorPin, Deaktiv);
      writeAktor(AktorPosition, 0);
      writeAktor(AktorPosition +1, 0);
    }
  }
  return 1;
}

int batteryAktor(int BatterySOC, int AktorPercentOn, int AktorPercentOff, int AktorPin, int AktorPosition)
{
  int AktorState = digitalRead(AktorPin);
  if (BatterySOC >= AktorPercentOn && AktorState == Deaktiv){
    int AktorCounterOn = readAktor(AktorPosition);
    AktorCounterOn++;
    writeAktor(AktorPosition, AktorCounterOn);
    if (AktorCounterOn == 5){
      digitalWrite( AktorPin, Aktiv);
      writeAktor(AktorPosition, 0);
    }
  }
  if (BatterySOC < AktorPercentOn && BatterySOC > AktorPercentOff){
    writeAktor(AktorPosition, 0);
    writeAktor(AktorPosition +1, 0);
  }
  if (BatterySOC <= AktorPercentOff && AktorState == Aktiv){
    int AktorCounterOff = readAktor(AktorPosition +1);
    AktorCounterOff++;
    writeAktor(AktorPosition +1, AktorCounterOff);
    if (AktorCounterOff == 5){
      digitalWrite( AktorPin, Deaktiv);
      writeAktor(AktorPosition +1, 0);
    }
  }
  return 1;
}

int Aktor(int PowerGrid, int PowerPVI, int BatterySOC,
              int AktorPowerOn, int AktorPowerOff,
              int AktorPercentOn, int AktorPercentOff,
              int AktorTyp, int AktorPin, int AktorPosition)
{
  if (AktorTyp > 0){
    if (AktorTyp == 1)
      netInAktor(PowerGrid, AktorPowerOn, AktorPin, AktorPosition);
    else if (AktorTyp == 2)
      solarAktor(PowerPVI, AktorPowerOn, AktorPowerOff, AktorPin, AktorPosition);
    else if (AktorTyp == 3)
      batteryAktor(BatterySOC, AktorPercentOn, AktorPercentOff, AktorPin, AktorPosition);
  }
  return 1;
}

int drawAktorFrame(int AktorTyp, int AktorPowerOn, int AktorPowerOff, int AktorPercentOn, int AktorPercentOff, char *designation, char *AktorName, int Line)
{
  char OUT[56];
  drawSquare(405,Line-20,348,60,GREY);
  drawCorner(405,Line-20,348,60,WHITE);
  drawSquare(405+120,Line-17,225,54,WHITE);
  drawCorner(405+120,Line-17,225,54,GREY);
  put_string(405+6, Line, designation, WHITE);
  if (AktorTyp > 0){
    createData(565-30, Line-28, AktorName);
    if (AktorTyp == 1){
      snprintf (OUT, (size_t)100, "\232berschuss On:  %i W", AktorPowerOn);
      createData(565-30, Line-16, OUT);
      snprintf (OUT, (size_t)100, "\232berschuss Off: Bezug");
      createData(565-30, Line-4, OUT);
    }
    else if (AktorTyp == 2){
      snprintf (OUT, (size_t)100, "Solar-Power On:  %i W", AktorPowerOn);
      createData(565-30, Line-16, OUT);
      snprintf (OUT, (size_t)100, "Solar-Power Off: %i W", AktorPowerOff);
      createData(565-30, Line-4, OUT);
    }
    else if (AktorTyp == 3){
      snprintf (OUT, (size_t)100, "SOC > On:  %i %%", AktorPercentOn);
      createData(565-30, Line-16, OUT);
      snprintf (OUT, (size_t)100, "SOC < Off: %i %%", AktorPercentOff);
      createData(565-30, Line-4, OUT);
    }
  }
  return 1;
}
int drawAktorState(int AktorTyp, int AktorPin, int Line, int PowerGrid, int PowerPVI, int BatterySOC)
{
  char OUT[56];
  if (AktorTyp > 0){
    int AktorState = digitalRead(AktorPin);
    if (AktorState == Aktiv){
      drawSquare(405+80, Line,20,20,LIGHT_GREEN);
      drawCorner(405+80, Line,20,20,GREY);
    }
    else {
      drawSquare(405+80, Line,20,20,LIGHT_RED);
      drawCorner(405+80, Line,20,20,GREY);
    }
    if (AktorTyp == 1){
      int NetInput = PowerGrid * -1;
      snprintf (OUT, (size_t)100, "%i W", PowerGrid);
      if (NetInput > 20)
        drawColorOutput(405+6,Line+18,70,12, OUT, WHITE, GREY);
      else
        drawColorOutput(405+6,Line+18,70,12, "BEZUG", WHITE, GREY);
    }
    if (AktorTyp == 2){
      snprintf (OUT, (size_t)100, "%i W", PowerPVI);
      drawColorOutput(405+6,Line+18,70,12, OUT, WHITE, GREY);
    }
    if (AktorTyp == 3){
      snprintf (OUT, (size_t)100, "%i %%", BatterySOC);
      drawColorOutput(405+6,Line+18,70,12, OUT, WHITE, GREY);
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
    drawAktorFrame(Aktor1Typ, Aktor1PowerOn, Aktor1PowerOff, Aktor1PercentOn, Aktor1PercentOff, "Aktor 1", Aktor1Name, R1);
    drawAktorFrame(Aktor2Typ, Aktor2PowerOn, Aktor2PowerOff, Aktor2PercentOn, Aktor2PercentOff, "Aktor 2", Aktor2Name, R2);
    drawAktorFrame(Aktor3Typ, Aktor3PowerOn, Aktor3PowerOff, Aktor3PercentOn, Aktor3PercentOff, "Aktor 3", Aktor3Name, R3);
    drawAktorFrame(Aktor4Typ, Aktor4PowerOn, Aktor4PowerOff, Aktor4PercentOn, Aktor4PercentOff, "Aktor 4", Aktor4Name, R4);
    drawAktorFrame(Aktor5Typ, Aktor5PowerOn, Aktor5PowerOff, Aktor5PercentOn, Aktor5PercentOff, "Aktor 5", Aktor5Name, R5);
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
    drawAktorState(Aktor1Typ, Aktor1Pin, R1, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor2Typ, Aktor2Pin, R2, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor3Typ, Aktor3Pin, R3, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor4Typ, Aktor4Pin, R4, PowerGrid, PowerPVI, BatterySOC);
    drawAktorState(Aktor5Typ, Aktor5Pin, R5, PowerGrid, PowerPVI, BatterySOC);
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
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor1PowerOn, Aktor1PowerOff, Aktor1PercentOn, Aktor1PercentOff, Aktor1Typ, Aktor1Pin, Aktor1Position);
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor2PowerOn, Aktor2PowerOff, Aktor2PercentOn, Aktor2PercentOff, Aktor2Typ, Aktor2Pin, Aktor2Position);
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor3PowerOn, Aktor3PowerOff, Aktor3PercentOn, Aktor3PercentOff, Aktor3Typ, Aktor3Pin, Aktor3Position);
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor4PowerOn, Aktor4PowerOff, Aktor4PercentOn, Aktor4PercentOff, Aktor4Typ, Aktor4Pin, Aktor4Position);
    Aktor(PowerGrid, PowerPVI, BatterySOC, Aktor5PowerOn, Aktor5PowerOff, Aktor5PercentOn, Aktor5PercentOff, Aktor5Typ, Aktor5Pin, Aktor5Position);
    return 1;
  }
  return 0;
}
#endif // __AKTOR_H_

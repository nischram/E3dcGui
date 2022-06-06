char SEND_OUT[128];

int wallboxAction(){
	if(WALLBOX_ACTION == 1){
		char wallboxSendNow[20], wallboxSendMode[20], wallboxSendCurrent[20], wallboxSendBtC[20], wallboxSendBbC[20], wallboxSendStop[20], wallboxSendPhC[20];
		char WbMode[24],WbBtC[24],WbBbC[24],WbSet[24];
		int WbCurrent, wallboxSendTime = 0;
		// Wallbox read and senden
		read_HM(ISE_WB_SEND_NOW, 4, wallboxSendNow);
		if (strcmp ("true",wallboxSendNow) == 0){
			read_HM(ISE_WB_SEND_MODE, 4, wallboxSendMode);
			read_HM(ISE_WB_SEND_CURRENT, 2, wallboxSendCurrent);
			read_HM(ISE_WB_SEND_BTC, 4, wallboxSendBtC);
			read_HM(ISE_WB_SEND_BBC, 4, wallboxSendBbC);
			read_HM(ISE_WB_SEND_STOP, 4, wallboxSendStop);
			read_HM(ISE_WB_SEND_PH_CHANGE, 4, wallboxSendPhC);
			if(strcmp ("true",wallboxSendMode) == 0) snprintf (WbMode, (size_t)128, "-sonne");
			else snprintf (WbMode, (size_t)128, "-mix");
			WbCurrent = atoi(wallboxSendCurrent);
			if(strcmp ("true",wallboxSendBtC) == 0) snprintf (WbBtC, (size_t)128, "-BtCyes");
			else snprintf (WbBtC, (size_t)128, "-BtCno");
			if(strcmp ("true",wallboxSendBbC) == 0) snprintf (WbBbC, (size_t)128, "-BbCyes");
			else snprintf (WbBbC, (size_t)128, "-BbCno");
			if(strcmp ("true",wallboxSendStop) == 0) snprintf (WbSet, (size_t)128, "-stop");
			else if(strcmp ("true",wallboxSendPhC) == 0) snprintf (WbSet, (size_t)128, "-swPh");
			else snprintf (WbSet, (size_t)128, "-no");
			snprintf (SEND_OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wb %s %i %s &", WbMode, WbCurrent, WbSet);
			system(SEND_OUT);
			sleep(1);
			snprintf (SEND_OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wbEMS %s %s &", WbBtC, WbBbC);
			system(SEND_OUT);
			printsendHM(ISE_WB_SEND_NOW, "false");
			printsendHM(ISE_WB_SEND_STOP, "false");
			printsendHM(ISE_WB_SEND_PH_CHANGE, "false");
		}
	}
}
int epReserveAction(){
	if(EP_RESERVE_ACTION == 1){
		char epreserveSendNow[20], hmSendEmergencyPowerSize[20];
		int S10CurrentBatEmergencyPowerSize;
		// EP-Reserve read and senden
		read_HM(ISE_EP_RESERVE_SEND_NOW, 4, epreserveSendNow);
		if (strcmp ("true",epreserveSendNow) == 0){
			read_HM(ISE_EP_RESERVE_SET_W, 4, hmSendEmergencyPowerSize);
			S10CurrentBatEmergencyPowerSize = (int)(atof(hmSendEmergencyPowerSize) * 1000); // Umrechnung von kW in W
			snprintf (SEND_OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -ep %i %i", readRscp(PosEpReservMaxW), S10CurrentBatEmergencyPowerSize);
			system(SEND_OUT);
			printsendHM(ISE_EP_RESERVE_SEND_NOW, "false");
		}
	}
}
int batterylimitsAction(){
	if(BATTERYLIMIT_ACTION == 1){
		char blSendNow[20], blSendMode[20], blSendCharge[20], blSendDischarge[20];
		char blMode[24];
		int blCharge, blDischarge;
		// Battery-Limits read and senden
		read_HM(ISE_BL_SEND_NOW, 4, blSendNow);
		if (strcmp ("true",blSendNow) == 0){
			read_HM(ISE_BL_SEND_MODE, 4, blSendMode);
			read_HM(ISE_BL_SEND_CHARGE, 4, blSendCharge);
			read_HM(ISE_BL_SEND_DISCHARGE, 4, blSendDischarge);
			if(strcmp ("true",blSendMode) == 0) snprintf (blMode, (size_t)128, "-blYes");
			else snprintf (blMode, (size_t)128, "-blNo");
			blCharge = atoi(blSendCharge)*1000;
			blDischarge = atoi(blSendDischarge)*1000;
			snprintf (SEND_OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -bl %s %i %i &", blMode, blCharge, blDischarge);
			system(SEND_OUT);
			printsendHM(ISE_BL_SEND_NOW, "false");
		}
	}
}
int powersaveAction(){
	if(POWERSAVE_ACTION == 1){
		char psSendNow[20], psSendMode[20];
		char psMode[24];
		// Battery-Limits read and senden
		read_HM(ISE_PS_SEND_NOW, 4, psSendNow);
		if (strcmp ("true",psSendNow) == 0){
			read_HM(ISE_PS_SEND_MODE, 4, psSendMode);
			if(strcmp ("true",psSendMode) == 0) snprintf (psMode, (size_t)128, "-psYes");
			else snprintf (psMode, (size_t)128, "-psNo");
			snprintf (SEND_OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -ps %s &", psMode);
			system(SEND_OUT);
			printsendHM(ISE_PS_SEND_NOW, "false");
		}
	}
}
int weatherRegAction(){
	if(WEATHER_REG_ACTION == 1){
		char wrSendNow[20], wrSendMode[20];
		char wrMode[24];
		// Weather regulated power read and send
		read_HM(ISE_WR_SEND_NOW, 4, wrSendNow);
		if (strcmp ("true",wrSendNow) == 0){
			read_HM(ISE_WR_SEND_MODE, 4, wrSendMode);
			if(strcmp ("true",wrSendMode) == 0) snprintf (wrMode, (size_t)128, "-wrYes");
			else snprintf (wrMode, (size_t)128, "-wrNo");
			snprintf (SEND_OUT, (size_t)128, "/home/pi/E3dcGui/Rscp/RscpSet -wr %s &", wrMode);
			system(SEND_OUT);
			printsendHM(ISE_WR_SEND_NOW, "false");
		}
	}
}
int actionCheckAll(){
	// Wallbox read and senden
	wallboxAction();
	// EP-Reserve read and senden
	epReserveAction();
	// Battery-Limits read and senden
	batterylimitsAction();
	// Weather regulated power read and send
	weatherRegAction();
}
// /home/pi/E3dcGui/Rscp/RscpSet -wb -sonne 8 -BtCno -BbCyes -swPh
// /home/pi/E3dcGui/Rscp/RscpSet -ep 10000 3230 // Testaufruf. Dieser Aufruf schreibt 3230W in EP.

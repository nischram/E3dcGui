/*
g++ -O3 RscpMain.cpp Rscp/RscpProtocol.cpp Rscp/AES.cpp Rscp/SocketConnection.cpp Rscp/RWData.cpp -o RscpMain
 */
 #include <stdio.h>
 #include <errno.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <cstdio>
 #include <iostream>
 #include <iomanip>
 #include <fstream>
 #include <string>
 #include <limits>
 #include <ctime>
 #include "Rscp/RscpProtocol.h"
 #include "Rscp/RscpTags.h"
 #include "Rscp/SocketConnection.h"
 #include "Rscp/AES.h"
 #include "checkPara.h"
 #include "Rscp/RWData.h"

static int iSocket = -1;
static int iAuthenticated = 0;
static AES aesEncrypter;
static AES aesDecrypter;
static uint8_t ucEncryptionIV[AES_BLOCK_SIZE];
static uint8_t ucDecryptionIV[AES_BLOCK_SIZE];
static int32_t TAG_EMS_OUT_UNIXTIME = 0, TAG_EMS_OUT_POWER_WB_ALL = 0;
static char TAG_EMS_OUT_DATE [20], TAG_EMS_OUT_TIME [20], TAG_EMS_OUT_TZ [20], TAG_EMS_OUT_SERIAL_NUMBER [17], TAG_INFO_OUT_SW_RELEASE[20];
static char checkTime [20];
static int checkTimeInt = -1, lastCheckTimeInt;
static int CounterHM = 0;
static int Counter900 = 0;
static int time_zone = 7200;
static uint8_t WbExt[8];

using namespace std;

int createRequestExample(SRscpFrameBuffer * frameBuffer) {
    RscpProtocol protocol;
    SRscpValue rootValue;
    // The root container is create with the TAG ID 0 which is not used by any device.
    protocol.createContainerValue(&rootValue, 0);

    //---------------------------------------------------------------------------------------------------------
    // Create a request frame
    //---------------------------------------------------------------------------------------------------------
    if(iAuthenticated == 0)
    {
        printf("\nRequest authentication\n");
        // authentication request
        SRscpValue authenContainer;
        protocol.createContainerValue(&authenContainer, TAG_RSCP_REQ_AUTHENTICATION);
        protocol.appendValue(&authenContainer, TAG_RSCP_AUTHENTICATION_USER, E3DC_USER);
        protocol.appendValue(&authenContainer, TAG_RSCP_AUTHENTICATION_PASSWORD, E3DC_PASS);
        // append sub-container to root container
        protocol.appendValue(&rootValue, authenContainer);
        // free memory of sub-container as it is now copied to rootValue
        protocol.destroyValueData(authenContainer);
    }
    else
    {
        printf("\n____________________\nRequest cyclic data\n");
        // request data information
        if(TAG_EMS_OUT_UNIXTIME == 0 || (Seriennummer == 1 && CounterHM == HM_Intervall)){
          protocol.appendValue(&rootValue, TAG_INFO_REQ_SERIAL_NUMBER);
        }
        if(checkTimeInt != -1 && lastCheckTimeInt != checkTimeInt){
          lastCheckTimeInt = checkTimeInt;
          protocol.appendValue(&rootValue, TAG_INFO_REQ_SW_RELEASE);
          protocol.appendValue(&rootValue, TAG_INFO_REQ_PRODUCTION_DATE);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_INSTALLED_PEAK_POWER);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_DERATE_AT_PERCENT_VALUE);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_DERATE_AT_POWER_VALUE);
        }
        protocol.appendValue(&rootValue, TAG_INFO_REQ_TIME);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_PV);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_BAT);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_HOME);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_GRID);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_BAT_SOC);
        if (Wallbox == 1){
          protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_WB_ALL);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_WB_SOLAR);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_BATTERY_TO_CAR_MODE);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_BATTERY_BEFORE_CAR_MODE);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_GET_WB_DISCHARGE_BAT_UNTIL);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_GET_WALLBOX_ENFORCE_POWER_ASSIGNMENT);
        }
        else {
          writeRscp(PosWbAll,0);
          writeRscp(PosWbSolar,0);
        }
        // request battery information
        SRscpValue batteryContainer;
        protocol.createContainerValue(&batteryContainer, TAG_BAT_REQ_DATA);
        protocol.appendValue(&batteryContainer, TAG_BAT_INDEX, (uint8_t)0);
        if (ISE_BAT_CHARGE_CYCLES != 0) protocol.appendValue(&batteryContainer, TAG_BAT_REQ_CHARGE_CYCLES);
        if (ISE_BAT_DCB_COUNT != 0) protocol.appendValue(&batteryContainer, TAG_BAT_REQ_DCB_COUNT);
        if (ISE_BAT_TRAINING_MODE != 0) protocol.appendValue(&batteryContainer, TAG_BAT_REQ_TRAINING_MODE);
        if (ISE_BAT_DEVICE_NAME != 0) protocol.appendValue(&batteryContainer, TAG_BAT_REQ_DEVICE_NAME);
        //protocol.appendValue(&batteryContainer, TAG_BAT_REQ_RSOC);
        protocol.appendValue(&batteryContainer, TAG_BAT_REQ_DEVICE_STATE);
        //protocol.appendValue(&batteryContainer, TAG_BAT_REQ_CURRENT);
        // append sub-container to root container
        protocol.appendValue(&rootValue, batteryContainer);
        // free memory of sub-container as it is now copied to rootValue
        protocol.destroyValueData(batteryContainer);

        protocol.appendValue(&rootValue, TAG_EMS_REQ_AUTARKY);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_SELF_CONSUMPTION);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_STATUS);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_EMERGENCY_POWER_STATUS);
        if (Additional == 1)
          protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_ADD);
        else
          writeRscp(PosADD,0);
        if(readTo(PosToIdlePeriod)){
          protocol.appendValue(&rootValue, TAG_EMS_REQ_GET_IDLE_PERIODS);
          writeTo(PosToIdlePeriod, 0);
        }
        // request PVI information
        SRscpValue PVIContainer;
        protocol.createContainerValue(&PVIContainer, TAG_PVI_REQ_DATA);
        protocol.appendValue(&PVIContainer, TAG_PVI_INDEX, (uint8_t)0);
        protocol.appendValue(&PVIContainer, TAG_PVI_REQ_ON_GRID);
        if (TRACKER_POWER == 1){
          if (PVI_TRACKER == 2)
            protocol.appendValue(&PVIContainer, TAG_PVI_REQ_DC_POWER, (uint8_t)1);
          protocol.appendValue(&PVIContainer, TAG_PVI_REQ_DC_POWER, (uint8_t)0);
        }
        if (TRACKER_VOLTAGE == 1){
          if (PVI_TRACKER == 2)
            protocol.appendValue(&PVIContainer, TAG_PVI_REQ_DC_VOLTAGE, (uint8_t)1);
          protocol.appendValue(&PVIContainer, TAG_PVI_REQ_DC_VOLTAGE, (uint8_t)0);
        }
        if (TRACKER_CURRENT == 1){
          if (PVI_TRACKER == 2)
            protocol.appendValue(&PVIContainer, TAG_PVI_REQ_DC_CURRENT, (uint8_t)1);
          protocol.appendValue(&PVIContainer, TAG_PVI_REQ_DC_CURRENT, (uint8_t)0);
        }
        // append sub-container to root container
        protocol.appendValue(&rootValue, PVIContainer);
        // free memory of sub-container as it is now copied to rootValue
        protocol.destroyValueData(PVIContainer);

        // request PM information
        SRscpValue PMContainer;
        protocol.createContainerValue(&PMContainer, TAG_PM_REQ_DATA);
        protocol.appendValue(&PMContainer, TAG_PM_INDEX, (uint8_t)PM_POSITION);
        protocol.appendValue(&PMContainer, TAG_PM_REQ_DEVICE_STATE);
        protocol.appendValue(&PMContainer, TAG_PM_REQ_ACTIVE_PHASES);
        // append sub-container to root container
        protocol.appendValue(&rootValue, PMContainer);
        // free memory of sub-container as it is now copied to rootValue
        protocol.destroyValueData(PMContainer);

        // request Wallbox information
        if (Wallbox == 1){
          SRscpValue WBContainer;
          protocol.createContainerValue(&WBContainer, TAG_WB_REQ_DATA) ;
          // add index 0 to select first wallbox
          protocol.appendValue(&WBContainer, TAG_WB_INDEX,0);
          protocol.appendValue(&WBContainer, TAG_WB_REQ_DEVICE_STATE);
          protocol.appendValue(&WBContainer, TAG_WB_REQ_PM_ACTIVE_PHASES);
          protocol.appendValue(&WBContainer, TAG_WB_REQ_EXTERN_DATA_ALG);
          protocol.appendValue(&WBContainer, TAG_WB_REQ_ABORT_CHARGING);
          // append sub-container to root container
          protocol.appendValue(&rootValue, WBContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(WBContainer);
        }
        // EP Reserve
        protocol.appendValue(&rootValue, TAG_SE_REQ_EP_RESERVE);
        // request Bat-Limits
        protocol.appendValue(&rootValue, TAG_EMS_REQ_GET_POWER_SETTINGS);
        SRscpValue BLContainer;
        /*protocol.createContainerValue(&BLContainer, TAG_EMS_REQ_SET_POWER_SETTINGS);
        // append sub-container to root container
        protocol.appendValue(&rootValue, BLContainer);
        // free memory of sub-container as it is now copied to rootValue
        protocol.destroyValueData(BLContainer);*/
    }

    // create buffer frame to send data to the S10
    protocol.createFrameAsBuffer(frameBuffer, rootValue.data, rootValue.length, true); // true to calculate CRC on for transfer
    // the root value object should be destroyed after the data is copied into the frameBuffer and is not needed anymore
    protocol.destroyValueData(rootValue);

    return 0;
}

int handleResponseValue(RscpProtocol *protocol, SRscpValue *response) {
    // check if any of the response has the error flag set and react accordingly
    if(response->dataType == RSCP::eTypeError) {
        // handle error for example access denied errors
        uint32_t uiErrorCode = protocol->getValueAsUInt32(response);
        printf("Tag 0x%08X received error code %u.\n", response->tag, uiErrorCode);
        return -1;
    }
    // check the SRscpValue TAG to detect which response it is
    switch(response->tag){
        case TAG_RSCP_AUTHENTICATION: {
            // It is possible to check the response->dataType value to detect correct data type
            // and call the correct function. If data type is known,
            // the correct function can be called directly like in this case.
            uint8_t ucAccessLevel = protocol->getValueAsUChar8(response);
            if(ucAccessLevel > 0) {
                iAuthenticated = 1;
            }
            printf("RSCP authentitication level %i\n", ucAccessLevel);
            break;
        }
        case TAG_INFO_SERIAL_NUMBER: {    // response for TAG_INFO_REQ_SERIAL_NUMBER
          string serialNr = protocol->getValueAsString(response);
          cout << "Serial-Number is " << serialNr << "\n";
          strcpy(TAG_EMS_OUT_SERIAL_NUMBER, serialNr.c_str());
          printsendCharHM(CounterHM, TAG_EMS_ISE_SERIAL_NUMBER, TAG_EMS_OUT_SERIAL_NUMBER);
          break;
        }
        case TAG_INFO_SW_RELEASE: {    // response for TAG_INFO_REQ_SW_RELEASE
          string swRelease = protocol->getValueAsString(response);
          cout << "Software Release is " << swRelease << "\n";
          strcpy(TAG_INFO_OUT_SW_RELEASE, swRelease.c_str());
          printsendCharHM(CounterHM, TAG_EMS_ISE_SW_RELEASE, TAG_INFO_OUT_SW_RELEASE);
          break;
        }
        case TAG_INFO_PRODUCTION_DATE: {    // response for TAG_INFO_REQ_PRODUCTION_DATE
    		  string  productiondate = protocol->getValueAsString(response);
    		  cout << "Production-Date is " << productiondate << "\n";
    		  productiondate = replaceinString(productiondate, " ", "%20");
          char TAG_EMS_OUT_PRODUCTION_DATE [17];
    		  strcpy(TAG_EMS_OUT_PRODUCTION_DATE, productiondate.c_str());
    		  printsendCharHM(CounterHM, ISE_INFO_PRODUCTION_DATE, TAG_EMS_OUT_PRODUCTION_DATE);
    		  break;
        }
        case TAG_INFO_TIME: {    // response for TAG_INFO_REQ_TIME
            int32_t unixTimestamp = protocol->getValueAsInt32(response);
            time_t timestamp;
            tm *sys;
            timestamp = unixTimestamp;
            sys = localtime(&timestamp);
            strftime (TAG_EMS_OUT_TZ,40,"%z",sys);
            if (strcmp ("+0200",TAG_EMS_OUT_TZ) == 0)
              time_zone = 7200;
            else if (strcmp ("+0100",TAG_EMS_OUT_TZ) == 0)
              time_zone = 3600;
            else
              time_zone = 7200;
            TAG_EMS_OUT_UNIXTIME = unixTimestamp - time_zone;
            timestamp = TAG_EMS_OUT_UNIXTIME;
            sys = localtime(&timestamp);
            strftime (TAG_EMS_OUT_DATE,40,"%d.%m.%Y",sys);
            strftime (TAG_EMS_OUT_TIME,40,"%H:%M:%S",sys);
            strftime (checkTime,20,"%H",sys);
            checkTimeInt = atoi(checkTime);
            writeUnixtime(UnixtimeE3dc, TAG_EMS_OUT_UNIXTIME);
            cout << "System Time is " << TAG_EMS_OUT_DATE << "_" << TAG_EMS_OUT_TIME << "\n";
            cout << "System Unix-Time is " << TAG_EMS_OUT_UNIXTIME << "\n";
            cout << "System Timezone is " << TAG_EMS_OUT_TZ << "\n";
            printsendHM(CounterHM, TAG_EMS_ISE_UNIXTIME, TAG_EMS_OUT_UNIXTIME);
            if (sendTime == 1){
              char SEND[64];
              snprintf (SEND, (size_t)64, "%s_%s", TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME);
              printsendCharHM(CounterHM, ISE_TIMESTAMP_HM, SEND);
            }
            break;
        }
        case TAG_EMS_INSTALLED_PEAK_POWER: {    // response for TAG_EMS_REQ_INSTALLED_PEAK_POWER
          uint32_t installedPeak = protocol->getValueAsUInt32(response);
          if (RSCP_DETAILED_OUTPUT) cout << "Installed peak power " << installedPeak << " Wp\n";
          writeRscp(PosInstalledPeak, installedPeak);
          printsendHM(CounterHM, ISE_INSTALLED_PEAK_POWER, installedPeak);
          break;
        }
        case TAG_EMS_DERATE_AT_PERCENT_VALUE: {    // response for TAG_EMS_REQ_DERATE_AT_PERCENT_VALUE
          float derateAtPercent = protocol->getValueAsFloat32(response);
          if (RSCP_DETAILED_OUTPUT) cout << "Derat at percent " << derateAtPercent*100 << " %\n";
          writeRscp(PosDerateAtPercent, derateAtPercent*100);
          printsendHM(CounterHM, ISE_DERATE_AT_PERCENT, derateAtPercent*100);
          break;
        }
        case TAG_EMS_DERATE_AT_POWER_VALUE: {    // response for TAG_EMS_REQ_DERATE_AT_POWER_VALUE
          float derateAtPower = protocol->getValueAsFloat32(response);
          if (RSCP_DETAILED_OUTPUT) cout << setprecision(0) << fixed << "Derat at power " << derateAtPower << " W\n";
          writeRscp(PosDerateAtPower, derateAtPower);
          printsendHM(CounterHM, ISE_DERATE_AT_POWER, derateAtPower);
          break;
        }
      case TAG_EMS_POWER_PV: {    // response for TAG_EMS_REQ_POWER_PV
          int32_t TAG_EMS_OUT_POWER_PV = protocol->getValueAsInt32(response);
          cout << "PV Power is " << TAG_EMS_OUT_POWER_PV <<" W\n";
          writeRscp(PosPVI,TAG_EMS_OUT_POWER_PV);
          char file[20];
          snprintf (file, (size_t)20, "Solar900");
          write900(PosPVI900, file, TAG_EMS_OUT_POWER_PV, Counter900);
          printsendHM(CounterHM, TAG_EMS_ISE_POWER_PV, TAG_EMS_OUT_POWER_PV);
          break;
        }
        case TAG_EMS_POWER_BAT: {    // response for TAG_EMS_REQ_POWER_BAT
          int32_t TAG_EMS_OUT_POWER_BAT = protocol->getValueAsInt32(response);
          cout << "Battery Power is " << TAG_EMS_OUT_POWER_BAT << " W\n";
          writeRscp(PosBat,TAG_EMS_OUT_POWER_BAT);
          char fileIN[20], fileOUT[20];
          snprintf (fileIN, (size_t)20, "BatIn900");
          snprintf (fileOUT, (size_t)20, "BatOut900");
          if (TAG_EMS_OUT_POWER_BAT < 0){
            write900(PosBatOut900, fileOUT, (TAG_EMS_OUT_POWER_BAT* -1), Counter900);
            write900(PosBatIn900, fileIN, 0 , Counter900);
          }
          else {
            write900(PosBatIn900, fileIN, TAG_EMS_OUT_POWER_BAT , Counter900);
            write900(PosBatOut900, fileOUT, 0, Counter900);
          }
          printsendHM(CounterHM, TAG_EMS_ISE_POWER_BAT, TAG_EMS_OUT_POWER_BAT);
          break;
        }
        case TAG_EMS_POWER_HOME: {    // response for TAG_EMS_REQ_POWER_HOME
          int32_t TAG_EMS_OUT_POWER_HOME = protocol->getValueAsInt32(response);
          cout << "House Power is " << TAG_EMS_OUT_POWER_HOME << " W\n";
          writeRscp(PosHome,TAG_EMS_OUT_POWER_HOME);
          char file[20];
          snprintf (file, (size_t)20, "Home900");
          write900(PosHome900, file, TAG_EMS_OUT_POWER_HOME, Counter900);
          printsendHM(CounterHM, TAG_EMS_ISE_POWER_HOME, TAG_EMS_OUT_POWER_HOME);
          break;
        }
        case TAG_EMS_POWER_GRID: {    // response for TAG_EMS_REQ_POWER_GRID
          int32_t TAG_EMS_OUT_POWER_GRID = protocol->getValueAsInt32(response);
          cout << "Grid Power is " << TAG_EMS_OUT_POWER_GRID << " W\n";
          writeRscp(PosGrid,TAG_EMS_OUT_POWER_GRID);
          printsendHM(CounterHM, TAG_EMS_ISE_POWER_GRID, TAG_EMS_OUT_POWER_GRID);
          char fileIN[20], fileOUT[20];
          snprintf (fileIN, (size_t)20, "NetIn900");
          snprintf (fileOUT, (size_t)20, "NetOut900");
          if(TAG_EMS_OUT_POWER_GRID >= 0) {
              int TAG_EMS_OUT_POWER_NET_IN = 0;
              int TAG_EMS_OUT_POWER_NET_OUT = TAG_EMS_OUT_POWER_GRID;
              write900(PosNetIn900, fileIN, TAG_EMS_OUT_POWER_NET_IN, Counter900);
              write900(PosNetOut900, fileOUT, TAG_EMS_OUT_POWER_NET_OUT, Counter900);
              printsendHM(CounterHM, TAG_EMS_ISE_POWER_NET_IN, TAG_EMS_OUT_POWER_NET_IN);
              printsendHM(CounterHM, TAG_EMS_ISE_POWER_NET_OUT, TAG_EMS_OUT_POWER_NET_OUT);
          }
          else {
            int neg_GRID = (TAG_EMS_OUT_POWER_GRID * -1);
            int TAG_EMS_OUT_POWER_NET_IN = neg_GRID;
            int TAG_EMS_OUT_POWER_NET_OUT = 0;
            write900(PosNetIn900, fileIN, TAG_EMS_OUT_POWER_NET_IN, Counter900);
            write900(PosNetOut900, fileOUT, TAG_EMS_OUT_POWER_NET_OUT, Counter900);
            printsendHM(CounterHM, TAG_EMS_ISE_POWER_NET_IN, TAG_EMS_OUT_POWER_NET_IN);
            printsendHM(CounterHM, TAG_EMS_ISE_POWER_NET_OUT, TAG_EMS_OUT_POWER_NET_OUT);
          }
          break;
        }
        case TAG_EMS_BAT_SOC: {     // response for TAG_EMS_REQ_BAT_SOC
          float fSOC = protocol->getValueAsUChar8(response);
          int TAG_EMS_OUT_SOC = fSOC;
          cout << "Battery SOC is " << TAG_EMS_OUT_SOC << " %\n";
          writeRscp(PosSOC,TAG_EMS_OUT_SOC);
          char file[20];
          snprintf (file, (size_t)20, "SOC900");
          write900(PosSOC900, file, TAG_EMS_OUT_SOC, Counter900);
          printsendHM(CounterHM, TAG_BAT_ISE_SOC, TAG_EMS_OUT_SOC);
          break;
        }
        case TAG_EMS_BATTERY_TO_CAR_MODE: {     // response for TAG_EMS_REQ_BATTERY_TO_CAR_MODE
          float fBTC = protocol->getValueAsUChar8(response);
          bool TAG_EMS_OUT_BATTERY_TO_CAR_MODE = fBTC;
          if (RSCP_DETAILED_OUTPUT) cout << "Battery to Car " << TAG_EMS_OUT_BATTERY_TO_CAR_MODE << "\n";
          writeRscpWb(PosWbBtC,TAG_EMS_OUT_BATTERY_TO_CAR_MODE);
          printsendHM(CounterHM, TAG_EMS_ISE_WB_BTC, TAG_EMS_OUT_BATTERY_TO_CAR_MODE);
          break;
        }
        case TAG_EMS_BATTERY_BEFORE_CAR_MODE: {     // response for TAG_EMS_REQ_BATTERY_BEFORE_CAR_MODE
          float fBBC = protocol->getValueAsUChar8(response);
          bool TAG_EMS_OUT_BATTERY_BEFORE_CAR_MODE = fBBC;
          if (RSCP_DETAILED_OUTPUT) cout << "Battery bevor Car " << TAG_EMS_OUT_BATTERY_BEFORE_CAR_MODE << "\n";
          writeRscpWb(PosWbBbC,TAG_EMS_OUT_BATTERY_BEFORE_CAR_MODE);
          printsendHM(CounterHM, TAG_EMS_ISE_WB_BTC, TAG_EMS_OUT_BATTERY_BEFORE_CAR_MODE);
          break;
        }
        case TAG_EMS_POWER_ADD: {    // response for TAG_EMS_REQ_POWER_ADD
          int32_t ADD_REAL = protocol->getValueAsInt32(response);
          int32_t TAG_EMS_OUT_POWER_ADD = ADD_REAL * -1;
          cout << "Additional Power is " << TAG_EMS_OUT_POWER_ADD << " W\n";
          writeRscp(PosADD,TAG_EMS_OUT_POWER_ADD);
          char file[20];
          snprintf (file, (size_t)20, "Add900");
          write900(PosAdd900, file, TAG_EMS_OUT_POWER_ADD, Counter900);
          printsendHM(CounterHM, TAG_EMS_ISE_POWER_ADD, TAG_EMS_OUT_POWER_ADD);
          break;
        }
        case TAG_EMS_POWER_WB_ALL: {    // response for TAG_EMS_REQ_POWER_WB_ALL
          TAG_EMS_OUT_POWER_WB_ALL = protocol->getValueAsInt32(response);
          cout << "Wallbox Power All is " << TAG_EMS_OUT_POWER_WB_ALL << " W\n";
          writeRscp(PosWbAll,TAG_EMS_OUT_POWER_WB_ALL);
          char file[20];
          snprintf (file, (size_t)20, "WBAll900");
          write900(PosWBAll900, file, TAG_EMS_OUT_POWER_WB_ALL, Counter900);
          printsendHM(CounterHM, TAG_EMS_ISE_POWER_WB_ALL, TAG_EMS_OUT_POWER_WB_ALL);
          break;
        }
        case TAG_EMS_POWER_WB_SOLAR: {    // response for TAG_EMS_REQ_POWER_WB_SOLAR
          int32_t TAG_EMS_OUT_POWER_WB_SOLAR = protocol->getValueAsInt32(response);
          cout << "Wallbox Power Solar is " << TAG_EMS_OUT_POWER_WB_SOLAR << " W\n";
          writeRscp(PosWbSolar,TAG_EMS_OUT_POWER_WB_SOLAR);
          char file[20];
          snprintf (file, (size_t)20, "WBSolar900");
          write900(PosWBSolar900, file, TAG_EMS_OUT_POWER_WB_SOLAR, Counter900);
          printsendHM(CounterHM, TAG_EMS_ISE_POWER_WB_SOLAR, TAG_EMS_OUT_POWER_WB_SOLAR);
          cout << "Wallbox Power Grid is " << TAG_EMS_OUT_POWER_WB_ALL - TAG_EMS_OUT_POWER_WB_SOLAR << " W\n";
          printsendHM(CounterHM, TAG_EMS_ISE_POWER_WB_GRID, TAG_EMS_OUT_POWER_WB_ALL - TAG_EMS_OUT_POWER_WB_SOLAR);
          break;
        }
        case TAG_EMS_GET_WB_DISCHARGE_BAT_UNTIL: {    // response for TAG_EMS_REQ_GET_WB_DISCHARGE_BAT_UNTIL
          float wbDischargeUntil = protocol->getValueAsUChar8(response);
          cout << "Wallbox discharge until " << wbDischargeUntil << " %\n";
          writeRscp(PosWbUntil,wbDischargeUntil);
          break;
        }
        case TAG_EMS_GET_WALLBOX_ENFORCE_POWER_ASSIGNMENT: {    // response for TAG_EMS_REQ_GET_WALLBOX_ENFORCE_POWER_ASSIGNMENT
          bool wbBaMres = protocol->getValueAsBool(response);
          cout << "Wallbox Battery at Mix-Mode " << wbBaMres << "\n";
          writeRscp(PosWbBaM,wbBaMres);
          break;
        }
        case TAG_EMS_AUTARKY: {    // response for TAG_EMS_REQ_AUTARKY
          float TAG_EMS_OUT_AUTARKY = protocol->getValueAsFloat32(response);
          cout << "Autarky is " << setprecision(3) << TAG_EMS_OUT_AUTARKY << " %\n";
          writeRscp(PosAutarky,TAG_EMS_OUT_AUTARKY);
          if (Autarky == 1){
            printsendHM(CounterHM, TAG_EMS_ISE_AUTARKY, TAG_EMS_OUT_AUTARKY);
          }
          break;
        }
        case TAG_EMS_SELF_CONSUMPTION: {    // response for TAG_EMS_REQ_SELF_CONSUMPTION
          float TAG_EMS_OUT_SELF_CONSUMPTION = protocol->getValueAsFloat32(response);
          cout << "Self Consumption is " << setprecision(3) << TAG_EMS_OUT_SELF_CONSUMPTION << " %\n";
          writeRscp(PosSelfCon,TAG_EMS_OUT_SELF_CONSUMPTION);
          if (Eigenstrom == 1){
            printsendHM(CounterHM, TAG_EMS_ISE_SELFCON, TAG_EMS_OUT_SELF_CONSUMPTION);
          }
          break;
        }
        case TAG_EMS_STATUS: {    // response for TAG_EMS_REQ_STATUS
          float TAG_EMS_OUT_STATUS = protocol->getValueAsUChar8(response);
          if (RSCP_DETAILED_OUTPUT) cout << setprecision(0) << fixed << "EMS status 0x0" << TAG_EMS_OUT_STATUS << "\n";
          writeRscp(PosEpState,TAG_EMS_OUT_STATUS);
          printsendHM(CounterHM, ISE_EMS_STATUS, TAG_EMS_OUT_STATUS);
          break;
        }
        case TAG_EMS_EMERGENCY_POWER_STATUS: {    // response for TAG_EMS_REQ_EMERGENCY_POWER_STATUS
          int TAG_EMS_OUT_EMERGENCY_POWER_STATUS = protocol->getValueAsUChar8(response);
          writeRscp(PosEpState,TAG_EMS_OUT_EMERGENCY_POWER_STATUS);
          printsendHM(CounterHM, ISE_EP_STATUS, TAG_EMS_OUT_EMERGENCY_POWER_STATUS);
          char TAG_OUT_DESCRIPTION [100];
          string description, descriptionReplace;
          switch(TAG_EMS_OUT_EMERGENCY_POWER_STATUS) {
    			case 0: {
    				description = "Nicht möglich";
    				descriptionReplace = replaceinString(description, " ", "%20");
    				strcpy(TAG_OUT_DESCRIPTION, descriptionReplace.c_str());
    				printsendCharHM(CounterHM, ISE_EMERGENCY_POWER_STATUS_INFO, TAG_OUT_DESCRIPTION);
    			break; }
    			case 1: {
    				description = "Aktiv";
    				descriptionReplace = replaceinString(description, " ", "%20");
    				strcpy(TAG_OUT_DESCRIPTION, descriptionReplace.c_str());
    				printsendCharHM(CounterHM, ISE_EMERGENCY_POWER_STATUS_INFO, TAG_OUT_DESCRIPTION);
    				break; }
    			case 2: {
    				description = "Nicht aktiv";
    				descriptionReplace = replaceinString(description, " ", "%20");
    				strcpy(TAG_OUT_DESCRIPTION, descriptionReplace.c_str());
    				printsendCharHM(CounterHM, ISE_EMERGENCY_POWER_STATUS_INFO, TAG_OUT_DESCRIPTION);
    				break; }
    			case 3: {
    				description = "Nicht verfügbar";
    				descriptionReplace = replaceinString(description, " ", "%20");
    				strcpy(TAG_OUT_DESCRIPTION, descriptionReplace.c_str());
    				printsendCharHM(CounterHM, ISE_EMERGENCY_POWER_STATUS_INFO, TAG_OUT_DESCRIPTION);
    				break; }
    			case 4: {
    				description = "Wechsel in Inselbetrieb";
    				descriptionReplace = replaceinString(description, " ", "%20");
    				strcpy(TAG_OUT_DESCRIPTION, descriptionReplace.c_str());
    				printsendCharHM(CounterHM, ISE_EMERGENCY_POWER_STATUS_INFO, TAG_OUT_DESCRIPTION);
    				break; }
    			}
          if (RSCP_DETAILED_OUTPUT) cout << setprecision(0) << fixed << "Emergency power status 0x0" << TAG_EMS_OUT_EMERGENCY_POWER_STATUS << " = " << description << "\n";
          break;
        }
        case TAG_EMS_GET_IDLE_PERIODS:{
          // resposne for TAG_EMS_REQ_GET_IDLE_PERIODS
          std::vector < SRscpValue > emsData = protocol->getValueAsContainer(response);
          idle_period_t periods[14];
          for (size_t i = 0; i < emsData.size(); ++i) {
            if (emsData[i].dataType == RSCP::eTypeError) {
              // handle error for example access denied errors
              uint32_t uiErrorCode = protocol->getValueAsUInt32(&emsData[i]);
              printf("Tag 0x%08X received error code %u.\n",emsData[i].tag, uiErrorCode);
              return -1;
            }
            handleResponseEMSGetIdlePeriods(protocol, &emsData[i], &periods[i]);
          }
          break;
        }
        case TAG_BAT_DATA: {        // resposne for TAG_BAT_REQ_DATA
            uint8_t ucBatteryIndex = 0;
            std::vector<SRscpValue> batteryData = protocol->getValueAsContainer(response);
            for(size_t i = 0; i < batteryData.size(); ++i) {
                if(batteryData[i].dataType == RSCP::eTypeError) {
                    // handle error for example access denied errors
                    uint32_t uiErrorCode = protocol->getValueAsUInt32(&batteryData[i]);
                    printf("Tag 0x%08X received error code %u.\n", batteryData[i].tag, uiErrorCode);
                    return -1;
                }
                // check each battery sub tag
                switch(batteryData[i].tag) {
                case TAG_BAT_INDEX: {
                    ucBatteryIndex = protocol->getValueAsUChar8(&batteryData[i]);
                    break;
                }
                case TAG_BAT_RSOC: {              // response for TAG_BAT_REQ_RSOC
                    float fSOC = protocol->getValueAsFloat32(&batteryData[i]);
                    int TAG_EMS_OUT_SOC = fSOC;
                    writeRscp(PosSOC,TAG_EMS_OUT_SOC);
                    cout << "Battery SOC is " << TAG_EMS_OUT_SOC << " %\n";
                    char file[20];
                    snprintf (file, (size_t)20, "SOC900");
                    write900(PosSOC900, file, TAG_EMS_OUT_SOC, Counter900);
                    printsendHM(CounterHM, TAG_BAT_ISE_SOC, TAG_EMS_OUT_SOC);
                    break;
                }
                case TAG_BAT_CHARGE_CYCLES: {              // response for TAG_BAT_REQ_CHARGE_CYCLES
                    uint32_t fBatCycles = protocol->getValueAsUInt32(&batteryData[i]);
    				        if (RSCP_DETAILED_OUTPUT) cout << "Battery cycles " << fBatCycles << " \n";
                    printsendHM(CounterHM, ISE_BAT_CHARGE_CYCLES, fBatCycles);
                    break;
                }
                case TAG_BAT_DCB_COUNT: {              // response for TAG_BAT_REQ_DCB_COUNT
            				int fDcbCount = protocol->getValueAsUChar8(&batteryData[i]) + 1;
            				if (RSCP_DETAILED_OUTPUT) cout << "Modules of batteries " << fDcbCount << " \n";
                    printsendHM(CounterHM, ISE_BAT_DCB_COUNT, fDcbCount);
                    break;
                }
                case TAG_BAT_DEVICE_STATE: {    // response for TAG_BAT_REQ_DEVICE_STATE
                    bool TAG_EMS_OUT_BAT_STATE = protocol->getValueAsBool(&batteryData[i]);
                    writeRscp(PosBatState,TAG_EMS_OUT_BAT_STATE);
                    if (RSCP_DETAILED_OUTPUT) cout << "Battery State = " << TAG_EMS_OUT_BAT_STATE << " \n";
                    break;
                }
                case TAG_BAT_TRAINING_MODE: {              // response for TAG_BAT_REQ_TRAINING_MODE
            				float fDcbTraining = protocol->getValueAsUChar8(&batteryData[i]);
            				int TAG_OUT_BAT_TRAINING_MODE = fDcbTraining;
                    printsendHM(CounterHM, ISE_BAT_TRAINING_MODE, TAG_OUT_BAT_TRAINING_MODE);
                    char TAG_OUT_DESCRIPTION [100];
            				string description, descriptionReplace;
            				switch(TAG_OUT_BAT_TRAINING_MODE) {
            				case 0: {
            					description = "Nicht im Training";
            					descriptionReplace = replaceinString(description, " ", "%20");
            					strcpy(TAG_OUT_DESCRIPTION, descriptionReplace.c_str());
            					printsendCharHM(CounterHM, ISE_BAT_TRAINING_MODE_INFO, TAG_OUT_DESCRIPTION);
            				break; }
            				case 1: {
            					description = "Trainingmodus Entladen";
            					descriptionReplace = replaceinString(description, " ", "%20");
            					strcpy(TAG_OUT_DESCRIPTION, descriptionReplace.c_str());
            					printsendCharHM(CounterHM, ISE_BAT_TRAINING_MODE_INFO, TAG_OUT_DESCRIPTION);
            					break; }
            				case 2: {
            					description = "Trainingmodus Laden";
            					descriptionReplace = replaceinString(description, " ", "%20");
            					strcpy(TAG_OUT_DESCRIPTION, descriptionReplace.c_str());
            					printsendCharHM(CounterHM, ISE_BAT_TRAINING_MODE_INFO, TAG_OUT_DESCRIPTION);
            					break; }
            				}
            				if (RSCP_DETAILED_OUTPUT) cout << "Training mode: " << description << " - (" << TAG_OUT_BAT_TRAINING_MODE << ") \n";
                            break;
                }
                case TAG_BAT_DEVICE_NAME: {              // response for TAG_BAT_REQ_DEVICE_NAME
            				string deviceName = protocol->getValueAsString(&batteryData[i]);
            				if (RSCP_DETAILED_OUTPUT) cout << "Device Name is " << deviceName << "\n";
                    char TAG_BAT_OUT_DEVICE_NAME [24];
            				strcpy(TAG_BAT_OUT_DEVICE_NAME, deviceName.c_str());
                    printsendCharHM(CounterHM, ISE_BAT_DEVICE_NAME, TAG_BAT_OUT_DEVICE_NAME);
                    break;
                }
                // ...
                default:
                    // default behaviour
                    printf("Unknown battery tag %08X\n", response->tag);
                    break;
                }
            }
            protocol->destroyValueData(batteryData);
            break;
        }
        case TAG_PVI_DATA: {        // resposne for TAG_PVI_REQ_DATA
            uint8_t ucPVIIndex = 0;
            std::vector<SRscpValue> PVIData = protocol->getValueAsContainer(response);
            for(size_t i = 0; i < PVIData.size(); ++i) {
                if(PVIData[i].dataType == RSCP::eTypeError) {
                    // handle error for example access denied errors
                    uint32_t uiErrorCode = protocol->getValueAsUInt32(&PVIData[i]);
                    printf("Tag 0x%08X received error code %u.\n", PVIData[i].tag, uiErrorCode);
                    return -1;
                }
                // check each battery sub tag
                switch(PVIData[i].tag) {
                case TAG_PVI_INDEX: {
                    ucPVIIndex = protocol->getValueAsUChar8(&PVIData[i]);
                    break;
                }
                case TAG_PVI_ON_GRID: {              // response for TAG_PVI_REQ_ON_GRID
                    bool TAG_EMS_OUT_PVI_STATE = protocol->getValueAsBool(&PVIData[i]);
                    if (RSCP_DETAILED_OUTPUT) cout << "PVI State = " << TAG_EMS_OUT_PVI_STATE << " \n";
                    writeRscp(PosPVIState,TAG_EMS_OUT_PVI_STATE);
                    break;
                }
                case TAG_PVI_DC_POWER: {
                    int index = -1;
                    float TAG_OUT_PVI_DC_POWER = 0;
                    std::vector<SRscpValue> container = protocol->getValueAsContainer(&PVIData[i]);
                    for(size_t n = 0; n < container.size(); n++) {
                        if((container[n].tag == TAG_PVI_INDEX) ) {
                            index = protocol->getValueAsUInt16(&container[n]);
                        }
                        else if((container[n].tag == TAG_PVI_VALUE)) {
                                TAG_OUT_PVI_DC_POWER = protocol->getValueAsFloat32(&container[n]);
                                if (index == 0){
                                  if (RSCP_DETAILED_OUTPUT) cout << "PVI DC-Power 1 = " << TAG_OUT_PVI_DC_POWER << " \n";
                                  writeRscp(PosPVIDCP1,TAG_OUT_PVI_DC_POWER);
                                  printsendHM(CounterHM, TAG_EMS_ISE_TRACKER_1, TAG_OUT_PVI_DC_POWER);
                                }
                                else if (index == 1){
                                  if (RSCP_DETAILED_OUTPUT) cout << "PVI DC-Power 2 = " << TAG_OUT_PVI_DC_POWER << " \n";
                                  writeRscp(PosPVIDCP2,TAG_OUT_PVI_DC_POWER);
                                  printsendHM(CounterHM, TAG_EMS_ISE_TRACKER_2, TAG_OUT_PVI_DC_POWER);
                                }
                        }
                    }
                    protocol->destroyValueData(container);
                    break;
                }
                case TAG_PVI_DC_VOLTAGE: {
                    int index = -1;
                    float TAG_OUT_PVI_DC_VOLTAGE = 0;
                    std::vector<SRscpValue> container = protocol->getValueAsContainer(&PVIData[i]);
                    for(size_t n = 0; n < container.size(); n++) {
                        if((container[n].tag == TAG_PVI_INDEX) ) {
                            index = protocol->getValueAsUInt16(&container[n]);
                        }
                        else if((container[n].tag == TAG_PVI_VALUE)) {
                                TAG_OUT_PVI_DC_VOLTAGE = protocol->getValueAsFloat32(&container[n]);
                                if (index == 0){
                                  if (RSCP_DETAILED_OUTPUT) cout << "PVI DC-Voltage 1 = " << TAG_OUT_PVI_DC_VOLTAGE << " \n";
                                  writeRscp(PosPVIDCU1,TAG_OUT_PVI_DC_VOLTAGE);
                                }
                                if (index == 1){
                                  if (RSCP_DETAILED_OUTPUT) cout << "PVI DC-Voltage 2 = " << TAG_OUT_PVI_DC_VOLTAGE << " \n";
                                  writeRscp(PosPVIDCU2,TAG_OUT_PVI_DC_VOLTAGE);
                                }
                        }
                    }
                    protocol->destroyValueData(container);
                    break;
                }
                case TAG_PVI_DC_CURRENT: {
                    int index = -1;
                    float TAG_OUT_PVI_DC_CURRENT = 0;
                    std::vector<SRscpValue> container = protocol->getValueAsContainer(&PVIData[i]);
                    for(size_t n = 0; n < container.size(); n++) {
                        if((container[n].tag == TAG_PVI_INDEX) ) {
                            index = protocol->getValueAsUInt16(&container[n]);
                        }
                        else if((container[n].tag == TAG_PVI_VALUE)) {
                                TAG_OUT_PVI_DC_CURRENT = protocol->getValueAsFloat32(&container[n]);
                                if (index == 0){
                                  if (RSCP_DETAILED_OUTPUT) cout << "PVI DC-Current 1 = " << TAG_OUT_PVI_DC_CURRENT << " \n";
                                  writeRscp(PosPVIDCI1,TAG_OUT_PVI_DC_CURRENT*100);
                                }
                                if (index == 1){
                                  if (RSCP_DETAILED_OUTPUT) cout << "PVI DC-Current 2 = " << TAG_OUT_PVI_DC_CURRENT << " \n";
                                  writeRscp(PosPVIDCI2,TAG_OUT_PVI_DC_CURRENT*100);
                                }
                        }
                    }
                    protocol->destroyValueData(container);
                    break;
                }
                // ...
                default:
                    // default behaviour
                    printf("Unknown PVI tag %08X\n", response->tag);
                    break;
                }
            }
            protocol->destroyValueData(PVIData);
            break;
        }
        case TAG_PM_DATA: {        // resposne for TAG_PM_REQ_DATA
            uint8_t ucPMIndex = 0;
            std::vector<SRscpValue> PMData = protocol->getValueAsContainer(response);
            for(size_t i = 0; i < PMData.size(); ++i) {
                if(PMData[i].dataType == RSCP::eTypeError) {
                    // handle error for example access denied errors
                    uint32_t uiErrorCode = protocol->getValueAsUInt32(&PMData[i]);
                    printf("Tag 0x%08X received error code %u.\n", PMData[i].tag, uiErrorCode);
                    return -1;
                }
                // check each battery sub tag
                switch(PMData[i].tag) {
                case TAG_PM_INDEX: {
                    ucPMIndex = protocol->getValueAsUChar8(&PMData[i]);
                    break;
                }
                case TAG_PM_DEVICE_STATE: {              // response for TAG_PM_REQ_DEVICE_STATE
                    bool TAG_EMS_OUT_PM_STATE = protocol->getValueAsBool(&PMData[i]);
                    if (RSCP_DETAILED_OUTPUT) cout << "LM0 State = " << TAG_EMS_OUT_PM_STATE << " \n";
                    writeRscp(PosPMState,TAG_EMS_OUT_PM_STATE);
                    break;
                }
                case TAG_PM_ACTIVE_PHASES: {              // response for TAG_PM_REQ_ACTIVE_PHASES
                    int32_t TAG_PM_OUT_ACTIVE_PHASES = protocol->getValueAsInt32(&PMData[i]);
                    if (RSCP_DETAILED_OUTPUT) cout << "LM0 Aktiv Phases = " << TAG_PM_OUT_ACTIVE_PHASES << " \n";
                    writeRscp(PosPMPhases,TAG_PM_OUT_ACTIVE_PHASES);
                    break;
                }
                // ...
                default:
                    // default behaviour
                    printf("Unknown PM tag %08X\n", response->tag);
                    break;
                }
            }
            protocol->destroyValueData(PMData);
            break;
        }
        case TAG_WB_DATA: {        // resposne for TAG_WB_REQ_DATA
            uint8_t ucWBIndex = 0;
            std::vector<SRscpValue> WBData = protocol->getValueAsContainer(response);
            for(size_t i = 0; i < WBData.size(); ++i) {
                if(WBData[i].dataType == RSCP::eTypeError) {
                    // handle error for example access denied errors
                    uint32_t uiErrorCode = protocol->getValueAsUInt32(&WBData[i]);
                    printf("Tag 0x%08X received error code %u.\n", WBData[i].tag, uiErrorCode);
                    return -1;
                }
                switch(WBData[i].tag) {
                  case TAG_WB_INDEX: {
                      ucWBIndex = protocol->getValueAsUChar8(&WBData[i]);
                      break;
                  }
                  case TAG_WB_DEVICE_STATE: {              // response for TAG_WB_REQ_DEVICE_STATE
                      bool TAG_EMS_OUT_WB_STATE = protocol->getValueAsBool(&WBData[i]);
                      if (RSCP_DETAILED_OUTPUT) cout << "WB0 State = " << TAG_EMS_OUT_WB_STATE << " \n";
                      writeRscpWb(PosWbState, TAG_EMS_OUT_WB_STATE);

                      break;
                  }
                  case TAG_WB_PM_ACTIVE_PHASES: {              // response for TAG_WB_REQ_ACTIVE_PHASES
                      int32_t TAG_WB_OUT_ACTIVE_PHASES = protocol->getValueAsInt32(&WBData[i]);
                      if (RSCP_DETAILED_OUTPUT) cout << "WB0 Aktiv Phases = " << TAG_WB_OUT_ACTIVE_PHASES << " \n";
                      writeRscpWb(PosWbActPhases, TAG_WB_OUT_ACTIVE_PHASES);
                      break;
                  }
                  case TAG_WB_ABORT_CHARGING: {              // response for TAG_WB_REQ_ABORT_CHARGING
                      int32_t TAG_WB_OUT_ABORT_CHARGING = protocol->getValueAsInt32(&WBData[i]);
                      if (RSCP_DETAILED_OUTPUT){
                        if (TAG_WB_OUT_ABORT_CHARGING == 1) cout << "WB0 ABORT_CHARGING = ABORT\n";
                        else cout << "WB0 ABORT_CHARGING = RESUME\n";
                        writeRscp(PosWbAbort, TAG_WB_OUT_ABORT_CHARGING);
                      }
                      break;
                  }
                  case TAG_WB_EXTERN_DATA_ALG: {              // response for TAG_WB_REQ_EXTERN_DATA_ALG
                    std::vector<SRscpValue> WbCont2 = protocol->getValueAsContainer(&WBData[i]);
                    for(size_t i = 0; i < WbCont2.size(); ++i) {
                      if(WbCont2[i].dataType == RSCP::eTypeError) {
                         uint32_t uiErrorCode = protocol->getValueAsUInt32(&WbCont2[i]);
                         printf("Tag 0x%08X received error code %u.\n", WbCont2[i].tag, uiErrorCode);
                         return -1;
                      }
                      switch(WbCont2[i].tag) {
                        case TAG_WB_EXTERN_DATA: {              // response for TAG_WB_REQ_EXTERN_DATA_ALG
                          /*
                          response for TAG_WB_REQ_EXTERN_DATA_ALG:
                          Byte 1: uint8, PreCharge in [%]
                          Byte 2: uint8, Anzahl akt. Phasen [0-3]
                          Byte 3: Einzelbits, Status 1
                          Byte 4: uint8, Strombegrenzung in [A]
                          Byte 5: Einzelbits, Status 2
                          Byte 6: n.b.
                          Byte 7: uint8,Anzeige von Info/Warn/Err
                          -- Status 1
                          Byte 3, Bit 7: 1: Sonnenmode aktiv 0: Mischmode aktiv
                          Byte 3, Bit 6: 1: Laden abgebrochen 0: Laden freigegeben
                          Byte 3, Bit 5: 1: Auto lädt 0: Auto lädt nicht
                          Byte 3, Bit 4: 1: Typ2 verriegelt 0: Typ2 entriegelt
                          Byte 3, Bit 3: 1: Typ2 gesteckt   0: Typ2 nicht gesteckt
                          Byte 3, Bit 2: 1: Schuko an 0: Schuko aus
                          Byte 3, Bit 1: 1: Schuko gesteckt 0: Schuko nicht gesteckt
                          Byte 3, Bit 0: 1: Schuko gesperrt 0: Schuko freigegeben
                          -- Staus 2
                          Byte 5, Bit 7: 1: LED-ERR an (rot) 0: LED-ERR aus
                          Byte 5, Bit 6: 1: LED-SON an (gelb) 0: LED-SON aus
                          Byte 5, Bit 5: 1: LED-BAT an (grün) 0: LED-BAT aus
                          Byte 5, Bit 4: 1: Relais an, 16A 1Phase, Schuko 0: Relais aus, 16A 1Phase, Schuko
                          Byte 5, Bit 3: 1: Relais an, 16A 3Phasen, Typ2 0: Relais aus, 16A 3Phasen, Typ2
                          Byte 5, Bit 2: 1: Relais an, 32A 3Phasen, Typ2 0: Relais aus, 32A 3Phasen, Typ2
                          Byte 5, Bit 1: 1: Fzg. Lüfter an 0: Fzg. Lüfter aus
                          Byte 5, Bit 0: n.b.
                          --
                          Byte 7, „Anzeige von Info/Warn/Err“ stellt den Inhalt des Infobalkens auf dem Display unten dar. Es gibt folgende Werte:
                          Keine Anzeige:
                              0x00
                          Infos:
                              0x10 Schuko nicht möglich, int. LM defekt
                              0x11 Schuko nicht möglich, Typ 2 > 16A
                              0x12 Schuko nicht möglich, Temp > 70*C
                              0x13 Schuko nicht möglich, Notstrombetrieb
                              0x14 Schuko nicht möglich, Typ 2 vor Schuko
                              0x15 Nicht möglich, Schuko oder Typ2 gesteckt
                              0x16 Schuko nicht möglich, extern gesperrt
                              0x18 Sonnenmodus nicht möglich, kein ext. LM
                              0x19 Ext. Abbruch gewünscht
                              0x1A Bereichsüberschreitung
                              0x1B Passwort falsch
                              0x1C Passwort geändert
                          Warnungen:
                              0x20 Gehäusetemperatur > 50*C, Derating
                              0x21 Interner LM nicht vorhanden
                              0x22 FRAM CRC falsch, Default Parameter
                              0x23 Flash CRC falsch
                              0x24 FRAM CRC falsch, Zählerstände
                              0x25 Notstrommode aktiv
                              0x26 Negative Leistung am Typ 2
                              0x27 T-Sensor nicht kalibriert, Imax 16A
                          Fehler:
                              0x40 FRAM defekt, Default Parameter
                              0x41 Flash defekt
                              0x42 CAN defekt
                              0x43 Gehäusetemperatur > 70*C, alles aus
                              0x44 Ladefehler, CP Pegel im Graubereich
                              0x45 Ladefehler, Diode defekt
                              0x46 Ladefehler, PP unbekannt
                              0x47 Ladegeschirr defekt, PP control
                          */
                            memcpy(&WbExt,&WbCont2[i].data[0],sizeof(WbExt));
                            writeRscpWb(PosWbUsePhases, WbExt[1]);
                            writeRscpWb(PosWbSLocked , checkBit(WbExt[2], 1));
                            writeRscpWb(PosWbSConnect , checkBit(WbExt[2], 2));
                            writeRscpWb(PosWbSOn      , checkBit(WbExt[2], 4));
                            writeRscpWb(PosWbConnect  , checkBit(WbExt[2], 8));
                            writeRscpWb(PosWbLocked   , checkBit(WbExt[2], 16));
                            writeRscpWb(PosWbCharge   , checkBit(WbExt[2], 32));
                            writeRscpWb(PosWbReady    , checkBit(WbExt[2], 64));
                            writeRscpWb(PosWbMode     , checkBit(WbExt[2], 128));
                            writeRscpWb(PosWbCurrent  , WbExt[3]);
                            writeRscpWb(PosWbRelais32 , checkBit(WbExt[4], 4));
                            writeRscpWb(PosWbRelais16 , checkBit(WbExt[4], 8));
                            writeRscpWb(PosWbSRelais  , checkBit(WbExt[4], 16));
                            writeRscpWb(PosWbLED_BAT  , checkBit(WbExt[4], 32));
                            writeRscpWb(PosWbLED_SON  , checkBit(WbExt[4], 64));
                            writeRscpWb(PosWbLED_ERR  , checkBit(WbExt[4], 128));
                            writeRscpWb(PosWbDebug    , WbExt[6]);
                            writeRscpWb(PosWbCheckSum , WbExt[1]+ WbExt[2]+ WbExt[3]+ WbExt[4]+ WbExt[6]);
                            if (RSCP_DETAILED_OUTPUT) printf("WB0 ALG EXTERN_DATA = ");
                            printsendHM(CounterHM, TAG_EMS_ISE_WB_PHASES, WbExt[1]);
                            printsendBitHM(CounterHM, TAG_EMS_ISE_WB_CONNECT, WbExt[2], 8);
                            printsendBitHM(CounterHM, TAG_EMS_ISE_WB_LOCKED, WbExt[2], 16);
                            printsendBitHM(CounterHM, TAG_EMS_ISE_WB_CHARGE, WbExt[2], 32);
                            printsendBitHM(CounterHM, TAG_EMS_ISE_WB_MODE, WbExt[2], 128);
                            printsendHM(CounterHM, TAG_EMS_ISE_WB_CURRENT, WbExt[3]);
                            for(size_t x = 0; x < sizeof(WbExt); ++x){
                              uint8_t y;
                              y=WbExt[x];
                              if (RSCP_DETAILED_OUTPUT) printf("%02X ", y);
                            }
                            if (RSCP_DETAILED_OUTPUT) printf("\n");
                            break;
                        }
                        case TAG_WB_EXTERN_DATA_LEN: {              // response for TAG_WB_REQ_EXTERN_DATA_ALG
                            uint8_t iLen = protocol->getValueAsUChar8(&WbCont2[i]);
                            break;
                        }
                      default:
                          printf("Unknown WB_EXT_ALG tag %08X\n", WbCont2[i].tag);
                      }
                    }
                    protocol->destroyValueData(WbCont2);
                  break;
                }
                // ...
                default:
                    // default behaviour
                    printf("Unknown WB tag %08X\n", response->tag);
                    break;
                }
            }
            protocol->destroyValueData(WBData);
            break;
        }
        case TAG_SE_EP_RESERVE: {        // resposne for TAG_SE_REQ_EP_RESERVE
            uint8_t ucSEIndex = 0;
            std::vector<SRscpValue> SEData = protocol->getValueAsContainer(response);
            for(size_t i = 0; i < SEData.size(); ++i) {
              if(SEData[i].dataType == RSCP::eTypeError) {
                  // handle error for example access denied errors
                  uint32_t uiErrorCode = protocol->getValueAsUInt32(&SEData[i]);
                  printf("Tag 0x%08X received error code %u.\n", SEData[i].tag, uiErrorCode);
                  return -1;
              }
              switch(SEData[i].tag) {
                case TAG_SE_PARAM_EP_RESERVE: {              // response for TAG_SE_PARAM_EP_RESERVE
                    float OUT_SE_PARAM_EP_RESERVE = protocol->getValueAsFloat32(&SEData[i]);
                    if (RSCP_DETAILED_OUTPUT) cout << setprecision(1) << fixed << "EP Reserve = " << OUT_SE_PARAM_EP_RESERVE << " %\n";
                    writeRscp(PosEpReserv, OUT_SE_PARAM_EP_RESERVE);
                    break;
                }
                case TAG_SE_PARAM_EP_RESERVE_W: {              // response for TAG_SE_PARAM_EP_RESERVE_W
                    float OUT_SE_PARAM_EP_RESERVE_W = protocol->getValueAsFloat32(&SEData[i]);
                    if (RSCP_DETAILED_OUTPUT) cout << setprecision(0) << fixed << "EP Reserve = " << OUT_SE_PARAM_EP_RESERVE_W << " Wh\n";
                    writeRscp(PosEpReservW, OUT_SE_PARAM_EP_RESERVE_W);
                    printsendHM(CounterHM, TAG_SE_ISE_EP_RESERVE_W, OUT_SE_PARAM_EP_RESERVE_W/1000);
                    break;
                }
                case TAG_SE_PARAM_EP_RESERVE_MAX_W: {              // response for TAG_SE_PARAM_EP_RESERVE_MAX_W
                    float OUT_SE_PARAM_EP_RESERVE_MAX_W = protocol->getValueAsFloat32(&SEData[i]);
                    if (RSCP_DETAILED_OUTPUT) cout << setprecision(0) << fixed << "EP Reserve Max = " << OUT_SE_PARAM_EP_RESERVE_MAX_W << " Wh\n";
                    writeRscp(PosEpReservMaxW, OUT_SE_PARAM_EP_RESERVE_MAX_W);
                    break;
                }
              // ...
              default:
                  // default behaviour
                  //printf("Unknown SE tag %08X\n", response->tag);
                  break;
              }
            }
            protocol->destroyValueData(SEData);
            break;
        }
        case TAG_EMS_GET_POWER_SETTINGS:
        case TAG_EMS_SET_POWER_SETTINGS: {
    			uint8_t ucBLIndex = 0;
    			std::vector<SRscpValue> BLData = protocol->getValueAsContainer(response);
    			for(size_t i = 0; i < BLData.size(); ++i) {
    				//printf("TAG_EMS_GET_POWER_SETTINGS/TAG_EMS_SET_POWER_SETTINGS Durchlauf # %i \n", i+1);
    				if(BLData[i].dataType == RSCP::eTypeError) {
    					// handle error for example access denied errors
    					uint32_t uiErrorCode = protocol->getValueAsUInt32(&BLData[i]);
    					printf("TAG_EMS_GET_POWER_SETTINGS 0x%08X received error code %u.\n", BLData[i].tag, uiErrorCode);
    					printf("TAG_EMS_GET_POWER_SETTINGS - Fehler beim Durchlauf!\n");
    					return -1;
    				}
    				// check each BL sub tag
    				switch(BLData[i].tag) {
    					case TAG_EMS_POWER_LIMITS_USED: {              // response for POWER_LIMITS_USED
                bool powerLimitUsed = protocol->getValueAsBool(&BLData[i]);
                writeRscp(PosBlUsed,powerLimitUsed);
    						if (powerLimitUsed){
    							if (RSCP_DETAILED_OUTPUT) cout << "Battery Limits = User\n";
    							printsendHM(CounterHM, ISE_BAT_POWER_LIMITS_USED, true);
    							}
    						else {
    							if (RSCP_DETAILED_OUTPUT) cout << "Battery Limits = System\n";
    							printsendHM(CounterHM, ISE_BAT_POWER_LIMITS_USED, false);
    						}
    						break;
    					}
    					case TAG_EMS_MAX_CHARGE_POWER: {              // 101 response for TAG_EMS_MAX_CHARGE_POWER
    						uint32_t uPower = protocol->getValueAsUInt32(&BLData[i]);
    						if (RSCP_DETAILED_OUTPUT) cout << "Battery Limits Charge = " << uPower << " W\n";
                writeRscp(PosBlCharge,uPower);
    						printsendHM(CounterHM, ISE_BAT_CHARGE_LIMIT, (float(uPower)/1000));
    						break;
    					}
    					case TAG_EMS_MAX_DISCHARGE_POWER: {              //102 response for TAG_EMS_MAX_DISCHARGE_POWER
    						uint32_t uPower = protocol->getValueAsUInt32(&BLData[i]);
    						if (RSCP_DETAILED_OUTPUT) cout << "Battery Limits Disharge = " << uPower << " W\n";
                writeRscp(PosBlDischarge,uPower);
    						printsendHM(CounterHM, ISE_BAT_DISCHARGE_LIMIT, (float(uPower)/1000));
    						break;
    					}
    					case TAG_EMS_DISCHARGE_START_POWER:{              //103 response for TAG_EMS_DISCHARGE_START_POWER
    						uint32_t uPower = protocol->getValueAsUInt32(&BLData[i]);
    						if (RSCP_DETAILED_OUTPUT) cout << "Battery Start Power = " << uPower << " W\n";
                writeRscp(PosBlStart,uPower);
    						printsendHM(CounterHM, ISE_BAT_DISCHARGE_START_POWER, uPower);
    						break;
    					}
    					case TAG_EMS_POWERSAVE_ENABLED: {              //104 response for TAG_EMS_POWERSAVE_ENABLED
                bool powerSave = protocol->getValueAsBool(&BLData[i]);
                writeRscp(PosPowerSave,powerSave);
    						if (powerSave){
    							if (RSCP_DETAILED_OUTPUT) cout << "Powersave = aktiv\n";
    							printsendHM(CounterHM, ISE_POWERSAVE_ENABLED, true);
    						}
    						else {
    							if (RSCP_DETAILED_OUTPUT) cout << "Powersave = deaktiv\n";
    							printsendHM(CounterHM, ISE_POWERSAVE_ENABLED, false);
    						}
    						break;
    					}
    					case TAG_EMS_WEATHER_REGULATED_CHARGE_ENABLED: {//105 resp WEATHER_REGULATED_CHARGE_ENABLED
                bool waetherReg = protocol->getValueAsBool(&BLData[i]);
                writeRscp(PosWeatherReg,waetherReg);
    						if (waetherReg){
    							if (RSCP_DETAILED_OUTPUT) cout << "Weather regulated charge = aktiv \n";
    							printsendHM(CounterHM, ISE_WEATHER_REGULATED_CHARGE_ENABLED, true);
    						}
    						else {
    							if (RSCP_DETAILED_OUTPUT) cout << "Weather regulated charge = deaktiv \n";
    							printsendHM(CounterHM, ISE_WEATHER_REGULATED_CHARGE_ENABLED, false);
    						}
    						break;
    					}
    						// ...
    					default:
    						// default behaviour
                /*printf("Unkonwn GET_POWER_SETTINGS tag %08X", BLData[i].tag);
    						printf(" len %08X", BLData[i].length);
    						printf(" datatype %08X", BLData[i].dataType);
    						uint32_t uPower = protocol->getValueAsUInt32(&BLData[i]);
    						printf(" Value  %i\n", uPower);
                */
                break;
    				}
    			}
    			protocol->destroyValueData(BLData);
                // sleep(10);
    			break;
    		}
        // ...
        default:
        // default behavior
        printf("Unknown tag %08X\n", response->tag);
        break;
    }
    return 1;
}
static int processReceiveBuffer(const unsigned char * ucBuffer, int iLength)
{
    RscpProtocol protocol;
    SRscpFrame frame;

    int iResult = protocol.parseFrame(ucBuffer, iLength, &frame);
    if(iResult < 0) {
        // check if frame length error occured
        // in that case the full frame length was not received yet
        // and the receive function must get more data
        if(iResult == RSCP::ERR_INVALID_FRAME_LENGTH) {
            return 0;
        }
        // otherwise a not recoverable error occured and the connection can be closed
        else {
            return iResult;
        }
    }

    int iProcessedBytes = iResult;

    // process each SRscpValue struct seperately
    for(unsigned int i = 0; i < frame.data.size(); i++) {
        handleResponseValue(&protocol, &frame.data[i]);
    }

    // destroy frame data and free memory
    protocol.destroyFrameData(frame);

    // returned processed amount of bytes
    return iProcessedBytes;
}

static void receiveLoop(bool & bStopExecution)
{
    //--------------------------------------------------------------------------------------------------------------
    // RSCP Receive Frame Block Data
    //--------------------------------------------------------------------------------------------------------------
    // setup a static dynamic buffer which is dynamically expanded (re-allocated) on demand
    // the data inside this buffer is not released when this function is left
    static int iReceivedBytes = 0;
    static std::vector<uint8_t> vecDynamicBuffer;

    // check how many RSCP frames are received, must be at least 1
    // multiple frames can only occur in this example if one or more frames are received with a big time delay
    // this should usually not occur but handling this is shown in this example
    int iReceivedRscpFrames = 0;
    while(!bStopExecution && ((iReceivedBytes > 0) || iReceivedRscpFrames == 0))
    {
        // check and expand buffer
        if((vecDynamicBuffer.size() - iReceivedBytes) < 4096) {
            // check maximum size
            if(vecDynamicBuffer.size() > RSCP_MAX_FRAME_LENGTH) {
                // something went wrong and the size is more than possible by the RSCP protocol
                printf("Maximum buffer size exceeded %i\n", vecDynamicBuffer.size());
                bStopExecution = true;
                break;
            }
            // increase buffer size by 4096 bytes each time the remaining size is smaller than 4096
            vecDynamicBuffer.resize(vecDynamicBuffer.size() + 4096);
        }
        // receive data
        int iResult = SocketRecvData(iSocket, &vecDynamicBuffer[0] + iReceivedBytes, vecDynamicBuffer.size() - iReceivedBytes);
        if(iResult < 0)
        {
            // check errno for the error code to detect if this is a timeout or a socket error
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                // receive timed out -> continue with re-sending the initial block
                printf("Response receive timeout (retry)\n");
                break;
            }
            // socket error -> check errno for failure code if needed
            printf("Socket receive error. errno %i\n", errno);
            bStopExecution = true;
            break;
        }
        else if(iResult == 0)
        {
            // connection was closed regularly by peer
            // if this happens on startup each time the possible reason is
            // wrong AES password or wrong network subnet (adapt hosts.allow file required)
            printf("Connection closed by peer\n");
            bStopExecution = true;
            break;
        }
        // increment amount of received bytes
        iReceivedBytes += iResult;

        // process all received frames
        while (!bStopExecution)
        {
            // round down to a multiple of AES_BLOCK_SIZE
            int iLength = ROUNDDOWN(iReceivedBytes, AES_BLOCK_SIZE);
            // if not even 32 bytes were received then the frame is still incomplete
            if(iLength == 0) {
                break;
            }
            // resize temporary decryption buffer
            std::vector<uint8_t> decryptionBuffer;
            decryptionBuffer.resize(iLength);
            // initialize encryption sequence IV value with value of previous block
            aesDecrypter.SetIV(ucDecryptionIV, AES_BLOCK_SIZE);
            // decrypt data from vecDynamicBuffer to temporary decryptionBuffer
            aesDecrypter.Decrypt(&vecDynamicBuffer[0], &decryptionBuffer[0], iLength / AES_BLOCK_SIZE);

            // data was received, check if we received all data
            int iProcessedBytes = processReceiveBuffer(&decryptionBuffer[0], iLength);
            if(iProcessedBytes < 0) {
                // an error occured;
                printf("Error parsing RSCP frame: %i\n", iProcessedBytes);
                // stop execution as the data received is not RSCP data
                bStopExecution = true;
                break;

            }
            else if(iProcessedBytes > 0) {
                // round up the processed bytes as iProcessedBytes does not include the zero TAG_EMS_OUT_POWER_ADDing bytes
                iProcessedBytes = ROUNDUP(iProcessedBytes, AES_BLOCK_SIZE);
                // store the IV value from encrypted buffer for next block decryption
                memcpy(ucDecryptionIV, &vecDynamicBuffer[0] + iProcessedBytes - AES_BLOCK_SIZE, AES_BLOCK_SIZE);
                // move the encrypted data behind the current frame data (if any received) to the front
                memcpy(&vecDynamicBuffer[0], &vecDynamicBuffer[0] + iProcessedBytes, vecDynamicBuffer.size() - iProcessedBytes);
                // decrement the total received bytes by the amount of processed bytes
                iReceivedBytes -= iProcessedBytes;
                // increment a counter that a valid frame was received and
                // continue parsing process in case a 2nd valid frame is in the buffer as well
                iReceivedRscpFrames++;
            }
            else {
                // iProcessedBytes is 0
                // not enough data of the next frame received, go back to receive mode if iReceivedRscpFrames == 0
                // or transmit mode if iReceivedRscpFrames > 0
                break;
            }
        }
    }
}

static void mainLoop(void)
{
    RscpProtocol protocol;
    bool bStopExecution = false;

    while(!bStopExecution)
    {
        //--------------------------------------------------------------------------------------------------------------
        // RSCP Transmit Frame Block Data
        //--------------------------------------------------------------------------------------------------------------
        SRscpFrameBuffer frameBuffer;
        memset(&frameBuffer, 0, sizeof(frameBuffer));

        // create an RSCP frame with requests to some example data
        createRequestExample(&frameBuffer);

        // check that frame data was created
        if(frameBuffer.dataLength > 0)
        {
            // resize temporary encryption buffer to a multiple of AES_BLOCK_SIZE
            std::vector<uint8_t> encryptionBuffer;
            encryptionBuffer.resize(ROUNDUP(frameBuffer.dataLength, AES_BLOCK_SIZE));
            // zero TAG_EMS_OUT_POWER_ADDing for data above the desired length
            memset(&encryptionBuffer[0] + frameBuffer.dataLength, 0, encryptionBuffer.size() - frameBuffer.dataLength);
            // copy desired data length
            memcpy(&encryptionBuffer[0], frameBuffer.data, frameBuffer.dataLength);
            // set continues encryption IV
            aesEncrypter.SetIV(ucEncryptionIV, AES_BLOCK_SIZE);
            // start encryption from encryptionBuffer to encryptionBuffer, blocks = encryptionBuffer.size() / AES_BLOCK_SIZE
            aesEncrypter.Encrypt(&encryptionBuffer[0], &encryptionBuffer[0], encryptionBuffer.size() / AES_BLOCK_SIZE);
            // save new IV for next encryption block
            memcpy(ucEncryptionIV, &encryptionBuffer[0] + encryptionBuffer.size() - AES_BLOCK_SIZE, AES_BLOCK_SIZE);

            // send data on socket
            int iResult = SocketSendData(iSocket, &encryptionBuffer[0], encryptionBuffer.size());
            if(iResult < 0) {
                printf("Socket send error %i. errno %i\n", iResult, errno);
                bStopExecution = true;
            }
            else {
                // go into receive loop and wait for response
                receiveLoop(bStopExecution);
            }
        }
        // free frame buffer memory
        protocol.destroyFrameData(&frameBuffer);

        if (CounterHM == HM_Intervall)
          CounterHM = 0;
        CounterHM ++;

        writeCharRscp(TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME, TAG_EMS_OUT_SERIAL_NUMBER, TAG_INFO_OUT_SW_RELEASE);

        if(Counter900 == 900){
          Counter900 = 0;
        }
        Counter900 ++;
        system ("cp /mnt/RAMDisk/E3dcRscpData.txt /mnt/RAMDisk/E3dcCache.txt");
        system ("cp /mnt/RAMDisk/E3dcRscpWbData.txt /mnt/RAMDisk/E3dcWbCache.txt");
        // main loop sleep / cycle time before next request
        int sleeptime = 1;
        if ( GUI == 1 && E3DC_S10 == 1)
          sleeptime = 1;
        else
          sleeptime = SleepTime;
        sleep(sleeptime);
    }
}

int main()
{
  //Parameter einbinden, checken oder default setzen
	checkDefinePara(0);

  //Dateien erstellen
  makeCharRscp();
  writeRscp(PosPVI, 0);
  writeRscp(PosBat, 0);
  writeRscp(PosHome, 0);
  writeRscp(PosGrid, 0);
  writeRscp(PosSOC, 0);
  writeRscp(PosBatState, 1);
  writeRscp(PosAutarky, 0);
  writeRscp(PosSelfCon, 0);
  writeRscp(PosADD, 0);
  writeRscp(PosWbAll, 0);
  writeRscp(PosWbSolar, 0);
  writeRscp(PosPVIState, 1);
  writeRscp(PosPMState, 1);
  writeRscp(PosTimeZone,time_zone);
  make900();
  writeTo(PosToIdlePeriod, 1);
  makeIdle();
  for(int i = 0; i < PosWbMAX; i++){
    writeRscpWb(i,0);
  }

    // endless application which re-connections to server on connection lost
    while(true){


        // connect to server
        printf("Connecting to server %s:%i\n", SERVER_IP, SERVER_PORT);
        iSocket = SocketConnect(SERVER_IP, SERVER_PORT);
        if(iSocket < 0) {
            printf("Connection failed\n");
            sleep(1);
            continue;
        }
        printf("Connected successfully\n");

        // reset authentication flag
        iAuthenticated = 0;

        // create AES key and set AES parameters
        {
            // initialize AES encryptor and decryptor IV
            memset(ucDecryptionIV, 0xff, AES_BLOCK_SIZE);
            memset(ucEncryptionIV, 0xff, AES_BLOCK_SIZE);

            // limit password length to AES_KEY_SIZE
            int iPasswordLength = strlen(AES_PASS);
            if(iPasswordLength > AES_KEY_SIZE)
                iPasswordLength = AES_KEY_SIZE;

            // copy up to 32 bytes of AES key password
            uint8_t ucAesKey[AES_KEY_SIZE];
            memset(ucAesKey, 0xff, AES_KEY_SIZE);
            memcpy(ucAesKey, AES_PASS, iPasswordLength);

            // set encryptor and decryptor parameters
            aesDecrypter.SetParameters(AES_KEY_SIZE * 8, AES_BLOCK_SIZE * 8);
            aesEncrypter.SetParameters(AES_KEY_SIZE * 8, AES_BLOCK_SIZE * 8);
            aesDecrypter.StartDecryption(ucAesKey);
            aesEncrypter.StartEncryption(ucAesKey);
        }

        // enter the main transmit / receive loop
        mainLoop();

        // close socket connection
        SocketClose(iSocket);
        iSocket = -1;
    }

    return 0;
}

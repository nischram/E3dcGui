#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "Rscp/RscpProtocol.h"
#include "Rscp/RscpTags.h"
#include "Rscp/SocketConnection.h"
#include "Rscp/AES.h"
#include "Rscp/RWData.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>
#include <ctime>
#include "parameter.h"

static int iSocket = -1;
static int iAuthenticated = 0;
static AES aesEncrypter;
static AES aesDecrypter;
static uint8_t ucEncryptionIV[AES_BLOCK_SIZE];
static uint8_t ucDecryptionIV[AES_BLOCK_SIZE];
static int32_t TAG_EMS_OUT_UNIXTIME = 0;
static char TAG_EMS_OUT_DATE [20], TAG_EMS_OUT_TIME [20], TAG_EMS_OUT_SERIAL_NUMBER [17];
static int CounterHM = 0;
static int Counter900 = 0;


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
      writeCharRscp(TAG_EMS_OUT_DATE, TAG_EMS_OUT_TIME, TAG_EMS_OUT_SERIAL_NUMBER);
      if(Counter900 == 900){
        Counter900 = 0;
      }
      Counter900 ++;

        printf("\n____________________\nRequest cyclic data\n");
        // request data information
        if(TAG_EMS_OUT_UNIXTIME == 0 || (Seriennummer == 1 && CounterHM == HM_Intervall)){
          protocol.appendValue(&rootValue, TAG_INFO_REQ_SERIAL_NUMBER);
        }
        protocol.appendValue(&rootValue, TAG_INFO_REQ_TIME);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_PV);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_BAT);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_HOME);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_GRID);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_BAT_SOC);

        // request battery information
        SRscpValue batteryContainer;
        protocol.createContainerValue(&batteryContainer, TAG_BAT_REQ_DATA);
        protocol.appendValue(&batteryContainer, TAG_BAT_INDEX, (uint8_t)0);
        //protocol.appendValue(&batteryContainer, TAG_BAT_REQ_RSOC);
        protocol.appendValue(&batteryContainer, TAG_BAT_REQ_DEVICE_STATE);
        //protocol.appendValue(&batteryContainer, TAG_BAT_REQ_CURRENT);
        // append sub-container to root container
        protocol.appendValue(&rootValue, batteryContainer);
        // free memory of sub-container as it is now copied to rootValue
        protocol.destroyValueData(batteryContainer);

        protocol.appendValue(&rootValue, TAG_EMS_REQ_AUTARKY);
        protocol.appendValue(&rootValue, TAG_EMS_REQ_SELF_CONSUMPTION);
        if (Additional == 1)
          protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_ADD);
        else
          writeRscp(PosADD,0);
        if (Wallbox == 1){
          protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_WB_ALL);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_POWER_WB_SOLAR);
        }
        else {
          writeRscp(PosWbAll,0);
          writeRscp(PosWbSolar,0);
        }

        // request PVI information
        SRscpValue PVIContainer;
        protocol.createContainerValue(&PVIContainer, TAG_PVI_REQ_DATA);
        protocol.appendValue(&PVIContainer, TAG_PVI_INDEX, (uint8_t)0);
        protocol.appendValue(&PVIContainer, TAG_PVI_REQ_ON_GRID);
        // append sub-container to root container
        protocol.appendValue(&rootValue, PVIContainer);
        // free memory of sub-container as it is now copied to rootValue
        protocol.destroyValueData(PVIContainer);

        // request PM information
        SRscpValue PMContainer;
        protocol.createContainerValue(&PMContainer, TAG_PM_REQ_DATA);
        protocol.appendValue(&PMContainer, TAG_PM_INDEX, (uint8_t)0);
        protocol.appendValue(&PMContainer, TAG_PM_REQ_DEVICE_STATE);
        // append sub-container to root container
        protocol.appendValue(&rootValue, PMContainer);
        // free memory of sub-container as it is now copied to rootValue
        protocol.destroyValueData(PMContainer);

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
      case TAG_INFO_TIME: {    // response for TAG_INFO_REQ_TIME
          TAG_EMS_OUT_UNIXTIME = protocol->getValueAsInt32(response);
          int timeZone = readRscp(PosTimeZone);
          TAG_EMS_OUT_UNIXTIME = TAG_EMS_OUT_UNIXTIME - timeZone;
          time_t timestamp;
          tm *sys;
          timestamp = TAG_EMS_OUT_UNIXTIME;
          sys = localtime(&timestamp);
          strftime (TAG_EMS_OUT_DATE,40,"%d.%m.%Y",sys);
          strftime (TAG_EMS_OUT_TIME,40,"%H:%M:%S",sys);
          writeUnixtime(UnixtimeE3dc, TAG_EMS_OUT_UNIXTIME);
          cout << "System Time is " << TAG_EMS_OUT_DATE << "_" << TAG_EMS_OUT_TIME << "\n";
          cout << "System Unix-Time is " << TAG_EMS_OUT_UNIXTIME << "\n";
          printsendHM(CounterHM, TAG_EMS_ISE_UNIXTIME, TAG_EMS_OUT_UNIXTIME);
          break;
      }
      case TAG_EMS_POWER_PV: {    // response for TAG_EMS_REQ_POWER_PV
        int32_t TAG_EMS_OUT_POWER_PV = protocol->getValueAsInt32(response);
        cout << "PV Power is " << TAG_EMS_OUT_POWER_PV <<" W\n";
        writeRscp(PosPVI,TAG_EMS_OUT_POWER_PV);
        char file[20];
        snprintf (file, (size_t)20, "PVI900");
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
      case TAG_EMS_POWER_ADD: {    // response for TAG_EMS_REQ_POWER_ADD
        int32_t TAG_EMS_OUT_POWER_ADD = protocol->getValueAsInt32(response);
        cout << "Additional Power is " << TAG_EMS_OUT_POWER_ADD << " W\n";
        writeRscp(PosADD,TAG_EMS_OUT_POWER_ADD);
        char file[20];
        snprintf (file, (size_t)20, "Add900");
        write900(PosAdd900, file, TAG_EMS_OUT_POWER_ADD, Counter900);
        printsendHM(CounterHM, TAG_EMS_ISE_POWER_ADD, TAG_EMS_OUT_POWER_ADD);
        break;
      }
      case TAG_EMS_POWER_WB_ALL: {    // response for TAG_EMS_REQ_POWER_WB_ALL
        int32_t TAG_EMS_OUT_POWER_WB_ALL = protocol->getValueAsInt32(response);
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
            case TAG_BAT_DEVICE_STATE: {    // response for TAG_BAT_REQ_DEVICE_STATE
                bool TAG_EMS_OUT_BAT_STATE = protocol->getValueAsBool(&batteryData[i]);
                writeRscp(PosBatState,TAG_EMS_OUT_BAT_STATE);
                cout << "Battery State = " << TAG_EMS_OUT_BAT_STATE << " \n";
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
                cout << "PVI State = " << TAG_EMS_OUT_PVI_STATE << " \n";
                writeRscp(PosPVIState,TAG_EMS_OUT_PVI_STATE);
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
                cout << "LM0 State = " << TAG_EMS_OUT_PM_STATE << " \n";
                writeRscp(PosPMState,TAG_EMS_OUT_PM_STATE);
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
    // ...
    default:
        // default behavior
        printf("Unknown tag %08X\n", response->tag);
        break;
    }
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

        // main loop sleep / cycle time before next request
        int screenState = readScreen(ScreenState);
        if (CounterHM == HM_Intervall){
          CounterHM = 0;
        }
        CounterHM ++;
        if (screenState == ScreenOff) {
          int i;
          while (1){
            screenState = readScreen(ScreenState);
            if (screenState == ScreenOff && i <= HM_Intervall){
              sleep(1);
              i++;
            }
            else {
              CounterHM = HM_Intervall;
              i = 0;
              break;
            }
          }
        }
        else
          sleep(SleepTime);
    }
}

int main()
{
  //Offset für Zeitzone lesen
  int time_zone = readTimeZone()    ;
  //Dateien erstellen
  makeCharRscp();
  makeRscp(PosPVI, 0);
  makeRscp(PosBat, 0);
  makeRscp(PosHome, 0);
  makeRscp(PosGrid, 0);
  makeRscp(PosSOC, 0);
  makeRscp(PosBatState, 1);
  makeRscp(PosAutarky, 0);
  makeRscp(PosSelfCon, 0);
  makeRscp(PosADD, 0);
  makeRscp(PosWbAll, 0);
  makeRscp(PosWbSolar, 0);
  makeRscp(PosPVIState, 1);
  makeRscp(PosPMState, 1);
  makeRscp(PosTimeZone,time_zone);
  make900(PosPVI900, 1);
  make900(PosBatOut900, 1);
  make900(PosBatIn900, 1);
  make900(PosHome900, 1);
  make900(PosNetIn900, 1);
  make900(PosNetOut900, 1);
  make900(PosSOC900, 1);
  make900(PosAdd900, 1);
  make900(PosWBAll900, 1);
  make900(PosWBSolar900, 1);

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

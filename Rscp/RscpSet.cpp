/*
g++ -O3 RscpMain.cpp Rscp/RscpProtocol.cpp Rscp/AES.cpp Rscp/SocketConnection.cpp Rscp/RWData.cpp -o RscpMain
 */
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "../Rscp/RscpProtocol.h"
#include "../Rscp/RscpTags.h"
#include "../Rscp/SocketConnection.h"
#include "../Rscp/AES.h"
#include "../checkPara.h"
#include "../Rscp/RWData.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>
#include <ctime>

static int iSocket = -1;
static int iAuthenticated = 0;
static AES aesEncrypter;
static AES aesDecrypter;
static uint8_t ucEncryptionIV[AES_BLOCK_SIZE];
static uint8_t ucDecryptionIV[AES_BLOCK_SIZE];
static char TAG_EMS_OUT_SERIAL_NUMBER [17];
static uint8_t WbExt[6];
static int WbBtC, WbBbC;

static bool setModeWB = false, setModeEP = false, setModeBL = false, setModePS = false, setModeWR = false;
static bool WbSwPh = false, WbStop = false;
static bool blUsed = false, psUsed = false, wrUsed = false;
static int inputEpReserveMax, inputEpReserve;
static uint32_t blChargeLimit, blDischargeLimit, blDischargeStart = 65;

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
        printf("Request authentication\n");
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
        printf("____________________\n");
        // request data information
        protocol.appendValue(&rootValue, TAG_INFO_REQ_SERIAL_NUMBER);
        if (setModeWB){
          printf("Set Wallbox Parameter\nBat to Car    = %i\nBat bevor Car = %i\nLadestrom     = %i A\nWallboxmodus  = %i 1=Sonne/2=Mix\n", WbBtC, WbBbC, WbExt[1], WbExt[0]);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_SET_BATTERY_TO_CAR_MODE,(uint8_t)WbBtC);
          protocol.appendValue(&rootValue, TAG_EMS_REQ_SET_BATTERY_BEFORE_CAR_MODE,(uint8_t)WbBbC);

          // request Wallbox information
          SRscpValue WBContainer;
          SRscpValue WBExtContainer;
          // request Wallbox data
          protocol.createContainerValue(&WBContainer, TAG_WB_REQ_DATA) ;
          // add index 0 to select first wallbox
          protocol.appendValue(&WBContainer, TAG_WB_INDEX,0);
          protocol.createContainerValue(&WBExtContainer, TAG_WB_REQ_SET_EXTERN);
          protocol.appendValue(&WBExtContainer, TAG_WB_EXTERN_DATA_LEN,6);
          protocol.appendValue(&WBExtContainer, TAG_WB_EXTERN_DATA,WbExt,6);
          protocol.appendValue(&WBContainer, WBExtContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(WBExtContainer);
          // append sub-container to root container
          protocol.appendValue(&rootValue, WBContainer);
          //protocol.appendValue(&rootValue, WBExtContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(WBContainer);
        }
        else if (setModeEP){
          printf("Set Notstrom-Reserve\nEP Reserve    = %i Wh\n", inputEpReserve);
          // Set EP_Reserve
          SRscpValue SEContainer;
          SRscpValue SESetContainer;
          // request Wallbox data
          protocol.createContainerValue(&SEContainer, TAG_SE_REQ_SET_EP_RESERVE) ;
          // add index 0 to select first wallbox
          //protocol.createContainerValue(&SESetContainer, TAG_SE_REQ_SET_EP_RESERVE);
          protocol.appendValue(&SEContainer, TAG_SE_PARAM_INDEX,0);
          protocol.appendValue(&SEContainer, TAG_SE_PARAM_EP_RESERVE,(float)0);
          protocol.appendValue(&SEContainer, TAG_SE_PARAM_EP_RESERVE_W,(float)inputEpReserve);
          //protocol.appendValue(&SEContainer, SESetContainer);
          // free memory of sub-container as it is now copied to rootValue
          //protocol.destroyValueData(SESetContainer);
          // append sub-container to root container
          protocol.appendValue(&rootValue, SEContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(SEContainer);
        }
        else if (setModeBL){
          // Set POWER_LIMITS
          SRscpValue BlSetContainer;
          // request data
          protocol.createContainerValue(&BlSetContainer, TAG_EMS_REQ_SET_POWER_SETTINGS);
          protocol.appendValue(&BlSetContainer, TAG_EMS_POWER_LIMITS_USED,blUsed);
          if (blUsed){
            protocol.appendValue(&BlSetContainer, TAG_EMS_MAX_CHARGE_POWER,blChargeLimit);
            protocol.appendValue(&BlSetContainer, TAG_EMS_MAX_DISCHARGE_POWER,blDischargeLimit);
    				protocol.appendValue(&BlSetContainer, TAG_EMS_DISCHARGE_START_POWER,blDischargeStart);
          }
          // append sub-container to root container
          protocol.appendValue(&rootValue, BlSetContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(BlSetContainer);
          SRscpValue BlGetContainer;
          protocol.createContainerValue(&BlGetContainer, TAG_EMS_REQ_GET_POWER_SETTINGS);
          // append sub-container to root container
          protocol.appendValue(&rootValue, BlGetContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(BlGetContainer);
        }
        else if (setModePS){
          // Set POWER_LIMITS
          SRscpValue BlSetContainer;
          // request data
          protocol.createContainerValue(&BlSetContainer, TAG_EMS_REQ_SET_POWER_SETTINGS);
          protocol.appendValue(&BlSetContainer, TAG_EMS_POWERSAVE_ENABLED,psUsed);
          // append sub-container to root container
          protocol.appendValue(&rootValue, BlSetContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(BlSetContainer);
          SRscpValue BlGetContainer;
          protocol.createContainerValue(&BlGetContainer, TAG_EMS_REQ_GET_POWER_SETTINGS);
          // append sub-container to root container
          protocol.appendValue(&rootValue, BlGetContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(BlGetContainer);
        }
        else if (setModeWR){
          // Set POWER_LIMITS
          SRscpValue BlSetContainer;
          // request data
          protocol.createContainerValue(&BlSetContainer, TAG_EMS_REQ_SET_POWER_SETTINGS);
          protocol.appendValue(&BlSetContainer, TAG_EMS_WEATHER_REGULATED_CHARGE_ENABLED,wrUsed);
          // append sub-container to root container
          protocol.appendValue(&rootValue, BlSetContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(BlSetContainer);
          SRscpValue BlGetContainer;
          protocol.createContainerValue(&BlGetContainer, TAG_EMS_REQ_GET_POWER_SETTINGS);
          // append sub-container to root container
          protocol.appendValue(&rootValue, BlGetContainer);
          // free memory of sub-container as it is now copied to rootValue
          protocol.destroyValueData(BlGetContainer);
        }
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
      cout << "Send to Serial-Number " << serialNr << "\n";
      strcpy(TAG_EMS_OUT_SERIAL_NUMBER, serialNr.c_str());
      break;
    }
    case TAG_EMS_SET_BATTERY_TO_CAR_MODE: {    // response for TAG_EMS_REQ_SET_BATTERY_TO_CAR_MODE
      uint8_t BtC = protocol->getValueAsUChar8(response);
      //cout << "Set Battery to Car = " << BtC << "\n";
      break;
    }
    case TAG_EMS_SET_BATTERY_BEFORE_CAR_MODE: {    // response for TAG_EMS_REQ_SET_BATTERY_BEFORE_CAR_MODE
      uint8_t BbC = protocol->getValueAsUChar8(response);
      //cout << "Set Battery bevor Car = " << BbC << "\n";
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
            // check each battery sub tag
            switch(WBData[i].tag) {
              case TAG_WB_INDEX: {
                  ucWBIndex = protocol->getValueAsUChar8(&WBData[i]);
                  break;
              }
              // ...
              default:
                // default behaviour
                //printf("Unknown WB tag %08X\n", response->tag);
                break;
            }
        }
        protocol->destroyValueData(WBData);
        break;
    }
    case TAG_SE_EP_RESERVE: {        // resposne for TAG_WB_REQ_DATA
        uint8_t ucSEIndex = 0;
        std::vector<SRscpValue> SEData = protocol->getValueAsContainer(response);
        for(size_t i = 0; i < SEData.size(); ++i) {
            if(SEData[i].dataType == RSCP::eTypeError) {
                // handle error for example access denied errors
                uint32_t uiErrorCode = protocol->getValueAsUInt32(&SEData[i]);
                printf("Tag 0x%08X received error code %u.\n", SEData[i].tag, uiErrorCode);
                return -1;
            }
            // check each battery sub tag
            switch(SEData[i].tag) {
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
              if (setModeBL) cout << "Battery Limits = User\n";
              }
            else {
              if (setModeBL) cout << "Battery Limits = System\n";
            }
            break;
          }
          case TAG_EMS_MAX_CHARGE_POWER: {              // 101 response for TAG_EMS_MAX_CHARGE_POWER
            uint32_t uPower = protocol->getValueAsUInt32(&BLData[i]);
            if (setModeBL) cout << "Battery Limits Charge = " << uPower << " W\n";
            break;
          }
          case TAG_EMS_MAX_DISCHARGE_POWER: {              //102 response for TAG_EMS_MAX_DISCHARGE_POWER
            uint32_t uPower = protocol->getValueAsUInt32(&BLData[i]);
            if (setModeBL) cout << "Battery Limits Disharge = " << uPower << " W\n";
            break;
          }
          case TAG_EMS_DISCHARGE_START_POWER:{              //103 response for TAG_EMS_DISCHARGE_START_POWER
            uint32_t uPower = protocol->getValueAsUInt32(&BLData[i]);
            if (setModeBL) cout << "Battery Start Power = " << uPower << " W\n";
            break;
          }
          case TAG_EMS_POWERSAVE_ENABLED: {              //104 response for TAG_EMS_POWERSAVE_ENABLED
            bool powerSave = protocol->getValueAsBool(&BLData[i]);
            writeRscp(PosPowerSave,powerSave);
            if (powerSave){
              if (setModePS) cout << "Powersave = aktiv\n";
            }
            else {
              if (setModePS) cout << "Powersave = deaktiv\n";
            }
            break;
          }
          case TAG_EMS_WEATHER_REGULATED_CHARGE_ENABLED: {//105 resp WEATHER_REGULATED_CHARGE_ENABLED
            bool waetherReg = protocol->getValueAsBool(&BLData[i]);
            if (waetherReg){
              if (setModeWR) cout << "Weather regulated charge = aktiv \n";
            }
            else {
              if (setModeWR) cout << "Weather regulated charge = deaktiv \n";
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
    int loop_count = 0;

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

        loop_count++;
    		if (loop_count > 1) {
    			bStopExecution = true;
    		}

    }
}

int main(int argc, char *argv[])
{
    //Parameter einbinden, checken oder default setzen
  	checkDefinePara(0);

    // Set Mode
    if (strcmp(argv[1], "-wb")==0) {
      printf("Wallbox Set Parameter\n");
      setModeWB = true;
    }
    else if (strcmp(argv[1], "-ep")==0) {
      printf("Notstromreserve setzen\n");
      setModeEP = true;
    }
    else if (strcmp(argv[1], "-bl")==0) {
      printf("Batterielimits setzen\n");
      setModeBL = true;
    }
    else if (strcmp(argv[1], "-ps")==0) {
      printf("Wechselrichter Powersave setzen\n");
      setModePS = true;
    }
    else if (strcmp(argv[1], "-wr")==0) {
      printf("Wetterprognose setzen\n");
      setModeWR = true;
    }
    else {
      printf("Falsche Eingabe!\n Bitte wählen:\n");
      printf("  Wallbox         = -wb\n");
      printf("  Notstromreserve = -ep\n");
      printf("  Batterielimits  = -bl\n");
      //printf("  Powersave       = -ps\n");
      //printf("  Wetterprognose  = -wr\n");
      printf(" Beispiel        = RscpSet -wb -sonne 16 -BtCno -BbCno\n");
      return 0;
    }
    // Wallbox Parameter
    if (setModeWB){
      // Sonnenmodus
      if (strcmp(argv[2], "-sonne")==0) {
        printf("Sonnenmodus\n");
        WbExt[0] = 1;
      }
      else if (strcmp(argv[2], "-mix")==0) {
        printf("Mischbetrieb\n");
        WbExt[0] = 2;
      }
      else {
        printf("Falsche Eingabe!\n Bitte wählen:\n  Sonnenmodus    = -sonne \n  Mischbetrieb   = -mix\n  Beispiel        = RscpSet -wb -sonne 16 -BtCno -BbCno\n");
        return 0;
      }
      // Ladestrom
      int maxAmp = atoi(argv[3]);
      if (maxAmp < 0 || maxAmp > 32){
        printf("Falsche Eingabe!\n Wert von 0 - 32 A wählen\n  Beispiel     = RscpSet -wb -sonne 16 -BtCno -BbCno\n");
        return 0;
      }
      printf("Ladestrom = %s\n", argv[3]);
      WbExt[1] = atoi(argv[3]);
      // Battery to Car
      if (strcmp(argv[4], "-BtCyes")==0) {
        printf("Batterie ins Auto = Ein\n");
        WbBtC = 1;
      }
      else if (strcmp(argv[4], "-BtCno")==0) {
        printf("Batterie ins Auto = Aus\n");
        WbBtC = 0;
      }
      else {
        printf("Falsche Eingabe!\n Wert: -BtCyes or -BtCno \n  Beispiel     = RscpSet -wb -sonne 16 -BtCno -BbCno\n");
      }
      // Battery bevor Car
      if (strcmp(argv[5], "-BbCyes")==0) {
        printf("Batterie vor Auto = Ein\n");
        WbBbC = 1;
      }
      else if (strcmp(argv[5], "-BbCno")==0) {
        printf("Batterie vor Auto = Aus\n");
        WbBbC = 0;
      }
      else {
        printf("Falsche Eingabe!\n Wert: -BbCyes or -BbCno \n  Beispiel     = RscpSet -wb -sonne 16 -BtCno -BbCno\n");
      }
      // Anzahl Phasen tauschen oder Ladung stoppen
      if (strcmp(argv[6], "-swPh")==0) {
        printf("Anzahl Phasen tauschen\n");
        WbExt[3] = 1;
      }
      else if (strcmp(argv[6], "-stop")==0) {
        printf("Ladung stoppen\n");
        WbExt[4] = 1;
      }
      else if (strcmp(argv[6], "-no")==0) {
        // für keine Eingabe bei Anzahl Phasen tauschen oder stoppen der Ladung
      }
      else {
        printf("Keine Eingabe für Anzahl Phasen tauschen \n Wert: -swPh \n  Beispiel     = RscpSet -wb -sonne 16 -BtCno -BbCno -swPh\n");
        printf("oder\nkeine Eingabe für das stoppen der Ladung\n Wert: -stop \n  Beispiel     = RscpSet -wb -sonne 16 -BtCno -BbCno -stop\n");
      }
    }
    else if (setModeEP){
      // EP Reserve Max argv[2]
      inputEpReserveMax = atoi(argv[2]);
      if (inputEpReserveMax < 0 || inputEpReserveMax > 65535){
        printf("Falsche Eingabe für argv[2] in Wh!\n Wert von 0 - 65535 wählen\n  Beispiel     = RscpSet -ep 12800 4000\n");
        return 0;
      }
      else printf("EP Reserve Max = %i\n",inputEpReserveMax);

      // EP Reserve argv[3]
      inputEpReserve = atoi(argv[3]);
      if (inputEpReserve < 0 || inputEpReserve > inputEpReserveMax * 0.8){
        printf("Falsche Eingabe für argv[3] in Wh!\n Wert von 0 - <= 80% von Max wählen\n  Beispiel     = RscpSet -ep 12800 4000\n");
        return 0;
      }
      else printf("EP Reserve     = %i\n",inputEpReserve);
    }
    else if (setModeBL){
      // BL Used argv[2]
      if (strcmp(argv[2], "-blYes")==0) {
        printf("Batterielimits = manuell\n");
        blUsed = true;
      }
      else if (strcmp(argv[2], "-blNo")==0) {
        printf("Batterielimits = automatisch\n");
        blUsed = false;
      }
      else {
        printf("Falsche Eingabe für argv[2] Option: -blYes -blNo\n");
        return 0;
      }
      if (blUsed){
        // BL Charge Limit argv[3]
        blChargeLimit = atoi(argv[3]);
        if (blChargeLimit < 0 || blChargeLimit > 65535){
          printf("Falsche Eingabe für argv[3] in W!\n Wert von 0 - 65535 wählen\n  Beispiel     = RscpSet -bl -blYes 3500 4000 65\n");
          return 0;
        }
        else printf("Batterielimit laden    = %i W\n",blChargeLimit);
        // BL Discharge Limit argv[4]
        blDischargeLimit = atoi(argv[4]);
        if (blDischargeLimit < 0 || blDischargeLimit > 65535){
          printf("Falsche Eingabe für argv[4] in W!\n Wert von 0 - 65535 wählen\n  Beispiel     = RscpSet -bl -blYes 3500 4000 65\n");
          return 0;
        }
        else printf("Batterielimit endladen = %i W\n",blDischargeLimit);
        // BL Discharge Start argv[5]
        if (argc > 5){
          blDischargeStart = atoi(argv[5]);
          if (blDischargeStart < 0 || blDischargeStart > 400){
            printf("Falsche Eingabe für argv[5] in W!\nWert von 0 - 400 wählen\n  Beispiel     = RscpSet -bl -blYes 3500 4000 65\n");
          }
          else printf("Batteriestart endladen = %i W\n",blDischargeStart);
        }
        else printf("Batteriestart endladen = %i\n",blDischargeStart);
      }
    }
  /* Das setzen von Powersave und Wetterprognose habe ich noch nicht hinbekommen.
    else if (setModePS){
      // BL Used argv[2]
      if (strcmp(argv[2], "-psYes")==0) {
        printf("Wechselrichter Powersave = aus\n");
        psUsed = true;
      }
      else if (strcmp(argv[2], "-psNo")==0) {
        printf("Wechselrichter Powersave = ein\n");
        psUsed = false;
      }
      else {
        printf("Falsche Eingabe für argv[2] Option: -psYes -psNo\n");
        return 0;
      }
    }
    else if (setModeWR){
      // PS Used argv[2]
      if (strcmp(argv[2], "-wrYes")==0) {
        printf("Wetterprognose = aus\n");
        wrUsed = true;
      }
      else if (strcmp(argv[2], "-wrNo")==0) {
        printf("Wetterprognose = ein\n");
        wrUsed = false;
      }
      else {
        printf("Falsche Eingabe für argv[2] Option: -wrYes -wrNo\n");
        return 0;
      }
    }
  */
    printf("____________________\n");


        // connect to server
        printf("Connecting to server %s:%i\n", SERVER_IP, SERVER_PORT);
        iSocket = SocketConnect(SERVER_IP, SERVER_PORT);
        if(iSocket < 0) {
            printf("Connection failed\n");
            return 0;
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
        printf("Parameter gesendet\n");

    return 0;
}

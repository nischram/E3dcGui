#ifndef __CHECKPARA_H_
#define __CHECKPARA_H_

#include "/home/pi/E3dcGui/parameter.h"
#include "/home/pi/E3dcGui/parameterHM.h"

void checkDefine(int print){
/*
// Nach diesem Beispiel wird eine neue Variable in der "parameter.h geprüft oder ein default Wert gesetzt".
  #ifndef GUI
    #define GUI 1
    if(print == 1){
      printf("Der Parameter \"GUI\" ist in der \"parameter.h\" nicht vorhanden.\n");
      printf("    Als default wurde für \"GUI\" folgender Wert gesetzt: %i\n", GUI);
      sleep(2);
    }
  #endif
*/
#ifndef WALLBOX_ACTION
  #define WALLBOX_ACTION 0
  if(print == 1){
    printf("Der Parameter \"WALLBOX_ACTION\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"WALLBOX_ACTION\" folgender Wert gesetzt: %i\n", WALLBOX_ACTION);
    sleep(2);
  }
#endif
#ifndef EP_RESERVE_ACTION
  #define EP_RESERVE_ACTION 0
  if(print == 1){
    printf("Der Parameter \"EP_RESERVE_ACTION\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"EP_RESERVE_ACTION\" folgender Wert gesetzt: %i\n", EP_RESERVE_ACTION);
    sleep(2);
  }
#endif
#ifndef TAG_SE_ISE_EP_RESERVE_W
  #define TAG_SE_ISE_EP_RESERVE_W 0
  if(print == 1){
    printf("Der Parameter \"TAG_SE_ISE_EP_RESERVE_W\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"TAG_SE_ISE_EP_RESERVE_W\" folgender Wert gesetzt: %i\n", TAG_SE_ISE_EP_RESERVE_W);
    sleep(2);
  }
#endif
#ifndef ISE_EP_RESERVE_SEND_NOW
  #define ISE_EP_RESERVE_SEND_NOW 0
  if(print == 1){
    printf("Der Parameter \"ISE_EP_RESERVE_SEND_NOW\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_EP_RESERVE_SEND_NOW\" folgender Wert gesetzt: %i\n", ISE_EP_RESERVE_SEND_NOW);
    sleep(2);
  }
#endif
#ifndef ISE_EP_RESERVE_SET_W
  #define ISE_EP_RESERVE_SET_W 0
  if(print == 1){
    printf("Der Parameter \"ISE_EP_RESERVE_SET_W\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_EP_RESERVE_SET_W\" folgender Wert gesetzt: %i\n", ISE_EP_RESERVE_SET_W);
    sleep(2);
  }
#endif
#ifndef TAG_EMS_ISE_SW_RELEASE
  #define TAG_EMS_ISE_SW_RELEASE 0
  if(print == 1){
    printf("Der Parameter \"TAG_EMS_ISE_SW_RELEASE\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"TAG_EMS_ISE_SW_RELEASE\" folgender Wert gesetzt: %i\n", TAG_EMS_ISE_SW_RELEASE);
    sleep(2);
  }
#endif
#ifndef ISE_INSTALLED_PEAK_POWER
  #define ISE_INSTALLED_PEAK_POWER 0
  if(print == 1){
    printf("Der Parameter \"ISE_INSTALLED_PEAK_POWER\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_INSTALLED_PEAK_POWER\" folgender Wert gesetzt: %i\n", ISE_INSTALLED_PEAK_POWER);
    sleep(2);
  }
#endif
#ifndef ISE_DERATE_AT_PERCENT
  #define ISE_DERATE_AT_PERCENT 0
  if(print == 1){
    printf("Der Parameter \"ISE_DERATE_AT_PERCENT\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_DERATE_AT_PERCENT\" folgender Wert gesetzt: %i\n", ISE_DERATE_AT_PERCENT);
    sleep(2);
  }
#endif
#ifndef ISE_DERATE_AT_POWER
  #define ISE_DERATE_AT_POWER 0
  if(print == 1){
    printf("Der Parameter \"ISE_DERATE_AT_POWER\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_DERATE_AT_POWER\" folgender Wert gesetzt: %i\n", ISE_DERATE_AT_POWER);
    sleep(2);
  }
#endif
#ifndef ISE_EMS_STATUS
  #define ISE_EMS_STATUS 0
  if(print == 1){
    printf("Der Parameter \"ISE_EMS_STATUS\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_EMS_STATUS\" folgender Wert gesetzt: %i\n", ISE_EMS_STATUS);
    sleep(2);
  }
#endif
#ifndef ISE_EP_STATUS
  #define ISE_EP_STATUS 0
  if(print == 1){
    printf("Der Parameter \"ISE_EP_STATUS\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_EP_STATUS\" folgender Wert gesetzt: %i\n", ISE_EP_STATUS);
    sleep(2);
  }
#endif
#ifndef ISE_EMERGENCY_POWER_STATUS_INFO
  #define ISE_EMERGENCY_POWER_STATUS_INFO 0
  if(print == 1){
    printf("Der Parameter \"ISE_EMERGENCY_POWER_STATUS_INFO\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_EMERGENCY_POWER_STATUS_INFO\" folgender Wert gesetzt: %i\n", ISE_EMERGENCY_POWER_STATUS_INFO);
    sleep(2);
  }
#endif
#ifndef TAG_EMS_ISE_WB_BBC
  #define TAG_EMS_ISE_WB_BBC 0
  if(print == 1){
    printf("Der Parameter \"TAG_EMS_ISE_WB_BBC\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"TAG_EMS_ISE_WB_BBC\" folgender Wert gesetzt: %i\n", TAG_EMS_ISE_WB_BBC);
    sleep(2);
  }
#endif
#ifndef TAG_EMS_ISE_WB_BTC
  #define TAG_EMS_ISE_WB_BTC 0
  if(print == 1){
    printf("Der Parameter \"TAG_EMS_ISE_WB_BTC\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"TAG_EMS_ISE_WB_BTC\" folgender Wert gesetzt: %i\n", TAG_EMS_ISE_WB_BTC);
    sleep(2);
  }
#endif
#ifndef TAG_EMS_ISE_POWER_WB_GRID
  #define TAG_EMS_ISE_POWER_WB_GRID 0
  if(print == 1){
    printf("Der Parameter \"TAG_EMS_ISE_POWER_WB_GRID\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"TAG_EMS_ISE_POWER_WB_GRID\" folgender Wert gesetzt: %i\n", TAG_EMS_ISE_POWER_WB_GRID);
    sleep(2);
  }
#endif
#ifndef RSCP_DETAILED_OUTPUT
  #define RSCP_DETAILED_OUTPUT false
  if(print == 1){
    printf("Der Parameter \"RSCP_DETAILED_OUTPUT\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"RSCP_DETAILED_OUTPUT\" folgender Wert gesetzt: %i\n", RSCP_DETAILED_OUTPUT);
    sleep(2);
  }
#endif
#ifndef ISE_INFO_PRODUCTION_DATE
  #define ISE_INFO_PRODUCTION_DATE 0
  if(print == 1){
    printf("Der Parameter \"ISE_INFO_PRODUCTION_DATE\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_INFO_PRODUCTION_DATE\" folgender Wert gesetzt: %i\n", ISE_INFO_PRODUCTION_DATE);
    sleep(2);
  }
#endif
#ifndef ISE_BAT_CHARGE_CYCLES
  #define ISE_BAT_CHARGE_CYCLES 0
  if(print == 1){
    printf("Der Parameter \"ISE_BAT_CHARGE_CYCLES\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BAT_CHARGE_CYCLES\" folgender Wert gesetzt: %i\n", ISE_BAT_CHARGE_CYCLES);
    sleep(2);
  }
#endif
#ifndef ISE_BAT_DCB_COUNT
  #define ISE_BAT_DCB_COUNT 0
  if(print == 1){
    printf("Der Parameter \"ISE_BAT_DCB_COUNT\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BAT_DCB_COUNT\" folgender Wert gesetzt: %i\n", ISE_BAT_DCB_COUNT);
    sleep(2);
  }
#endif
#ifndef ISE_BAT_TRAINING_MODE
  #define ISE_BAT_TRAINING_MODE 0
  if(print == 1){
    printf("Der Parameter \"ISE_BAT_TRAINING_MODE\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BAT_TRAINING_MODE\" folgender Wert gesetzt: %i\n", ISE_BAT_TRAINING_MODE);
    sleep(2);
  }
#endif
#ifndef ISE_BAT_TRAINING_MODE_INFO
  #define ISE_BAT_TRAINING_MODE_INFO 0
  if(print == 1){
    printf("Der Parameter \"ISE_BAT_TRAINING_MODE_INFO\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BAT_TRAINING_MODE_INFO\" folgender Wert gesetzt: %i\n", ISE_BAT_TRAINING_MODE_INFO);
    sleep(2);
  }
#endif
#ifndef ISE_BAT_DEVICE_NAME
  #define ISE_BAT_DEVICE_NAME 0
  if(print == 1){
    printf("Der Parameter \"ISE_BAT_DEVICE_NAME\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BAT_DEVICE_NAME\" folgender Wert gesetzt: %i\n", ISE_BAT_DEVICE_NAME);
    sleep(2);
  }
#endif
#ifndef ISE_BAT_POWER_LIMITS_USED
  #define ISE_BAT_POWER_LIMITS_USED 0
  if(print == 1){
    printf("Der Parameter \"ISE_BAT_POWER_LIMITS_USED\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BAT_POWER_LIMITS_USED\" folgender Wert gesetzt: %i\n", ISE_BAT_POWER_LIMITS_USED);
    sleep(2);
  }
#endif
#ifndef ISE_BAT_CHARGE_LIMIT
  #define ISE_BAT_CHARGE_LIMIT 0
  if(print == 1){
    printf("Der Parameter \"ISE_BAT_CHARGE_LIMIT\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BAT_CHARGE_LIMIT\" folgender Wert gesetzt: %i\n", ISE_BAT_CHARGE_LIMIT);
    sleep(2);
  }
#endif
#ifndef ISE_BAT_DISCHARGE_LIMIT
  #define ISE_BAT_DISCHARGE_LIMIT 0
  if(print == 1){
    printf("Der Parameter \"ISE_BAT_DISCHARGE_LIMIT\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BAT_DISCHARGE_LIMIT\" folgender Wert gesetzt: %i\n", ISE_BAT_DISCHARGE_LIMIT);
    sleep(2);
  }
#endif
#ifndef ISE_BAT_DISCHARGE_START_POWER
  #define ISE_BAT_DISCHARGE_START_POWER 0
  if(print == 1){
    printf("Der Parameter \"ISE_BAT_DISCHARGE_START_POWER\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BAT_DISCHARGE_START_POWER\" folgender Wert gesetzt: %i\n", ISE_BAT_DISCHARGE_START_POWER);
    sleep(2);
  }
#endif
#ifndef ISE_POWERSAVE_ENABLED
  #define ISE_POWERSAVE_ENABLED 0
  if(print == 1){
    printf("Der Parameter \"ISE_POWERSAVE_ENABLED\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_POWERSAVE_ENABLED\" folgender Wert gesetzt: %i\n", ISE_POWERSAVE_ENABLED);
    sleep(2);
  }
#endif
#ifndef ISE_WEATHER_REGULATED_CHARGE_ENABLED
  #define ISE_WEATHER_REGULATED_CHARGE_ENABLED 0
  if(print == 1){
    printf("Der Parameter \"ISE_WEATHER_REGULATED_CHARGE_ENABLED\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_WEATHER_REGULATED_CHARGE_ENABLED\" folgender Wert gesetzt: %i\n", ISE_WEATHER_REGULATED_CHARGE_ENABLED);
    sleep(2);
  }
#endif
#ifndef BATTERYLIMIT_ACTION
  #define BATTERYLIMIT_ACTION 0
  if(print == 1){
    printf("Der Parameter \"BATTERYLIMIT_ACTION\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"BATTERYLIMIT_ACTION\" folgender Wert gesetzt: %i\n", BATTERYLIMIT_ACTION);
    sleep(2);
  }
#endif
#ifndef ISE_BL_SEND_NOW
  #define ISE_BL_SEND_NOW 0
  if(print == 1){
    printf("Der Parameter \"ISE_BL_SEND_NOW\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BL_SEND_NOW\" folgender Wert gesetzt: %i\n", ISE_BL_SEND_NOW);
    sleep(2);
  }
#endif
#ifndef ISE_BL_SEND_MODE
  #define ISE_BL_SEND_MODE 0
  if(print == 1){
    printf("Der Parameter \"ISE_BL_SEND_MODE\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BL_SEND_MODE\" folgender Wert gesetzt: %i\n", ISE_BL_SEND_MODE);
    sleep(2);
  }
#endif
#ifndef ISE_BL_SEND_CHARGE
  #define ISE_BL_SEND_CHARGE 0
  if(print == 1){
    printf("Der Parameter \"ISE_BL_SEND_CHARGE\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BL_SEND_CHARGE\" folgender Wert gesetzt: %i\n", ISE_BL_SEND_CHARGE);
    sleep(2);
  }
#endif
#ifndef ISE_BL_SEND_DISCHARGE
  #define ISE_BL_SEND_DISCHARGE 0
  if(print == 1){
    printf("Der Parameter \"ISE_BL_SEND_DISCHARGE\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_BL_SEND_DISCHARGE\" folgender Wert gesetzt: %i\n", ISE_BL_SEND_DISCHARGE);
    sleep(2);
  }
#endif
#ifndef POWERSAVE_ACTION
  #define POWERSAVE_ACTION 0
  if(print == 1){
    printf("Der Parameter \"POWERSAVE_ACTION\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"POWERSAVE_ACTION\" folgender Wert gesetzt: %i\n", POWERSAVE_ACTION);
    sleep(2);
  }
#endif
#ifndef ISE_PS_SEND_NOW
  #define ISE_PS_SEND_NOW 0
  if(print == 1){
    printf("Der Parameter \"ISE_PS_SEND_NOW\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_PS_SEND_NOW\" folgender Wert gesetzt: %i\n", ISE_PS_SEND_NOW);
    sleep(2);
  }
#endif
#ifndef ISE_PS_SEND_MODE
  #define ISE_PS_SEND_MODE 0
  if(print == 1){
    printf("Der Parameter \"ISE_PS_SEND_MODE\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_PS_SEND_MODE\" folgender Wert gesetzt: %i\n", ISE_PS_SEND_MODE);
    sleep(2);
  }
#endif
#ifndef WEATHER_REG_ACTION
  #define WEATHER_REG_ACTION 0
  if(print == 1){
    printf("Der Parameter \"WEATHER_REG_ACTION\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"WEATHER_REG_ACTION\" folgender Wert gesetzt: %i\n", WEATHER_REG_ACTION);
    sleep(2);
  }
#endif
#ifndef ISE_WR_SEND_NOW
  #define ISE_WR_SEND_NOW 0
  if(print == 1){
    printf("Der Parameter \"ISE_WR_SEND_NOW\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_WR_SEND_NOW\" folgender Wert gesetzt: %i\n", ISE_WR_SEND_NOW);
    sleep(2);
  }
#endif
#ifndef ISE_WR_SEND_MODE
  #define ISE_WR_SEND_MODE 0
  if(print == 1){
    printf("Der Parameter \"ISE_WR_SEND_MODE\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"ISE_WR_SEND_MODE\" folgender Wert gesetzt: %i\n", ISE_WR_SEND_MODE);
    sleep(2);
  }
#endif
#ifndef WDsendEmailRelease
  #define WDsendEmailRelease 0
  if(print == 1){
    printf("Der Parameter \"WDsendEmailRelease\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"WDsendEmailRelease\" folgender Wert gesetzt: %i\n", WDsendEmailRelease);
    sleep(2);
  }
#endif
#ifndef TAG_EMS_ISE_POWER_WB_GRID
  #define TAG_EMS_ISE_POWER_WB_GRID 0
  if(print == 1){
    printf("Der Parameter \"TAG_EMS_ISE_POWER_WB_GRID\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"TAG_EMS_ISE_POWER_WB_GRID\" folgender Wert gesetzt: %i\n", TAG_EMS_ISE_POWER_WB_GRID);
    sleep(2);
  }
#endif
#ifndef TAG_EMS_ISE_POWER_WB_GRID
  #define TAG_EMS_ISE_POWER_WB_GRID 0
  if(print == 1){
    printf("Der Parameter \"TAG_EMS_ISE_POWER_WB_GRID\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"TAG_EMS_ISE_POWER_WB_GRID\" folgender Wert gesetzt: %i\n", TAG_EMS_ISE_POWER_WB_GRID);
    sleep(2);
  }
#endif
#ifndef DHT1_OFFSET_TEMP
  #define DHT1_OFFSET_TEMP 0
  if(print == 1){
    printf("Der Parameter \"DHT1_OFFSET_TEMP\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"DHT1_OFFSET_TEMP\" folgender Wert gesetzt: %i\n", DHT1_OFFSET_TEMP);
    sleep(2);
  }
#endif
#ifndef DHT2_OFFSET_TEMP
  #define DHT2_OFFSET_TEMP 0
  if(print == 1){
    printf("Der Parameter \"DHT2_OFFSET_TEMP\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"DHT2_OFFSET_TEMP\" folgender Wert gesetzt: %i\n", DHT2_OFFSET_TEMP);
    sleep(2);
  }
#endif
#ifndef DHT3_OFFSET_TEMP
  #define DHT3_OFFSET_TEMP 0
  if(print == 1){
    printf("Der Parameter \"DHT3_OFFSET_TEMP\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"DHT3_OFFSET_TEMP\" folgender Wert gesetzt: %i\n", DHT3_OFFSET_TEMP);
    sleep(2);
  }
#endif
#ifndef DHT4_OFFSET_TEMP
  #define DHT4_OFFSET_TEMP 0
  if(print == 1){
    printf("Der Parameter \"DHT4_OFFSET_TEMP\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"DHT4_OFFSET_TEMP\" folgender Wert gesetzt: %i\n", DHT4_OFFSET_TEMP);
    sleep(2);
  }
#endif
#ifndef DHT5_OFFSET_TEMP
  #define DHT5_OFFSET_TEMP 0
  if(print == 1){
    printf("Der Parameter \"DHT5_OFFSET_TEMP\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"DHT5_OFFSET_TEMP\" folgender Wert gesetzt: %i\n", DHT5_OFFSET_TEMP);
    sleep(2);
  }
#endif
#ifndef PM_POSITION
  #define PM_POSITION 0
  if(print == 1){
    printf("Der Parameter \"PM_POSITION\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"PM_POSITION\" folgender Wert gesetzt: %i\n", PM_POSITION);
    sleep(2);
  }
#endif
#ifndef HM_XML_TOKEN
  #define HM_XML_TOKEN "aaAAbbBBccCC"
  if(print == 1){
    printf("Der Parameter \"HM_XML_TOKEN\" ist in der \"parameter.h\" nicht vorhanden.\n");
    printf("    Als default wurde für \"HM_XML_TOKEN\" folgender Wert gesetzt: %i\n", HM_XML_TOKEN);
    sleep(2);
  }
#endif
}

void checkDefineHM(int print){
/*
// Nach diesem Beispiel wird eine neue Variable in der "parameterHM.h geprüft oder ein default Wert gesetzt".
  #ifndef ISE_TEST
    #define ISE_TEST 7
    if(print == 1){
      printf("Der Parameter \"ISE_TEST\" ist in der \"parameterHM.h\" nicht vorhanden.\n");
      printf("    Als default wurde für \"ISE_TEST\" folgender Wert gesetzt: %i\n", ISE_TEST);
      sleep(2);
    }
  #endif
*/
}

void checkDefinePara(int print){
  //Parameter Checken oder default setzen
  checkDefine(print);
  if(Homematic_GUI == 1)	checkDefineHM(print);
}
#endif // __CHECKPARA_H_

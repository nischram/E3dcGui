#ifndef __RWData_H_
#define __RWData_H_

void printsendHM(int CounterHM, int id, int value);
void printsendCharHM(int CounterHM, int id, char value[32]);
void readWrite900(char *fileName, int NewValue);
int writeData(char *Path, int Position, int NewValue, int max);
int makeData(char *Path, int NewValue, int max);
int writeCharData(char *Path, char *CHAR1, char *CHAR2, char *CHAR3);
int readData(char *Path, int Position, int max);
int writeRscp(int Position, int NewValue);
int makeRscp();
int writeCharRscp(char *TAG_EMS_OUT_DATE, char *TAG_EMS_OUT_TIME, char *TAG_EMS_OUT_SERIAL_NUMBER);
int makeCharRscp();
int writeUnixtime(int Position, int NewTime);
int write900(int Position, char *fileName, int NewValue, int Counter900);
int make900();
int writeHistory(int Position, int NewValue, int writedata);
int writeHistoryTime(int Position,int AktuellTime, int NewValue, int writedata);

#endif // __RWData_H_

#ifndef __RWData_H_
#define __RWData_H_

void printsendHM(int CounterHM, int id, int value);
void printsendCharHM(int CounterHM, int id, char value[32]);
int readTimeZone();
void readWriteData(char fileName[40], int NewValue);
int writeData(char Path[128], int Position, int NewValue, int max);
int writeCharData(char Path[128], char *CHAR1, char *CHAR2, char *CHAR3);
int readData(char Path[128], int Position, int max);
int writeRscp(int Position, int NewValue);
int makeRscp(int Position, int NewValue);
int writeCharRscp(char *TAG_EMS_OUT_DATE, char *TAG_EMS_OUT_TIME, char *TAG_EMS_OUT_SERIAL_NUMBER);
int readRscp(int Position);
int makeCharRscp();
int writeUnixtime(int Position, int NewTime);
int write900(int Position, char fileName[40], int NewValue, int Counter900);
int make900(int Position, int NewValue);
int readScreen(int Position);
int writeHistory(int Position, int NewValue, int writedata);

#endif // __RWData_H_

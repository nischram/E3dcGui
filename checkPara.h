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

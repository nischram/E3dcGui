/*
gcc -g -o touchtest  touchtest.c
*/

#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "../parameter.h"
#include "touch.h"
#include "touch.c"
#include "framebuffer.c"

#define BUTTON_ON 1
#define BUTTON_OFF 0
#define X 0
#define Y 1
#define W 2
#define H 3

void  INThandler(int sig)
{

        signal(sig, SIG_IGN);
	closeFramebuffer();
        exit(0);
}
int mymillis()
{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

int main()
{
  system("fbset -fb /dev/fb0 -depth 16");    //Umschalten auf 16Bit Display

  signal(SIGINT, INThandler);

	int  xres,yres;

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	if (openTouchScreen() == 1)
		perror("error opening touch screen");

	getTouchScreenDetails(&screenXmin,&screenXmax,&screenYmin,&screenYmax);

	framebufferInitialize(&xres,&yres);

	scaleXvalue = ((float)screenXmax-screenXmin) / xres;
	printf ("X Scale Factor = %f\n", scaleXvalue);
	scaleYvalue = ((float)screenYmax-screenYmin) / yres;
	printf ("Y Scale Factor = %f\n", scaleYvalue);

	int	buttonLO= BUTTON_OFF;
	int buttonTimerLO = mymillis();
	int buttonCordsLO[4] = {4,4,30,30};
	createButton(buttonCordsLO[X],buttonCordsLO[Y],buttonCordsLO[W],buttonCordsLO[H],"1",WHITE,GREY);
	int	buttonRO= BUTTON_OFF;
	int buttonTimerRO = mymillis();
	int buttonCordsRO[4] = {770,4,30,30};
  createButton(buttonCordsRO[X],buttonCordsRO[Y],buttonCordsRO[W],buttonCordsRO[H],"2",GREY,BLACK);
	int	buttonRU= BUTTON_OFF;
	int buttonTimerRU = mymillis();
	int buttonCordsRU[4] = {770,450,30,30};
  createButton(buttonCordsRU[X],buttonCordsRU[Y],buttonCordsRU[W],buttonCordsRU[H],"3",GREY,BLACK);
	int	buttonLU= BUTTON_OFF;
	int buttonTimerLU = mymillis();
	int buttonCordsLU[4] = {4,450,30,30};
  createButton(buttonCordsLU[X],buttonCordsLU[Y],buttonCordsLU[W],buttonCordsLU[H],"4",GREY,BLACK);
	int buttonCordsM[4] = {300,210,200,60};
	int buttonCordsM1[4] = {310,220,180,20};
  int buttonCordsM2[4] = {310,240,180,20};
  int	buttonExit= BUTTON_OFF;
  int buttonTimerExit = mymillis();
  int buttonCordsExit[4] = {350,310,100,40};
  int buttonCordsOff[4] = {4,4,796,476};

	while(1){
		getTouchSample(&rawX, &rawY, &rawPressure);
		scaledX = rawX/scaleXvalue;
		scaledY = rawY/scaleYvalue;
		printf("scaledX = %i / scaledY = %i\n", scaledX, scaledY);
		if((scaledX  > buttonCordsLO[X] && scaledX < (buttonCordsLO[X]+buttonCordsLO[W])) && (scaledY > buttonCordsLO[Y] && scaledY < (buttonCordsLO[Y]+buttonCordsLO[H]))){
			if (mymillis() - buttonTimerLO > 250)
				if(buttonLO){
					buttonLO= BUTTON_OFF;
					buttonTimerLO = mymillis();
					createButton(buttonCordsLO[X],buttonCordsLO[Y],buttonCordsLO[W],buttonCordsLO[H],"1",WHITE,GREY);
          createButton(buttonCordsRO[X],buttonCordsRO[Y],buttonCordsRO[W],buttonCordsRO[H],"2",GREY,BLACK);
				}
				else{
					buttonLO= BUTTON_ON;
					buttonTimerLO = mymillis();
          createButton(buttonCordsLO[X],buttonCordsLO[Y],buttonCordsLO[W],buttonCordsLO[H],"OK",WHITE,GREEN);
          createButton(buttonCordsRO[X],buttonCordsRO[Y],buttonCordsRO[W],buttonCordsRO[H],"2",WHITE,GREY);
          printf("Links Oben OK\n");
				}
			}
			if(buttonLO == BUTTON_ON && (scaledX  > buttonCordsRO[X] && scaledX < (buttonCordsRO[X]+buttonCordsRO[W])) && (scaledY > buttonCordsRO[Y] && scaledY < (buttonCordsRO[Y]+buttonCordsRO[H]))){
				if (mymillis() - buttonTimerRO > 250)
					if(buttonRO){
						buttonRO= BUTTON_OFF;
						buttonTimerRO = mymillis();
						createButton(buttonCordsRO[X],buttonCordsRO[Y],buttonCordsRO[W],buttonCordsRO[H],"2",WHITE,GREY);
            createButton(buttonCordsRU[X],buttonCordsRU[Y],buttonCordsRU[W],buttonCordsRU[H],"3",GREY,BLACK);
					}
					else{
						buttonRO= BUTTON_ON;
						buttonTimerRO = mymillis();
						createButton(buttonCordsRO[X],buttonCordsRO[Y],buttonCordsRO[W],buttonCordsRO[H],"OK",WHITE,GREEN);
            createButton(buttonCordsRU[X],buttonCordsRU[Y],buttonCordsRU[W],buttonCordsRU[H],"3",WHITE,GREY);
            printf("Rechts Oben OK\n");
					}
				}
				if(buttonLO == BUTTON_ON && buttonRO == BUTTON_ON && (scaledX  > buttonCordsRU[X] && scaledX < (buttonCordsRU[X]+buttonCordsRU[W])) && (scaledY > buttonCordsRU[Y] && scaledY < (buttonCordsRU[Y]+buttonCordsRU[H]))){
					if (mymillis() - buttonTimerRU > 250)
						if(buttonRU){
							buttonRU= BUTTON_OFF;
							buttonTimerRU = mymillis();
							createButton(buttonCordsRU[X],buttonCordsRU[Y],buttonCordsRU[W],buttonCordsRU[H],"3",WHITE,GREY);
              createButton(buttonCordsLU[X],buttonCordsLU[Y],buttonCordsLU[W],buttonCordsLU[H],"4",GREY,BLACK);
						}
						else{
							buttonRU= BUTTON_ON;
							buttonTimerRU = mymillis();
							createButton(buttonCordsRU[X],buttonCordsRU[Y],buttonCordsRU[W],buttonCordsRU[H],"OK",WHITE,GREEN);
              createButton(buttonCordsLU[X],buttonCordsLU[Y],buttonCordsLU[W],buttonCordsLU[H],"4",WHITE,GREY);
              printf("Rechts Unten OK\n");
						}
					}
          if(buttonLO == BUTTON_ON && buttonRO == BUTTON_ON && buttonRU == BUTTON_ON && (scaledX  > buttonCordsLU[X] && scaledX < (buttonCordsLU[X]+buttonCordsLU[W])) && (scaledY > buttonCordsLU[Y] && scaledY < (buttonCordsLU[Y]+buttonCordsLU[H]))){
						if (mymillis() - buttonTimerLU > 250)
							if(buttonLU){
								buttonLU= BUTTON_OFF;
								buttonTimerLU = mymillis();
								createButton(buttonCordsLU[X],buttonCordsLU[Y],buttonCordsLU[W],buttonCordsLU[H],"4",WHITE,GREY);
							}
							else{
								buttonLU= BUTTON_ON;
								buttonTimerLU = mymillis();
								createButton(buttonCordsLU[X],buttonCordsLU[Y],buttonCordsLU[W],buttonCordsLU[H],"OK",WHITE,GREEN);
								createButton(buttonCordsM[X],buttonCordsM[Y],buttonCordsM[W],buttonCordsM[H],"",WHITE,GREEN);
								createButton(buttonCordsM1[X],buttonCordsM1[Y],buttonCordsM1[W],buttonCordsM1[H],"Hallo!",GREEN,GREEN);
                createButton(buttonCordsM2[X],buttonCordsM2[Y],buttonCordsM2[W],buttonCordsM2[H],"Dein Display ist OK!",GREEN,GREEN);
                createButton(buttonCordsExit[X],buttonCordsExit[Y],buttonCordsExit[W],buttonCordsExit[H],"EXIT",WHITE,RED);
                printf("Links Unten OK\n");
							}
						}
            if((scaledX  > buttonCordsExit[X] && scaledX < (buttonCordsExit[X]+buttonCordsExit[W])) && (scaledY > buttonCordsExit[Y] && scaledY < (buttonCordsExit[Y]+buttonCordsExit[H]))){
  						if (mymillis() - buttonTimerExit > 250)
  							if(buttonExit){
  								buttonExit= BUTTON_OFF;
  								buttonTimerExit = mymillis();
  							}
  							else{
  								buttonExit= BUTTON_ON;
  								buttonTimerExit = mymillis();
                  createButton(buttonCordsExit[X],buttonCordsExit[Y],buttonCordsExit[W],buttonCordsExit[H],"EXIT",WHITE,GREEN);
                  printf("Display OK\n");
                  sleep(1);
                  createButton(buttonCordsOff[X],buttonCordsOff[Y],buttonCordsOff[W],buttonCordsOff[H],"",BLACK,BLACK);
                  return 0;
  							}
  						}

	}  //while
} //main

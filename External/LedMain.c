/*
 *
 * Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     1.  Redistributions of source code must retain the above copyright notice, this list of
 *         conditions and the following disclaimer.
 *     2.  Redistributions in binary form must reproduce the above copyright notice, this list
 *         of conditions and the following disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *     3.  Neither the name of the owner nor the names of its contributors may be used to endorse
 *         or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>


#include "../rpi_ws281x/clk.h"
#include "../rpi_ws281x/gpio.h"
#include "../rpi_ws281x/dma.h"
#include "../rpi_ws281x/pwm.h"

#include "../rpi_ws281x/ws2811.h"

#include <time.h>
#include <linux/input.h>
#include <linux/ioctl.h>
#include <sys/sysinfo.h>
#include "../checkPara.h"
#include "../Frame/framebuffer.c"
#include "../Frame/DrawImage.h"
#include "../Frame/DrawCorner.c"
#include "../funktion.h"


#define TARGET_FREQ                 WS2811_TARGET_FREQ
#define GPIO_PIN                    18
#define DMA                         5

//#define STRIP_TYPE                WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE                WS2811_STRIP_RBG		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE                WS2811_STRIP_GBR		// WS2812/SK6812RGB integrated chip+leds
#define STRIP_TYPE                  WS2811_STRIP_GRB		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE                WS2811_STRIP_BRG		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE                WS2811_STRIP_BGR		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE                SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)

#define LED_COUNT                   12                  // Anzahl der LED's
#define BRIGHTNESS                  150                 // Helligkeit 0-255 100= ideal < 80 niedrige Werte werden nicht angeziegt > 150 zu hell
#define maxValue                    100                 // Maximale Helligkeit der Anzeigen 50-255 100= ideal
#define SOC_stripeUse               1                   // 1= SOC Anzeige mit 4x LED grün, 0= 1x LED grün (Nummern der weiteren LED's anpassen)
#define SOC_LED                     4                   // 1x LED grün oder 4x LED grün
#define PVI_LED                     2                   // 1x LED gelbe
#define Grid_LED                    0                   // 1x LED blau für Einspeisung oder rot für Bezug
#define Home_LED                    8                   // 1x LED orange
#define Bat_LED                     3                   // 1x LED glün Laden oder purpele für Endladen
#define Status_LED                  1                   // 1x LED bei Fehler hellrot, Warnung orange, sonst grün
#define PVI_TRACKER_LED             9                   // 2x LED orange (Nummern der weiteren LED's beachten)
#define ADD_LED                     11                  // 1x LED cyan
//Wenn du ein Wert nicht Anzeigen möchtest, dann nicht 0 verwenden sondern den "LED_COUNT" anpassen und der LED eine hohe Nr vergeben, denn die 0 ist die erste LED und dann würdest du alles auf der ersten LED anzeigen.

int clear_on_exit = 1;
static uint8_t running = 1;

ws2811_t ledstring =
{
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN,
            .count = LED_COUNT,
            .invert = 0,
            .brightness = BRIGHTNESS,
            .strip_type = STRIP_TYPE,
        },
        [1] =
        {
            .gpionum = 0,
            .count = 0,
            .invert = 0,
            .brightness = 0,
        },
    },
};

ws2811_led_t dotcolors[] =
{
    0x00200000,  // [1] = red
    0x00201000,  // [2] = orange
    0x00202000,  // [3] = yellow
    0x00002000,  // [4] = green
    0x00002010,  // [5] = cyan
    0x00002020,  // [6] = lightblue
    0x00000020,  // [7] = blue
    0x00100010,  // [8] = purple
    0x00200010,  // [9] = pink
};

static void ctrl_c_handler(int signum)
{
	(void)(signum);
    running = 0;
}

static void setup_handlers(void)
{
    struct sigaction sa =
    {
        .sa_handler = ctrl_c_handler,
    };

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

int readRscpLed(int RscpPosition)
{
  int ret = BitRead("/mnt/RAMDisk/E3dcLedCache.txt", RscpPosition,PosMAX);
  return ret;
}

int read100RscpLed(int RscpPosition)
{
  int ret = readRscpLed(RscpPosition);
  if (ret > 100)
    return 100;
  else if (ret < 0)
    return 0;
  else
    return ret;
}

int minMax(int color)
{
  if (color > 255)
    color = 255;
  if (color < 0)
    color = 0;

    return color;
}

int clearAll()
{
  int led_c;
  for (led_c = 0; led_c < LED_COUNT; led_c++) {
    ledstring.channel[0].leds[led_c] = 0x00000000;
  }
  return 1;
}

int rgbColor(int ro, int gr, int bl)
{
  unsigned long rgbHEX;
  ro = minMax(ro);
  gr = minMax(gr);
  bl = minMax(bl);
  rgbHEX = (ro << 16) + (gr << 8) + bl;
  return rgbHEX;
}

int SOC_Color()
{
  float ro;
  float gr;
  float bl;
  unsigned long HEX;
  int TAG_SOC = read100RscpLed(PosSOC);
  ro = 0;
  gr = maxValue * (TAG_SOC +1) / 100;
  bl = 0;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[SOC_LED] = HEX ;
  return HEX;
}

int SOC_Stripe()
{
  int led_c = 1;
  float ro;
  float gr;
  float bl;
  int value;
  unsigned long HEX;
  int TAG_SOC = read100RscpLed(PosSOC);
  ro = 0;
  gr = maxValue * TAG_SOC / 100 * 4;
  bl = 0;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[SOC_LED] = HEX;
  gr = maxValue * (TAG_SOC - 20 ) / 100 * 4;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[SOC_LED +1] = HEX;
  gr = maxValue * (TAG_SOC - 45 ) / 100 * 4;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[SOC_LED +2] = HEX;
  gr = maxValue * (TAG_SOC - 70 ) / 100 * 4;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[SOC_LED +3] = HEX;
  return 1;
}

int PVI_Color()
{
  float ro;
  float gr;
  float bl;
  unsigned long HEX;
  int TAG_PVI = readRscpLed(PosPVI);
  ro = maxValue * TAG_PVI / PowerMax;
  gr = ro;
  bl = 0;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[PVI_LED] = HEX ;
  return HEX;
}

int Grid_Color()
{
  float ro;
  float gr;
  float bl;
  unsigned long HEX;
  int TAG_Grid = readRscpLed(PosGrid);
  if (TAG_Grid >= 0){
    ro = maxValue * TAG_Grid / PowerMax;
    gr = 0;
    bl = 0;
  }
  else {
    TAG_Grid = TAG_Grid * -1;
    ro = 0;
    gr = maxValue * TAG_Grid / PowerMax;
    bl = gr;
  }
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[Grid_LED] = HEX ;
  return HEX;
}

int Home_Color()
{
  float ro;
  float gr;
  float bl;
  unsigned long HEX;
  int TAG_Home = readRscpLed(PosHome);
  ro = maxValue * TAG_Home / PowerMax;
  gr = ro / 2;
  bl = 0;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[Home_LED] = HEX ;
  return HEX;
}

int Bat_Color()
{
  float ro;
  float gr;
  float bl;
  unsigned long HEX;
  int TAG_Bat = readRscpLed(PosBat);
  if (TAG_Bat >= 0){
    ro = 0;
    gr = maxValue * TAG_Bat / BatMax;
    bl = 0;
  }
  else {
    TAG_Bat = TAG_Bat * -1;
    ro = maxValue * TAG_Bat / BatMax;
    gr = 0;
    bl = ro;
  }
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[Bat_LED] = HEX ;
  return HEX;
}
int Status_Color()
{
  float ro;
  float gr;
  float bl;
  unsigned long HEX;
  int TAG_BatState = readRscpLed(PosBatState);
  int TAG_PMState = readRscpLed(PosPMState);
  int TAG_PMPhases = readRscpLed(PosPMPhases);
  if (TAG_BatState == 0){
    ro = maxValue;
    gr = 0;
    bl = 0;
  }
  else if (TAG_PMState == 0){
    ro = maxValue;
    gr = 0;
    bl = 0;
  }
  else if (TAG_PMPhases == 0){
    ro = maxValue;
    gr = 0;
    bl = 0;
  }
  else if (TAG_PMPhases < 7){
    ro = maxValue;
    gr = maxValue /2;
    bl = 0;
  }
  else {
    ro = 0;
    gr = 16;
    bl = 0;
  }
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[Status_LED] = HEX ;
  return HEX;
}

int ADD_Color()
{
  float ro;
  float gr;
  float bl;
  unsigned long HEX;
  int TAG_ADD = readRscpLed(PosADD);
  ro = 0;
  gr = maxValue * TAG_ADD / PowerMax;
  bl = gr / 1.5;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[ADD_LED] = HEX ;
  return HEX;
}

int PVI_TRACKER_Color()
{
  float ro;
  float gr;
  float bl;
  unsigned long HEX;
  int TAG_PVIDCP1 = readRscpLed(PosPVIDCP1);
  ro = maxValue * TAG_PVIDCP1 / PowerMax;
  gr = ro / 2;
  bl = 0;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[PVI_TRACKER_LED] = HEX ;
  int TAG_PVIDCP2 = readRscpLed(PosPVIDCP2);
  ro = maxValue * TAG_PVIDCP2 / PowerMax;
  gr = ro / 2;
  bl = 0;
  HEX = rgbColor(ro,gr,bl);
  ledstring.channel[0].leds[PVI_TRACKER_LED +1] = HEX ;
  return HEX;
}

int main(void)
{
  //Parameter einbinden, checken oder default setzen
	checkDefinePara(0);

  ws2811_return_t ret;
  setup_handlers();

  if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS){
      fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
      return ret;
  }

  //int ro = 100;
  //int gr = 100;
  //int bl = 100;
  //unsigned long rgbHEX;

  while (running) {

    system("cp /mnt/RAMDisk/E3dcGuiData.txt /mnt/RAMDisk/E3dcLedCache.txt");

    if (SOC_stripeUse == 1)
      SOC_Stripe();
    else
      SOC_Color();
    PVI_Color();
    Grid_Color();
    Home_Color();
    Bat_Color();
    Status_Color();
    if (Additional == 1)
      ADD_Color();
    PVI_TRACKER_Color();

    //ledstring.channel[0].leds[8] = dotcolors[4];
    //rgbHEX = rgbColor(ro,gr,bl);
    //printf("%#08x\n", rgbHEX);
    //ledstring.channel[0].leds[9] = rgbHEX ;

    ws2811_render(&ledstring);
    sleep(1);
  }

  if (clear_on_exit) {
    clearAll();
    ws2811_render(&ledstring);
  }
  ws2811_fini(&ledstring);
  return 0;
}

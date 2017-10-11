CXX ?=g++
CC ?=gcc
ROOT_LM=LedMain

all: $(ROOT_LM)

$(ROOT_LM): clean
	$(CC) -O1 LedMain.c ../rpi_ws281x/mailbox.c ../rpi_ws281x/ws2811.c ../rpi_ws281x/pwm.c ../rpi_ws281x/pcm.c ../rpi_ws281x/dma.c ../rpi_ws281x/rpihw.c -o $@

clean:
	date
	-rm $(ROOT_LM) $(VECTOR)

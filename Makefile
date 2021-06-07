CXX ?=g++
CC ?=gcc
ROOT_CP=copyPara
ROOT_WD=watchdog
ROOT_SS=screenSave
ROOT_SSHM=screenSaveHM
ROOT_GM=GuiMain
ROOT_GB=External/gruenSave
ROOT_START=start
ROOT_STOP=stop
ROOT_RM=RscpMain
ROOT_RSET=Rscp/RscpSet
ROOT_WBCK=Rscp/wbCheckHM
ROOT_HISTORY=S10history/S10history
ROOT_LM=External/LedMain
ROOT_TT=Frame/touchtest

all: $(ROOT_CP) $(ROOT_WD) $(ROOT_SS) $(ROOT_SSHM) $(ROOT_GM) $(ROOT_START) $(ROOT_STOP) $(ROOT_RM) $(ROOT_RSET) $(ROOT_HISTORY)

$(ROOT_CP): cleanCP
	$(CC) -O1 copyPara.c -o $@
$(ROOT_WD): cleanWD
	$(CXX) -O1 Watchdog.cpp -o $@
$(ROOT_SS): cleanSS
	$(CC) -O1 screenSave.c -o $@
$(ROOT_SSHM): cleanSSHM
	$(CC) -O1 screenSaveHM.c -o $@ -lwiringPi
$(ROOT_GM): cleanGM
	$(CC) -O1 GuiMain.c -lcurl -o $@ -lwiringPi
$(ROOT_GB): cleanGB
	$(CC) -O1 External/gruenSave.c -o $@
$(ROOT_START): cleanSTART
	$(CC) -O1 start.c -o $@
$(ROOT_STOP): cleanSTOP
	$(CC) -O1 stop.c -o $@
$(ROOT_RM): cleanRM
	$(CXX) -O3 RscpMain.cpp Rscp/RscpProtocol.cpp Rscp/AES.cpp Rscp/SocketConnection.cpp -o $@
$(ROOT_RSET): cleanRSet
	$(CXX) -O3 Rscp/RscpSet.cpp Rscp/RscpProtocol.cpp Rscp/AES.cpp Rscp/SocketConnection.cpp -o $@
$(ROOT_WBCK): cleanWBCK
	$(CC) -O1 Rscp/wbCheckHM.c -o $@
$(ROOT_HISTORY): cleanHISTORY
	$(CXX) -O3 S10history/S10history.cpp S10history/RscpReader.cpp Rscp/RscpProtocol.cpp Rscp/AES.cpp Rscp/SocketConnection.cpp -o $@
$(ROOT_LM): cleanLM
	$(CC) -O1 External/LedMain.c rpi_ws281x/mailbox.c rpi_ws281x/ws2811.c rpi_ws281x/pwm.c rpi_ws281x/pcm.c rpi_ws281x/dma.c rpi_ws281x/rpihw.c -o $@ -lwiringPi
$(ROOT_TT): cleanTT
	$(CC) -O1 Frame/touchtest.c  -o $@

cleanCP:
		-rm $(ROOT_CP) $(VECTOR)
cleanWD:
		-rm $(ROOT_WD) $(VECTOR)
cleanSS:
		-rm $(ROOT_SS) $(VECTOR)
cleanSSHM:
		-rm $(ROOT_SSHM) $(VECTOR)
cleanGM:
		-rm $(ROOT_GM) $(VECTOR)
cleanGB:
		-rm $(ROOT_GB) $(VECTOR)
cleanSTART:
		-rm $(ROOT_START) $(VECTOR)
cleanSTOP:
		-rm $(ROOT_STOP) $(VECTOR)
cleanRM:
		-rm $(ROOT_RM) $(VECTOR)
cleanRSet:
		-rm $(ROOT_RSET) $(VECTOR)
cleanWBCK:
		-rm $(ROOT_WBCK) $(VECTOR)
cleanHISTORY:
		-rm $(ROOT_HISTORY) $(VECTOR)
cleanLM:
		-rm $(ROOT_LM) $(VECTOR)
cleanTT:
		-rm $(ROOT_TT) $(VECTOR)

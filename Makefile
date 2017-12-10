CXX ?=g++
CC ?=gcc
ROOT_WD=watchdog
ROOT_SS=screenSave
ROOT_SSHM=screenSaveHM
ROOT_GM=GuiMain
ROOT_GB=External/gruenSave
ROOT_START=start
ROOT_STOP=stop
ROOT_RM=RscpMain
ROOT_HISTORY=S10history/S10history
ROOT_TT=Frame/touchtest

all: $(ROOT_WD) $(ROOT_SS) $(ROOT_SSHM) $(ROOT_GM) $(ROOT_GB) $(ROOT_START) $(ROOT_STOP) $(ROOT_RM) $(ROOT_HISTORY) $(ROOT_TT)

$(ROOT_WD): cleanWD
	$(CXX) -O1 Watchdog.cpp -o $@
$(ROOT_SS): cleanSS
	$(CC) -O1 screenSave.c -o $@
$(ROOT_SSHM): cleanSSHM
	$(CC) -O1 screenSaveHM.c -o $@
$(ROOT_GM): cleanGM
	$(CC) -O1 GuiMain.c -o $@ -lwiringPi
$(ROOT_GB): cleanGB
	$(CC) -O1 External/gruenSave.c -o $@
$(ROOT_START): cleanSTART
	$(CC) -O1 start.c -o $@
$(ROOT_STOP): cleanSTOP
	$(CC) -O1 stop.c -o $@
$(ROOT_RM): cleanRM
	$(CXX) -O3 RscpMain.cpp Rscp/RscpProtocol.cpp Rscp/AES.cpp Rscp/SocketConnection.cpp Rscp/RWData.cpp -o $@
$(ROOT_HISTORY): cleanHISTORY
	$(CXX) -O3 S10history/S10history.cpp S10history/RscpReader.cpp Rscp/RscpProtocol.cpp Rscp/AES.cpp Rscp/SocketConnection.cpp Rscp/RWData.cpp -o $@
$(ROOT_TT): cleanTT
	$(CC) -O1 Frame/touchtest.c  -o $@

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
cleanHISTORY:
		-rm $(ROOT_HISTORY) $(VECTOR)
cleanTT:
		-rm $(ROOT_TT) $(VECTOR)

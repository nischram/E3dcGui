CXX ?=g++
CC ?=gcc
ROOT_WD=watchdog
ROOT_SS=screenSave
ROOT_GM=GuiMain
ROOT_START=start
ROOT_STOP=stop
ROOT_RM=RscpMain
ROOT_TT=Frame/touchtest

all: $(ROOT_WD) $(ROOT_SS) $(ROOT_GM) $(ROOT_START) $(ROOT_STOP) $(ROOT_RM) $(ROOT_TT)

$(ROOT_WD): clean
	$(CXX) -O1 Watchdog.cpp -o $@
$(ROOT_SS): clean
	$(CC) -O1 screenSave.c -o $@
$(ROOT_GM): clean
	$(CC) -O1 GuiMain.c -o $@
$(ROOT_START): clean
	$(CC) -O1 start.c -o $@
$(ROOT_STOP): clean
	$(CC) -O1 stop.c -o $@
$(ROOT_RM): clean
	$(CXX) -O3 RscpMain.cpp Rscp/RscpProtocol.cpp Rscp/AES.cpp Rscp/SocketConnection.cpp Rscp/RWData.cpp -o $@
$(ROOT_TT): clean
	$(CC) -O1 Frame/touchtest.c  -o $@

clean:
	date
	-rm $(ROOT_WD) $(ROOT_SS) $(ROOT_GM) $(ROOT_START) $(ROOT_STOP) $(ROOT_RM) $(ROOT_TT) $(VECTOR)

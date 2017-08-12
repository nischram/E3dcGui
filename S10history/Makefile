CXX=/usr/bin/g++
ROOT_VALUE=S10history

all: $(ROOT_VALUE)

$(ROOT_VALUE): clean
	$(CXX)   S10history.cpp RscpReader.cpp ../Rscp/RscpProtocol.cpp ../Rscp/AES.cpp ../Rscp/SocketConnection.cpp ../Rscp/RWData.cpp -o $@


clean:
	-rm $(ROOT_VALUE) $(VECTOR)

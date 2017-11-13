#------------------------------------------------------------------------------

SOURCE_CLIENT=./client/main.cpp ./client/Module.hpp ./common/Messages.hpp ./common/Protocol.hpp ./common/Socket.hpp
SOURCE_SERVER=server/main.cpp server/Module.hpp common/Messages.hpp common/Protocol.hpp common/Socket.hpp

PROGRAM_CLIENT=run_client
PROGRAM_SERVER=run_server

MYLIBRARIES=pthread
CC=g++

#------------------------------------------------------------------------------

all: $(PROGRAM_CLIENT) $(PROGRAM_SERVER)

client: $(PROGRAM_CLIENT)
server: $(PROGRAM_SERVER)

$(PROGRAM_CLIENT): $(SOURCE_CLIENT)
	$(CC) $(SOURCE_CLIENT) -o$(PROGRAM_CLIENT) -l$(MYLIBRARIES) -std=c++17

$(PROGRAM_SERVER): $(SOURCE_SERVER)
	$(CC) $(SOURCE_SERVER) -o$(PROGRAM_SERVER) -l$(MYLIBRARIES) -std=c++17

clean:
	rm -f $(PROGRAM_CLIENT) $(PROGRAM_SERVER)
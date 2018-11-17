CC = g++
FLAGS = -Wall -std=c++11
SOURCES = algebra.cpp aes.cpp modeofop.cpp cbcmode.cpp ofbmode.cpp ctrmode.cpp aes_main.cpp
EXE = aes_main

all:
	$(CC) $(FLAGS) $(SOURCES) -o $(EXE)

run:
	./$(EXE) dummymessage 4 ofb

clean:
	rm -rf $(EXE) *.o

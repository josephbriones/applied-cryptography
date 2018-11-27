CC = g++
FLAGS = -Wall -std=c++11
SOURCES = algebra.cpp aes.cpp modeofop.cpp cbcmode.cpp ofbmode.cpp ctrmode.cpp aes_main.cpp
EXE = aes_main

all:
	$(CC) $(FLAGS) $(SOURCES) -o $(EXE)

run:
	./$(EXE) helloworldaescpp01234567891011 4 cbc
	./$(EXE) helloworldaescpp01234567891011 6 cbc
	./$(EXE) helloworldaescpp01234567891011 8 cbc
	./$(EXE) helloworldaescpp01234567891011 4 ofb
	./$(EXE) helloworldaescpp01234567891011 6 ofb
	./$(EXE) helloworldaescpp01234567891011 8 ofb
	./$(EXE) helloworldaescpp01234567891011 4 ctr
	./$(EXE) helloworldaescpp01234567891011 6 ctr
	./$(EXE) helloworldaescpp01234567891011 8 ctr

clean:
	rm -rf $(EXE) *.o

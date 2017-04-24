RPI?=25
SRC=src
APP=NRF24L01_base
DST=~/ayrault-sivarajah/pperi/

CROSSDIR        = /users/enseig/franck/peri
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-

INC=$(HOME)/rf24/include
LIB=$(HOME)/rf24/lib
CFLAGS=-Wall -Wfatal-errors -O2 -I$(INC)
LDFLAGS=-L$(LIB) -lrf24


all: recever.x #baseSender.x #recevercever.x

baseSender.x: baseSender.cpp
	$(CROSS_COMPILE)g++ -o $@ -I$(INC) $<  -O2 $(LDFLAGS)

recever.x: recever.cpp sqlite3.o
	$(CROSS_COMPILE)g++ -o $@ -I$(INC) $^ -lpthread -ldl -O2 $(LDFLAGS)

sqlite3.o: sqlite3.c
	$(CROSS_COMPILE)gcc -o $@ -c $< -lpthread -ldl


upload: 
#	scp -P5025 baseSender.x pi@peri:$(DST)
	scp -P5025 recever.x pi@peri:$(DST)

clean:
	rm -f *.x *~ *.a
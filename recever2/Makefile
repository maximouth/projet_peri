RPI?=25
SRC=src
APP=NRF24L01_base
DST=~/ayrault-sivarajah/

CROSSDIR        = /users/enseig/franck/peri
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-

INC=$(HOME)/rf24/include
LIB=$(HOME)/rf24/lib
CFLAGS=-Wall -Wfatal-errors -O2 -I$(INC)
LDFLAGS=-L$(LIB) -lrf24

GPIO=libgpio
LIBGPIO=-L. -lgpio


all:  $(GPIO).a recever2.x

$(GPIO).o: $(GPIO).c
	$(CROSS_COMPILE)g++ -c $(GPIO).c -o $(GPIO).o

$(GPIO).a: $(GPIO).o
	$(CROSS_COMPILE)ar rcs $(GPIO).a $(GPIO).o

$(APP).x: $(APP).cpp
	$(CROSS_COMPILE)g++ -o $@ -I$(INC) $<  -O2 $(LDFLAGS)

recever2.x: recever2.cpp
	$(CROSS_COMPILE)g++ -o $@ -I$(INC) $<  -O2 $(LDFLAGS) $(LIBGPIO)


upload: 
	scp -P5025 recever2.x pi@peri:$(DST)

clean:
	rm -f *.o *.x *~ *.a
JOBNAME=space-payload
CPP=g++
CC=gcc
MICROCPP=microblazeel-xilinx-linux-gnu-g++
MICROCC=microblazeel-xilinx-linux-gnu-gcc
MICROCCPATH=/home/ty/XiphosDevEnv/petalinux-v12.12-mbel/bin
#MICROCC=$(MICROCCPATH)/microblazeel-xilinx-linux-gnu-g++
CFLAGS=-Wall
MICROCFLAGS=-mcpu=v8.40.b -mxl-barrel-shift -mxl-multiply-high -mxl-pattern-compare -mno-xl-soft-mul -mno-xl-soft-div -mxl-float-sqrt -mhard-float -mxl-float-convert -mlittle-endian -Wall
DEBUGFLAGS=-ggdb -g -gdwarf-2 -g3 #gdwarf-2 + g3 provides macro info to gdb

SHAKESPEARE_PATH=../space-lib/shakespeare/
SPACELIBINCLUDE_PATH=../space-lib/include/
INCPATH=-I./inc/ -I$(SPACELIBINCLUDE_PATH) -I$(SHAKESPEARE_PATH)/inc
LIBPATH=-L./lib/ -L$(SHAKESPEARE_PATH)/lib

buildBin: mkdir $(SHAKESPEARE_PATH)/inc/shakespeare.h $(SPACELIBINCLUDE_PATH)/SpaceDecl.h
	$(CPP) $(CFLAGS) $(INCPATH) $(LIBPATH) src/*.cpp -lshakespeare -o bin/$(JOBNAME)-x86 -lshakespeare

buildQ6: mkdir $(SHAKESPEARE_PATH)/shakespeare.h $(SPACELIBINCLUDE_PATH)/SpaceDecl.h
	$(MICROCPP) $(MICROCFLAGS) $(INCPATH) $(LIBPATH) src/*.cpp -lshakespeare-mbcc -o bin/$(JOBNAME) -lshakespeare-mbcc

mkdir:
	mkdir -p bin

clean:
	rm -f bin/*

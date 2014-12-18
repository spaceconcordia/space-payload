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
INCPATH=-I./inc/ -I../space-lib/include/ -I../space-lib/shakespeare/inc
LIBPATH=-L./lib/ -L../space-lib/lib/ -L../space-lib/shakespeare/lib

makedeps:
	mkdir bin

buildBin: makedeps
	$(CPP) $(CFLAGS) $(INCPATH) $(LIBPATH) src/*.cpp -lshakespeare -lcs1_utls -o bin/$(JOBNAME)-x86

buildQ6: makedeps
	$(MICROCPP) $(MICROCFLAGS) $(INCPATH) $(LIBPATH) src/*.cpp -lshakespeare-mbcc -lcs1_utlsQ6 -o bin/$(JOBNAME)

clean:
	rm -f bin/*

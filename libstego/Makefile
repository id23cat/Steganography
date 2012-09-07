 #Makefile for Hello World project
# archive -- static
CC=gcc
CXX=g++
CFLAGS=-Wall -pedantic

STGSRC = StegoEncoder.cpp StegoDecoder.cpp StegoArray.cpp Array.cpp BitArray.cpp Exception.cpp JpegStegoEncoder.cpp \
		JpegStegoDecoder.cpp JpegStegoLog.cpp JpegStegoAnalysis.cpp AP_CRC16.cpp AP_CRC32.cpp Methods.cpp

LIBDIR=../libs
STGOBJS=$(STGSRC:%.cpp=%.o)

#static
#bitarray_st: libbitarray.a
#	$(CC) -o hello main.o -L. -lhello
#dinamic
#bitarray_dn: main.o libhello.so
#	$(CC) -o hello main.o -L. -lhello -Wl,-rpath,.
#main.o: main.c
#	$(CC) -c main.c

#libstego.a: Array.o BitArray.o Exception.o StegoDecoder.o JpegStegoDecoder.o StegoEncoder.o JpegStegoEncoder.o
#	ar cr libstego.a Array.o BitArray.o Exception.o StegoDecoder.o JpegStegoDecoder.o StegoEncoder.o JpegStegoEncoder.o
#libbitarray.a: Array.o BitArray.o Exception.o
#	ar cr libbitarray.a Array.o BitArray.o Exception.o
#libbitarray.so: Array.o BitArray.o Exception.o
#	$(CXX) -shared -o libbitarray.so Array.o BitArray.o Exception.o
#OggStegoEncoder.o: OggStegoEncoder.cpp
#	$(CXX) -c -fPIC OggStegoEncoder.cpp
#StegoEncoder.o: StegoEncoder.cpp
#	$(CXX) -c -fPIC StegoEncoder.cpp
#OggStegoDecoder.o: OggStegoDecoder.cpp
#	$(CXX) -c -fPIC OggStegoDecoder.cpp
#StegoDecoder.o: StegoDecoder.cpp
#	$(CXX) -c -fPIC StegoDecoder.cpp
#BitArray.o: BitArray.cpp
#	$(CXX) -c -fPIC BitArray.cpp
#Array.o: Array.cpp
#	$(CXX) -c -fPIC Array.cpp
#Exception.o: Exception.cpp
#	$(CXX) -c -fPIC Exception.cpp
all: libstego.a

libstego.a: $(STGOBJS)
	echo $(STGOBJS)
	ar rc $(LIBDIR)/$@ $(STGOBJS)
	ranlib $(LIBDIR)/$@

.c.o: 
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.a *.so *~ hello

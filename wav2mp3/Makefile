CPP = g++
CC = gcc
CFLAGS = -O2 -fPIC
CPPFLAGS = -O2 -fPIC -std=c++17

MYPATH = /usr/local/src/noise_suppression/wav2mp3

LIB = -lpthread
INCLUDE = -I${MYPATH}/include
 
SOURCE = main.cpp $(wildcard ${MYPATH}/src/*.c) $(wildcard ${MYPATH}/src/*.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

all: main

main: $(OBJS)
	${CPP} ${CPPFLAGS} -o wav2mp3 $(OBJS) $(LIB)

%.o : %.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

%.o : %.cpp
	$(CPP) $(CPPFLAGS) ${INCLUDE} -c $< -o $@

clean:
	rm -rf ${OBJS} wav2mp3
CPP = g++
CC = gcc
CFLAGS = -O2 -fPIC
CPPFLAGS = -O2 -fPIC -std=c++17

MYPATH = /usr/local/src/noise_suppression/mp3towav

LIB = -lpthread
INCLUDE = -I${MYPATH}
 
SOURCE = mp3decode.cpp
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

all: main

main: $(OBJS)
	${CPP} ${CPPFLAGS} -o mp3decode $(OBJS) $(LIB)

%.o : %.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

%.o : %.cpp
	$(CPP) $(CPPFLAGS) ${INCLUDE} -c $< -o $@

clean:
	rm -rf ${OBJS} mp3decode
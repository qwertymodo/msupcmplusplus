CC = gcc
CCFLAGS =

CXX = g++
CXXFLAGS = -std=gnu++11

LD = g++

INC = -I ../3rdparty/json/src -I ../3rdparty/sox/src
LIBS = ../3rdparty/sox/src/.libs/libsox.a

TARGET = msupcm

SOURCES := $(wildcard *.cpp) sox_main.c
OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
LIBS += -lgomp -lltdl -lmad -lmp3lame -lpulse-simple -lpulse -lvorbis -lvorbisenc -lvorbisfile -logg -lopus -lopusfile -lFLAC -lz

.PHONY: all

all: $(TARGET)

FORCE:

clean: FORCE
	rm -f *.o $(TARGET)

.c.o:
	$(CC) $(CCFLAGS) $(INC) -c $? -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INC) -c $? -o $@

$(TARGET): $(OBJECTS)
	$(LD) -o $(TARGET) $(OBJECTS) $(LIBS)

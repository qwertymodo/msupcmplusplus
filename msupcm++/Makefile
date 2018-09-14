CC = gcc
CCFLAGS = 

CXX = g++
CXXFLAGS = -std=gnu++11

LD = g++

INC = -I ../3rdparty/json/src -I ../3rdparty/sox/src

TARGET = msupcm

SOURCES := $(wildcard *.cpp) sox_main.c
OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
LIBS = -lgomp -lltdl -lmad -lmp3lame -lpulse-simple -lpulse -lvorbis -lvorbisenc -lvorbisfile -logg -lopus -lopusfile -lFLAC -lz

.PHONY: clean

clean:
	rm *.o $(TARGET)

.c.o:
	$(CC) $(CCFLAGS) $(INC) -c $? -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INC) -c $? -o $@

$(TARGET): $(OBJECTS)
	$(LD) -o $(TARGET) $(OBJECTS) ../3rdparty/sox/src/.libs/libsox.a $(LIBS)

all: $(TARGET)

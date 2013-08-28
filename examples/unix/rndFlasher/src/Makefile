# Makefile for RGB LED flasher example

SOURCES= \
	main.cpp \
	flasher.cpp \
	deviceFinder.cpp \
	rgbledCollection.cpp

CFLAGS= \
	-c \
	-Wall \
	-Wextra \
	-Wstrict-prototypes \
	-Wunused-result \
	-O3 \
	-std=c99 \
	-fmessage-length=0

CXXFLAGS= \
	-c \
	-Wall \
	-Wextra \
	-Wunused-result \
	-O3 \
	-fmessage-length=0

LDFLAGS= \
	-s

LDLIBS= \
	-lrgbledctrl \
	-lstdc++

EXECUTABLE=rndflasher

CC=gcc
CXX=g++
TMP=$(SOURCES:.cpp=.o)
OBJECTS=$(TMP:.c=.o)


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)

.PHONY: clean all

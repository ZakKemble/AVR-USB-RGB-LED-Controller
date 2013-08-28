# Makefile for CPU Usage LEDs example

SOURCES= \
	main.c \
	backend.c \
	util.c

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
	-lm

EXECUTABLE=cpuusageleds

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

.PHONY: all clean

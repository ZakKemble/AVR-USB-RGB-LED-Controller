# Makefile for main library

SOURCES= \
	rgbled.c \
	rgbled_cpp.cpp \
	eeprom.c

CFLAGS= \
	-c \
	-Wall \
	-Wextra \
	-Wstrict-prototypes \
	-Wunused-result \
	-O3 \
	-std=c99 \
	-fmessage-length=0 \
	-fPIC \
	-I/root/rgbled/

CXXFLAGS= \
	-c \
	-Wall \
	-Wextra \
	-Wunused-result \
	-O3 \
	-fmessage-length=0 \
	-fPIC \
	-I/root/rgbled/

LDFLAGS= \
	-s \
	-shared

LDLIBS= \
	-lhidapi-hidraw

EXECUTABLE=librgbled.so
ARCHIVE=librgbled.a

CC=gcc
CXX=g++
TMP=$(SOURCES:.cpp=.o)
OBJECTS=$(TMP:.c=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $@
	ar rcs $(ARCHIVE) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE) $(ARCHIVE)

.PHONY: clean all

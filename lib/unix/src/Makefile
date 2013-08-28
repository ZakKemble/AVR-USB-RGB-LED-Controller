# Makefile for main library

SOURCES= \
	rgbledctrl.c \
	rgbledctrl_cpp.cpp \
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
	-fPIC

CXXFLAGS= \
	-c \
	-Wall \
	-Wextra \
	-Wunused-result \
	-O3 \
	-fmessage-length=0 \
	-fPIC

LDFLAGS= \
	-s \
	-shared

LDLIBS= \
	-lusb

EXECUTABLE=librgbledctrl.so
ARCHIVE=librgbledctrl.a

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

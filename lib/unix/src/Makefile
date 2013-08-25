# Makefile for main library

SOURCES= \
	rgbledctrl.c \
	eeprom.c \
	opendevice.c

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

LDFLAGS= \
	-lusb \
	-s \
	-shared

EXECUTABLE=librgbledctrl.so
ARCHIVE=librgbledctrl.a

CC=gcc
OBJECTS=$(SOURCES:.c=.o)


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	ar rcs $(ARCHIVE) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE) $(ARCHIVE)

.PHONY: all clean

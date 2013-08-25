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

LDFLAGS= \
	-lrgbledctrl \
	-lm \
	-s

EXECUTABLE=cpuusageleds

CC=gcc
OBJECTS=$(SOURCES:.c=.o)


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)

.PHONY: all clean

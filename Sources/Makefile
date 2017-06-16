SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

EXEC = ../OrionisOptions

CC 			= gcc
CFLAGS 		= -Wall
GTKFLAGS	= `pkg-config --libs --cflags gtk+-3.0`

all : $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(CFLAGS) $(GTKFLAGS)

%.o : %.c 
	$(CC) -c $(CFLAGS) $(GTKFLAGS) -o $@ $<

.PHONY : clean

clean:
	rm $(OBJECTS)


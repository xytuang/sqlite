CC = gcc

CFLAGS = -g -Wall

LDFLAGS =

LDLIBS =

main: main.o constants.o table.o row.o

main.o: main.c

constants.o: constants.c

table.o: table.c

row.o: row.c

.PHONY: clean
clean:
	rm -f *.o a.out core main

.PHONY: all
all: clean main

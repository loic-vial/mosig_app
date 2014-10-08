
.PHONY: all clean

CC=gcc

CFLAGS=-g -Wall -O0 -Werror
LDFLAGS=

all: maze

maze: main.o maze.o svg.o

clean:
	rm -f *.o maze

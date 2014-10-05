CC = gcc -g -Wall -Wextra

all : maze

maze : main.c maze.c svg.c
	$(CC) main.c maze.c svg.c -o maze

clean :
	rm -f maze

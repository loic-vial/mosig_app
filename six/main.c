#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"


int main(int argc, char **argv) {
	//unsigned int len;
	if (argc < 3) {
		printf("Usage: %s <width> <height>\n", *argv);
		return 1;
	}
	srand(time(NULL));

	/* You need to define this function in "maze.c" */
	maze_t *m = maze_random(atoi(argv[1]), atoi(argv[2]));

	/* And also this one, 
	 * using the SVG drawing function provided in "svg.h" */
	maze_svg(m, "maze.svg");
	free_maze(m);

	return 0;
}

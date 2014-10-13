#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

int main(int argc, char **argv) {
  unsigned int len;
  if (argc < 3) {
    printf("Usage: %s <width> <height>\n", *argv);
    return 1;
  }

  /* You need to define this function in "maze.c" */
  maze *m = maze_random(atoi(argv[1]), atoi(argv[2]));

  /* And also this one, using the SVG drawing function provided in "svg.h" */
  maze_svg(m, "maze.svg");

  return 0;
}

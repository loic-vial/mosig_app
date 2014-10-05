#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Usage: %s <width> <height>\n", *argv);
        return 1;
    }

    srand(time(NULL));
    maze* m = maze_random(atoi(argv[1]), atoi(argv[2]));

    maze_svg(m, "maze.svg");

    maze_free(m);

    return 0;
}

#pragma once

typedef struct _maze maze;

maze* maze_random(int width, int height);

void maze_svg(maze* m, char* filename);

void maze_free(maze* m);

void maze_resolve(maze* m);

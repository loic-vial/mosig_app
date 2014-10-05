#pragma once

typedef struct _maze maze;

maze* maze_random(int width, int height, int pos_x, int pos_y);

void maze_svg(maze* m, char* filename);

void maze_free(maze* m);

void maze_resolve(maze* m);

typedef struct room room_t;
typedef struct maze maze_t;

maze_t *maze_random (int width, int height);
void maze_svg (maze_t *maze, char *filename);
void free_maze(maze_t *maze);


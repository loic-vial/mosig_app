#include <stdbool.h>

struct _tree{
    int width, height;
    int offset_w, offset_h;
    int door_position,wall_position;
    struct _tree* left_bot_child;
    struct _tree* right_top_child;
    struct _tree* parent;
    bool is_wall_vertical;
};

struct _maze {
    struct _tree* root;
};

typedef struct _tree tree;
typedef struct _maze maze;


void maze_init (maze * myMaze, int width, int height);

maze *maze_random (int width, int height);
void maze_svg (maze *maze, char *filename);

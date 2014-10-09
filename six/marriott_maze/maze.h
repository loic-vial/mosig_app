typedef struct _tree tree;

typedef struct _maze maze;

/**
 * The additional parameters parent and is_top_left_child shall not be used
 * during the first call.
 *
 * So the first call should be maze_random(NULL, w, h, false)
 */
maze *maze_random (maze *parent, int width, int height, bool is_right_bottom_child);
void maze_svg (maze *maze, char *filename);
void maze_free (maze *maze);

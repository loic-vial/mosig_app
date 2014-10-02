typedef struct _tree tree;

struct _tree {
	int width, height;
	int offset_w, offset_h;
	int door_position;
	int wall_position;
	tree *left_child_bottom;
	tree *right_child_top;
	tree *parent;
	int is_wall_vertical;
};

typedef struct _maze {
	tree* room;
} maze;

maze *maze_random (int width, int height);
void maze_svg (maze *maze, char *filename);


#define "solving.h"

static void draw_path(FILE *fp, maze *start, maze *end)
{
	maze *anc; // ancestor

	if (start == end)
		return;

	anc = common_ancestor(start, end);
	draw_door_path(anc);

	/* TODO after the generation of the grid */
}

void solve_maze(FILE *fp, maze *maze)
{
	draw_path(fp, entrance(maze), exit(maze));
}



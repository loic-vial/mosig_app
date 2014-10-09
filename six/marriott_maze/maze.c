#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "maze.h"
#include "svg.h"

/* TODO : implement offset, parent, and the grid */

struct _maze {
	int width;
	int height;
	int offset_w, offset_h;
	int door_position;
	int wall_position;
	maze *left_top_child;
	maze *right_bottom_child;
	maze *parent;
	bool wall_is_vertical;    // Needed for square rooms
	/* It's allocated only for the root room ; it contains a map of all
	 * room adresses, to compute the solution faster */
	maze **maze_map;
};

maze *maze_random (maze *parent, int width, int height, 
		bool is_right_bottom_child){

	maze *chamber = malloc(sizeof(maze));

	/* Initialize chamber */
	chamber->width  = width;
	chamber->height = height;
	chamber->parent = parent;
	chamber->offset_w = parent->offset_w;
	chamber->offset_h = parent->offset_h;

	if (parent == NULL)
		maze_map = malloc(width * height * sizeof(maze *));
	else
		maze_map = NULL;

	if (is_bottom_right_child){
		/* The parent->left_top_child should be already computed at this
		 * time ; which is ensured by the order in which we call the
		 * recursion, and the initial call of maze_random */
		if (parent->wall_is_vertical)
			chamber->offset_h += parent->left_top_child->height;
		else
			chamber->offset_w += parent->left_top_child->width;
	}
		

	if(width == 1 && height == 1) {
		/* TODO : in this case, complete the maze_map */
		return NULL;
	} else {
		/* Divide chamber */
		chamber->wall_is_vertical = true;
		if(width < height) {
			chamber->wall_is_vertical = false;
		} else if(width == height) {
			if(rand() % 1 == 1) {
				/* If square, make random decision to change
				   wall_is_vertical from the default of true. */
				chamber->wall_is_vertical = false;
			}
		}

		if(chamber->wall_is_vertical) {
			chamber->wall_position = ( rand() % (width-1) ) + 1;
			chamber->door_position = rand() % height;
			chamber->left_top_child = maze_random(chamber, chamber->wall_position, height, true);
			chamber->right_bottom_child = maze_random(chamber, width - chamber->wall_position, height, false);
		} else { // horizontal wall
			chamber->wall_position = ( rand() % (height-1) ) + 1;
			chamber->door_position = rand() % width;
			chamber->left_top_child = maze_random(chamber, width, chamber->wall_position, true);
			chamber->right_bottom_child = maze_random(chamber, width, height - chamber->wall_position, false);
		}
	}

	return chamber;
}

void plot_wall(FILE *fp, maze *chamber, int room_pos_x, int room_pos_y) {

	if(chamber == NULL) {
		return;
	} else {

		if(chamber->wall_is_vertical) {
			svg_line (fp, room_pos_x + chamber->wall_position, \
					room_pos_y,                          \
					room_pos_x + chamber->wall_position, \
					room_pos_y + chamber->door_position);

			svg_line (fp, room_pos_x + chamber->wall_position,     \
					room_pos_y + chamber->door_position + 1, \
					room_pos_x + chamber->wall_position,     \
					room_pos_y + chamber->height);

			plot_wall(fp, chamber->left_top_child, room_pos_x, room_pos_y);
			plot_wall(fp, chamber->right_bottom_child, room_pos_x + chamber->wall_position, room_pos_y);

		} else { // horizontal
			svg_line (fp, room_pos_x, \
					room_pos_y + chamber->wall_position, \
					room_pos_x + chamber->door_position, \
					room_pos_y + chamber->wall_position);

			svg_line (fp, room_pos_x + chamber->door_position + 1, \
					room_pos_y + chamber->wall_position,     \
					room_pos_x + chamber->width,             \
					room_pos_y + chamber->wall_position);

			plot_wall(fp, chamber->left_top_child, room_pos_x, room_pos_y);
			plot_wall(fp, chamber->right_bottom_child, room_pos_x, room_pos_y + chamber->wall_position);
		}

	}
}

void maze_svg (maze *maze, char *filename) {

	FILE *fp;  

	fp = fopen(filename, "w");

	svg_header(fp, maze->width + 2, maze->height + 2);

	/************* PLOTTING *************/

	/* Drawing a white rectangle */
	set_svg_color("white");
	svg_rect(fp, 0, 0, maze->width + 2, maze->height + 2);

	/* Plot the main "hall" with fixed entrance and exit
	   positions in the bottom-left and top-right. */

	// Top
	svg_line (fp, 1, \
			1, \
			1 + maze->width, \
			1);

	// Bottom
	svg_line (fp, 1, \
			1 + maze->height, \
			1 + maze->width,  \
			1 + maze->height);

	// Left
	svg_line (fp, 1, \
			1, \
			1, \
			1 + maze->height - 1);

	// Right
	svg_line (fp, 1 + maze->width,      \
			2, \
			1 + maze->width,      \
			1 + maze->height);

	/* Plot the internal walls recursively. */

	plot_wall(fp, maze, 1, 1);

	/*************************************/

	/* TODO Solving of the maze */
	//solve_maze(fp, maze);

	svg_footer(fp);

	fclose(fp);

}


#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "maze.h"
#include "svg.h"
#include <string.h>
#include <errno.h>


/* Debugging Tool */
void print_room (tree *room) {
	printf("Width, Height : %i, %i\n",room->width,room->height);
	printf("Offset_w, offset_h : %i, %i\n",room->offset_w,room->offset_h);
	printf("Wall_position : %i\n",room->wall_position);
	printf("Door_position : %i\n",room->door_position);
	printf("Is vertical : %i\n\n",room->is_wall_vertical);
}

void maze_room_rec (tree *room, maze *maze) {
	/* Stop condition */
	if (room->height <= 1 || room->width <= 1) {
		room->left_child_bottom = NULL;
		room->right_child_top = NULL;

		if (room->offset_w == 0 && (room->offset_h + room->height) == maze->height)
			maze->entrance = room;

		if (room->offset_h == 0 && (room->offset_w + room->width) == maze->width)
			maze->exit = room;

		return;
	}

	/* Decide if wall will be horizontal or vertical */
	if (room->height < room->width)
		room->is_wall_vertical = 1;
	else if (room->height > room->width)
		room->is_wall_vertical = 0;
	else
		room->is_wall_vertical = rand() % 2;
	

	/* Malloc the children */
	room->left_child_bottom = malloc(sizeof(tree));
	room->right_child_top = malloc(sizeof(tree));

	/* Two cases : vertical or horizontal wall */
	if (room->is_wall_vertical) {
		/* Generate door and wall position
		 * Wall_pos must be superior to 1 and inferior to width - 1 
		 * Door_pos must be from 0 to height - 1*/
		room->door_position = (rand() % (room->height - 1));
		room->wall_position = (rand() % (room->width - 1)) + 1;

		/* New width and height */
		room->left_child_bottom->width = room->wall_position;
		room->right_child_top->width = room->width - room->wall_position;

		room->left_child_bottom->height = room->height;
		room->right_child_top->height = room->height;

		/* No different offset for left child */
		room->left_child_bottom->offset_w = room->offset_w;
		room->left_child_bottom->offset_h = room->offset_h;

		/* Width offset for right child */
		room->right_child_top->offset_w = room->offset_w + room->wall_position;
		room->right_child_top->offset_h = room->offset_h;
	} else {
		room->door_position = (rand() % (room->width - 1));
		room->wall_position = (rand() % (room->height - 1)) + 1;

		room->left_child_bottom->height = room->height - room->wall_position;
		room->right_child_top->height = room->wall_position;

		room->left_child_bottom->width = room->width;
		room->right_child_top->width = room->width;

		room->left_child_bottom->offset_w = room->offset_w;
		room->left_child_bottom->offset_h = room->offset_h + room->wall_position;

		room->right_child_top->offset_w = room->offset_w;
		room->right_child_top->offset_h = room->offset_h;
	}

/* 	print_room(room);
 */
		
	maze_room_rec(room->left_child_bottom,maze);
	maze_room_rec(room->right_child_top,maze);

}

maze *maze_random (int width, int height) {
	maze *my_maze = malloc(sizeof(maze));
	
	my_maze->first_room = malloc(sizeof(tree));

	my_maze->first_room->width = width;
	my_maze->first_room->height = height;
	my_maze->width = width;
	my_maze->height = height;
	my_maze->first_room->offset_w = 0;
	my_maze->first_room->offset_h = 0;

	my_maze->first_room->parent = (tree*) my_maze;

	srand(time(NULL));

	maze_room_rec(my_maze->first_room,my_maze);

	return my_maze;
}



void maze_svg_explore_draw_tree (maze *maze, tree *room, FILE *f) {
	if (!room || room->width <= 1 || room->height <= 1 || room->wall_position == 0)
		return;

	int offset_wall_w = room->offset_w + room->wall_position;
	int offset_wall_h = room->offset_h + room->wall_position;

	/* If biggest room, we have to draw the 4 walls */
	if (room->width == maze->width && room->height == maze->height) {
		svg_line(f,0,0,maze->width,0);
		svg_line(f,maze->width,0,maze->width,maze->height);
		svg_line(f,maze->width,maze->height,0,maze->height);
		svg_line(f,0,maze->height,0,0);
	}

/* 	print_room(room);
 */

	/* Draw the wall */
	if (room->is_wall_vertical) {
		svg_line(f,
				offset_wall_w,room->offset_h,
				offset_wall_w,room->offset_h + room->door_position);

		svg_line(f,
				offset_wall_w,room->offset_h + room->door_position + 1, 
				offset_wall_w,room->offset_h + room->height);
	} else {
		svg_line(f,
				room->offset_w,offset_wall_h,
				room->offset_w + room->door_position,offset_wall_h);

		svg_line(f,
				room->offset_w + room->door_position + 1, offset_wall_h, 
				room->offset_w + room->width, offset_wall_h);
	}

	maze_svg_explore_draw_tree(maze,room->left_child_bottom,f);
	maze_svg_explore_draw_tree(maze,room->right_child_top,f);
}	


void maze_svg_draw_entrance_exit (maze *maze, FILE *f) {
	svg_rect(f,
			maze->entrance->offset_w,maze->entrance->offset_h,
			maze->entrance->offset_w + maze->entrance->width,maze->entrance->offset_h + maze->entrance->height);

	svg_rect(f,
			maze->exit->offset_w,maze->exit->offset_h,
			maze->exit->offset_w+maze->exit->width,maze->exit->offset_h+maze->exit->height);

}


void maze_svg (maze *maze, char *filename) {

	FILE *my_file;
	my_file	= fopen(filename, "w");
	if (!my_file) {
		fprintf (stderr, "couldn't open file '%s'; %s\n",
				filename, strerror(errno));
		exit (EXIT_FAILURE);
	}

	svg_header(my_file, maze->width, maze->height);

	maze_svg_draw_entrance_exit(maze,my_file);
	maze_svg_explore_draw_tree(maze,maze->first_room,my_file);

	svg_footer(my_file);

	if (fclose(my_file) == EOF) {
		fprintf (stderr, "couldn't close file '%s'; %s\n",
				filename, strerror(errno));
		exit (EXIT_FAILURE);
	}
}

void tree_free (tree *tree) {
	if (!tree)
		return;
	
/* 	tree_free(tree->left_child_bottom);
 * 	free(tree->left_child_bottom);
 * 
 * 	tree_free(tree->right_child_top);
 * 	free(tree->right_child_top);
 */

	tree_free(tree->left_child_bottom);
	tree_free(tree->right_child_top);
	free(tree);
}

void maze_free (maze *maze) {
	tree_free(maze->first_room);
	free(maze);
}

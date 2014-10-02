#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "maze.h"


void print_room (tree *room) {
	printf("Width, Height : %i, %i\n",room->width,room->height);
}

void maze_room_rec (tree *room) {
	if (room->height <= 1 || room->width <= 1)
		return;

	if (room->height < room->width)
		room->is_wall_vertical = 1;
	else if (room->height > room->width)
		room->is_wall_vertical = 0;
	else
		room->is_wall_vertical = rand() % 2;
	

	room->left_child_bottom = malloc(sizeof(tree));
	room->right_child_top = malloc(sizeof(tree));

	print_room(room);

	if (room->is_wall_vertical) {
		room->door_position = rand() % room->width;
		room->wall_position = rand() % room->height;

		room->left_child_bottom->width = room->wall_position;
		room->right_child_top->width = room->width - room->wall_position;

		room->left_child_bottom->height = room->height;
		room->right_child_top->height = room->height;

		room->left_child_bottom->offset_w = room->offset_w;
		room->left_child_bottom->offset_h = room->offset_h + room->wall_position;

		room->right_child_top->offset_w = room->offset_w;
		room->right_child_top->offset_h = room->offset_h;
	} else {
		room->door_position = rand() % room->height;
		room->wall_position = rand() % room->width;

		room->left_child_bottom->height = room->height - room->wall_position;
		room->right_child_top->height = room->wall_position;

		room->left_child_bottom->width = room->width;
		room->right_child_top->width = room->width;

		room->left_child_bottom->offset_w = room->offset_w;
		room->left_child_bottom->offset_h = room->offset_h;

		room->right_child_top->offset_w = room->offset_w + room->wall_position;
		room->right_child_top->offset_h = room->offset_h;
	}
		
	maze_room_rec(room->left_child_bottom);
	maze_room_rec(room->right_child_top);


}

maze *maze_random (int width, int height) {
	maze *my_maze = malloc(sizeof(maze));
	
	my_maze->first_room = malloc(sizeof(tree));

	my_maze->first_room->width = width;
	my_maze->first_room->height = height;
	my_maze->first_room->offset_w = 0;
	my_maze->first_room->offset_h = 0;

	my_maze->first_room->parent = (tree*) my_maze;

	srand(time(NULL));

	maze_room_rec(my_maze->first_room);

	return my_maze;
}

void maze_svg (maze *maze, char *filename) {

}

void tree_free (tree *tree) {
	if (!tree)
		return;
	
	tree_free(tree->left_child_bottom);
	free(tree->left_child_bottom);

	tree_free(tree->right_child_top);
	free(tree->right_child_top);
}

void maze_free (maze *maze) {
	tree_free(maze->first_room);
	free(maze);
}

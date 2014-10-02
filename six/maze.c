/**
 * File : maze.c
 * Author : Cyril SIX
 *
 * Implements maze.h
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "maze.h"
#include "svg.h"

#define STEP 30
#define RAND_INT(MIN, MAX) (rand()%((MAX)-(MIN)+1)+(MIN))
struct room {
	int w, h; // width and height
	int off_w, off_h; // offset relative to the parent
	int wall_pos; // between 1 and w-1 (or h-1)
	int door_pos; // between 0 and w-1 (or h-1)
	bool is_vertical;
	bool is_pathed;

	room_t *lb_child, *rt_child; // left/bottom or right/top
};

struct maze {
	int w, h;
	room_t *core;
	room_t *in, *out;
};

/**
 * Updates the w, h, off_w and off_h of the children
 */
static void init_children(room_t *parent, room_t *child1, room_t *child2)
{
	if (parent->is_vertical){
		child1->h = parent->wall_pos;
		child2->h = parent->h - parent->wall_pos;

		child1->w = parent->w;
		child2->w = parent->w;
	} else {
		child1->w = parent->wall_pos;
		child2->w = parent->w - parent->wall_pos;

		child1->h = parent->h;
		child2->h = parent->h;
	}

	child1->off_w = parent->off_w;
	child1->off_h = parent->off_h;

	child2->off_h = parent->off_h;
	child2->off_w = parent->off_w;
	if (parent->is_vertical)
		child2->off_h += child1->h;
	else
		child2->off_w += child1->w;
}

/**
 * Returns true if the room is in the border of the maze
 * false if it's not the case
 */
static bool is_in_border(room_t *room, maze_t *maze)
{
	if (room->off_w == 0 || room->off_h == 0)
		return true;
	else if (room->off_h + room->h == maze->h)
		return true;
	else if (room->off_w + room->w == maze->w)
		return true;

	return false;
}

/**
 * If the room was an entrance or an exit
 * It legates them each to one of its children
 * The children must have their w, h, off_w and off_h parameters completed
 */
static void in_out_legacy(room_t *room, maze_t *maze)
{
	const bool was_entrance = (maze->in == room);
	const bool was_exit = (maze->out == room);
	const bool lb_compatible = is_in_border(room->lb_child, maze);
	const bool rt_compatible = is_in_border(room->rt_child, maze);
	const bool both_compatible = (lb_compatible && rt_compatible);
	bool decision;

	if (!was_entrance && !was_exit)
		return;
	if (!lb_compatible && !rt_compatible){
		printf("In_out_legacy : can't find a heir !\n");
		exit(EXIT_FAILURE);
	}

	if (both_compatible)
		decision = RAND_INT(0, 1);
	else
		decision = lb_compatible;

	if (was_entrance && was_exit) // should occur only at beginning
	{
		maze->in = decision ? room->lb_child : room->rt_child;
		maze->out = decision ? room->rt_child : room->lb_child;
	}
	else if (was_entrance)
		maze->in = decision ? room->lb_child : room->rt_child;
	else if (was_exit)
		maze->out = decision ? room->lb_child : room->rt_child;

	//printf("NEW ENTRANCE : %i ; NEW OUTRANCE : %i\n", maze->in, maze->out);
}

/**
 * Generate the maze in a room
 * The room must have his w, h, off_w and off_h attributes already defined
 *
 * If it can't be done, it sets lb_child and rt_child to NULL, 
 * and the other attributes are then undefined.
 *
 * This is a recursive function
 */
static void generate(room_t *room, maze_t *maze)
{
	int shortest_edge, longest_edge;

	if (room->w < 2 || room->h < 2){
		room->lb_child = NULL;
		room->rt_child = NULL;
		return;
	}

	if (room->h > room->w)
		room->is_vertical = true;
	else if (room->h < room->w)
		room->is_vertical = false;
	else
		room->is_vertical = (bool)RAND_INT(0, 1);

	shortest_edge = room->is_vertical ? room->w : room->h;
	longest_edge = room->is_vertical ? room->h : room->w;

	room->wall_pos = RAND_INT(1, longest_edge-1);
	room->door_pos = RAND_INT(0, shortest_edge-1);

	room->lb_child = malloc(sizeof(room_t));
	room->rt_child = malloc(sizeof(room_t));

	init_children(room, room->lb_child, room->rt_child);
	in_out_legacy(room, maze);

	//display_room(room);
	generate(room->lb_child, maze);
	generate(room->rt_child, maze);
}

maze_t *maze_random(int w, int h)
{
	maze_t *maze = malloc(sizeof(maze_t));
	room_t *core = malloc(sizeof(room_t));

	core->w = w;
	core->h = h;
	core->off_w = 0;
	core->off_h = 0;
	core->is_pathed = true;

	maze->w = w;
	maze->h = h;
	maze->core = core;
	maze->in = core;
	maze->out = core;

	generate(core, maze);

	return maze;
}

/**
 * Recursively frees the tree
 */
static void free_room(room_t *room)
{
	if (room->lb_child != NULL)
		free_room(room->lb_child);
	if (room->rt_child != NULL)
		free_room(room->rt_child);
	free(room);
}

void free_maze(maze_t *maze)
{
	free_room(maze->core);
	free(maze);
	printf("Freeing complete\n");
}

/**
 * Writes the wall of the room
 * And does the same recursively with its sons
 */
static void draw_wall(room_t *room, FILE *f)
{
	unsigned int x1, x2, y1, y2;
	unsigned int xmid, ymid;

	if (room->lb_child == NULL || room->rt_child == NULL)
		return;

	set_svg_color("black");
	if (room->is_vertical){
		y1 = room->off_h + room->wall_pos;
		y2 = y1;
		x1 = room->off_w;
		x2 = room->off_w + room->w;
		xmid = room->door_pos + room->off_w;
		svg_line(f, STEP*x1, STEP*y1, STEP*xmid, STEP*y2);
		svg_line(f, STEP*(xmid+1), STEP*y1, STEP*x2, STEP*y2);
	} else {
		x1 = room->off_w + room->wall_pos;
		x2 = x1;
		y1 = room->off_h;
		y2 = room->off_h + room->h;
		ymid = room->door_pos + room->off_h;
		svg_line(f, STEP*x1, STEP*y1, STEP*x1, STEP*ymid);
		svg_line(f, STEP*x1, STEP*(ymid+1), STEP*x2, STEP*y2);
	}

	draw_wall(room->lb_child, f);
	draw_wall(room->rt_child, f);
}

void maze_svg(maze_t *maze, char *filename)
{
	FILE *f = fopen(filename, "w");
	room_t *cur;

	svg_header(f, STEP*maze->w, STEP*maze->h);
	set_svg_color("white");

	svg_rect(f, 0, 0, STEP*maze->w, STEP*maze->h);

	draw_wall(maze->core, f);
	
	//printf("ENTRANCE : %i, EXIT : %i\n", maze->in, maze->out);
	set_svg_color("red");
	cur = maze->in;
	svg_rect(f, STEP*cur->off_w+2, STEP*cur->off_h+2,\
			STEP*(cur->off_w + cur->w)-2, \
			STEP*(cur->off_h + cur->h)-2);
	set_svg_color("green");
	cur = maze->out;
	svg_rect(f, STEP*cur->off_w+2, STEP*cur->off_h+2,\
			STEP*(cur->off_w + cur->w)-2, \
			STEP*(cur->off_h + cur->h)-2);

	

	svg_footer(f);
}

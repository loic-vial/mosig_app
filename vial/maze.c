#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "maze.h"
#include "svg.h"

struct _maze
{
    int width;
    int height;
    int door_position;
    int wall_position;
    maze* left_top_child;
    maze* right_bottom_child;
    //maze *parent;           // Not needed just yet
    bool wall_is_vertical;    // Needed for square rooms
};

maze* maze_random(int width, int height)
{
    if (width == 1 && height == 1)
    {
        return NULL;
    }
    else
    {
        maze* chamber = malloc(sizeof(maze));
        chamber->width = width;
        chamber->height = height;

        if (width == height) chamber->wall_is_vertical = (rand() % 2 == 1);
        else chamber->wall_is_vertical = (width > height);

        if(chamber->wall_is_vertical)
        {
            chamber->door_position = rand() % height;
            chamber->wall_position = (rand() % (width-1)) + 1;
            chamber->left_top_child = maze_random(chamber->wall_position, height);
            chamber->right_bottom_child = maze_random(width - chamber->wall_position, height);
        }
        else
        {
            chamber->door_position = rand() % width;
            chamber->wall_position = (rand() % (height-1)) + 1;
            chamber->left_top_child = maze_random(width, chamber->wall_position);
            chamber->right_bottom_child = maze_random(width, height - chamber->wall_position);
        }

        return chamber;
    }
}

void plot_wall(FILE* fp, maze* chamber, int room_pos_x, int room_pos_y)
{
    if (chamber == NULL)
    {
        return;
    }
    else
    {
        if (chamber->wall_is_vertical)
        {
            svg_line (fp, room_pos_x + chamber->wall_position,
                      room_pos_y,
                      room_pos_x + chamber->wall_position,
                      room_pos_y + chamber->door_position);

            svg_line (fp, room_pos_x + chamber->wall_position,
                      room_pos_y + chamber->door_position+1,
                      room_pos_x + chamber->wall_position,
                      room_pos_y + chamber->height);

            plot_wall(fp, chamber->left_top_child, room_pos_x, room_pos_y);
            plot_wall(fp, chamber->right_bottom_child, room_pos_x + chamber->wall_position, room_pos_y);
        }
        else
        {
            svg_line(fp, room_pos_x,
                      room_pos_y + chamber->wall_position,
                      room_pos_x + chamber->door_position,
                      room_pos_y + chamber->wall_position);

            svg_line(fp, room_pos_x + chamber->door_position+1,
                      room_pos_y + chamber->wall_position,
                      room_pos_x + chamber->width,
                      room_pos_y + chamber->wall_position);

            plot_wall(fp, chamber->left_top_child, room_pos_x, room_pos_y);
            plot_wall(fp, chamber->right_bottom_child, room_pos_x, room_pos_y + chamber->wall_position);
        }
    }
}

void maze_svg(maze* m, char* filename)
{
    FILE* fp = fopen(filename, "w");
    int width = m->width;
    int height = m->height;

    svg_header(fp, width, height);

    set_svg_color("white");
    svg_rect(fp, 0, 0, width, height);

    svg_line(fp, 0,     0, width, 0);
    svg_line(fp, width, 0, width, height);
    svg_line(fp, width, height, 0, height);
    svg_line(fp, 0,     height, 0, 0);

    plot_wall(fp, m, 0, 0);

    svg_footer(fp);
    fclose(fp);
}

void maze_free(maze* m)
{
    if (m == NULL) return;
    maze_free(m->left_top_child);
    maze_free(m->right_bottom_child);
    free(m);
}


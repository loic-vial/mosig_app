#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "maze.h"
#include "svg.h"

struct _maze
{
    int width;
    int height;
    int wall_position;
    int door_position;
    int pos_x;
    int pos_y;
    maze* top_left_child;
    maze* bottom_right_child;
    maze* parent;
    bool wall_is_vertical;
    bool is_on_path;
};

maze* maze_random(int width, int height, int pos_x, int pos_y)
{
    maze* chamber = malloc(sizeof(maze));
    chamber->width = width;
    chamber->height = height;
    chamber->is_on_path = false;
    chamber->pos_x = pos_x;
    chamber->pos_y = pos_y;

    if (width == 1 && height == 1)
    {
        chamber->wall_position = 0;
        chamber->door_position = 0;
        chamber->top_left_child = NULL;
        chamber->bottom_right_child = NULL;
    }
    else
    {
        if (width == height) chamber->wall_is_vertical = (rand() % 2 == 1);
        else chamber->wall_is_vertical = (width > height);

        if (chamber->wall_is_vertical)
        {
            chamber->wall_position = (rand() % (width-1)) + 1;
            chamber->door_position = rand() % height;
            chamber->top_left_child = maze_random(chamber->wall_position, height,
                                                  chamber->pos_x, chamber->pos_y);
            chamber->bottom_right_child = maze_random(width - chamber->wall_position, height,
                                                      chamber->pos_x + chamber->wall_position, chamber->pos_y);
        }
        else
        {
            chamber->wall_position = (rand() % (height-1)) + 1;
            chamber->door_position = rand() % width;
            chamber->top_left_child = maze_random(width, chamber->wall_position,
                                                  chamber->pos_x, chamber->pos_y);
            chamber->bottom_right_child = maze_random(width, height - chamber->wall_position,
                                                      chamber->pos_x, chamber->pos_y + chamber->wall_position);
        }
    }

    if (chamber->top_left_child != NULL) chamber->top_left_child->parent = chamber;
    if (chamber->bottom_right_child != NULL) chamber->bottom_right_child->parent = chamber;

    return chamber;
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

            plot_wall(fp, chamber->top_left_child, room_pos_x, room_pos_y);
            plot_wall(fp, chamber->bottom_right_child, room_pos_x + chamber->wall_position, room_pos_y);
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

            plot_wall(fp, chamber->top_left_child, room_pos_x, room_pos_y);
            plot_wall(fp, chamber->bottom_right_child, room_pos_x, room_pos_y + chamber->wall_position);
        }
        if (chamber->is_on_path)
        {
            set_svg_color("red");
            svg_rect(fp, room_pos_x, room_pos_y, room_pos_x + chamber->width, room_pos_y + chamber->height);
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
    maze_free(m->top_left_child);
    maze_free(m->bottom_right_child);
    free(m);
}

bool is_child_of(maze* child, maze* parent)
{
    if (parent == NULL) return false;
    while (child != NULL && child != parent) child = child->parent;
    if (child == NULL) return false;
    return true;
}

maze* find_common_ancestor(maze* child_1, maze* child_2)
{
    while (child_1 != NULL && !is_child_of(child_2, child_1)) child_1 = child_1->parent;
    return child_1;
}
/*
maze* find_chamber_who_has_the_door(maze* parent)
{
    int x = (parent->wall_is_vertical) ? parent->wall_position : parent->door_position;
    int y = (parent->wall_is_vertical) ? parent->door_position : parent->wall_position;

    int cur_x = 0;
    int cur_y = 0;
    while (cur_x )

}
*/
/*
maze* killme(maze* chamber, int x, int y)
{
    if (chamber->pos_x == x && chamber->pos_y == y) return chamber;
    else if () return killme(chamber->top_left_child);
    else return killme(chamber->bottom_right_child);
}

void osef(maze* child_1, maze* child_2)
{
    maze* common_ancestor = find_common_ancestor(child_1, child_2);

}
*/
void maze_resolve(maze* m)
{
    maze* entrance = m;
    while (entrance->top_left_child != NULL) entrance = entrance->top_left_child;
    maze* exit = m;
    while (exit->bottom_right_child != NULL) exit = exit->bottom_right_child;
    entrance->is_on_path = true;
    exit->is_on_path = true;

    //maze* common_ancestor = find_common_ancestor(entrance, exit);

}



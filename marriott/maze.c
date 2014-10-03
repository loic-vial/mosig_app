#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "maze.h"
#include "svg.h"

struct _maze {
  int width;
  int height;
  //int offset_w, offset_h; // Not needed as implementing relative coords
  int door_position;
  int wall_position;
  maze *left_top_child;
  maze *right_bottom_child;
  //maze *parent;           // Not needed just yet
  bool wall_is_vertical;    // Needed for square rooms
};

maze *maze_random (int width, int height) {

  maze *chamber = malloc(sizeof(maze));

  /* Initialize chamber */
  chamber->width  = width;
  chamber->height = height;
  //chamber->offset_w
  //chamber->offset_w
  //chamber->parent

  if(width == 1 && height == 1) {
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
      chamber->left_top_child = maze_random(chamber->wall_position, height);
      chamber->right_bottom_child = maze_random(width - chamber->wall_position, height);
    } else { // horizontal wall
      chamber->wall_position = ( rand() % (height-1) ) + 1;
      chamber->door_position = rand() % width;
      chamber->left_top_child = maze_random(width, chamber->wall_position);
      chamber->right_bottom_child = maze_random(width, height - chamber->wall_position);
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

  svg_footer(fp);

  fclose(fp);

}



















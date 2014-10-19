#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "maze.h"
#include "svg.h"
#include <time.h>

struct _maze {
  int width;
  int height;
  int x_offset;
  int y_offset;
  int door_position;
  int wall_position;
  maze *left_top_child;
  maze *right_bottom_child;
  maze *parent;
  bool wall_is_vertical;    // Needed for square rooms
  maze *blood_line;         // Used for path finding
  bool door_plotted;
};

maze ***leaf_array;

/* A recursively-called function to split "chambers" into two child nodes
  (either left/top or right/bottom, and to store the wall/door position
  in the parent node. */
maze *maze_random (maze *parent, int width, int height) {

  maze *chamber = malloc(sizeof(maze));

  /* Initialize chamber */
  chamber->width  = width;
  chamber->height = height;
  chamber->parent = parent;
  chamber->blood_line = NULL;
  chamber->door_plotted = false;

  if(width == 1 && height == 1) {
    /* Return a 1x1 "leaf room" rather than a NULL pointer. (Convenient when path finding.) */
    chamber->door_position = 0;
    chamber->wall_position = 0;
    chamber->left_top_child = NULL;
    chamber->right_bottom_child = NULL;
    chamber->wall_is_vertical = true; // This doesn't matter as there isn't really a wall

    return chamber;
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

      /* Here, the -1 and +1 prevent the wall coinciding
         with the existing walls of the chamber. */
      chamber->wall_position = ( rand() % (width-1) ) + 1;
      chamber->door_position = rand() % height;

      // Left child
      chamber->left_top_child = maze_random(chamber, chamber->wall_position, height);
      // Right child
      chamber->right_bottom_child = maze_random(chamber, width - chamber->wall_position, height);

    } else { // Horizontal wall

      /* Here, the -1 and +1 prevent the wall coinciding
         with the existing walls of the chamber. */
      chamber->wall_position = ( rand() % (height-1) ) + 1;
      chamber->door_position = rand() % width;

      // Top child
      chamber->left_top_child = maze_random(chamber, width, chamber->wall_position);
      // Bottom child
      chamber->right_bottom_child = maze_random(chamber, width, height - chamber->wall_position);
    }
  }

  return chamber;
}


/* A recursively-called function to plot the separating walls.
   wall_position and door_position are stored as relative coordinates
   and so the position of the top-left corner of the chamber is passed
   to the function as an argument. */
void plot_wall(FILE *fp, maze *chamber, int room_pos_x, int room_pos_y) {

  if(chamber == NULL) {
    return;
  } else {

    /* Populate leaf_array when possible. */
    if(chamber->width == 1 && chamber->height == 1) {
      /* N.B. The -1s here are because of the maze's border. */
      leaf_array[room_pos_x - 1][room_pos_y - 1] = chamber;
    }

    /* Store chamber offsets for use when plotting the path. */
    chamber->x_offset = room_pos_x;
    chamber->y_offset = room_pos_y;

    if(chamber->wall_is_vertical) {
      // Draw line at fixed x-coordinate between the top of the chamber and the door.
      // Args are (fp, x1, y1, x2, y2).
      svg_line (fp, room_pos_x + chamber->wall_position, \
                    room_pos_y,                          \
                    room_pos_x + chamber->wall_position, \
                    room_pos_y + chamber->door_position);

      // Continue the wall from door_position + 1 to the bottom of the chamber.
      // Args are (fp, x1, y1, x2, y2).
      svg_line (fp, room_pos_x + chamber->wall_position,     \
                    room_pos_y + chamber->door_position + 1, \
                    room_pos_x + chamber->wall_position,     \
                    room_pos_y + chamber->height);

      // Draw walls for child chambers.
      plot_wall(fp, chamber->left_top_child, room_pos_x, room_pos_y);
      plot_wall(fp, chamber->right_bottom_child, room_pos_x + chamber->wall_position, room_pos_y);

    } else { // horizontal

      // Draw line at fixed y-coordinate between the left wall of the chamber and the door.
      // Args are (fp, x1, y1, x2, y2).
      svg_line (fp, room_pos_x, \
                    room_pos_y + chamber->wall_position, \
                    room_pos_x + chamber->door_position, \
                    room_pos_y + chamber->wall_position);

      // Continue the wall from door_position + 1 to the right wall of the chamber.
      // Args are (fp, x1, y1, x2, y2).
      svg_line (fp, room_pos_x + chamber->door_position + 1, \
                    room_pos_y + chamber->wall_position,     \
                    room_pos_x + chamber->width,             \
                    room_pos_y + chamber->wall_position);

      // Draw walls for child chambers.
      plot_wall(fp, chamber->left_top_child, room_pos_x, room_pos_y);
      plot_wall(fp, chamber->right_bottom_child, room_pos_x, room_pos_y + chamber->wall_position);
    }

  }
}


maze *find_common_ancestor(maze *start, maze *end) {

  maze* current;

  /* Mark the "blood_line" of the "start" leaf-node,
     all the way back to the root node. (By setting
     a pointer, it means we don't have to go back through
     and un-mark the nodes. */
  current = start;
  while(current != NULL) {
    current->blood_line = start;
    current = current->parent;
  }

  /* Now follow the "blood line" of the end node
     and return the first point at which it coincides
     with that of the start node. */
  current = end;
  while(current->blood_line != start) {
    current = current-> parent;

<<<<<<< HEAD
    if(current == NULL) {
      printf("ERROR: Blood-line not detected. (This shouldn't happen!)\n");
      return NULL;
    }
  }
=======
  /* Drawing a white rectangle */
  set_svg_color("white");
  svg_rect(fp, 0, 0, maze->width + 2, maze->height + 2);

  /* Plot the main "hall" with fixed entrance and exit
     positions in the bottom-left and top-right. */
>>>>>>> 302e53ff24ac2be60cf86d8a0d11a520ccb21e10

  return current;
}

void plot_path(FILE *fp, maze *start, maze *end) {

  maze *ancestor;
  maze *left_top, *right_bottom;

  if(start == end) {
    return;
  } else {

    ancestor = find_common_ancestor(start, end);

    /* Draw line passing through the door of the common ancestor. */
    if(ancestor->wall_is_vertical) {
      if(ancestor->door_plotted == false) {
        red_line (fp, ancestor->x_offset + ancestor->wall_position - 0.5, \
                      ancestor->y_offset + ancestor->door_position + 0.5, \
                      ancestor->x_offset + ancestor->wall_position + 0.5, \
                      ancestor->y_offset + ancestor->door_position + 0.5);

        ancestor->door_plotted = true;
      }

      /* Identify which leaf-rooms we have just coloured in by checking
         our array of leaf-nodes. */
      left_top      = leaf_array[ancestor->x_offset + ancestor->wall_position - 1 - 1] \
                                [ancestor->y_offset + ancestor->door_position - 1]; // left of door
      right_bottom  = leaf_array[ancestor->x_offset + ancestor->wall_position     - 1] \
                                [ancestor->y_offset + ancestor->door_position - 1]; // right of door
    } else { // horizontal
      if(ancestor->door_plotted == false) {
        red_line (fp, ancestor->x_offset + ancestor->door_position + 0.5, \
                      ancestor->y_offset + ancestor->wall_position - 0.5, \
                      ancestor->x_offset + ancestor->door_position + 0.5, \
                      ancestor->y_offset + ancestor->wall_position + 0.5);

        ancestor->door_plotted = true;
      }

      /* Identify which leaf-rooms we have just coloured in by checking
         our array of leaf-nodes. */
      left_top      = leaf_array[ancestor->x_offset + ancestor->door_position - 1] \
                                [ancestor->y_offset + ancestor->wall_position - 1 - 1]; // above door
      right_bottom  = leaf_array[ancestor->x_offset + ancestor->door_position - 1] \
                                [ancestor->y_offset + ancestor->wall_position     - 1]; // below door
    }

    /* Now try and plot the paths between the entrance/exit and the common ancestor. */
    if(ancestor->wall_is_vertical) {
      if(start->x_offset == ancestor->x_offset) {
        plot_path(fp, start, left_top);
        plot_path(fp, right_bottom, end);
      } else { // if(start->x_offset == ancestor->x_offset + ancestor->width - 1)
        plot_path(fp, right_bottom, start);
        plot_path(fp, end, left_top);
      } /*else {
        printf("ERROR: The test to find the side of the wall on which the leaf-room exists has failed (1)\n");
      }*/
    } else { // horizontal
      if(start->y_offset == ancestor->y_offset) {
        plot_path(fp, start, left_top);
        plot_path(fp, right_bottom, end);
      } else { // if(start->y_offset == ancestor->y_offset + ancestor->height - 1)
        plot_path(fp, right_bottom, start);
        plot_path(fp, end, left_top);
      } /*else {
        printf("ERROR: The test to find the side of the wall on which the leaf-room exists has failed (2)\n");
      }*/
    }

  }
}

<<<<<<< HEAD
void find_path(FILE *fp, maze *m) {

  maze *current;
  maze *entrance;
  maze *exit;

  /* Find left/top-most node (i.e. the entrance) */
  current = m;
  while(current->left_top_child != NULL) {
    current = current->left_top_child;
  }
  entrance = current;

  /* Find right/bottom-most node (i.e. the exit) */
  current = m;
  while(current->right_bottom_child != NULL) {
    current = current->right_bottom_child;
  }
  exit = current;

  /* Plot the path between the entrance and exit */
  plot_path(fp, entrance, exit);

  /* Plot the tiny entrance and exit half-lines. */
  red_line (fp, 1.0, \
                1.5, \
                1.5, \
                1.5); // Into the entrance room

  red_line (fp, 1.0 + m->width  - 0.5, \
                1.0 + m->height - 0.5, \
                1.0 + m->width,        \
                1.0 + m->height - 0.5); // Out of the exit room

}

void maze_svg (maze *m, char *filename) {

  FILE *fp;
  int i;

  srand(time(NULL));

  /* Setup leaf_array to be the size of the maze.
     This will be used later by find_path() but needs
     to be set up now as we populate it during plotting
     of the maze. */
  leaf_array = (maze***)malloc(m->width*sizeof(maze**));
  for(i = 0; i < m->width; i++) {
    leaf_array[i] = (maze**)malloc(m->height*sizeof(maze*));
  }


  fp = fopen(filename, "w");

  // The +2s provide space for a border of width 1 around the edge of the maze.
  svg_header(fp, m->width + 2, m->height + 2);

  /************* PLOTTING *************/

  /* Plot the main hall with fixed entrance and exit
     positions in the bottom-left and top-right.
     All the 1s here allow a border of 1 around the
     edge of the maze. */

  // Args are (fp, x1, y1, x2, y2).

  // Top
  svg_line (fp, 1, \
                1, \
                1 + m->width, \
                1);

  // Bottom
  svg_line (fp, 1, \
                1 + m->height, \
                1 + m->width,  \
                1 + m->height);

  // Left
  svg_line (fp, 1, \
                2, \
                1, \
                1 + m->height); // -1 leaves a gap for the entrance

  // Right
  svg_line (fp, 1 + m->width, \
                1,               \
                1 + m->width, \
                1 + m->height - 1); // The 2 here leaves a gap for the exit

  /* Plot the internal walls recursively. */
  plot_wall(fp, m, 1, 1);

  /*************************************/

  find_path(fp, m);

  /*************************************/

  svg_footer(fp);

  fclose(fp);

}

=======
>>>>>>> 302e53ff24ac2be60cf86d8a0d11a520ccb21e10

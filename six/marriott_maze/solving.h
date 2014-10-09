#ifndef __SOLVING_H__
#define __SOLVING_H__

/**
 * Solve the maze and draw the path in the same time
 *
 * fp must have been already opened, and the header of the svg
 * file should be already written
 */
void solve_maze(maze *maze, FILE *fp);

#endif

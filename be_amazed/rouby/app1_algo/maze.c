#include "maze.h"
#include "svg.h"
#include "stdlib.h"
#include "stdio.h"
#include <stdbool.h>
#include <time.h>

void maze_init (maze * myMaze, int width, int height)
{
    myMaze->root->door_position = 0;
    myMaze->root->height = height;
    myMaze->root->is_wall_vertical = false;
    myMaze->root->left_bot_child = NULL;
    myMaze->root->offset_h = 0;
    myMaze->root->offset_w = 0;
    myMaze->root->parent = NULL;
    myMaze->root->right_top_child= NULL;
    myMaze->root->wall_position = 0;
    myMaze->root->width = width;
}


int random_number(int min_num, int max_num)
{
    int result=0,low_num=0,hi_num=0;
    if(min_num<max_num)
    {
        low_num=min_num;
        hi_num=max_num+1;
    }else{
        low_num=max_num+1;
        hi_num=min_num;
    }
    srand(time(NULL));
    result = (rand()%(hi_num-low_num))+low_num;
    return result;
}

void create_random_wall(maze * myMaze)
{
    if (myMaze->root->height < myMaze->root->width) {
        myMaze->root->is_wall_vertical = true;
        myMaze->root->wall_position = random_number(1,myMaze->root->width );
        myMaze->root->door_position = random_number(1,myMaze->root->height);
    }else if (myMaze->root->height > myMaze->root->width) {
        myMaze->root->is_wall_vertical = false;
        myMaze->root->wall_position = random_number(1,myMaze->root->height);
        myMaze->root->door_position = random_number(1,myMaze->root->width);
    }else
    {
        if (rand()%2 == 0) {
            myMaze->root->is_wall_vertical = false;
            myMaze->root->wall_position = random_number(1,myMaze->root->height);
            myMaze->root->door_position = random_number(1,myMaze->root->width);
        }else {
            myMaze->root->is_wall_vertical = true;
            myMaze->root->wall_position = random_number(1,myMaze->root->width);
            myMaze->root->door_position = random_number(1,myMaze->root->height);
        }
    }
}

void divide_room (maze * myMaze)
{
    tree *left = NULL ;
    tree *right= NULL;
    if (myMaze->root->is_wall_vertical == true)
    {
        left->width = myMaze->root->wall_position;
        left->height = myMaze->root->height;
        right->width = myMaze->root->width - myMaze->root->wall_position;
        right->height = myMaze->root->height;
    }
    else
    {
        left->height = myMaze->root->wall_position;
        left->width = myMaze->root->width;
        right->height = myMaze->root->height - myMaze->root->wall_position;
        right->width = myMaze->root->width;
    }
    left->parent = myMaze->root;
    right->parent = myMaze->root;
    left->left_bot_child = NULL;
    left->right_top_child = NULL;
    right->left_bot_child = NULL;
    right->right_top_child = NULL;
    myMaze->root->left_bot_child = left;
    myMaze->root->right_top_child = right;
}

void * recursive_maze (tree * myTree)
{
    while (myTree->height >1 && myTree->width >1)
    {
            create_random_wall(myTree);
        divide_room(myTree);
            recursive_maze(myTree->left_bot_child);
                recursive_maze(myTree->right_top_child);

    }
}

maze *maze_random (int width, int height)
{
    maze* myMaze = NULL;
    maze * myRoot = myMaze;
    tree* myTree = NULL;
    myMaze->root = myTree;
    maze_init(myMaze,width,height);
    create_random_wall(myMaze);
    divide_room(myMaze);

    recursive_maze(myMaze->root->left_bot_child);
    recursive_maze(myMaze->root->right_top_child);


    return myRoot;
}

void maze_svg (maze *maze, char *filename)
{

}

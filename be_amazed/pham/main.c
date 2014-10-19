/*
 * Main.c
 *
 *  Created on: Sep 29, 2014
 *      Author: Luan
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_H 4
#define SIZE_V 3

typedef struct room {

	int sizeH;
	int sizeV;

	// position of the door
	int isWallVertical;
	int wallPosition;
	int doorPostion;

	struct room* leftChild;
	struct room* rightChild;

	struct room* parent;
} ROOM;

// reference: http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c
int random_number(int min_num, int max_num)
{
	int result=0,low_num=0,hi_num=0;

	//check special case
	if (min_num == max_num) return min_num;

	if(min_num<max_num)
	{
		low_num=min_num;
		hi_num=max_num+1; // this is done to include max_num in output.
	}else{
		low_num=max_num+1;// this is done to include max_num in output.
		hi_num=min_num;
	}
	srand(time(NULL));
	result = (rand()%(hi_num-low_num))+low_num;
	return result;
}

void putWall(ROOM* currentRoom){
	int sizeH = currentRoom -> sizeH;
	int sizeV = currentRoom -> sizeV;

	//validation
	if (sizeH * sizeV <= 1) return;

	int isWallVertical;

	//put wall
	if (sizeH >= sizeV) {
		currentRoom -> isWallVertical = 1;
		isWallVertical = 1;
	}
	else {
		currentRoom -> isWallVertical = 0;
		isWallVertical = 0;

	}

	if (isWallVertical) {
		currentRoom -> wallPosition = random_number(1, currentRoom -> sizeH - 1);
		currentRoom -> doorPostion = random_number(0, currentRoom -> sizeV - 1);
	}
	else {
		currentRoom -> wallPosition = random_number(1, currentRoom -> sizeV - 1);
		currentRoom -> doorPostion = random_number(0, currentRoom -> sizeH - 1);
	}

	printf("Size %d x %d. Wall at: %d, door at %d\n", currentRoom->sizeH, currentRoom->sizeV, currentRoom->wallPosition, currentRoom->doorPostion);

	// create 2 new rooms
	ROOM* room1 = (ROOM*) malloc (sizeof(ROOM));
	ROOM* room2 = (ROOM*) malloc (sizeof(ROOM));

	if (isWallVertical) {
		room1 -> sizeH = currentRoom->wallPosition;
		room1 -> sizeV = currentRoom->sizeV;

		room2 -> sizeH = currentRoom->sizeH - currentRoom->wallPosition;
		room2 -> sizeV = currentRoom->sizeV;
	}
	else {
		room1 -> sizeH = currentRoom->sizeH;
		room1 -> sizeV = currentRoom->wallPosition;

		room2 -> sizeH = currentRoom->sizeH;
		room2 -> sizeV = currentRoom->sizeV - currentRoom->wallPosition;
	}

	room1 -> parent = currentRoom;
	room2 -> parent = currentRoom;

	currentRoom -> leftChild = room1;
	currentRoom -> rightChild = room2;

	putWall(room1);
	putWall(room2);
}

ROOM* createMaze(){

	ROOM* header = (ROOM*) malloc(sizeof(ROOM));

	//add detail for header
	header -> sizeH = SIZE_H;
	header -> sizeV = SIZE_V;

	header -> parent = NULL;

	//creating walls
	putWall(header);

	return header;
}


int main()
{
	ROOM* header = createMaze();

    printf("Random number: %d\n", random_number(1,1) );
    //printf("Testing: %d", header->sizeH);
    return 0;
}

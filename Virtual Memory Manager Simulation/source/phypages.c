/* Physical Pages Library
 *      Contains the functions used to update entries in a virtual array of frames of memory of set size
 *      By Kevin Moore
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "phypages.h"
#include "pagetable.h"

// Set size required by assignment
#define MAX_FRAMES 8

// 2d Array of physical frames
// 		one entry for each frame to hold it's page and it's last access time, 0 is reserved for OS
int phyFrames[8][2] = { { 0, -1 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1 ,0 } };
// Fault counter
static int faults;

// Initializer
//		Sets the fault counter to 0
void initPhyPages(){
	faults = 0;
}

// Frame Updater
//		Checks if a frame was updated or accessed and makes relevant changes
void updateFrame(int frame, int update, int time){
	if (frame == 0){
		fprintf(stderr, "Error: Attempt to change OS frame!");
		//exit(1);
	}
    
	if (phyFrames[frame][0]==update){
		//fprintf(stderr, "\tIn phypages.c: Accessing frame #%d with page #%d at time %d\n", frame, update, time);
		phyFrames[frame][1] = time;
	}
	else{
		//fprintf(stderr, "\tIn phypages.c: Old page #%d New page #%d\n", phyFrames[frame][0], update);
		//fprintf(stderr, "\tIn phypages.c: Updating frame #%d with page #%d at time %d\n", frame, update, time);
		//getchar();
		faults++;
		fprintf(stderr, "\t Page fault\n");
		phyFrames[frame][0] = update;
		phyFrames[frame][1] = time;
		removeMapping(update, frame);
		
	}
}

// LRU check function
// 		Searches phyFrames for the minimum access time which will correspond to the least recently used frame
int getLRUframe(){
	int i, minVal, min;
	minVal = INT_MAX;
        //fprintf(stderr, "\tIn phypages.c: Searching for minimum time in phyFrames\n");

	for (i = 1; i < MAX_FRAMES; i++){
		if (phyFrames[i][1] < minVal){
                //fprintf(stderr, "\tIn phypages.c: Found new minumum at frame #%d with time %d\n", i, phyFrames[i][1]);
                minVal = phyFrames[i][1];
                min = i;
			}
	}
	return min;
}

// helper function
// 		Returns the number of faults, allowing other files read access only
int getFaults(){
    return faults;
}

// helper function
//		Returns the access time for a given frame without updating it, allowing other files read access only
int getFrameTime(int frame){
    return phyFrames[frame][1];
}

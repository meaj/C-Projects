/* Expanded Physical Pages Library
 *      Contains the functions used to update entries in a virtual array of frames of memory of variable size
 *      Built upon file "phypages.c"
 *      By Kevin Moore
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "phypagesEX.h"
#include "pagetableEX.h"

// Fault counter
static int faults;
// Maximum number of frames allowed
static int maxFrames;
// 2d Array of frames
// 		The first index is the frame number, second index stores either the associated page (0) or the last update time (1)
int ** phyFrames;

// Initializer
// 		Sets the maximum number of frames
// 		Sets up the phyFrames in memory and gives each frame an initial page value and access time
void initPhyPages(int numFrames){
	int i;
	maxFrames = numFrames;
	phyFrames = (int**) malloc(maxFrames * sizeof(int *));
	for(i = 0; i < maxFrames; i++){
		phyFrames[i] = (int*) malloc(2 * sizeof(int));
	}
	// First frame is reserved for the OS
	phyFrames[0][0] = -1;
	phyFrames[0][1] = -1;
	// Initializes each frame
	for(i = 1; i < maxFrames; i++){
		phyFrames[i][0] = -1; // set initial page to -1
		phyFrames[i][1] = 0; // set initial time to 0
	}
	
	faults = 0;
}

// Frame Updater
//		Checks if a frame was updated or accessed and makes relevant changes
void updateFrame(int frame, int update, int time){
	// Check for reserved OS Frame
	if (frame == 0){
		fprintf(stderr, "Error: Attempt to change OS frame!");
		exit(1);
	}

	// If frame is accessed, update time
	if (phyFrames[frame][0]==update){
		//fprintf(stderr, "\tIn phypages.c: Accessing frame #%d with page #%d at time %d\n", frame, update, time);
		phyFrames[frame][1] = time;
	}
	// If frame is updated, update page and time values for the frame, and increment the fault counter
	else{
		//fprintf(stderr, "\tIn phypages.c: Old page #%d New page #%d\n", phyFrames[frame][0], update);
		//fprintf(stderr, "\tIn phypages.c: Updating frame #%d with page #%d at time %d\n", frame, update, time);
		//getchar();
		faults++;
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

	for (i = 1; i < maxFrames; i++){
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

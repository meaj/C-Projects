/* Page Table Library
 *      Contains the functions used to update entries in a virtual page table of fixed size
 *      By Kevin Moore
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "pagetable.h"
#include "phypages.h"

#define MAX_FRAMES 8
#define MAX_PAGES 32

// one entry for each page to hold its mapping and last access time
int pageTable[32][2];

// Initializer function
//      Initializes each pageTable entry's mapping and access time to 0
void initTable(){
	int i;
	for(i = 0; i < MAX_PAGES; i++){
		pageTable[i][0] = 0;
		pageTable[i][1] = 0;
	}
}

// Page Table updater
//      Updates either the access time or access time and mapping of a page table entry depending on inputs
//      Returns the last updated frame
int updateTable(int page, int time){
	int i, retValue, minFrame, curFrame;
	bool updated = false;
	//fprintf(stderr, "\tIn pagetable.c: Starting Update of page table entry #%d\n", page);
	// each time the table is updated, we check the minimum time,
	// and search the table for our entry. If found, we update the time, note an update occurred,
	for (i = 1; i < MAX_FRAMES; i++){
		if (pageTable[page][0] == i){
			//fprintf(stderr, "\tIn pagetable.c: Updating page table entry #%d with entry #%d at time %d\n", page, i, time+1);
            pageTable[page][1] = time+1;
			updateFrame(i, page, time+1);
			updated = true;
			retValue = i;
			break;
		}
	}
	// If no update has occurred, we update the least recently used entry with the data and time of this entry
	if (updated == false){
		minFrame = getLRUframe();
        //minFrame = getLRUpage();
		curFrame = pageTable[page][0];
        //fprintf(stderr, "\tIn pagetable.c: Recieved minFrame #%d from phypages.c\n", minFrame);
        //fprintf(stderr, "\tIn pagetable.c: Updating page table entry #%d with minFrame #%d at time %d\n", page, minFrame, time+1);
        pageTable[page][0] = minFrame;
		pageTable[page][1] = time+1;
		updateFrame(minFrame, page, time + 1);
		retValue = minFrame;
	}
	return retValue;
}

// Mapping updater
//      Removes the mapping between an entry in pageTable and an entry in phyFrames and sets to default value of 0
void removeMapping(int page, int frame){
	int i;
	for (i = 0; i < MAX_PAGES; i++){
		if (pageTable[i][0] == frame && i!=page){
			pageTable[i][0] = 0;
		}
	}
}

// LRU check function
// 		Searches pageTable for the minimum access time which will correspond to the least recently used page table entry
int getLRUpage(){
	int i, min, minPage;
	minPage = INT_MAX;
	for (i=0; i < MAX_PAGES; i++){
		if (pageTable[i][1] < minPage){
			min = i;
			minPage = pageTable[i][1];
		}
	}
	return min;
}

// helper function
//      Returns the mapping of a particular entry in pageTable, allowing other files read access only
int checkTable(int page){
	return pageTable[page][0];
}

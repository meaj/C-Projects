/* Expanded Page Table Library
 *      Contains the functions used to update entries in a virtual page table of variable size
 *      Built upon file "pagetable.c"
 *      By Kevin Moore
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "pagetableEX.h"
#include "phypagesEX.h"

// 2d array of page table entries
// 		One entry for each page to hold its mapping and last access time
int **pageTable;
// The maximum number of pages
static int maxPages;

// Initializer function
//      Sets the maximum number of pages
//      Sets up pageTable in memory and initializes each entry's mapping and access time to 0
void initTable(int numPages){
	int i;
	maxPages = numPages;
	pageTable = (int**) malloc(maxPages * sizeof(int *));
	for(i = 0; i < numPages; i++){
		pageTable[i] = (int*) malloc(2 * sizeof(int));
	}
	
	for(i = 0; i < numPages; i++){
		pageTable[i][0] = 0;
		pageTable[i][1] = 0;
	}
}

// Page Table updater
//      Updates either the access time or access time and mapping of a page table entry depending on inputs
//      Returns the last updated frame
int updateTable(int page, int time, int maxFrames){
	int i, retValue, minFrame, curFrame;
	bool updated = false;
	//fprintf(stderr, "\tIn pagetable.c: Starting Update of page table entry #%d\n", page);
	// each time the table is updated, we check the minimum time,
	// and search the table for our entry. If found, we update the time, note an update occurred,
	for (i = 1; i < maxFrames; i++){
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
	if (! updated){
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
	for (i = 0; i < maxPages; i++){
		if (pageTable[i][0] == frame && i != page){
			pageTable[i][0] = 0;
		}
	}
}

// helper function
//      Returns the mapping of a particular entry in pageTable, allowing other files read access only
int checkTable(int page){
	return pageTable[page][0];
}

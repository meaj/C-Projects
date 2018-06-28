/* Virtual Memory Management Simulator 1
 *      Preforms memory address translation converting the addresses from the input file into physical addresses
 *      This is done with a fixed amount of physical memory, virtual memory, and page size
 *      A Least Recently Used function is used to handle the memory management
 *      By Kevin Moore
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include "pagetable.h"
#include "phypages.h"

#define PHYSICAL_ADDRESSES 1024
#define VIRTUAL_ADDRESSES 4096
#define PAGE_SIZE 128

inline size_t alignup(size_t size, size_t alignto){
    return ((size + (alignto - 1)) & ~(alignto - 1));
}

// Driver function
//      Calls relevant setup functions and runs the virtual memory simulation
//      Opens input and output files for read and write
//      Reports the number of faults encountered by the program
void analyzeAccessSequenceFromFile(char * fileName){
    int i, p, pageStart, fd, frame;
    struct stat st;
    unsigned long filesize;
    unsigned long mapsize;
    unsigned long address;
    unsigned long * memAccesses;
    FILE *write_p;
	
	stat(fileName, &st);
	
	write_p = fopen("output-part2", "wb");
	
    filesize = st.st_size;

    // Open input sequence file
    fd = open(fileName, O_RDONLY);
    if(fd == -1){
        fprintf(stderr, "File open error: %s: %d", fileName, strerror(errno));
        exit(-1);
    }

    // Calculate the aligned size and setup mapping
    mapsize = alignup(filesize, VIRTUAL_ADDRESSES);
    memAccesses = (unsigned long *)mmap(0, mapsize, PROT_READ, MAP_PRIVATE, fd, 0);
    if(memAccesses == MAP_FAILED){
        fprintf(stderr, "mmap to the input file failed\n");
        exit(-1);
    }

    // Initialize the frames
	initPhyPages();
	
    fprintf(stderr, "map starting %p filesize %ld\n", memAccesses, filesize);
    // Run simulation
    for(i = 0; i < filesize/sizeof(unsigned long); i++){
        fprintf(stderr, "virtual address %d: 0x%lx\n", i, memAccesses[i]);
        address = memAccesses[i];
        pageStart = 0;
        p = 0;
        while(pageStart < VIRTUAL_ADDRESSES){
            if(address < pageStart + PAGE_SIZE){
                address = address - pageStart;
                break;
            }
            p++;
            pageStart = pageStart + PAGE_SIZE;
        }
       
        fprintf(stderr, "\toffset: 0x%lx\n\tpage: %d\n", address, p);

        // pass page number(p) and time(i) to update the page table
        frame = updateTable(p, i);
        

        fprintf(stderr, "\tframe: %d\n", frame);
        address = address + frame * PAGE_SIZE;
        fprintf(stderr, "physical address %d: 0x%lx\n", i, address);
		fwrite((const void*) &address, sizeof(unsigned long), 1, write_p);
    }
	int faults = getFaults();
	fprintf(stderr, "Total Faults: %d\n", faults);
	fclose(write_p);
	close(fd);
}

// Main function
//      Gets the input file name from the command line
//      Calls the driver function
int main(int argc, char* argv[]){
    char *  sequenceFileName = argv[1];

    if(access(sequenceFileName, F_OK)){
        fprintf(stderr, "The sequence file %s does not exist\n", sequenceFileName);
        exit(-1);
    }
    
    analyzeAccessSequenceFromFile(sequenceFileName);
	return 0;
}
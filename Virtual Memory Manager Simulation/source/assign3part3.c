/* Virtual Memory Management Simulator 2
 *      Preforms memory address translation converting the addresses from the input file into physical addresses
 *      This is done with a variable amount of physical memory, virtual memory, and page size
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

#include "pagetableEX.h"
#include "phypagesEX.h"

inline size_t alignup(size_t size, size_t alignto){
    return ((size + (alignto - 1)) & ~(alignto - 1));
}

// Driver function
//      Calls relevant setup functions and runs the virtual memory simulation
//      Opens input and output files for read and write
//      Reports the number of faults encountered by the program
void analyzeAccessSequenceFromFile(char * fileName, int pageSize, int virtualAddresses, int physicalAddresses){
    int i, p, pageStart, fd, frame, numPages, numFrames;
    struct stat st;
    unsigned long filesize;
    unsigned long mapsize;
    unsigned long address;
    unsigned long * memAccesses;
    FILE *write_p;
	
	numPages = virtualAddresses/pageSize;
	numFrames = physicalAddresses/pageSize;
	
	stat(fileName, &st);
	
	write_p = fopen("output-part3", "wb");
	
    filesize = st.st_size;

    // Open input sequence file
    fd = open(fileName, O_RDONLY);
    if(fd == -1){
        fprintf(stderr, "File open error: %s: %d", fileName, strerror(errno));
        exit(-1);
    }

    // Calculate the aligned size and setup mapping
    mapsize = alignup(filesize, virtualAddresses);
    memAccesses = (unsigned long *)mmap(0, mapsize, PROT_READ, MAP_PRIVATE, fd, 0);
    if(memAccesses == MAP_FAILED){
        fprintf(stderr, "mmap to the input file failed\n");
        exit(-1);
    }

    // Initialize frames and page table for simulation
	initPhyPages(numFrames);
	initTable(numPages);
	
    fprintf(stderr, "map starting %p filesize %ld\n", memAccesses, filesize);
    // Run the simulation
    for(i = 0; i < filesize/sizeof(unsigned long); i++){
        //fprintf(stderr, "virtual address %d: 0x%lx\n", i, memAccesses[i]);
        address = memAccesses[i];
        pageStart = 0;
        p = 0;
        while(pageStart < virtualAddresses){
            if(address < pageStart + pageSize){
                address = address - pageStart;
                break;
            }
            p++;
            pageStart = pageStart + pageSize;
        }
       
        //fprintf(stderr, "\toffset: 0x%lx\n\tpage: %d\n", address, p);

        // pass page number(p) and time(i) to update the page table
        frame = updateTable(p, i, numPages);

        //fprintf(stderr, "\tframe: %d\n", frame);
        address = address + frame * pageSize;
        //fprintf(stderr, "physical address %d: 0x%lx\n", i, address);
		fwrite((const void*) &address, sizeof(unsigned long), 1, write_p);
    }
	int faults = getFaults();
	fprintf(stderr, "Total Faults: %d\n", faults);
	fclose(write_p);
	close(fd);
}

// Main function
//      Gets the input file name, page size, virtual memory size, and physical memory size from the command line
//      Calls the driver function
int main(int argc, char* argv[]){
    char *  sequenceFileName = argv[4];
	int pageSize = atoi(argv[1]);
	int virtualMemSize = atoi(argv[2]);
	int physicalMemSize = atoi(argv[3]);
	
    if(access(sequenceFileName, F_OK)){
        fprintf(stderr, "The sequence file %s does not exist\n", sequenceFileName);
        exit(-1);
    }
    
    analyzeAccessSequenceFromFile(sequenceFileName, pageSize, virtualMemSize, physicalMemSize);
	return 0;
}
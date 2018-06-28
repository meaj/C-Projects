/* Process Scheduling 2
 *      This program takes a sequence of 7 ints and runs a variety of scheduling algorithms on them.
 *      This program makes use of functions in the file "pslibrary.c"
 *      The scheduling algorithms used are FCFS, SJF, PSJF, and RR scheduling
 *      By Kevin Moore
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pslibrary.h"

int main(int argc, char *argv[]){
    if(argc != 8){
        printf("Error: Expected 7 arguments, %d provided\n", argc-1);
        exit(1);
    }

    int i, qu, x1, y1, z1, x2, y2, z2;
    int max_size = 0;

    // prints assignment details and imputs, and calculates max_size
    printf("Assignment 2 program was written by Kevin Moore\n");
    printf("inputs:");
    for(i = 1; i < argc; i++){
        printf(" %s", argv[i]);
        max_size += strtol(argv[i], NULL, 10);
    }
    printf("\n");
    //printf("maxSize: %d\n", maxSize);

    // setup string 1 and string 2
    char *str1 = (char *) malloc(max_size);
    char *str2 = (char *) malloc(max_size);

    qu = strtol(argv[1], NULL, 10);
    x1 = strtol(argv[2], NULL, 10);
    y1 = strtol(argv[3], NULL, 10);
    z1 = strtol(argv[4], NULL, 10);
    x2 = strtol(argv[5], NULL, 10);
    y2 = strtol(argv[6], NULL, 10);
    z2 = strtol(argv[7], NULL, 10);

    // First come first serve scheduling
    fcfs(str1, str2, x1, y1, z1, x2, y2, z2);
    display("FCFS\n", str1, str2);

    // Shortest job first scheduling
    sjf(str1, str2, x1, y1, z1, x2, y2, z2);
    display("SJF\n", str1, str2);

    // Preemptive shortest job first scheduling
    psjf(str1, str2, x1, y1, z1, x2, y2, z2);
    display("PSJF\n", str1, str2);

    // Round robin scheduling
    rr(str1, str2, qu, x1, y1, z1, x2, y2, z2);
    display("RR\n", str1, str2);
	
    free(str1);
    free(str2);
}

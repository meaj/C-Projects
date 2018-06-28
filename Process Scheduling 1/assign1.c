/* Process Scheduling 1
 *      This program takes a sequence of 7 ints and runs and implementation of first come first serve scheduling on them.
 *      This program makes use of functions in the file "pslibrary.c" to produce this implementation
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

    int i, x1, y1, z1, x2, y2, z2;
    int max_size = 0;

    // prints assignment details and inputs, and calculates max_size
    printf("Assignment 1 program was written by Kevin Moore\n");
    printf("inputs:");
    for(i = 1; i < argc; i++){
        printf(" %s", argv[i]);
        max_size += strtol(argv[i], NULL, 10);
    }
    printf("\n");
    //printf("maxSize: %d\n", maxSize);

    // setup string 1 and string 2
    char *str1;
    char *str2;
    str1 = (char *) malloc(max_size);
    str2 = (char *) malloc(max_size);

    x1 = strtol(argv[2], NULL, 10);
    y1 = strtol(argv[3], NULL, 10);
    z1 = strtol(argv[4], NULL, 10);
    x2 = strtol(argv[5], NULL, 10);
    y2 = strtol(argv[6], NULL, 10);
    z2 = strtol(argv[7], NULL, 10);

    fcfsa(str1, str2, x1, y1, z1, x2, y2, z2);
    display("Part 1\n", str1, str2);

    free(str1);
    free(str2);
}

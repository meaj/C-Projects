/* Process Scheduling 0
 *      This program is a test file that the process scheduler will be built from
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

    int i;
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

    part0(str1, str2);
    display("Part 0\n", str1, str2);
    
    free(str1);
    free(str2);
}

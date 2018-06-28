/*  Dining Philosophers 0
 *      This program is the setup for a trio of solutions to the Dining Philosopher problem
 *      This program creates a variable number of threads which state their respective ID number and exit
 *      By Kevin Moore
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_t *threads;

// Philosopher Thread function
//      Prints the ID of the thread and exits
void *PhilosopherThread(void *threadid){
    long philosopherID;
    philosopherID = (long)threadid;
    printf("This is philosopher %ld\n", philosopherID+1);
    pthread_exit(NULL);
}

// Thread creation function
//      Creates nthreads philosopher threads
void createPhilosophers(int nthreads){
    long i;
    int r;

    // Thread creation
    printf("Creating Threads\n");
    for (i = 0; i < nthreads; i++){
        r  = pthread_create(&threads[i], NULL, PhilosopherThread, (void*)i);
        if(r){
            fprintf(stderr, "Error: return code from pthread_create() is %d\n", r);
            exit(-1);
        }
    }
    for(i = 0; i < nthreads; i++){
        pthread_join(threads[i], NULL);
    }
    printf("%ld threads have been joined successfully now\n", i);
}

// Main function
//      Allocates memory for philosopher threads based on user specifications
//      Calls thread creation function
int main(int argc, char *argv[]){
    // Input validation
    if (argc < 2){
        fprintf(stderr, "Error: too few arguments, expected 1\n");
        exit(-1);
    }
    if (argc > 2){
        fprintf(stderr, "Error: too many arguments,expected 1\n");
        exit(-1);
    }

    int nthreads;

    // Allocate memory based on user specification
    nthreads = atoi(argv[1]);
    threads = (pthread_t*) malloc(nthreads*sizeof(pthread_t));

    // Calls thread creation function
    createPhilosophers(nthreads);

    // Frees allocated memory
    free(threads);
    return 0;
}

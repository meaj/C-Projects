/*  Dining Philosophers 2
 *      Solution to the Dining Philosophers problem using an array of threads to represent the philosophers,
 *      a single mutex representing a pair of chopsticks, and a shared conditional variable.
 *      After ~200 test runs no deadlocking occurred
 *      By Kevin Moore
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lab4.h"

// Array of philosopher threads
pthread_t *threads;
// Chopstick mutex
pthread_mutex_t chopsticks;
// Shared conditional variable
static int canEat;
// Index of last thread in array
static int maxIndex;

// thinking() function
//      Causes the thread to wait for a pseudorandom number of microseconds
void thinking(int threadIndex){
    printf("Philosopher %d is thinking\n", threadIndex+1);
    usleep(random()/(RAND_MAX/(500-1+1)+1));
}

// eating() function
//      Causes the thread to wait for a pseudorandom number of microseconds and calls putDownChopsticks()
void eating(int threadIndex){
    printf("Philosopher %d is eating\n", threadIndex+1);
    usleep(random()/(RAND_MAX/(500-1+1)+1));
    putDownChopsticks(threadIndex);
}

// pickUpChopsticks() function
//      Uses the chopstick mutex to check if a given philosopher thread can call eating()
void pickUpChopsticks(int threadIndex){ 
        printf("Philosopher %d has picked up chopsticks\n", threadIndex + 1);
        pthread_mutex_lock(&chopsticks);
        eating(threadIndex);
}

// putDownChopsticks() function
//      Releases the chopstick mutex so that other threads may execute
void putDownChopsticks(int threadIndex){
    pthread_mutex_unlock(&chopsticks);
    printf("Philosopher %d has put down chopsticks\n", threadIndex+1);
}

// Philosopher Thread function
//      Prints the ID of the thread, and checks if it is its turn to eat
//      If not it calls thinking(), if so it calls pickUpChopsticks()
//      Lastly it advances the conditional variable and calls pthread_exit()
void *PhilosopherThread(void *threadid){
    int status;
    long philosopherID;
    philosopherID = (long)threadid;
    status = (int)philosopherID;
    printf("This is philosopher %ld\n", philosopherID+1);
    
    while (canEat != status){
        thinking(status);
    }
    pickUpChopsticks(status);
    canEat++;
    pthread_exit(NULL);
}

// Thread creation function
//      Creates nthreads philosopher threads
void createPhilosophers(int nthreads){
    long i;
    int r;
    
    // seed the random function
    srandom(time(NULL));
    
    canEat = 0;
    
    printf("Creating Threads\n");
    for (i = 0; i < nthreads; i++){
        r  = pthread_create(&threads[i], NULL, PhilosopherThread, (void*)i);
        if(r){
            fprintf(stderr, "Error: return code from pthread_create() is %d\n", r);
            exit(-1);
        }
    }

    for(i=0; i < nthreads; i++){
        pthread_join(threads[i],NULL );
    }
    printf("%ld threads have been joined successfully now\n", i);
}

// Main function
//      Allocates memory for philosopher threads based on user specifications
//      Initializes the chopsick mutex
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


    int i, nthreads;

    // Allocate memory based on user specification
    nthreads = atoi(argv[1]);
    maxIndex = nthreads;
    threads = (pthread_t*) malloc(nthreads*sizeof(pthread_t));

    // Initializes the chopstick mutex
    if (pthread_mutex_init(&chopsticks, NULL) != 0){
        fprintf(stderr, "Mutex %d init failed\n", i+1);
        exit(-1);
    }

    // Calls thread creation function
    createPhilosophers(nthreads);
    // Frees allocated memory
    free(threads);
    return 0;
}

/*  Dining Philosophers 1
 *      Solution to the Dining Philosophers problem using an array of threads to represent the philosophers and an,
 *      array of mutexes to represent the philosopher's chopsticks. It does not make use of pthread_mutex_trylock()
 *      After ~500 test runs no deadlocking occurred
 *      By Kevin Moore
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "lab4.h"

// Array of philosopher threads
pthread_t *threads;
// Array of chopstick mutexes
pthread_mutex_t *chopsticks;
// Index of last thread in array
static int maxIndex;

// thinking() function
//      Causes the thread to wait for a pseudorandom number of microseconds
void thinking(int threadIndex){
    printf("Philosopher %d is thinking\n", threadIndex);
    usleep(random()/(RAND_MAX/(500-1+1)+1));
}

// eating() function
//      Causes the thread to wait for a pseudorandom number of microseconds and calls putDownChopsticks()
void eating(int threadIndex){
    printf("Philosopher %d is eating\n", threadIndex);
    usleep(random()/(RAND_MAX/(500-1+1)+1));
    putDownChopsticks(threadIndex);
}

// pickUpChopsticks() function
//      Uses the chopstick mutexes to check if a given philosopher thread can call eating()
void pickUpChopsticks(int threadIndex){
    int stick1, stick2;

    pthread_mutex_lock(&chopsticks[threadIndex]);
    // Check if the philosopher is the last in the array, if not it must 'grab' the next philosopher's stick
    // If so it must grab the first philosopher's stick
    if(threadIndex != maxIndex){
	    pthread_mutex_lock(&chopsticks[threadIndex+1]);
	    stick2 = threadIndex+1;
    }
    else{
	    pthread_mutex_lock(&chopsticks[0]);
	    stick2 = 0;
    }
    printf("Philosopher %d has picked up chopsticks %d and %d\n", threadIndex, stick1, stick2);
    eating(threadIndex);
}

// putDownChopsticks() function
//      Releases the chopstick mutexes so that other threads may execute
void putDownChopsticks(int threadIndex){
    int stick1, stick2;
    stick1 = threadIndex;
    if(threadIndex != maxIndex){
	    stick2 = threadIndex+1;
    }
    else{
	    stick2 = 0;
    }
    printf("Philosopher %d has put down chopsticks %d and %d\n", threadIndex, stick1, stick2);
    pthread_mutex_unlock(&chopsticks[stick1]);
    pthread_mutex_unlock(&chopsticks[stick2]);
}

// Philosopher Thread function
//      Prints the ID of the thread, calls thinking(), pickUpChopsticks(), and phtead_exit()
void *PhilosopherThread(void *threadid){
    long philosopherID;
    int threadIndex;
    threadIndex = (uintptr_t)threadid;
    philosopherID = (long)threadid;
    printf("This is philosopher %ld\n", philosopherID);
    thinking(threadIndex);
    pickUpChopsticks(threadIndex);
    pthread_exit(NULL);
}

// Thread creation function
//      Creates nthreads philosopher threads
void createPhilosophers(int nthreads){
    long i;
    int r;
    
    // seed the random function
    srandom(time(NULL));

    printf("Creating Threads\n");
    for (i = 0; i < nthreads; i++){
        r  = pthread_create(&threads[i], NULL, PhilosopherThread, (void*)(uintptr_t)i);
        if(r){
            fprintf(stderr, "Error: return code from pthread_create() is %d\n", r);
            exit(1);
        }
    }

    for(i=0; i < nthreads; i++){
        pthread_join(threads[i],NULL );
    }
    printf("%ld threads have been joined successfully now\n", i);
}

// Main function
//      Allocates memory for philosopher threads and chopstick mutexes based on user specifications
//      Initializes the chopstick mutexes
//      Calls thread creation function
int main(int argc, char *argv[]){
    // Input validation
    if (argc < 2){
        fprintf(stderr, "Error: too few arguments, expected 1\n");
        exit(1);
    }
    if (argc > 2){
        fprintf(stderr, "Error: too many arguments,expected 1\n");
        exit(1);
    }

    int i, nthreads;

    // Allocate memory based on user specification
    nthreads = atoi(argv[1]);
    maxIndex = nthreads - 1;
    threads = (pthread_t*) malloc(nthreads*sizeof(pthread_t));
    chopsticks = (pthread_mutex_t*) malloc(nthreads*sizeof(pthread_mutex_t));

    // Initializes the chopstick mutexes
    for(i = 0 ; i < nthreads; i++){
        if (pthread_mutex_init(&chopsticks[i], NULL) != 0){
            fprintf(stderr, "Mutex %d init failed\n", i+1);
            exit(1);
        }
    }

    // Calls thread creation function
    createPhilosophers(nthreads);

    // Frees allocated memory
    free(threads);
    free(chopsticks);
    return 0;
}

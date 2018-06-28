#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void thinking(int threadIndex);
void eating(int threadIndex);
void pickUpChopsticks(int threadIndex);
void putDownChopsticks(int threadIndex);
void *PhilosopherThread(void *threadid);
void createPhilosophers(int nthreads);
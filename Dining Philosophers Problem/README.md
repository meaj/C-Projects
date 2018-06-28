# Dining Philosopher Problem Solutions
### Description:
These C files represent various solutions to the [Dining Philosopher problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem).
They are an exercise in timing thread execution using mutexes, thread waiting, and conditional variables and were completed as part of my University coursework.

***

### Contents:

 **assign4part1.c** : Base file used to create the other three files. Allows the user to control the number of threads created.
 
 **assign4part2.c** : First solution, uses thread waiting and an array of mutexes to control thread execution. Does not _appear_ to deadlock after ~500 tests with 50 threads.
 
 **assign4part3.c** : Second solution, uses thread waiting, a single mutex, and a shared counter which acts as a conditional variable to control thread execution. Each thread executes sequentailly, preventing deadlock.
 
 **assign4part4.c** : Third solution, uses thread waiting, an array of mutexes, and the pthread_mutex_trylock() function to control thread execution. Does not _appear_ to deadlock after ~200 tests with 50 threads.
 
 **lab4.h** : The .h file which has the function prototypes used by parts 2, 3, and 4.
 
 **makefile** : The makefile which was used to build and test the solutions.

# Process Scheduling 2 Project
### Description:
These C files represent an implementation of first come first serve process scheduling, shortest job first scheduling, preemptive shortest job first scheduling, and round robin scheduling.
They were completed as part of my University coursework.

***

### Contents:

 **assign2.c** : Runs the various algorithms for process scheduling which are defined in `pslibrary.c`.
 
 **pslibrary.c** : Library of functions used to emulate process scheduling. Contains FCFS, SJF, PSJF, and RR algorithms and helper functions.
 
 **runalla.c** : Used to run all possible input values from `0,0,0,0,0,0,0` to `60,60,60,60,60,60,60` on all sheduling algorithms.
 
 **pslibrary.h** : The .h file which has the function prototypes used by `pslibrary.c`.
 
 **makefile** : The makefile which was used to build and test the project.

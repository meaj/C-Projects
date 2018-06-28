/* Process Scheduling Library
 *      This file contains the functions that are a part of the process scheduling library.
 *      By Kevin Moore
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int E_TASK_READY = 0;
static int E_TASK_RUNNING = 1;
static int E_TASK_WAITING = 2;
static int E_TASK_DONE = 3;

static char stateChars[] = {'r','R','w',0};

void part0(char *s1, char *s2){
    strcpy(s1, "RRwwwwwRRRRRRRRR");
    strcpy(s2, "rrRRRRwwwwwwwwrrRRRRRRR");
}

// helper function
// finds the number of 'r' chars in a string str
int rValues(char *str){
    int i;
    int out = 0;
    for (i = 0; i<strlen(str); i++){
        if (str[i] == 'r')
            out++;
    }
    return out;
}

// helper function
// finds the total number of 'R' chars in s1 and s2 and returns the
// value over the lenght of the longer string
float avgRValues(char *s1, char *s2){
    int i, s1_len, s2_len;
    int s1_R = 0;
    int s2_R = 0;
    float out = 0;

    s1_len = strlen(s1);
    s2_len = strlen(s2);
    // get number of 'R' chars in s1
    for (i = 0; i < s1_len ; i++){
        if (s1[i] == 'R')
            s1_R++; 
    }
    // get number of 'R' chars in s2
    for (i = 0; i < s2_len; i++){
        if(s2[i] == 'R')
            s2_R++;
    }

    if (s1_len >= s2_len)
        out = (s1_R + s2_R)/(float)s1_len;
    else
        out = (s1_R + s2_R)/(float)s2_len;
    
    /*
    Print statements used for testing
    printf("'R' chars in str1: %d\n", s1_R);
    printf("str1 len: %d\n", s1_len);
    printf("'R' chars in str2: %d\n", s2_R);
    printf("str2 len: %d\n", s2_len);
    printf("out value: %f\n", out);
    */
    return out;
}

// displays the heading, the two strigns s1 and s2
// calculates and displays information about the strings s1 and s2
void display(char *heading, char *s1, char *s2){
    int i, num_r_s1, num_r_s2;
    float avg_r, num_R_over_length;

    // print header, s1, and s3
    printf("\n%s%s\n%s\n", heading, s1, s2);

    //calculate num_r_s1 and num_r_s2
    num_r_s1 = rValues(s1);
    num_r_s2 = rValues(s2);

    //calculate the average number of 'r' chars
    avg_r = (num_r_s1 + num_r_s2)/2.0;
    
    // calculates the total number of R values over the length of the  longer string
    num_R_over_length = avgRValues (s1, s2);

    printf("%d %d %.1f %0.5f\n", num_r_s1, num_r_s2, avg_r, num_R_over_length);

}

// A stateless implementation of FCFS scheduling
void fcfsa(char* s1, char* s2, int x1, int y1, int z1, int x2, int y2, int z2){
    int max = x1+x2+y1+y2+z1+z2;
	char* out_string_1;
    out_string_1 = (char *) calloc(max, sizeof(char*));
    char* out_string_2;
    out_string_2 = (char *) calloc(max, sizeof(char*)); 

    int i;
	int out_pos_1 = 0;
	int	out_pos_2 = 0;

    for(i = 0; i < x1; i++){
        out_string_1[out_pos_1] = 'R';
        out_string_2[out_pos_2] = 'r';
        out_pos_1++;
        out_pos_2++;
    }
    for(i = 0; i < x2; i++){
        out_string_2[out_pos_2] = 'R';
        out_pos_2++;
    }
    for(i = 0; i < y1; i++){
        out_string_1[out_pos_1] = 'w';
        out_pos_1++;
    }
    for(i = 0; i < y2; i++){
        out_string_2[out_pos_2] = 'w';
        out_pos_2++;
    }
    for(i = 0; i < z1; i++){
        if(out_string_2[out_pos_1] == 'R'){
            out_string_1[out_pos_1] = 'r';
            i--;
        }
        else
            out_string_1[out_pos_1] = 'R';
        out_pos_1++;
    }
    for(i = 0; i < z2; i++){
        if(out_string_1[out_pos_2] =='R' && out_pos_2 <= out_pos_1){
            out_string_2[out_pos_2]= 'r';
            i--;
        }
        else
            out_string_2[out_pos_2] = 'R';
        out_pos_2++;
    } 
    
    //printf("s1: %s\ns2: %s\n",out_string_1, out_string_2);
    strcpy(s1, out_string_1);
    strcpy(s2, out_string_2);
    free(out_string_1);
    free(out_string_2);
}

// First Come First Serve scheduling implemented with a state machine
void fcfs(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2) {
	int i;                                   /* next string position (time) */
	
	/* start with both ready */
	int state1 = E_TASK_READY;      
	int state2 = E_TASK_READY;
	int cpuLeft1 = x1;       /* P1 next CPU burst remaining */
	int cpuLeft2 = x2;       /* P2 next CPU burst remaining */
	int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
	int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */

	for (i=0; (state1 != E_TASK_DONE) || (state2 != E_TASK_DONE); i++) {
		/* running process completes its CPU burst */
		if((state1 == E_TASK_RUNNING) && (cpuLeft1== 0)) {
			// If process finishes all its cpu-related workload.
			// Let's check the io of process1
			if (ioLeft1 == 0) {
				state1 = E_TASK_DONE;
				s1[i] = stateChars[state1];            /* terminate the string */
			}
			else{
				state1 = E_TASK_WAITING;
			}
		}  
		else if ((state2 == E_TASK_RUNNING) && (cpuLeft2 == 0) ) {
			if (ioLeft2 == 0) {
				state2 = E_TASK_DONE;
				s2[i] = stateChars[state2];
			}
			else{
				state2 = E_TASK_WAITING;
			}
		}  
		
		/* handle IO complete */
		if ((state1 == E_TASK_WAITING) && (ioLeft1 == 0)) {
			state1 = E_TASK_READY;
			cpuLeft1 = z1;
		}  
		if ((state2 == E_TASK_WAITING) && (ioLeft2 == 0)) {
			state2 = E_TASK_READY;
			cpuLeft2 = z2;
		}  
		
		/* if both ready, depends on algorithm */
		if ( (state1 == E_TASK_READY) && (state2 == E_TASK_READY)) {
			state1 = E_TASK_RUNNING;
		}  
		
		/* handle one ready and CPU available */
		else if ( (state1 == E_TASK_READY) && (state2 != E_TASK_RUNNING)) {
			state1 = E_TASK_RUNNING;
		}  
		else if ( (state2 == E_TASK_READY) && (state1 != E_TASK_RUNNING)) {
			state2 = E_TASK_RUNNING;
		}  

		/* insert chars in string, but avoid putting in extra string terminators */
		if (state1 != E_TASK_DONE){
			s1[i] = stateChars[state1];
		}
		if (state2 != E_TASK_DONE){
			s2[i] = stateChars[state2];
		}
		
		/* decrement counts */
		if (state1 == E_TASK_RUNNING){
			cpuLeft1--;
		}
		if (state1 == E_TASK_WAITING){
			ioLeft1--;
		}
		if (state2 == E_TASK_RUNNING){
			cpuLeft2--;
		}
		if (state2 == E_TASK_WAITING){
			ioLeft2--;
		}
	}  /* end of main for loop */
}

// Shortest Job First scheduling implemented with a state machine
void sjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2){
	int i;
	int state1 = E_TASK_READY;      
	int state2 = E_TASK_READY;
	int cpuLeft1 = x1;       /* P1 next CPU burst remaining */
	int cpuLeft2 = x2;       /* P2 next CPU burst remaining */
	int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
	int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */
	
	
	for (i=0; (state1 != E_TASK_DONE) || (state2 != E_TASK_DONE); i++) {
		/* running process completes its CPU burst */
		if((state1 == E_TASK_RUNNING) && (cpuLeft1== 0)) {
			// If process finishes all its CPU-related workload.
			// Let's check the IO of process1
			if (ioLeft1 == 0) {
				state1 = E_TASK_DONE;
				s1[i] = stateChars[state1];            /* terminate the string */
			}
			else{
				state1 = E_TASK_WAITING;
			}
		}  
		else if ((state2 == E_TASK_RUNNING) && (cpuLeft2 == 0) ) {
			if (ioLeft2 == 0) {
				state2 = E_TASK_DONE;
				s2[i] = stateChars[state2];
			}
			else{
				state2 = E_TASK_WAITING;
			}
		}  
		
		/* handle IO complete */
		if ((state1 == E_TASK_WAITING) && (ioLeft1 == 0)) {
			state1 = E_TASK_READY;
			cpuLeft1 = z1;
		}  
		if ((state2 == E_TASK_WAITING) && (ioLeft2 == 0)) {
			state2 = E_TASK_READY;
			cpuLeft2 = z2;
		}  
		
		/* if both ready, depends on algorithm */
		if ( (state1 == E_TASK_READY) && (state2 == E_TASK_READY)) {
			if(cpuLeft1 <= cpuLeft2){
				state1 = E_TASK_RUNNING;
			}
			else{
				state2 = E_TASK_RUNNING;	
			}				
		}  
		
		/* handle one ready and CPU available */
		else if ( (state1 == E_TASK_READY) && (state2 != E_TASK_RUNNING)) {
			state1 = E_TASK_RUNNING;
		}  
		else if ( (state2 == E_TASK_READY) && (state1 != E_TASK_RUNNING)) {
			state2 = E_TASK_RUNNING;
		}  

		/* insert chars in string, but avoid putting in extra string terminators */
		if (state1 != E_TASK_DONE){
			s1[i] = stateChars[state1];
		}
		if (state2 != E_TASK_DONE){
			s2[i] = stateChars[state2];
		}
		
		/* decrement counts */
		if (state1 == E_TASK_RUNNING){
			cpuLeft1--;
		}
		if (state1 == E_TASK_WAITING){
			ioLeft1--;
		}
		if (state2 == E_TASK_RUNNING){
			cpuLeft2--;
		}
		if (state2 == E_TASK_WAITING){
			ioLeft2--;
		}
	}  /* end of main for loop */
}

// Preemptive Shortest Job First scheduling implemented with a state machine
void psjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2){
	int i;
	int state1 = E_TASK_READY;      
	int state2 = E_TASK_READY;
	int cpuLeft1 = x1;       /* P1 next CPU burst remaining */
	int cpuLeft2 = x2;       /* P2 next CPU burst remaining */
	int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
	int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */
	
	// same as sjf EXCEPT, if at any time the cpuLeft value of the process not running
	// is shorter than the current process' cpuLeft, we switch the processes
	for (i=0; (state1 != E_TASK_DONE) || (state2 != E_TASK_DONE); i++) {
		/* running process completes its CPU burst */
		if(state1 == E_TASK_RUNNING) {
			if(cpuLeft1== 0){
				// If process finishes all its CPU-related workload.
				// Let's check the IO of process1
				if (ioLeft1 == 0) {
					state1 = E_TASK_DONE;
					s1[i] = stateChars[state1];            /* terminate the string */
				}
				else{
					state1 = E_TASK_WAITING;
				}
			}
			
			if(cpuLeft1 > cpuLeft2){
				state1 = E_TASK_READY;
			}
		}  
		else if (state2 == E_TASK_RUNNING) {
			if(cpuLeft2 == 0){
				if (ioLeft2 == 0) {
					state2 = E_TASK_DONE;
					s2[i] = stateChars[state2];
				}
				else{
					state2 = E_TASK_WAITING;
				}
			}
			if(cpuLeft2 > cpuLeft1){
				state2 = E_TASK_READY;
			}
		}  
		
		/* handle IO complete */
		if ((state1 == E_TASK_WAITING) && (ioLeft1 == 0)) {
			state1 = E_TASK_READY;
			cpuLeft1 = z1;
		}  
		if ((state2 == E_TASK_WAITING) && (ioLeft2 == 0)) {
			state2 = E_TASK_READY;
			cpuLeft2 = z2;
		}  
		
		/* if both ready, depends on algorithm */
		if ( (state1 == E_TASK_READY) && (state2 == E_TASK_READY)) {
			if(cpuLeft1 <= cpuLeft2){
				state1 = E_TASK_RUNNING;
			}
			else{
				state2 = E_TASK_RUNNING;	
			}				
		}  
		
		/* handle one ready and CPU available */
		else if ( (state1 == E_TASK_READY) && (state2 != E_TASK_RUNNING)) {
			state1 = E_TASK_RUNNING;
		}  
		else if ( (state2 == E_TASK_READY) && (state1 != E_TASK_RUNNING)) {
			state2 = E_TASK_RUNNING;
		}  

		/* insert chars in string, but avoid putting in extra string terminators */
		if (state1 != E_TASK_DONE){
			//printf("value of i: %d\nvalue of state1: %d", i, state1);
			s1[i] = stateChars[state1];
		}
		if (state2 != E_TASK_DONE){
			//printf("value of i: %d\nvalue of state2: %d", i, state2);
			s2[i] = stateChars[state2];
		}
		
		/* decrement counts */
		if (state1 == E_TASK_RUNNING){
			cpuLeft1--;
		}
		if (state1 == E_TASK_WAITING){
			ioLeft1--;
		}
		if (state2 == E_TASK_RUNNING){
			cpuLeft2--;
		}
		if (state2 == E_TASK_WAITING){
			ioLeft2--;
		}
	}  /* end of main for loop */
}

// Round Robin scheduling implemented with a state machine
void rr(char *s1, char *s2, int quantum, int x1, int y1, int z1, int x2, int y2, int z2) {
	int i;                   /* next string position (time) */
	
	/* start with both ready */
	int state1 = E_TASK_READY;      
	int state2 = E_TASK_READY;
	int cpuLeft1 = x1;       /* P1 next CPU burst remaining */
	int cpuLeft2 = x2;       /* P2 next CPU burst remaining */
	int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
	int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */
	int qleft = quantum;               /* quantum remaining */
	int last = 0;			 /* The last process to exec*/

	for (i=0; (state1 != E_TASK_DONE) || (state2 != E_TASK_DONE); i++) {
		/* running process completes its CPU burst */
		if((state1 == E_TASK_RUNNING) && (cpuLeft1== 0)) {
			// If process finishes all its cpu-related workload.
			// Let's check the io of process1
			if (ioLeft1 == 0) {
				state1 = E_TASK_DONE;
				s1[i] = stateChars[state1];            /* terminate the string */
			}
			else{
				state1 = E_TASK_WAITING;
			}
		}  
		else if ((state2 == E_TASK_RUNNING) && (cpuLeft2 == 0) ) {
			if (ioLeft2 == 0) {
				state2 = E_TASK_DONE;
				s2[i] = stateChars[state2];
			}
			else{
				state2 = E_TASK_WAITING;
			}
		}  
		
		/* running process has quantum expire */
		if ((state1 == E_TASK_RUNNING) && (qleft == 0) ) {
			state1 = E_TASK_READY;
			last = 1;
			qleft = quantum;
		}  
		if ((state2 == E_TASK_RUNNING) && (qleft == 0) ) {
			last = 2;
			state2 = E_TASK_READY;
			qleft = quantum;
		}  
		
		/* handle IO complete */
		if ((state1 == E_TASK_WAITING) && (ioLeft1 == 0)) {
			state1 = E_TASK_READY;
			cpuLeft1 = z1;
		}  
		if ((state2 == E_TASK_WAITING) && (ioLeft2 == 0)) {
			state2 = E_TASK_READY;
			cpuLeft2 = z2;
		}  
		
		/* if both ready, depends on algorithm */
		if ( (state1 == E_TASK_READY) && (state2 == E_TASK_READY)) {
			if(last == 1){
				state2 = E_TASK_RUNNING;
			}
			else if (last == 2){
				state1 = E_TASK_RUNNING;
			}
			else{
				state1 = E_TASK_RUNNING;
			}
		}  
		
		/* handle one ready and CPU available */
		else if ( (state1 == E_TASK_READY) && (state2 != E_TASK_RUNNING)) {
			state1 = E_TASK_RUNNING;
			qleft = quantum;
		}  
		else if ( (state2 == E_TASK_READY) && (state1 != E_TASK_RUNNING)) {
			state2 = E_TASK_RUNNING;
			qleft = quantum;
		}  

		/* insert chars in string, but avoid putting in extra string terminators */
		if (state1 != E_TASK_DONE){
			s1[i] = stateChars[state1];
		}
		if (state2 != E_TASK_DONE){
			s2[i] = stateChars[state2];
		}
		
		/* decrement counts */
		qleft--;   /* OK to decrement even if nothing running */
		if (state1 == E_TASK_RUNNING){
			cpuLeft1--;
		}
		if (state1 == E_TASK_WAITING){
			ioLeft1--;
		}
		if (state2 == E_TASK_RUNNING){
			cpuLeft2--;
		}
		if (state2 == E_TASK_WAITING){
			ioLeft2--;
		}
	}  /* end of main for loop */
}

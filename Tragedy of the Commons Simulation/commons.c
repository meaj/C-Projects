/* Commons
 * 		This program is a simulation of the Tragedy of the Commons metaphor
 * 		It uses a linked list to create a simulated population and track various qualities of each member of the sim-pop
 * 		It's purpose is to illustrate the danger of unchecked greed
 * 		By Kevin Moore
 * */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linkedList.h"
//These color codes do not appear to function on windows systems(at least those using MinGW) and 
//instead output their values defined here. Tested on linux systems (and with cygiwn on windows)
//and color works as expected.
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_RESET   "\x1b[0m"

//Linked List Code
//All functions should be understood by name

int length(node *head)
{
	if (head == NULL)
		return 0;
	else
		return 1 + length(head->next);
}

node *insertEnd(node *head, int type)
{	
	node *t1;
	node *t2=head;
	
	t1 = (node*)malloc(sizeof(node));
	t1->ident = 24000 + length(head);
	t1->hapy = 0;
	t1->type = type;
	t1->lBhv = 0;
	t1->next = NULL;
	
	if(head == NULL)
		return t1;
	else if(head->next == NULL)
	{
		head->next=t1;
		return head;
	}
	while(head->next != NULL)
		head = head->next;
	head->next = t1;
	return t2;
}

void printNodeVals(node* head)
{
		if(head->type==0)
			printf("No:"ANSI_RED" %i\t"ANSI_RESET,head->ident);
		else if(head->type==1)
			printf("No:"ANSI_GREEN" %i\t"ANSI_RESET,head->ident);
		else
			printf("No:"ANSI_YELLOW" %i\t"ANSI_RESET,head->ident);
		
		if(head->lBhv==0)
			printf("Behavior:"ANSI_RED" Selfish\t"ANSI_RESET);
		else if(head->lBhv>0)
			printf("Behavior:"ANSI_GREEN" Altruistic\t"ANSI_RESET);
		
		if(head->hapy<0)
			printf("Happiness:"ANSI_RED" %i\n"ANSI_RESET, head->hapy);
		else if(head->hapy>0)
			printf("Happiness:"ANSI_GREEN" %i\n"ANSI_RESET, head->hapy);
		else
			printf("Happiness:"ANSI_YELLOW" %i\n"ANSI_RESET, head->hapy);
}

void printList(node * tmp)
{
	if(tmp==NULL)
		printf("\n");
	else
	{
		printNodeVals(tmp);
		printList(tmp->next);
	}
}

void deleteList(node *temp)
{
	if (temp->next==NULL)
	{
		free(temp);
		temp = NULL;
	}
	else
	{
		deleteList(temp->next);
		temp->next = NULL;
		free(temp);
	}
}

//Simulation Code
//Advances node Happiness and population counters
node* happyAdj(node* head, int *pS, int *pA)
{
	int r;
	if(head->type ==0)
	{
		head->lBhv = 0;
		head->hapy += 5;
		*pS = *pS+1;
	}
	else if(head->type ==1)
	{
		head->lBhv = 1;
		head->hapy -=2;
		*pA = *pA+1;
	}	
	else if(head->type == 2)
	{
		r=rand()%2;
		if(r==0)
		{
			head->lBhv = 0;
			head->hapy += 5;
			*pS = *pS+1;
		}
		else if(r==1)
		{		
			head->lBhv = 1;
			head->hapy -=2;
			*pA = *pA+1;
		}
	}
	return head;	
}
//Runs the Simulation
node *simHappy(node *head, int *pS, int *pA)
{
	//Base Case: End of linked list, return NULL
	//Recursive Case: Advance current node, check next node, return node
	if(head==NULL)
		return;
	else
	{
		head = happyAdj(head, pS, pA);
		head->next = simHappy(head->next, pS, pA);
		return head;
	}
}

//Adjusts scores based on how population behaved
node *govAdj(node* head, int type)
{
	int r;
	head->hapy+=type;
	if(type>0)
	{
		if(head->lBhv ==0)
		{
			r=rand()%2;
			if(r==0)
			{
				head->hapy-=10;
				printf(ANSI_RED"No:%i Minor penalty for being selfish(-10)\n"ANSI_RESET,head->ident);
			}
		}
		else if(head->lBhv ==1)
		{
			r=rand()%10;
			if(r==0)
			{
				head->hapy+=10;
				printf(ANSI_GREEN"No:%i Minor bonus for being altruistic(+10)\n"ANSI_RESET,head->ident);
			}
		}
	}
	else if(type<0)
	{
		if(head->lBhv ==0)
		{
			r=rand()%10;
			if(r==0)
			{			
				head->hapy-=50;	
				printf(ANSI_RED"No:%i Major penalty for being selfish(-50)\n"ANSI_RESET,head->ident);
			}			
		}
		else if(head->lBhv ==1)
		{
			r=rand()%100;
			if(r==0)
			{
				head->hapy+=50;
				printf(ANSI_GREEN"No:%i Major bonus for being altruistic(+50)\n"ANSI_RESET,head->ident);
			}
		}
	}
	return head;
}
//Government simulation
node *govSim(node *head, int type)
{
	//Base Case: End of linked list, return NULL
	//Recursive Case: Advance current node, check next node, return node
	if(head==NULL)
		return;
	else
	{
		head = govAdj(head, type);
		head->next = govSim(head->next, type);
		return head;
	}
}

int main(int argc, char *argv[])
{
	//Variable declarations
	node *head=NULL;
	time_t t;
	int nSelf=0, nAlt=0, nSoA=0, gTotal=0;
	int pS, pA, nPop, nRnds,gScore, i, r;
	char c;
	srand((unsigned)time(&t));
	
	//Introduction and population creation
	printf("\e[1;1H\e[2J");
	printf("Welcome user, to our rendition of The Tragedy of the Commons.\nWould you like to set the population type manually?\nY/N: ");
	scanf("%c", &c);
	switch(c)
	{
		case 'N':
		case 'n':
			printf("How large of a cast would you like to see?\nPopulation: ");
			scanf("%d", &nPop);
			for(i = 0; i < nPop; i++)
			{
				r=rand()%3;
				if (r==0)
					nSelf++;
				else if(r==1)
					nAlt++;
				else if(r==2)
					nSoA++;
				head = insertEnd(head, r);
			}
			printf("There are "ANSI_RED"%i Selfish"ANSI_RESET" members.\n",nSelf);
			printf("There are "ANSI_GREEN"%i Altruistic"ANSI_RESET" members.\n",nAlt);
			printf("There are "ANSI_YELLOW"%i Variable"ANSI_RESET" members.\n",nSoA);
			break;
		case 'Y':
		case 'y':
			printf("Enter the size of the "ANSI_RED"Selfish Populaion: "ANSI_RESET);
			scanf("%d", &nSelf);
			printf("Enter the size of the "ANSI_GREEN"Altruistic Populaion: "ANSI_RESET);
			scanf("%d", &nAlt);
			printf("Enter the size of the "ANSI_YELLOW"Variable Populaion: "ANSI_RESET);
			scanf("%d", &nSoA);
			nPop = nSelf+nAlt+nSoA;
			for(i = 0; i < nSelf; i++)
			{
				head = insertEnd(head, 0);
			}
			for(i = 0; i < nSoA; i++)
			{
				head = insertEnd(head, 2);
			}
			for(i = 0; i < nAlt; i++)
			{
				head = insertEnd(head, 1);
			}
			break;
		default:
			printf(ANSI_RED"Invalid selection, setting a cast of 5.\n"ANSI_RESET);
			for(i = 0; i < 5; i++)
			{
				head = insertEnd(head, (rand()%2));
			}
			break;
	}
	printf("The Population:\n");
	printList(head);
	
	//Round Simulations
	printf("Enter the number of rounds to test: ");
	scanf("%d", &nRnds);

	for(i=0; i<nRnds; i++)
	{
		pS=0;
		pA=0;
		gScore=0;
		head = simHappy(head, &pS, &pA);
		printf("\nRound %i:\n", i+1);
		if(pS>pA)
			gScore = (pS-pA)*-1;
		else if(pS<pA)
			gScore = pA - pS;
		gTotal+=gScore;
		printf(ANSI_RED"The Selfish Population: %i"ANSI_GREEN"\tThe Altruistic Population: %i\n"ANSI_RESET,pS,pA);
		head = govSim(head,gScore);
		printf("Government score: %i\t",gScore);
		printf("Individual scores:\n");
		printList(head);
	}
	//Final output
	printf("\nThe Total Government Score: %i\t The Total Individual Scores:\n",gTotal);
	printList(head);
	//End of simulation.
	//Deletion of LL
	deleteList(head);
	printf("Press any key to exit.\n");
	scanf("%c", &c);
	printf("Do come again, user.\n");
	return 0;
}

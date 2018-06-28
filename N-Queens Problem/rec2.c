/* NQueens problem:
 *		Provides solutions to the NQueens problem with any input greater than 0.
 *		Outputs a visual display of the game board to illustrate the found solution.
 *		Provides one solution per input.
 *		By Kevin Moore
 */

#include <stdio.h>
#include <stdlib.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_BCKGRND_WHT   "\x1b[47m"
#define ANSI_BCKGRND_BLK   "\x1b[40m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Allocates memory for a board of size n
// Returns an indirect pointer to the board
int **nBoard(int n)
{
    int i,j;
    int **arr = (int **)malloc(n * sizeof(int *));
    for (i=0; i<n; i++)
        arr[i] = (int *)malloc(n * sizeof(int));

    return arr;
}

// Displays the board
void pBoard(int** arr, int n)
{
    int i,j, Q;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {

            if(arr[i][j]==1)
            {
                if(i%2==j%2)
                    printf(ANSI_BCKGRND_BLK ANSI_COLOR_WHITE"Q"ANSI_COLOR_RESET);
                else
                    printf(ANSI_BCKGRND_WHT ANSI_COLOR_BLACK"Q"ANSI_COLOR_RESET)	;

            }
            else
            {
                if(i%2==j%2)
                    printf(ANSI_BCKGRND_BLK" "ANSI_COLOR_RESET);
                else
                    printf(ANSI_BCKGRND_WHT" "ANSI_COLOR_RESET)	;
            }
        }
        printf("\n");
    }
}

// Checks if location on the 'board' is a valid location for a queen
// Returns 0 if successful or 1 if failed
int ckPlace(int** arr, int row, int col, int n)
{
    int i, j;
    for(i=0;i<col;i++)
    {
        if(arr[row][i]==1)
            return 0;
    }
    for(i=row, j=col; i>=0 && j>=0;i--,j--)
    {
        if(arr[i][j]==1)
            return 0;
    }
    for(i=row, j=col; i<n && j>=0; i++, j--)
    {
        if(arr[i][j]==1)
            return 0;
    }
    return 1;
}

// Checks the placement of queens on the board to see if there is a solution or not
// Returns a solution or null
int** ckBoard(int** board, int Q, int n)
{
    int i;
    int **tst;
    //tst = nBoard(n);
    tst = board;

    if(Q==n)
        return board;

    for(i=0; i<n; i++)
    {
        if(ckPlace(tst, i, Q,n)==1)
        {
            tst[i][Q]=1;
            if(ckBoard(tst, Q+1, n)!=NULL)
            {
                return tst;
            }
            tst[i][Q]=0;
        }
    }
    return NULL;
}

// Main function
//      Runs loop that generates and test boards
//      Frees allocated memory
int main(int argc, char *argv[])
{
    int n, tst;
    int **board;

    // Loops through boards of size n until user enters 0
    printf("Enter the size of the NxN board: ");
    scanf("%i",&n);
    while(n!=0)
    {
        // Warning regarding very large boards
        if(n>25)
        {
            printf(ANSI_COLOR_RED "!!WARNING!!"ANSI_COLOR_RESET " Testing a board larger than 29 has not returned a result(1847:4/2/16).\nEnter %i again to proceed, or maybe try a size smaller than 25: ",n);
            scanf("%i",&n);
        }

        board = nBoard(n);
        board = ckBoard(board, 0, n);
        // If a solution exists, display the board, otherwise say no solution exists
        if(board!=NULL)
            pBoard(board,n);
        else
            printf(ANSI_COLOR_RED"NO SOLUTION\n"ANSI_COLOR_RESET);

        printf("To try again enter the size of the next NxN board.\nTo exit enter 0. Your choice: ");
        scanf("%i",&n);
    }

    // Free allocated memory
    free(board);
    return 0;
}

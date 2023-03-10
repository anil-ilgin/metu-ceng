
/*board has 9 cells. We will assume that the cell numbering is as follows:
|
|   c0 | c1 | c2
|   ------------
|   c3 | c4 | c5
|   ------------
|   c6 | c7 | c8
|
****************************************************************************************************************************************************/




/* Includes ****************************************************************************************************************************************/

#include "Utilities.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>   /* For standard input/output functions */

/***************************************************************************************************************************************************/




/* Definitions *************************************************************************************************************************************/

#define AUTHOR  ( "Anıl Utku Ilgın" )  /* Author's name                                   */

#define EMPTY   ( ' '             )  /* Label for empty cells                           */
#define PLAYER1 ( 'X'             )  /* Label for player 1 and cells marked by player 1 */
#define PLAYER2 ( 'O'             )  /* Label for player 2 and cells marked by player 2 */

/***************************************************************************************************************************************************/




/* Type Definitions ********************************************************************************************************************************/

typedef unsigned char boolean ;

/***************************************************************************************************************************************************/




/* Global Variables ********************************************************************************************************************************/

char c0 , c1 , c2 , c3 , c4 , c5 , c6 , c7 , c8;  /* Cells of the Tic-Tac-Toe board */
/***************************************************************************************************************************************************/




/* Functions ***************************************************************************************************************************************/

/* Initializes the global variables. */

void initializeGlobalVariables ( void )
{
  c0 = EMPTY, c1 = EMPTY, c2 = EMPTY, c3 = EMPTY, 
  c4 = EMPTY, c5 = EMPTY, c6 = EMPTY, c7 = EMPTY, c8 = EMPTY;
}

/*=================================================================================================================================================*/

/* Prints a banner including the author name. */

void printBanner ( void )
{
  printf(                                                                                   "\n"          ) ;
  printf( "*******************************************************************************" "\n"          ) ;
  printf( "* Tic-Tac-Toe                                                                 *" "\n"          ) ;
  printf( "* by %-72s *"                                                                    "\n" , AUTHOR ) ;
  printf( "*******************************************************************************" "\n"          ) ;
}

/*=================================================================================================================================================*/

/* Prints the current board configuration. */

void printBoard ( void )
{
  
  printf( "                                     \n");
  printf( "                                     \n");
  printf( "    Cells                  Board     \n");
  printf( ".-----------.          .-----------.\n");
  printf( "| 0 | 1 | 2 |          | %c | %c | %c |\n", c0, c1, c2 );
  printf( "| 3 | 4 | 5 |          | %c | %c | %c |\n", c3, c4, c5 );
  printf( "| 6 | 7 | 8 |          | %c | %c | %c |\n", c6, c7, c8 );
  printf( ".-----------.          .-----------.\n ");
}

/*=================================================================================================================================================*/

/* Gets the next move of "player", and marks it on the board.                     */
/* Loops (and asks again) if the input is not valid, or if the cell is not empty. */
void getNextMove ( char player )
{
  int k;
  int p = 0;
  do {
    printf("\n");
    printf("   Player %c, enter your next move (0-8): ", player );
    scanf("%d", &k);
  	switch (k) {        
        case 0: 
			    if  ( c0 == EMPTY ) {
                c0 = player;
		            p = 1;
                clearInputBuffer();
                break;		
        }
        	else                {
        		printf("   Cell is not empty! Please try again.\n");
            clearInputBuffer();
        		break;
  		}
        case 1: 
			    if  ( c1 == EMPTY ) {
                c1 = player;
                p = 1;
                clearInputBuffer();
                break;
        }
        	else                {
        		printf("   Cell is not empty! Please try again.\n");
            clearInputBuffer();
        		break;
  		}
        	
        case 2: 
			    if  ( c2 == EMPTY ) {
                c2 = player;
                p = 1;
                clearInputBuffer();
                break;
        }
        	else                {
        		printf("   Cell is not empty! Please try again.\n");
            clearInputBuffer();
        		break;
  		}
        case 3: 
			    if  ( c3 == EMPTY ) {
                c3 = player;
                p = 1;
                clearInputBuffer();
                break;
        }
        	else                {
        		printf("   Cell is not empty! Please try again.\n");
            clearInputBuffer();
        		break;
  		}
		    case 4: 
			    if  ( c4 == EMPTY ) {
                c4 = player;
                p = 1;
                clearInputBuffer();
                break;
        }
        	else                {
        		printf("   Cell is not empty! Please try again.\n");
            clearInputBuffer();
        		break;
  		}
        case 5: 
			    if  ( c5 == EMPTY ) {
                c5 = player;
                p = 1;
                clearInputBuffer();
                break;
        }
        	else                {
        		printf("   Cell is not empty! Please try again.\n");
            clearInputBuffer();
        		break;
  		}
        case 6: 
			    if  ( c6 == EMPTY ) {
                c6 = player;
                p = 1;
                clearInputBuffer();
                break;
        }
        	else                {
        		printf("   Cell is not empty! Please try again.\n");
            clearInputBuffer();
        		break;
  		}
        case 7: 
			    if  ( c7 == EMPTY ) {
                c7 = player;
                p = 1;
                clearInputBuffer();
                break;
        }
        	else                {
        		printf("   Cell is not empty! Please try again.\n");
            clearInputBuffer();
        		break;
  		}
        case 8: 
			    if  ( c8 == EMPTY ) {
                c8 = player;
                p = 1;
                clearInputBuffer();
                break;
        }
        	else              {
        		printf("   Cell is not empty! Please try again.\n");
            clearInputBuffer();
        		break;
  		}
      
        default : 
        		printf("   Incorrect input! Please try again.\n");
            clearInputBuffer();
      } } while (p == 0);
}


/*=================================================================================================================================================*/

/* Checks if one of the players wins the game, or if the board is full and the game ended with a draw. Prints how the game ended. */
/* Returns TRUE if the game ended, otherwise, does not print anything and just returns FALSE.                                     */

boolean checkAndPrintGameResult ( void )
{

  
   if ( (c0 == c1 && c1 == c2 && c0 != EMPTY) ) {
  	printf("Player %c WINS (top row)!\n \n", c0);
  	return TRUE;
  } 
  else if ( (c3 == c4 && c4 == c5 && c3 != EMPTY) ) {
  	printf(" Player %c WINS (middle row)!\n \n", c3 );
  	return TRUE;
  }
  else if ( (c6 == c7 && c7 == c8 && c6 != EMPTY) ) {
  	printf(" Player %c WINS (bottom row)!\n \n", c6);
  	return TRUE;
  }
  else if ( (c0 == c3 && c3 == c6 && c3 != EMPTY) ) {
  	printf(" Player %c WINS (left column)!\n \n", c0);
  	return TRUE;
  }
  else if ( (c1 == c4 && c4 == c7 && c1 != EMPTY) ) {
  	printf(" Player %c WINS (middle column)!\n \n", c1);
  	return TRUE;
  }
  else if ( (c2 == c5 && c5 == c8 && c2 != EMPTY) ) {
  	printf(" Player %c WINS (right column)! \n \n", c2);
  	return TRUE;
  }
  else if ( (c0 == c4 && c4 == c8 && c0 != EMPTY) ) {
  	printf(" Player %c WINS (left diagonal)!\n \n", c0);
  	return TRUE;
  }
  else if ( (c2 == c4 && c4 == c6 && c2 != EMPTY) ) {
  	printf(" Player %c WINS (right diagonal)!\n \n ", c2);
  	return TRUE;
  }
  else if ( c0 != EMPTY && c1 != EMPTY && c2 != EMPTY && c3 != EMPTY && 
            c4 != EMPTY && c5 != EMPTY && c6 != EMPTY && c7 != EMPTY && 
			c8 != EMPTY ) {
			printf("DRAW!\n \n");
			return TRUE;
			}
	else { 
		return FALSE;
	}
}

/***************************************************************************************************************************************************/




/* Main Function ***********************************************************************************************************************************/

/* Initializes the global variables.                                                                                                          */
/* Prints banner.                                                                                                                             */
/* Randomly chooses one of the players to start first.                                                                                        */
/* Asks each player to make a move in turn until the game finishes (prints the board configuration each time before a player makes her move). */
/* Prints the game result.                                                                                                                    */
/* Prints a goodbye message.                                                                                                                  */
/* Prints the number of random numbers that were generated during the game play.                                                              */

int main ( void )
{
  char currentPlayer = ( ( randomIntegerMinMaxUnBiased( 0 , 1 ) == 0 ) ? PLAYER1 : PLAYER2 ) ;

  initializeGlobalVariables() ;
  printBanner              () ;
  printBoard               () ;

  do
  {
    getNextMove( currentPlayer ) ;
    printBoard (               ) ;

    if   ( currentPlayer == PLAYER1 )  { currentPlayer = PLAYER2 ; }
    else                               { currentPlayer = PLAYER1 ; }
  }
  while ( checkAndPrintGameResult() == FALSE ) ;

  printf( "\nGood game. Have a nice day!\n" ) ;

  printf( "\n%d random numbers were generated during the game play.\n\n" , numberOfRandomNumbersGenerated ) ;

  return 0 ;
}

/***************************************************************************************************************************************************/

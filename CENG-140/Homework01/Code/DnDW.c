/* Explanations *************************************************************************************************************************************
|
| - The dungeon consists of a "start" room, an "exit" room, one room with "gold", one or more rooms with "wumpuses", and zero or more empty rooms.
| - "Start" and "exit" rooms are empty.
| - Other rooms can be empty, or can contain only a single entity (either gold, or wumpus). A room cannot contain more than one entities.
| - At the beginning of the game, the player sets the total number of rooms, and also the number of wumpuses.
| - Player starts in room "start".
| - All rooms are magical, and they all have 2 doors.
| - Doors might appear randomly in North, East, South or West directions.
| - Doors lead to randomly selected other rooms.
| - If the player visits the room with the gold, the player grabs the gold.
| - If the player visits a room with a wumpus in it, the wumpus eats the player, and the game finishes.
| - If the player visits the "exit" room, the game finishes. If the player has the gold with her, this is a successful finish.
|
****************************************************************************************************************************************************/




/* Includes ****************************************************************************************************************************************/

#include "Utilities.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>   /* For standard input/output functions */

/***************************************************************************************************************************************************/




/* Definitions *************************************************************************************************************************************/

#define AUTHOR       ( "Anıl Utku Ilgın" )  /* Author's name                                                                   */

#define MIN_ROOMS    (  2              )  /* Minimum number of magical rooms in the dungeon (excluding start and exit rooms) */
#define MAX_ROOMS    (  8              )  /* Maximum number of magical rooms in the dungeon (excluding start and exit rooms) */
#define MIN_WUMPUSES (  1              )  /* Minimum number of wumpuses in the dungeon                                       */
#define MAX_WUMPUSES (  4              )  /* Maximum number of wumpuses in the dungeon                                       */

#define START        ( 'S'             )  /* Label for the start room                                                        */
#define EXIT         ( 'X'             )  /* Label for the exit room                                                         */
#define GOLD         ( 'G'             )  /* Label for the single room containing the gold                                   */
#define WUMPUS       ( 'W'             )  /* Label for room(s) containing a wumpus                                           */
#define EMPTY        ( 'E'             )  /* Label for empty room(s)                                                         */

/***************************************************************************************************************************************************/




/* Type Definitions ********************************************************************************************************************************/

typedef unsigned char boolean ;

/***************************************************************************************************************************************************/




/* Global Variables ********************************************************************************************************************************/

int     numberOfMagicalRooms                            ;
int     numberOfWumpuses                                ;
int     currentRoomNo                                   ;
boolean playerFoundGold                                 ;
char    r0 , r1 , r2 , r3 , r4 , r5 , r6 , r7 , r8 , r9 ; /* r0 is start room, r1 is exit room, others are magical rooms (at most 8 magical rooms) */

/***************************************************************************************************************************************************/




/* Helper (Minor) Functions ************************************************************************************************************************/

/* The input is the room number, and the return value is the label for the room. */

char getRoomInformation ( int roomNo )
{
  switch ( roomNo )
  {
    default :
    case 0  : return r0 ;
    case 1  : return r1 ;
    case 2  : return r2 ;
    case 3  : return r3 ;
    case 4  : return r4 ;
    case 5  : return r5 ;
    case 6  : return r6 ;
    case 7  : return r7 ;
    case 8  : return r8 ;
    case 9  : return r9 ;
  }
}

/*=================================================================================================================================================*/

/* The input is the room label. The function prints out the information associated with the label. */

void printRoomInformation1 ( char room )
{
  switch ( room )
  {
    case START  : printf( "START"  ) ;  break ;
    case EXIT   : printf( "EXIT"   ) ;  break ;
    case GOLD   : printf( "GOLD"   ) ;  break ;
    case WUMPUS : printf( "WUMPUS" ) ;  break ;
    case EMPTY  : printf( "EMPTY"  ) ;  break ;
  }
}

/*=================================================================================================================================================*/

/* The input is the room number. The function prints out the information associated with the room. */

void printRoomInformation2 ( int roomNo )
{
  switch ( roomNo )
  {
    case 0 : printRoomInformation1( r0 ) ;  break ;
    case 1 : printRoomInformation1( r1 ) ;  break ;
    case 2 : printRoomInformation1( r2 ) ;  break ;
    case 3 : printRoomInformation1( r3 ) ;  break ;
    case 4 : printRoomInformation1( r4 ) ;  break ;
    case 5 : printRoomInformation1( r5 ) ;  break ;
    case 6 : printRoomInformation1( r6 ) ;  break ;
    case 7 : printRoomInformation1( r7 ) ;  break ;
    case 8 : printRoomInformation1( r8 ) ;  break ;
    case 9 : printRoomInformation1( r9 ) ;  break ;
  }
}

/*=================================================================================================================================================*/

/* The input is the direction. The function prints out the direction as a word, as an acronym and also displays an arrowhead. */

void printDirection ( int direction )
{
  switch ( direction )
  {
    case 0 : printf( "NORTH (N) ^" ) ;  break ;
    case 1 : printf( "EAST (E) >"  ) ;  break ;
    case 2 : printf( "SOUTH (S) v" ) ;  break ;
    case 3 : printf( "WEST (W) <"  ) ;  break ;
  }
}

/***************************************************************************************************************************************************/




/* Functions ***************************************************************************************************************************************/

/* Prints a banner including the author name. */

void printBanner ( void )
{

  printf(                                                                                   "\n"          ) ;
  printf( "*******************************************************************************" "\n"          ) ;
  printf( "* Dungeons and Dragons with Wumpuses                                          *" "\n"          ) ;
  printf( "* by %-72s *"                                                                    "\n" , AUTHOR ) ;
  printf( "*******************************************************************************" "\n"          ) ;
  printf( "Welcome to Dungeons and Dragons with Wumpuses Game! In this game, your very purpose is to find the gold in the dungeon and escape from it before getting eaten by ferocious Wumpus. But beware, an evil wizard has cursed the dungeons doors with a broken teleportation spell and now every door is opening to a random door in dungeon. Good luck with Wumpus, don't get eaten, and have fun! \n");
  printf( "\n");
}

/*=================================================================================================================================================*/

/* Initializes all global variables.                                                                                    */
/* The number of magical rooms and the number of wumpuses are set by the player as part of the configuration process.   */
/* All possible errors in input should be handled by clearing the input buffer and asking the user again for the input. */

void configureWorld ( void )
{

  int i;
  r4 = EMPTY, r5 = EMPTY, r6 = EMPTY, r7 = EMPTY, r8 = EMPTY, r9 = EMPTY;
  printf("Let's configure the D&D World First!\n");
  printf("Please enter the number of magical rooms [2,8] : ");
  scanf(" %d", &numberOfMagicalRooms);
  while ( numberOfMagicalRooms > 8 || numberOfMagicalRooms < 2) {
	clearInputBuffer();
	printf(" Incorrect input! Please try again.\n");
	printf("Please enter the number of magical rooms [2,8] : ");
	scanf(" %d", &numberOfMagicalRooms); }

  printf("Please enter the number of Wumpuses [1,4]  : ");
  scanf(" %d", &numberOfWumpuses);
  while ( numberOfMagicalRooms -1 < numberOfWumpuses ) {
	clearInputBuffer();
	printf(" Incorrect input! Please try again.\n");
        printf("Please enter the number of Wumpuses [1,4] : ");
	scanf(" %d", &numberOfWumpuses); }
  currentRoomNo = 0;
  r0 = START;
  r1 = EXIT;
  r2 = GOLD;
  for (i=0; i < numberOfWumpuses; ++i) {
	switch(i) {
		case 0: r3 = WUMPUS; break;
		case 1: r4 = WUMPUS; break;
		case 2: r5 = WUMPUS; break;
		case 3: r6 = WUMPUS; break;
        }
  }
  printf("Configuration is finished. Good luck with your game play!\n");
} 

/*=================================================================================================================================================*/

/* Prints the configured world. */

void printWorld ( void )
{

  int i;
  printf("\n");
  printf("DnD World:\n");
  printf("----------\n");
  for (i=0; i <= numberOfMagicalRooms + 1; i++) {
  	printf("Room %d : ", i);
	printRoomInformation2(i);
	printf("\n"); 
  }
  printf("\n");
  
  
}

/*=================================================================================================================================================*/

/* Prints the current room information and the doors of the current room.                                               */
/* Gets player's door choice as one of 'N', 'E', 'S', 'W' (small letters should also be accepted).                      */
/* All possible errors in input should be handled by clearing the input buffer and asking the user again for the input. */
/* Prints where the doors were actually opening to.                                                                     */
/* Updates current room number.                                                                                         */
/* If the current room contains gold, lets the player know that she now has gold in her posession.                      */

void getNextMove ( void )
{

  int k,l,m=0, rx, ry;
  char d, dk, dl, dK, dL;
  printf("You are in Room %d \n", currentRoomNo);
  printf("Room information : ");
  printRoomInformation2(currentRoomNo);
  printf("\n");
  printf("There is a door to your ");
  k = randomIntegerMinMaxUnBiased(0,3);
  printDirection(k);
  printf("\n");
  printf("And, there is another door to your ");
  l = randomIntegerMinMaxUnBiasedExcept1 (0,3,k);
  printDirection(l);
  printf("\n");
  
  if ( k == 0 ) { dk = 'N'; dK = 'n';}
  else if ( k == 1 ) { dk = 'E'; dK = 'e';}
  else if ( k == 2 ) { dk = 'S'; dK = 's';}   
  else if ( k == 3 ) { dk = 'W'; dK = 'w';}
  if ( l == 0 ) { dl = 'N'; dL = 'n';}
  else if ( l == 1 ) { dl = 'E'; dL = 'e';}
  else if ( l == 2 ) { dl = 'S'; dL = 's';}
  else if ( l == 3 ) { dl = 'W'; dL = 'w';}
  rx = randomIntegerMinMaxUnBiasedExcept1(0,numberOfMagicalRooms,currentRoomNo);
  ry = randomIntegerMinMaxUnBiasedExcept2(0,numberOfMagicalRooms,currentRoomNo,rx);
  printf("Which one will you choose? : ");
  scanf(" %c", &d);
  while ( m < 1 ) {   
	if ( d == dk || d == dK ) {
		currentRoomNo = rx;
		m += 1;
	}
	else if ( d == dl  || d == dL ) {
		currentRoomNo = ry;
		m += 1;
	}
	else {
		clearInputBuffer(); 
		printf("Incorrect input! Please try again : ");
		scanf(" %c", &d);
	}
 }
 printf("Room to your ");
 printDirection(k);
 printf(" was Room %d", rx);
 printf("\n");
 printf("Room to your ");
 printDirection(l);
 printf(" was Room %d", ry);
 printf("\n");
 if ( currentRoomNo == 2) {
	printf("You have GOLD now!\n");
	printf("\n");
	playerFoundGold = TRUE; 
 }
} 

/*=================================================================================================================================================*/

/* Checks if the current room that the player is in is the exit room.                                           */
/* If so, checks if the player already has the gold or not, and prints out an appropriate message in each case. */
/* Also checks if the player is in a room with a wumpus.                                                        */
/* If so, informs the player about the unhappy ending of the game.                                              */
/* Returns TRUE if the game ended, otherwise, does not print anything and just returns FALSE.                   */

boolean checkAndPrintGameResult ( void )
{

  if ( getRoomInformation(currentRoomNo) == EXIT) {
  	if (playerFoundGold == TRUE) {
		printf("You are safely out of the dungeon with the GOLD! Congratulations!\n");
		return TRUE;
	}
	else {
		printf("You are safely out of the dungeon, but you did not find the GOLD! Better luck next time!\n");
		return TRUE;
	}
  }
  else if ( getRoomInformation(currentRoomNo) == WUMPUS) {
		printf("Wumpus is full, and you are in heaven. What a pity!\n");
		return TRUE;
  }
  else { 
		return FALSE;
  }  
}

/***************************************************************************************************************************************************/




/* Main Function ***********************************************************************************************************************************/

/* Prints banner.                                                                */
/* Configures the dungeon world.                                                 */
/* Prints the configured world.                                                  */
/* Asks the player to make a move until the game finishes .                      */
/* Prints the game result.                                                       */
/* Prints a goodbye message.                                                     */
/* Prints the number of random numbers that were generated during the game play. */

int main ( void )
{

  printBanner();
  configureWorld();
  printWorld();
  do {
  getNextMove(); }
  while (checkAndPrintGameResult() == FALSE);
  printf( "\nGood game. Have a nice day!\n" );
  printf( "\n%d random numbers were generated during the game play.\n\n" , numberOfRandomNumbersGenerated );
  return 0;
}

/***************************************************************************************************************************************************/

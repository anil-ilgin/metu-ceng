/* Explanations *************************************************************************************************************************************
|
| This file implements CengBook, which is a realistic simulation of a mini social-media application.
|
****************************************************************************************************************************************************/




/* Includes ****************************************************************************************************************************************/

#include "Utilities.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************************************************************************************************/




/* Macro Definitions *******************************************************************************************************************************/

#define AUTHOR ( "Anıl Utku Ilgın" )  /* Author's name */
/***************************************************************************************************************************************************/




/* Global Variable Definitions *********************************************************************************************************************/

BookPtr bookPtr ;

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

#if   defined(_WIN16) || defined(_WIN32) || defined(_WIN64)

  char operatingSystem[] = "Windows" ;  /* This is a Windows environment.                            */

#elif defined(__linux__)

  char operatingSystem[] = "Linux"   ;  /* This is a Linux environment (any GNU/Linux distribution). */

#elif defined(__APPLE__)

  char operatingSystem[] = "MacOSX"  ;  /* This is a Mac OS X environment.                           */

#endif

/***************************************************************************************************************************************************/




/* Functions ***************************************************************************************************************************************/

void printUsage ( String applicationName )
{
  NL ;

  printf( "Usage: %s <MaxNumberOfUsers> <MaxUserNameLength> <MaxNumberOfFriendsPerUser>\n\n" , applicationName ) ;
}

/*=================================================================================================================================================*/

void printBanner ( void )
{
  NL ;

  printf( "*******************************************************************************" "\n"          ) ;
  printf( "* CengBook                                                                    *" "\n"          ) ;
  printf( "* by %-72s *"                                                                    "\n" , AUTHOR ) ;
  printf( "*******************************************************************************" "\n"          ) ;
}

/*=================================================================================================================================================*/

void printMenu ( void )
{
  NL ;

  printf( "*******************************************************************************" "\n" ) ;
  printf( "Help                       : Print this menu"                                    "\n" ) ;
  printf( "List                       : List all users"                                     "\n" ) ;
  printf( "Plot                       : Create a plot of CengBook (requires GraphViz)"      "\n" ) ;
  printf( "Quit                       : Exit application"                                   "\n" ) ;
  printf( "-------------------------------------------------------------------------------" "\n" ) ;
  printf( "Register   <user>          : Create a new user and add it to CengBook"           "\n" ) ;
  printf( "Delete     <user>          : Delete user from CengBook"                          "\n" ) ;
  printf( "Find       <user>          : Search CengBook for user, display user information" "\n" ) ;
  printf( "Connect    <user1> <user2> : Add a connection from user1 to user2"               "\n" ) ;
  printf( "Disconnect <user1> <user2> : Remove connection from user1 to user2"              "\n" ) ;
  printf( "Path       <user1> <user2> : Find shortest path from user1 to user2"             "\n" ) ;
  printf( "-------------------------------------------------------------------------------" "\n" ) ;
  printf( "Sort <abc|pop> <asc|desc>  : Sort all users (alphabetically or by popularity)"   "\n" ) ;
  printf( "*******************************************************************************" "\n" ) ;
}

/*=================================================================================================================================================*/

void commandHelp ( void )
{
  NL ;

  printf( "*******************************************************************************" "\n" ) ;
  printf( "Help                       : Print this menu"                                    "\n" ) ;
  printf( "List                       : List all users"                                     "\n" ) ;
  printf( "Plot                       : Create a plot of CengBook (requires GraphViz)"      "\n" ) ;
  printf( "Quit                       : Exit application"                                   "\n" ) ;
  printf( "-------------------------------------------------------------------------------" "\n" ) ;
  printf( "Register   <user>          : Create a new user and add it to CengBook"           "\n" ) ;
  printf( "Delete     <user>          : Delete user from CengBook"                          "\n" ) ;
  printf( "Find       <user>          : Search CengBook for user, display user information" "\n" ) ;
  printf( "Connect    <user1> <user2> : Add a connection from user1 to user2"               "\n" ) ;
  printf( "Disconnect <user1> <user2> : Remove connection from user1 to user2"              "\n" ) ;
  printf( "Path       <user1> <user2> : Find shortest path from user1 to user2"             "\n" ) ;
  printf( "-------------------------------------------------------------------------------" "\n" ) ;
  printf( "Sort <abc|pop> <asc|desc>  : Sort all users (alphabetically or by popularity)"   "\n" ) ;
  printf( "*******************************************************************************" "\n" ) ;

}

/*=================================================================================================================================================*/

void commandList ( void )
{

  int i,length,j;
  UserPtr user;
  if (bookPtr->userPointers[0] == NULL) { printf("\n  CengBook is empty\n"); return;}
  for (i=0; i< getMaxNumberOfUsers() ; i++)
    {
      if( bookPtr->userPointers[i] == NULL) { continue; }
      else
        {
        NL;
        user = bookPtr->userPointers[i];
        printf("  %s",user->name );
        length = strlen(user->name);
        for (j=0; j < 16 - length; j++)
          {
            printf(" ");
          }
        }
        printf(": %d Follower(s) , %d Friend(s)", user->numberOfFollowers, user->numberOfFriends );
    }
    NL;

}

/*=================================================================================================================================================*/

void commandPlot ( void )
{
  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  UserPtr   userPtr             , friendPtr              ;
  int       userIndex           , friendIndex            ;
  String    runDotSystemCommand , openImageSystemCommand ;

  FILE    * file = fopen( "CengBook.dot" , "w" )         ;

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  fprintf( file , "digraph CengBook"                                                       "\n"                            ) ;
  fprintf( file , "{"                                                                      "\n"                            ) ;
  fprintf( file , "  graph [label=\"CengBook\\nNumber of users = %d\" , labelloc=t] ;"     "\n\n" , bookPtr->numberOfUsers ) ;
  fprintf( file , "  node  [shape=ellipse , fontcolor=black , color=red , penwidth=2.0] ;" "\n\n"                          ) ;
  fprintf( file , "  edge  [color=blue , penwidth=2.0] ;"                                  "\n\n"                          ) ;

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  for ( userIndex = 0 ; userIndex < bookPtr->numberOfUsers ; userIndex++ )
  {
    userPtr = bookPtr->userPointers[ userIndex ] ;

    fprintf( file , "  %s [label=\"%s\\n%d Friend(s)\\n%d Follower(s)\"] ;\n" ,
             userPtr->name                                                    ,
             userPtr->name                                                    ,
             userPtr->numberOfFriends                                         ,
             userPtr->numberOfFollowers                                       ) ;
  }

  if ( bookPtr->numberOfUsers > 0 )  { fprintf( file , "\n" ) ; }

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  for ( userIndex = 0 ; userIndex < bookPtr->numberOfUsers ; userIndex++ )
  {
    userPtr = bookPtr->userPointers[ userIndex ] ;

    for ( friendIndex = 0 ; friendIndex < userPtr->numberOfFriends ; friendIndex++ )
    {
      friendPtr = userPtr->friendPointers[ friendIndex ] ;

      fprintf( file , "  %s -> %s ;\n" , userPtr->name , friendPtr->name ) ;
    }
  }

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  fprintf( file , "}\n" ) ;
  fclose ( file         ) ;

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  if ( strcmp( operatingSystem , "Windows" ) == 0 )
  {
    runDotSystemCommand    = "CMD /C dot -Tgif CengBook.dot -o CengBook.gif" ;
    openImageSystemCommand = "CMD /C START CengBook.gif"                     ;
  }
  else  /* Assumption: Linux */
  {
    runDotSystemCommand    = "dot -Tgif CengBook.dot -o CengBook.gif"        ;
    openImageSystemCommand = "xdg-open CengBook.gif &"                       ;
  }

  if ( system( runDotSystemCommand    ) ) ;  /* "if" statement is used to eliminate warning about unused return value of "system" call */
  if ( system( openImageSystemCommand ) ) ;  /* "if" statement is used to eliminate warning about unused return value of "system" call */

  NL ;  I2 ;  printf(" Plot prepared\n" ) ;

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/
}

/*=================================================================================================================================================*/

void commandQuit ( void )
{

	printf("\n  Have a nice day\n\n");
	exit(0);
}

/*=================================================================================================================================================*/

void commandRegister ( String userName )
{

  int max,i;
  max = getMaxNumberOfUsers();
  if (strlen(userName) > getMaxUserNameLength())
    {
      printf("\n  User name is longer than %d characters!\n", getMaxUserNameLength());
      return;
    }
  if (getUserPtrByName(bookPtr, userName) != NULL )
    { printf("\n  User '%s' already registered!\n", userName);
      return;
    }
  if (bookPtr->numberOfUsers >= max)
    { printf("\n  CengBook is full, cannot register any new users!\n" );
      return;
    }
  for (i= 0; i < getMaxNumberOfUsers(); i++) {
    if (bookPtr->userPointers[i] == NULL) {
      bookPtr->userPointers[i] = allocateUser(userName);
      bookPtr->numberOfUsers += 1;
      break;
    }
  }
  NL;
  printf("  User '%s' registered successfully", userName);
  NL;

}

/*=================================================================================================================================================*/

void commandDelete ( String userName )
{

  UserPtr temp;
  int temp2, i;
  if ( getUserPtrByName(bookPtr, userName) == NULL )
    {
      printf("\n  User '%s' not found in CengBook!\n", userName);
      return;
    }
  temp = getUserPtrByName(bookPtr, userName);
  temp2 = getUserIndexByPtr(bookPtr, temp);
   for (i=0; i < bookPtr->numberOfUsers; i++)
    {
      if (temp == bookPtr->userPointers[i] ) { continue; }
        if (removeConnection(bookPtr->userPointers[i],temp) == TRUE) {
          printf("\n  User '%s' is removed from friends of user '%s'", userName, bookPtr->userPointers[i]->name);
        }
    }
    for (i=0; i < bookPtr->numberOfUsers; i++)
     {
       if (temp == bookPtr->userPointers[i] ) { continue; }
         if (removeConnection(temp, bookPtr->userPointers[i]) == TRUE ) {

           printf("\n  User '%s' is discounted from followers of user '%s'", userName, bookPtr->userPointers[i]->name );
         }
     }
  deallocateUser(temp);
  for (i=temp2; i<bookPtr->numberOfUsers; i++ ) {
    bookPtr->userPointers[i] = bookPtr->userPointers[i+1];
  }
  bookPtr->userPointers[bookPtr->numberOfUsers - 1] = NULL;
  bookPtr->numberOfUsers -= 1;
  printf("\n  User '%s' is deleted from CengBook\n", userName);

}

/*=================================================================================================================================================*/

void commandFind ( String userName )
{

  UserPtr temp = getUserPtrByName(bookPtr, userName);
  if (getUserPtrByName(bookPtr, userName) == NULL)
    {
      printf("\n  User '%s' not found in CengBook!", userName);
      return;
    }
  NL;
  printUser(temp);
}

/*=================================================================================================================================================*/

void commandConnect ( String userName1 , String userName2 )
{

  UserPtr temp1;
  UserPtr temp2;
  int i=0;
  temp1 = getUserPtrByName(bookPtr,userName1);
  temp2 = getUserPtrByName(bookPtr,userName2);
  NL;
  if (temp1 == NULL || temp2 == NULL)
    {
      if (temp1 == NULL)  { printf("  User '%s' not found in Cengbook!\n", userName1);}
      if (temp2 == NULL)  { printf("  User '%s' not found in Cengbook!\n", userName2);}
      return;
    }
  if (isConnected(temp1,temp2) == TRUE)
    {
      printf("  User '%s' is already connected to user '%s'!\n", userName1, userName2);
      return;
    }
  if (strcmp(userName1,userName2) == 0)
    {
      printf("  Connection to self is not allowed!\n");
      return;
    }
  while (i <= getMaxNumberOfFriendsPerUser())
    {
      if ( i == getMaxNumberOfFriendsPerUser())
           {
             printf("  Friend capacity of user '%s' is full!\n", userName1);
             break;
           }
      if( temp1->friendPointers[i] != NULL )
           {
             i++;
             continue;
           }
      else {
             temp1->friendPointers[i] = temp2;
             temp1->numberOfFriends += 1;
             temp2->numberOfFollowers += 1;
             printf("  User '%s' successfully connected to user '%s'\n", userName1, userName2);
             break;
           }

    }


}

/*=================================================================================================================================================*/

void commandDisconnect ( String userName1 , String userName2 )
{

  UserPtr temp1;
  UserPtr temp2;
  temp1 = getUserPtrByName(bookPtr,userName1);
  temp2 = getUserPtrByName(bookPtr,userName2);
  NL;
  if (temp1 == NULL || temp2 == NULL)
    {
      if (temp1 == NULL)  { printf("  User '%s' not found in Cengbook!\n", userName1);}
      if (temp2 == NULL)  { printf("  User '%s' not found in Cengbook!\n", userName2);}
      return;
    }
  if (strcmp(userName1,userName2) == 0)
    {
      printf("  Disconnecting from self is not allowed!\n");
      return;
    }
  if (isConnected(temp1,temp2) == TRUE)
    {
      removeConnection(temp1,temp2);
      printf("  User '%s' successfully disconnected from user '%s'\n", userName1, userName2);
      return;
    }
  else
    {
    printf("  User '%s' and user '%s' are not connected!\n", userName1, userName2);
    return;
    }

}

/*=================================================================================================================================================*/

void commandPath ( String userName1 , String userName2 )
{

  int i=0;
  int j;
  UserPtr user1;
  UserPtr user2;
  UserPtrArray path;
  user2 = getUserPtrByName(bookPtr,userName2);
  user1 = getUserPtrByName(bookPtr,userName1);
  if (user1 == NULL || user2 == NULL)
    {
      if (user1 == NULL)  { NL; printf("  User '%s' not found in Cengbook!", userName1);}
      if (user2 == NULL)  { NL; printf("  User '%s' not found in Cengbook!", userName2);}
      NL;
      return;
    }
  if (strcmp(userName1,userName2) == 0)
    {
      printf("\n  Path to self queried!\n");
      return;
    }
  path = shortestPath(bookPtr, user1, user2);
  if (path == NULL)
    {
      NL;
      printf("  There is no path from user '%s' to user '%s'\n", userName1, userName2);
      return;
    }
  while (path[i] != NULL) {
    i++;
  }
  NL;
  I2;
  for (j=0; j < i; j++) {
    printf("'%s'", path[j]->name);
    if ( j != (i-1)) {
      printf(" -> ");
    }
  }
  NL;
}

/*=================================================================================================================================================*/

void commandSort ( String criterion , String order )
{

  if (strcmp(criterion,"abc") != 0 && strcmp(criterion,"pop")!= 0 ) {
    printf("\n  Invalid sort criterion '%s'!\n",criterion);
    if (strcmp(order,"asc") != 0 && strcmp(order,"desc")!= 0 ) {
      printf("  Invalid sort order '%s'!\n", order);
      return;
    }
    return;
  }
    if (strcmp(order,"asc") != 0 && strcmp(order,"desc") != 0) {
      printf("\n  Invalid sort order '%s'!\n", order);
      return;
    }
  if (strcmp(criterion,"abc") == 0 )
  {
    if (strcmp(order,"asc") == 0) {
      sortBook(bookPtr, compareByName, 1);
      printf("\n  CengBook sorted 'alphabetically' in 'ascending' order\n");
      return;
    }
    else if(strcmp(order,"desc") == 0) {
      sortBook(bookPtr, compareByName, 0);
      printf("\n  CengBook sorted 'alphabetically' in 'descending' order\n");
      return;

    }
  }
  else if (strcmp(criterion,"pop") == 0)
  {
    if (strcmp(order,"asc") == 0) {
      sortBook(bookPtr, compareByPopularity, 1);
      printf("\n  CengBook sorted 'by popularity' in 'ascending' order\n");
      return;
    }
    else if(strcmp(order,"desc") == 0) {
      sortBook(bookPtr, compareByPopularity, 0);
      printf("\n  CengBook sorted 'by popularity' in 'descending' order\n");
      return;
    }
  }
}

/*=================================================================================================================================================*/

void processInput ( String input )
{
  int         i                                                    ;
  int         numberOfTokens = 0                                   ;
  StringArray tokens         = tokenize( input , &numberOfTokens ) ;
  String      command                                              ;

  if ( numberOfTokens < 1 )  { return ; }

  command = tokens[0] ;

  for ( i = 0 ; i < strlen( command ) ; i++ )  { if ( ( command[i] >= 'A' ) && ( command[i] <= 'Z' ) )  { command[i] += 'a' - 'A' ; } }

  if      ( strcmp( command , "help"       ) == 0 )  { if ( numberOfTokens == 1 ) { commandHelp      (                       ) ;  return ; } }
  else if ( strcmp( command , "list"       ) == 0 )  { if ( numberOfTokens == 1 ) { commandList      (                       ) ;  return ; } }
  else if ( strcmp( command , "plot"       ) == 0 )  { if ( numberOfTokens == 1 ) { commandPlot      (                       ) ;  return ; } }
  else if ( strcmp( command , "quit"       ) == 0 )  { if ( numberOfTokens == 1 ) { commandQuit      (                       ) ;  return ; } }
  else if ( strcmp( command , "register"   ) == 0 )  { if ( numberOfTokens == 2 ) { commandRegister  ( tokens[1]             ) ;  return ; } }
  else if ( strcmp( command , "delete"     ) == 0 )  { if ( numberOfTokens == 2 ) { commandDelete    ( tokens[1]             ) ;  return ; } }
  else if ( strcmp( command , "find"       ) == 0 )  { if ( numberOfTokens == 2 ) { commandFind      ( tokens[1]             ) ;  return ; } }
  else if ( strcmp( command , "connect"    ) == 0 )  { if ( numberOfTokens == 3 ) { commandConnect   ( tokens[1] , tokens[2] ) ;  return ; } }
  else if ( strcmp( command , "disconnect" ) == 0 )  { if ( numberOfTokens == 3 ) { commandDisconnect( tokens[1] , tokens[2] ) ;  return ; } }
  else if ( strcmp( command , "path"       ) == 0 )  { if ( numberOfTokens == 3 ) { commandPath      ( tokens[1] , tokens[2] ) ;  return ; } }
  else if ( strcmp( command , "sort"       ) == 0 )  { if ( numberOfTokens == 3 ) { commandSort      ( tokens[1] , tokens[2] ) ;  return ; } }
  else                                               { NL ;  I2 ;  printf( "Invalid command!\n" ) ;                               return ;   }

  /* If we reach here, the user must have provided incorrect number of arguments to a valid command */

  command[0] += 'A' - 'a' ;

  NL ;  I2 ;  printf( "Incorrect number of arguments provided for the command '%s'!\n" , command ) ;
}

/***************************************************************************************************************************************************/




/* Main Function ***********************************************************************************************************************************/

int main ( int argc , char * argv[] )
{
  char input [ MAX_INPUT_LENGTH ] ;

  if ( argc < 4 )  { printUsage( argv[0] ) ;  return 0 ; }

  configureCengBook( /* maxNumberOfUsers          */ atoi( argv[1] ) ,
                     /* maxUserNameLength         */ atoi( argv[2] ) ,
                     /* maxNumberOfFriendsPerUser */ atoi( argv[3] ) ) ;

  bookPtr = allocateBook() ;

  if ( bookPtr == NULL )  { NL ;  printf( "Error: Cannot allocate memory to store CengBook! Exiting!\n\n" ) ;  exit( 0 ) ; }

  printBanner() ;
  printMenu  () ;

  while ( TRUE )
  {
    NL                        ;  /* New line     */
    printf( "[CENGBOOK] > " ) ;  /* Print prompt */

    if ( fgets( input , MAX_INPUT_LENGTH , stdin ) == NULL )  /* Read input line, 'fgets' is safer than 'gets'. */
    {
      /* In the following two lines, the "if" statements surrounding the "freopen" function calls are used to eliminate warnings. */

      if   ( strcmp( operatingSystem , "Windows" ) == 0 )  { if ( freopen( "CON"      , "r" , stdin ) == NULL ) ; }
      else /* Assumption:               Linux          */  { if ( freopen( "/dev/tty" , "r" , stdin ) == NULL ) ; }

      continue ;
    }

    processInput( input ) ;  /* Tokenize and process the input command */
  }

  return 0 ;
}

/***************************************************************************************************************************************************/

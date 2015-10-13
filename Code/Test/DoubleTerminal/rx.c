/**
@author Gianluca Savaia
@last update 2015-09-11
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "board.h"
#include "packet.h"

char getchar_tty()
{
    char ch = getchar();

    while( getchar() != '\n' );

    return ch;
}

int main()
{
    struct termios boardSettings;
    struct termios oldBoardSettings;

    int board;
    int sent;

    printf("OUPUT TERMINAL\n\n");

    printf("Trying to connect to the board...");

    board = open_board(&oldBoardSettings, &boardSettings);

    if( board < 0 )
    {
        printf("Error: connection to board failed.");
        return 1;
    }

    char ctty = 0;
    char cboard = 0;

    printf("Connection successful!\n\n");

    while(1)
    {
        while( (cboard = getchar_board(board)) )
            printf("%c", cboard);
    }

    printf("\nEnd of communication.\n");

    close_board(board, &oldBoardSettings);

    return 0;
}


/**
@author Gianluca Savaia
@version 1.0
@last update 2015-09-11
*/

#include <stdio.h>
#include <stdlib.h>

#include "../interface/packet.h"
#include "board.h"

int main()
{
    struct termios boardSettings;
    struct termios oldBoardSettings;

    int board;
    int sent;

    printf("TERMINAL\n\n");

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

    do
    {
        printf("user> ");

        packet_t p;
        p.header = 0x0;
        p.command = 0x0;

        ctty = getchar();
        getchar();

        switch(ctty)
        {
            case '1':
                printf("pc> turn on led1.\n");
                p.header = SET_LED;
                p.command = LED1;
                break;
            case '2':
                p.header = SET_LED;
                p.command = LED2;
                break;
            case '3':
                p.header = SET_LED;
                p.command = LED3;
                break;
            case '4':
                p.header = SET_LED;
                p.command = LED4;
                break;
            case 'q':
                break;
            default:
                ctty = 0;
                printf("pc> Command not recognized.\n");
        };

        if(ctty == 'q')
            break;

        if(ctty)
            send_command(board, p);

        while( (cboard = getchar_board(board)) )
            printf("%c", cboard);
    }
    while(ctty);

    printf("\nEnd of communication.\n");

    close_board(board, &oldBoardSettings);

    return 0;
}


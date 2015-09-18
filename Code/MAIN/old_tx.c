/**
@author Gianluca Savaia
@last update 2015-09-11
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>

#include "../interface/packet.h"
#include "../interface/hamming.h"
#include "board.h"

char getchar_tty()
{
    char ch = getchar();

    //while( getchar() != '\n' );

    return ch;
}

int main()
{
    struct termios boardSettings;
    struct termios oldBoardSettings;

    int board;
    int sent;

    char ctty = 0;
    char cboard = 0;
    char ack_received = 0;
    char counter = 0;

    printf("TERMINAL\n\n");

    printf("Trying to connect to the board...");

    board = open_board(&oldBoardSettings, &boardSettings);

    if( board < 0 )
    {
        printf("Error: connection to board failed.");
        return 1;
    }

    printf("Connection successful!\n\n");

    printf("TERMINAL\n\n");

    printf("Trying to connect to the board...");

    board = open_board(&oldBoardSettings, &boardSettings);

    if( board < 0 )
    {
        printf("Error: connection to board failed.");
        return 1;
    }

    printf("Connection successful!\n\n");

    while(ctty != 'q')
    {
        printf("user> ");

        packet_t p;
        p.header = 0x0;
        p.command = 0x0;

        ctty = getchar_tty();

        switch(ctty)
        {
            case '1':
                printf("pc> turn on led1.\n");
                p.header = SET_MODE;
                p.command = SAFE_MODE;
                break;
            case '2':
                p.header = SET_MODE;
                p.command = MANUAL_MODE;
                break;
            case '3':
                p.header = SET_LED;
                p.command = LED3;
                break;
            case '4':
                p.header = SET_LED;
                p.command = LED4;
                break;
            case 'a':
                p.header = SET_LED;
                p.command = ALL_ON;
                break;
            case 'q':
                p.header = STOP;
                p.command = 0;
                break;
            case 'l':
                p.header = LOG;
                p.command = LOG_START;
                break;
            case 'u':
                p.header = LOG;
                p.command = LOG_GET;
                break;

            default:
                ctty = 0;
                ack_received = 1;
                printf("pc> Command not recognized.\n");
        };

        compute_hamming(&p);

        while(!ack_received)
        {
            if(ctty)
                send_packet(board, p);

            sleep(1); //give time to board to write output

            while( (cboard = getchar_board(board)) )
            {
                if( cboard == ACK ) //ack coming
                {
                    cboard = getchar_board(board); //response: positive or negative

                    getchar_board(board); //ignore crc

                    if( cboard == ACK_POSITIVE )
                        printf("pc> Positive acknowledge received.\n"), ack_received = 1;
                    else
                        if( cboard == ACK_NEGATIVE )
                            printf("pc> Negative acknowledge received.\n");
                    else
                        printf("pc> Unexpected message from board.\n");
                }
                else
                    printf("%c", cboard);
            }

            if(counter++>3)
            {
                printf("pc> Timeout: ACK not received.\n");
                break;
            }
        }

        ack_received = 0;
        counter = 0;

        printf("\n");
    }

    printf("End of communication.\n");

    close_board(board, &oldBoardSettings);

    return 0;
}



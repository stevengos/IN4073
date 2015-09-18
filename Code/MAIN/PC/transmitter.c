/**
@author Gianluca Savaia
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../interface/packet.h"
#include "../interface/hamming.h"
#include "board.h"
#include "keyboard.h"

pthread_mutex_t lock_board;

void *is_alive(void* board)
{
    int idboard = *( (int*)(board) );

    while(1)
    {
        packet_t p;
        p.header = ALIVE;
        p.command = EMPTY;

        compute_hamming(&p);

        pthread_mutex_lock( &lock_board );

        send_packet(idboard, p);

        pthread_mutex_unlock( &lock_board );

        usleep(50000); //50ms
    }
}

int main()
{
    struct termios boardSettings;
    struct termios oldBoardSettings;

    struct termios oldKeyboardSettings;
    struct termios keyboardSettings;

    int board;

    char ctty = 0;
    char cboard = 0;
    char ack_received = 0;
    char counter = 0;

    pthread_t polling;
    int status;

    open_keyboard(&oldKeyboardSettings, &keyboardSettings);

    printf("TERMINAL\n\n");

    printf("Trying to connect to the board...");

    board = open_board(&oldBoardSettings, &boardSettings);

    if( board < 0 )
    {
        printf("Error: connection to board failed.");
        return 1;
    }

    printf("Connection successful!\n\n");

    pthread_mutex_init(&lock_board, NULL);

    status = pthread_create(&polling, NULL, is_alive, (void*)&board);

    if( status )
    {
        printf("pc> Error while creating polling thread. Session Aborted.\n");
        ctty = ESC;
    }

    while(ctty != ESC)
    {
        printf("user> ");

        packet_t p;

        ctty = getchar_keyboard();

        printf("pc> Command received: %d.\n", ctty);

        p = encapsulate( ctty );

        do
        {
            printf("pc> Sending packet...\n");

            pthread_mutex_lock( &lock_board );

            send_packet(board, p);

            pthread_mutex_unlock( &lock_board );

            sleep(1); //give time to board to write output 1s

            pthread_mutex_lock( &lock_board );

            while( (cboard = getchar_board(board)) )
            {
                if( cboard == ACK ) //ack coming
                {
                    cboard = getchar_board(board); //response: positive or negative or invalid

                    getchar_board(board); //ignore crc

                    if( cboard == ACK_POSITIVE )
                        printf("pc> Positive acknowledge received.\n"), ack_received = ACK_POSITIVE;
                    else
                        if( cboard == ACK_NEGATIVE )
                            printf("pc> Negative acknowledge received.\n\n"), ack_received = ACK_NEGATIVE;
                    else
                        if( cboard == ACK_INVALID )
                            printf("pc> Board signaled invalid message.\n"), ack_received = ACK_INVALID;
                    else
                        printf("pc> Unexpected message from board.\n");
                }
                else
                    printf("%c", cboard);
            }

            pthread_mutex_unlock( &lock_board );

//            if(counter++>0)
//            {
//                printf("pc> Timeout: ACK not received.\n");
//                break;
//            }
            break;
        }
        while( ack_received == ACK_NEGATIVE );

        counter = 0;
        printf("\n");
    }

    while( (cboard = getchar_board(board)) )
        {
            if( cboard == ACK ) //ack coming
            {
                cboard = getchar_board(board); //response: positive or negative or invalid

                getchar_board(board); //ignore crc

                if( cboard == ACK_POSITIVE )
                    printf("pc> Positive acknowledge received.\n"), ack_received = ACK_POSITIVE;
                else
                    if( cboard == ACK_NEGATIVE )
                        printf("pc> Negative acknowledge received.\n\n"), ack_received = ACK_NEGATIVE;
                else
                    if( cboard == ACK_INVALID )
                        printf("pc> Board signaled invalid message.\n"), ack_received = ACK_INVALID;
                else
                    printf("pc> Unexpected message from board.\n");
            }
            else
                printf("%c", cboard);
        }

    printf("End of communication.\n");

    close_keyboard(&oldKeyboardSettings);
    close_board(board, &oldBoardSettings);

    return 0;
}


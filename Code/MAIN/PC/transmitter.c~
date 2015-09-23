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
#include "joystick.h"

pthread_mutex_t lock_board;
packet_t p_arr[10];
int packet_counter = 0;

void push_packet_t(char header, char command){
	packet_t p;

	p.header = header;
	p.command = command;
	compute_hamming(&p);

	p_arr[packet_counter++] = p;
}

void empty_packet_t(){
	int i = 0;

	packet_counter = 0;
}

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
    int status, i = 0;

//---- open joystick
	int 		fd, js_exit = 0;

	if ((fd = open(JS_DEV, O_RDONLY)) < 0) {
		perror("jstest");
		exit(1);
	}

	/* non-blocking mode
	 */
	fcntl(fd, F_SETFL, O_NONBLOCK);
//----

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

    while(js_exit != 1 && ctty != ESC)
    {
        //printf("user> ");

       

//---- read JS command
	js_exit = set_js_command(fd);
	//continue; //DEBUG
//----

	//if(p.header == EMPTY){
		//printf("into keyboard \n");
		ctty = getchar_keyboard();
		
		printf("pc> Command received: %d.\n", ctty);

		p_arr[packet_counter] = encapsulate( ctty );
		packet_counter++;
	//}

	for(i = 0; i < packet_counter; i++){
	packet_t p = p_arr[i];
        do
        {
            printf("pc> Sending packet...\n");

            pthread_mutex_lock( &lock_board );

            send_packet(board, p);

            pthread_mutex_unlock( &lock_board );

            //sleep(1); //give time to board to write output 1s
	    mon_delay_ms(4);

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
}
	empty_packet_t();
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


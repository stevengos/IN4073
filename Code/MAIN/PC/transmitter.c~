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

	/** DEBUG **/
struct termios old_tio, new_tio;
unsigned char c;
unsigned int arrow_key = 0;

//using termios code from http://shtrom.ssji.net/skb/getc.html
/* get the terminal settings for stdin */
tcgetattr(STDIN_FILENO,&old_tio);

/* we want to keep the old setting to restore them a the end */
new_tio=old_tio;

/* disable canonical mode (buffered i/o) and local echo */
new_tio.c_lflag &=(~ICANON & ~ECHO);

/* set the new settings immediately */
tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

do {
	packet_t p;
        p.header = 0x0;
        p.command = 0x0;
	
	printf("user> ");

	c=getchar();

	printf("%d \n",c);

	//detect esc key
	if(c == 27 && arrow_key){
		break;	
	}

	//detection of arrow key press
	//for example the sequence kan be 27, 91, 66 or 27, 91, 68
	if(c == 27){
		arrow_key = 1;
	}

	//left key
	if(c == 68 && arrow_key == 1){
		printf("Left pressed! \n");
		arrow_key = 0;
	}

	//up key
	if(c == 65 && arrow_key == 1){
		printf("Up pressed! \n");
		arrow_key = 0;
	}

	//right key
	if(c == 67 && arrow_key == 1){
		printf("Right pressed! \n");
		arrow_key = 0;
	}

	//down key
	if(c == 66 && arrow_key == 1){
		printf("Down pressed! \n");
		arrow_key = 0;
	}

	//0 - SAFE_MODE
	if(c == 48){
		p.header	= SET_MODE;
        	p.command	= SAFE_MODE;
	}

	//1 - PANIC_MODE
	if(c == 49){
		p.header	= SET_MODE;
        	p.command	= PANIC_MODE;
	}

	//2 - MANUAL_MODE
	if(c == 50){
		p.header	= SET_MODE;
        	p.command	= MANUAL_MODE;
	}

	//3 - CALIBRATION_MODE
	if(c == 51){
		p.header	= SET_MODE;
        	p.command	= CALIBRATION_MODE;
	}

	//4 - YAW_MODE
	if(c == 52){
		p.header	= SET_MODE;
        	p.command	= YAW_MODE;
	}

	//5 - FULL_MODE
	if(c == 53){
		p.header	= SET_MODE;
        	p.command	= FULL_MODE;
	}

	//a - lift up
	if(c == 97){
		p.header	= INC_LIFT;
	}

	//z - lift down
	if(c == 122){
		//p.header	= INC_LIFT;
	}

	//w - yaw up
	if(c = 119){
		p.header	= INC_YAW;	
	}

	//q - yaw down
	if(c = 133){
		//p.header	= INC_YAW;
	}

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
} while(c!='x');

/* restore the former settings */
tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

printf("quit\n");

	/** 	  **/
/*
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
            case 'a':
                p.header = SET_LED;
                p.command = ALL_ON;
                break;
            case 'q':
                p.header = STOP;
                p.command = 0;
                break;
            default:
                ctty = 0;
                ack_received = 1;
                printf("pc> Command not recognized.\n");
        };

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

        printf("\n");
    }

    printf("End of communication.\n");

    close_board(board, &oldBoardSettings);
*/
    return 0;
}


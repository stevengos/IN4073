/**
@author Gianluca Savaia
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>

#include "../interface/packet.h"
#include "board.h"
#include "keyboard.h"
#include "joystick.h"

#define REFRESH_TIME    50 //ms

pthread_mutex_t lock_board; //semaphore which allows atomic access to the communication
int end_communication = 0;  //flag which controls the exit from the main loop

packet_t packet_buffer[10];
int packet_counter = 0;

//{

char* get_current_time_string(){
	time_t current_time;
	struct tm * time_info;
	char* timeString;  // space for "%Y_%m_%d_%H_%M_%S\0"
	timeString = malloc(sizeof(char) * 21);

	time(&current_time);
	time_info = localtime(&current_time);

	strftime(timeString, sizeof(char) * 21, "%Y_%m_%d_%H_%M_%S", time_info);
	puts(timeString);

	return timeString;
}

void safe_measurement_to_file(char* filename, char* t_string){
	FILE *fp;
	fp = fopen(filename, "aw+");
	fprintf(fp, "%s", t_string);
	fclose(fp);
}

void close_PC_log_errors(){
	FILE *fp;
	fp = fopen("last_errors.txt", "w");
	fprintf(fp, "%s", "-x\n");
	fclose(fp);
}

void PC_log_errors(char* t_string){
	 safe_measurement_to_file("last_errors.txt", t_string);
}

void safe_int_measurement_to_file(char* filename, int t_int){
	char temp_value[100];

	sprintf(temp_value, "%d ", t_int);

	safe_measurement_to_file(filename, temp_value);
}

void push_packet_t(char header, char command)
{
	packet_t p;

	p.header = header;
	p.command = command;
	compute_hamming(&p);

	packet_buffer[packet_counter++] = p;
}

void empty_packet_t()
{
	packet_counter = 0;
}

/**
    Pooling method which sends "keep-alive" messages in order to keep the board awake.
*/
void *is_alive(void* board)
{
    int idboard = *( (int*)(board) );

    while(!end_communication)
    {
        packet_t p;
        p.header = ALIVE;
        p.command = EMPTY;

        compute_hamming(&p);

        pthread_mutex_lock( &lock_board );

        send_packet(idboard, p);

        pthread_mutex_unlock( &lock_board );

        usleep(REFRESH_TIME*1000); //50ms
    }
}

/**
    Log retrival session: it allows to download log data into a file on the PC.
    Standard communication protocol is interrupted, PC reads log data from the board in a continous stream.
*/
void logging(int board, packet_t p)
{
    printf("pc> Retrieving Log Data...\n");

    int incoming_int;
    short incoming_short;
    char file_name[100];

    strcpy(file_name, "measurements/meas_");
    strcat(file_name, get_current_time_string());
    strcat(file_name, ".txt");

    int number_logs;
    int status = 1;
    int debug = 0;

    pthread_mutex_lock( &lock_board ); //****************************** BOARD IS LOCKED **********************

    /********* receive number of logs ******/
    status = getint_board(board, &incoming_int);
    number_logs = incoming_int;

    /********* read logs **********************/
    while( number_logs-- > 1 ) //while there's a new log line
    {
        status = getint_board(board, &incoming_int); //receive timestamp

        if( !status )
        {
            while( getchar_board(board) );
            printf("\npc>Flushing the shit out of the buffer.\n");
	    PC_log_errors("\npc>Flushing the shit out of the buffer.\n");
            break;
        }

        printf("#%d %d ", number_logs, incoming_int);
        safe_int_measurement_to_file(file_name, incoming_int);

        debug = 0;
        while(debug < 50)
        {
            status = getshort_board(board, &incoming_short);

            if( !status )
            {
                printf("pc> Error while reading integer.\n");
		PC_log_errors("pc> Error while reading integer.\n");
                break;
            }

            if(incoming_short == LOG_NEWLINE)
            {
                printf("\n");
                break;
            }

            if( incoming_short == ACK_NEGATIVE ){
                printf("\n\npc> Error while reading logs from the board. Abort!\n");
		PC_log_errors("\n\npc> Error while reading logs from the board. Abort!\n");
		exit(0);
	    }
            printf("%d ", incoming_short);
            safe_int_measurement_to_file(file_name, incoming_short);

            debug++;

            if( debug == 50 ){
                printf("\n\npc> Got in a loop whilst reading logging!\n");
		PC_log_errors("\n\npc> Got in a loop whilst reading logging!\n");
	    }
        }

        safe_measurement_to_file(file_name, "\n");
    }

    mon_delay_ms(1000);
    while( getchar_board(board) ); //flush spurious data

    pthread_mutex_unlock( &lock_board );//****************************** BOARD IS UNLOCKED **************************

    printf("\npc>Log retrival is over. Press any key to continue.\n\n");
    getchar();
}

void* open_status_terminal(){
     
}

int main()
{
    struct termios boardSettings;
    struct termios oldBoardSettings;

    struct termios oldKeyboardSettings;
    struct termios keyboardSettings;

    int board;
    int joystick;

    char ctty = 0;
    char cboard = 0;
    char ack_received = 0;
    char counter = 0;

    pthread_t polling;
    int status, i = 0;

    int js_exit = 0;

    /******************* Open Status Terminal **********************************/
    
    //clearup the old file
    unlink("last_errors.txt");

    PC_log_errors("startup\n");
    system("gnome-terminal -x sh -c \"./status_terminal;\"");

    /************* Open Joystick ********************************/
//    joystick = open(JS_DEV0, O_RDONLY);
//
//	if ( joystick < 0)
//	{
//        joystick = open(JS_DEV1, O_RDONLY);
//
//        if( joystick < 0 )
//            perror("jstest"), exit(1);
//	}
//
//	fcntl(joystick, F_SETFL, O_NONBLOCK); // non-blocking mode

    /************* Open Keyboard ********************************/
    open_keyboard(&oldKeyboardSettings, &keyboardSettings);

    printf("TERMINAL\n\n");

    printf("Trying to connect to the board...\n");
	
    /************* Open Board ***********************************/

    board = open_board(&oldBoardSettings, &boardSettings);

    if( board < 0 )
    {
	close_keyboard(&oldKeyboardSettings);
        printf("Error: connection to board failed.\n");
        return 1;
    }

    printf("Connection successful!\n\n");

    /************* Create Polling (keep_alive) ********************/

    pthread_mutex_init(&lock_board, NULL);

    status = pthread_create(&polling, NULL, is_alive, (void*)&board);

    if( status )
    {
        printf("pc> Error while creating polling thread. Session Aborted.\n");
        PC_log_errors("pc> Error while creating polling thread. Session Aborted.\n");
        ctty = ESC;
    }

    /************* Main Loop ********************************/

    while(js_exit != 1 && ctty != ESC)
    {
        //js_exit = set_js_command(joystick); //read the joystick configuration

		ctty = getchar_keyboard();          //read the keyboard

		packet_buffer[packet_counter] = encapsulate( ctty ); //decode the character from keyboard

//        if( packet_buffer[packet_counter].header == LOG && packet_buffer[packet_counter].command == LOG_GET)
//        {
//            close_keyboard(&oldKeyboardSettings);
//
//            logging(board, packet_buffer[packet_counter]);
//
//            open_keyboard(&oldKeyboardSettings, &keyboardSettings);
//
//            continue;
//        }

		if( packet_buffer[packet_counter].header != EMPTY )  //if keyboard command is a valid one then push it, else ignore
            packet_counter++;

        for(i = 0; i < packet_counter; i++) //send a burst of packets
        {
            packet_t p = packet_buffer[i];

            do //send the same packet until I receive a positive ack
            {
                int counter_waitack = 0;
                ack_received = EMPTY;

                //get exclusive access to the board and send the packet
                pthread_mutex_lock( &lock_board );
                send_packet(board, p);
                pthread_mutex_unlock( &lock_board );

                printf("pc> Sending packet %d... ", i);

                mon_delay_ms(REFRESH_TIME);

                /********* WAIT FOR ACKNOWLEDGMENT ***************/
                pthread_mutex_lock( &lock_board );

                for(counter_waitack=0; counter_waitack < 5; counter_waitack++)
                    if( (cboard = getchar_board(board)) == ACK ) //wait for acknowledgment
                    {
                        ack_received = getchar_board(board);    //response
                        getchar_board(board);                   //ignore crc
                        break;
                    }
                    else
                        usleep(500);

                pthread_mutex_unlock( &lock_board );

                /********* EVALUATE ACKNOWLEDGMENT ************/
                switch(ack_received)
                {
                    case ACK_NEGATIVE:
                        printf("NACK received, trying again...\n"), counter++;
                        PC_log_errors("NACK received, trying again...\n");

                        pthread_mutex_lock( &lock_board );

                        while( getchar_board(board) ) mon_delay_ms(1);

                        pthread_mutex_unlock( &lock_board );

                        break;

                    case ACK_HAMMING:
                        printf("Checksum is wrong, rejected\n");
                        PC_log_errors("Checksum is wrong, rejected\n");

                        pthread_mutex_lock( &lock_board );

                        while( getchar_board(board) ) mon_delay_ms(1);

                        pthread_mutex_unlock( &lock_board );

                        break;

                    case ACK_INVALID:
                        printf("Invalid command, rejected.\n");
                        PC_log_errors("Invalid command, rejected.\n");

                        break;

                    case ACK_POSITIVE:
                        printf("Command executed correctly.\n");
                        PC_log_errors("Command executed correctly.\n");

                        if( packet_buffer[i].header == LOG && packet_buffer[i].command == LOG_GET)
                        {
                            close_keyboard(&oldKeyboardSettings);
                            logging(board, packet_buffer[packet_counter]);
                            open_keyboard(&oldKeyboardSettings, &keyboardSettings);
                        }

                        break;

                    case EMPTY:
                        printf("No answer received, trying again...\n"), counter++;
                        PC_log_errors("No answer received, trying again...\n");

                        while( getchar_board(board) ) mon_delay_ms(1);

                        break;
                };

            }
            while( (ack_received == ACK_NEGATIVE || ack_received == EMPTY) && counter < 5);
        }

        empty_packet_t();
        counter = 0;

        if(ctty != ESC)
            system("clear");
    }

    printf("End of communication.\n");

    close_PC_log_errors();
    end_communication = 1;
    mon_delay_ms(500);

    pthread_cancel(polling);

    close_keyboard(&oldKeyboardSettings);
    close_board(board, &oldBoardSettings);

    return 0;
}

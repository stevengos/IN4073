/**
@author Gianluca Savaia
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "../interface/packet.h"
#include "../interface/hamming.h"
#include "../interface/log.h"
#include "board.h"
#include "keyboard.h"
#include "joystick.h"

#define REFRESH_TIME    50

pthread_mutex_t lock_board;

packet_t packet_buffer[10];
int packet_counter = 0;

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

        usleep(REFRESH_TIME*1000); //50ms
    }
}

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

char* readFile(char* filename)
{
    FILE* file = fopen(filename,"r");
    if(file == NULL)
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    rewind(file);

    char* content = calloc(size + 1, 1);

    fread(content,1,size,file);

    return content;
}

void safe_measurement_to_file(char* filename, char* t_string){
	//int sz;
	//char* buff_string;

	FILE *fp;
	fp = fopen (filename, "aw+");
	
	/*fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	buff_string = malloc(sz);
*/
	fprintf(fp, "%s", t_string);

 	fclose(fp);
}

void safe_int_measurement_to_file(char* filename, int t_int){
	char temp_value[100];

	sprintf(temp_value, "%d ", t_int);

	safe_measurement_to_file(filename, temp_value);
}

void logging(int board, packet_t p)
{
    printf("pc> Sending packet...\n");

    pthread_mutex_lock( &lock_board );
    send_packet(board, p);
    pthread_mutex_unlock( &lock_board );

    sleep(1); //give time to board to write output 1s

    int incoming_int;
    short incoming_short;
    char file_name[100];
    

    strcpy(file_name, "measurements/meas_");
    strcat(file_name, get_current_time_string());
    strcat(file_name, ".txt");

    //safe_measurement_to_file(file_name, "hello world!\n");

    int status = 1;

    while( status = getint_board(board, &incoming_int) ) //while there's a new log line
    {
        //read the log id
        //printf("\t#%d ", incoming_int);
	safe_int_measurement_to_file(file_name, incoming_int);

        //read timestamp
        status = getint_board(board, &incoming_int);

        if( !status )
            break;

        //printf("%d ", incoming_int);
	safe_int_measurement_to_file(file_name, incoming_int);

        //read data
        while(1)
        {
            status = getshort_board(board, &incoming_short);

            if(incoming_short == LOG_END)
                break;

            //printf("%d ", incoming_short);
		safe_int_measurement_to_file(file_name, incoming_short);
        }

        //printf("\n");
	safe_measurement_to_file(file_name, "\n");
    }

    printf("\npc>Log retrival is over. Press any key to continue.\n\n");

    getchar();
}

int main()
{
    /*char temp_string[] = "hello world!\n";

	char* file_name;
	file_name = malloc(sizeof(char) * 100);

    //strcpy(file_name, "../measurements/meas_");
    strcpy(file_name, "measurements/meas_");
    strcat(file_name, get_current_time_string());
    strcat(file_name, ".txt");

    safe_measurement_to_file(file_name, temp_string);
    safe_measurement_to_file(file_name, temp_string);
    return 0;*/

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

    /************* Open Joystick ********************************/
    joystick = open(JS_DEV0, O_RDONLY);

	if ( joystick < 0)
	{
        joystick = open(JS_DEV1, O_RDONLY);

        if( joystick < 0 )
            perror("jstest"), exit(1);
	}

	fcntl(joystick, F_SETFL, O_NONBLOCK); // non-blocking mode

    /************* Open Keyboard ********************************/
    open_keyboard(&oldKeyboardSettings, &keyboardSettings);

    printf("TERMINAL\n\n");

    printf("Trying to connect to the board...");

    /************* Open Board ***********************************/

    board = open_board(&oldBoardSettings, &boardSettings);

    if( board < 0 )
    {
        printf("Error: connection to board failed.");
        return 1;
    }

    printf("Connection successful!\n\n");

    /************* Create Polling (keep_alive) ********************/ //do we still need it?

    pthread_mutex_init(&lock_board, NULL);

    status = pthread_create(&polling, NULL, is_alive, (void*)&board);

    if( status )
    {
        printf("pc> Error while creating polling thread. Session Aborted.\n");
        ctty = ESC;
    }

    /************* Main Loop ********************************/

    while(js_exit != 1 && ctty != ESC)
    {
        js_exit = set_js_command(joystick); //read the joystick configuration

		ctty = getchar_keyboard();          //read the keyboard

		packet_buffer[packet_counter] = encapsulate( ctty ); //decode the character from keyboard

        if(packet_buffer[packet_counter].command == LOG_GET)
        {
            close_keyboard(&oldKeyboardSettings);

            logging(board, packet_buffer[packet_counter]);

            open_keyboard(&oldKeyboardSettings, &keyboardSettings);

            continue;
        }

		if( packet_buffer[packet_counter].header != EMPTY )  //if it is a valid one, then push it, else ignore
            packet_counter++;

        for(i = 0; i < packet_counter; i++) //send a burst of packets
        {
            packet_t p = packet_buffer[i];

            do //send the same packet until I receive a positive ack
            {
                int counter_waitack = 0;

                //get exclusive access to the board and send the packet
                pthread_mutex_lock( &lock_board );
                send_packet(board, p);
                pthread_mutex_unlock( &lock_board );

                printf("pc> Sending packet %d... ", i);

                mon_delay_ms(REFRESH_TIME);

                //pthread_mutex_lock( &lock_board );

                for(counter_waitack=0; counter_waitack < 5; counter_waitack++)
                    if( (cboard = getchar_board(board)) == ACK ) //wait for acknowledgment
                    {
                        ack_received = getchar_board(board);    //response
                        getchar_board(board);                   //ignore crc
                        break;
                    }
                    else
                        usleep(500);

                if( ack_received == ACK_NEGATIVE )
                    printf("NACK received, trying again...\n"), counter++;
                //pthread_mutex_unlock( &lock_board );
            }
            while( ack_received == ACK_NEGATIVE && counter < 5);

            printf("Command executed correctly.\n");
        }

        empty_packet_t();
        counter = 0;

        if(ctty != ESC)
            system("clear");
    }

    printf("End of communication.\n");

    close_keyboard(&oldKeyboardSettings);
    close_board(board, &oldBoardSettings);
    //does the joystick need closing?

    return 0;
}

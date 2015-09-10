#include <static.h>

void main(){
	char str[100];
	int board;

	printf("Welcome to the communication protocol of this group\n");
	
	//setup conneciton to the board
	printf("Setting up connection...\n");	
	board = setup_connection();
	printf("Connection succesfull!\n");
	
	//get input
	printf("Please enter command to send:\n");
	

	gets( str );

	//led 1 ON
	if(strcmp(str, "LED_ON") == 0){
		send_command(board, SET_LED, 0x17);
	}

	//led 2 OFF
	if(strcmp(str, "LED_OFF") == 0){
		send_command(board, SET_LED, 0x00);
	}
}

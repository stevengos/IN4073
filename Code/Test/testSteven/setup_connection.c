#include "static.h"

/**
*	Setup connection with the board
*	
*	Returns int that holds the refer to the file
*/
int setup_connection(){
	int board;

	//open the board
	board = open(BOARD_PATH, O_RDWR | O_NOCTTY); // open as read&write and not as controlling tty

	//check if the board port exists
	assert( board >= 0 );

	/* setup tty */
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; /* 8 bits-per-character */
	tty.c_cflag |= CLOCAL | CREAD; /* Ignore model status + read input */

        tty.c_iflag = IGNBRK; /* ignore break condition */
        tty.c_iflag &= ~(IXON|IXOFF|IXANY);

        tty.c_oflag = 0;

        tty.c_lflag = 0;

        cfsetospeed(&tty, B115200); /* set output baud rate */
        cfsetispeed(&tty, B115200); /* set input baud rate */

        tty.c_cc[VMIN]  = 0;
        tty.c_cc[VTIME] = 0;

        tcflush(board, TCIOFLUSH); /* flush I/O buffer */
        tcsetattr (board, TCSANOW, &tty); /* non-canonical */
	
	return board;
}

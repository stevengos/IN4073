/**
@author Gianluca Savaia
@version 1.0
@last update 2015-09-11
*/

#include "board.h"

int open_board(struct termios* oldtty, struct termios* tty)
{
    //open the board
    int board = open(BOARD_PATH, O_RDWR | O_NOCTTY ); // open as read&write and not as controlling tty

    if( board < 0 )
        return -1;

    tcgetattr(board, oldtty); /* save current serial port settings */
    //memset(&tty, 0, sizeof(tty)); /* clear struct for new port settings */

    /* setup tty */
        tty->c_cflag = (tty->c_cflag & ~CSIZE) | CS8; /* 8 bits-per-character */
        tty->c_cflag |= CLOCAL | CREAD; /* Ignore model status + read input */

        tty->c_iflag = IGNBRK; /* ignore break condition */
        tty->c_iflag &= ~(IXON|IXOFF|IXANY);

        tty->c_oflag = 0;

        tty->c_lflag = 0;

        cfsetospeed(tty, B115200); /* set output baud rate */
        cfsetispeed(tty, B115200); /* set input baud rate */

        tty->c_cc[VMIN]  = 0;
        tty->c_cc[VTIME] = 0;

        tcflush(board, TCIOFLUSH); /* flush I/O buffer */
        tcsetattr (board, TCSANOW, tty); /* non-canonical */

    while( getchar_board(board) ); //flush

    return board;
}

int close_board(int board, struct termios* oldtty)
{
    tcsetattr(board, TCSADRAIN, oldtty);
    return close(board);
}

char getchar_board(int board)
{
    char stat;
    char c;

    stat = read(board, &c, 1);

    if( !stat )
        return 0;
    else
        return c;
}

int send_command(int board, packet p)
{
    int done = 0;

    do
    {
        done = write(board, &p, sizeof(p));
    }
    while(!done);

    return done;
}

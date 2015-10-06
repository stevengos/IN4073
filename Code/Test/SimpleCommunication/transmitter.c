#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

#include "packet.h"

#define BOARD_PATH "/dev/ttyUSB0"

int send_command(int board)
{
    char label = 0x9;
    char command = 0xF;

    int done = 0;

    packet p;
    p.label = label;
    p.command = command;

    do
    {
        done = write(board, &p, sizeof(p));
    }
    while(!done);

    return done;
}

int main()
{
    struct termios tty;
    struct termios oldtty;

    int board;

    int sent;

    //open the board
    board = open(BOARD_PATH, O_RDWR | O_NOCTTY); // open as read&write and not as controlling tty
    assert( board >= 0 );

    tcgetattr(board, &oldtty); /* save current serial port settings */
    //memset(&tty, 0, sizeof(tty)); /* clear struct for new port settings */

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

    printf("Press any key to send a command.");
    getchar();

    sent = send_command(board);

    printf("Byte sent: %d", sent);

    tcsetattr(board, TCSADRAIN, &oldtty);

    return 0;
}

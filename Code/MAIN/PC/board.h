/**
@author Gianluca Savaia
@version 1.0
@last update 2015-09-11
*/

#ifndef BOARD_H
#define BOARD_H

#include <termios.h>
#include <fcntl.h>

#include "../interface/packet.h"

#define BOARD_PATH "/dev/ttyUSB0"

int open_board(struct termios* oldtty, struct termios* tty);
int close_board(int board, struct termios* oldtty);

int close_board(int board, struct termios* oldtty);
char getchar_board(int board);
int send_command(int board, packet_t p);

#endif // BOARD_H


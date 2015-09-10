#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

#include "../Core/packet.h"

//path to the board
#define BOARD_PATH "/dev/ttyUSB0"

struct termios tty;

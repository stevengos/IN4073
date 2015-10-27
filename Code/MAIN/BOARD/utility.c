/**
@author Gianluca Savaia
This file contains useful functions which logically does not belong to any module
but that they mey be often used all over the code.
*/

#include "utility.h"
#include "../interface/packet.h"

/**
@author Gianluca Savaia
Leds blinks for 'sec' seconds and restore the leds to their previous status
*/
void panic_blink(int sec)
{
    char save_status = X32_LEDS;
    int i;

    for(i=0; i < sec; i++, catnap(500))
            X32_LEDS = ALL_ON, catnap(500), X32_LEDS = ALL_OFF;

    X32_LEDS = save_status;
}
/**
@author Gianluca Savaia
Flush input buffer
*/
void flush_buffer()
{
    while(X32_RS232_READ){ char c = X32_RS232_DATA; }
}

/**
@author Gianluca Savaia
Resync communication between PC and Board
*/
void synchro()
{
    char c;

    while( (c = X32_RS232_DATA) != ALIVE );
    while( (c = X32_RS232_DATA) != EMPTY );
    while( (c = X32_RS232_DATA) != 0x9 );
}

/**
@author Gianluca Savaia
Idle for 'ms' milliseconds. (the built-in function 'sleep' gave us problems)
*/
void catnap(int ms)
{
    int now = X32_CLOCK_MS;

    while( X32_CLOCK_MS < now + ms);
}

/**
@author Gianluca Savaia
Idle for 'us' microseconds. (the built-in function 'usleep' gave us problems)
*/
void ucatnap(int us)
{
    int now = X32_CLOCK_US;

    while( X32_CLOCK_US < now + us);
}

/**
@author Gianluca Savaia
Square root algorithm. The Babylonian method is implemented.
The trick which makes it faster (convergence in 4 iterations on avg) is the starting point
tuned properly for our range of values.
*/
int sqrt(int square)
{
    int prev = 0;
    int diff = MAGIC_NUMBER;
    int result = 0;

    result = square/MAGIC_NUMBER == 0 ? 1 : square/MAGIC_NUMBER;

    for(prev = -MAGIC_NUMBER; diff > 2 || diff < -2; diff = result - prev )
        prev = result, result = (result + square/result) >> 1;

    return result;
}


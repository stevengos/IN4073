/**
@author Gianluca Savaia
*/

#include "utility.h"
#include "../interface/packet.h"


void panic_blink(int sec)
{
    char save_status = X32_LEDS;
    int i;

    for(i=0; i < sec; i++, catnap(500))
            X32_LEDS = ALL_ON, catnap(500), X32_LEDS = ALL_OFF;

    X32_LEDS = save_status;
}

void flush_buffer()
{
    while(X32_RS232_READ){ char c = X32_RS232_DATA; }
}

void synchro()
{
    char c;

    while( (c = X32_RS232_DATA) != ALIVE );
    while( (c = X32_RS232_DATA) != EMPTY );
    while( (c = X32_RS232_DATA) != 0x9 );
}

void catnap(int ms)
{
    int now = X32_CLOCK_MS;

    while( X32_CLOCK_MS < now + ms);
}

void ucatnap(int us)
{
    int now = X32_CLOCK_US;

    while( X32_CLOCK_US < now + us);
}

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


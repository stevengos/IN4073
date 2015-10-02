/**
@author Gianluca Savaia
*/

#include "utility.h"

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
    register int prev;
    register int diff;
    register int result;

    result = square/MAGIC_NUMBER == 0 ? 1 : square/MAGIC_NUMBER;

    for(prev = -MAGIC_NUMBER; diff > 2 || diff < -2; diff = result - prev )
        prev = result, result = (result + square/result) >> 1;

    return result;
}


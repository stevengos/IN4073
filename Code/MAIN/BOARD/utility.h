/**
@author Gianluca Savaia
*/

#ifndef UTILITY_H
#define UTILITY_H

#include "x32.h"
#include "static.h"

#define MAGIC_NUMBER 650

#define BLINK_LED(X) if( X32_CLOCK_MS % 1000 > 750 ) X32_LEDS |= X; else X32_LEDS &= !X;
#define TURNON_LED(X) X32_LEDS |= X;
#define TURNOFF_LED(X) X32_LEDS &= !X;

void flush_buffer(void);
void synchro(void);
void catnap(int);
void ucatnap(int);
int sqrt(int);

#endif // UTILITY_H

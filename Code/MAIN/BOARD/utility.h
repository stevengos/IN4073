/**
@author Gianluca Savaia
*/

#ifndef UTILITY_H
#define UTILITY_H

#include "x32.h"
#include "static.h"

#define MAGIC_NUMBER 650

void flush_buffer(void);
void synchro(void);
void catnap(int);
void ucatnap(int);
int sqrt(int);

#endif // UTILITY_H

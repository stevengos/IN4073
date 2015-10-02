/**
@author Gianluca Savaia
*/

#ifndef LOG_H
#define LOG_H

#include "drone.h"
#include "transmitter.h"
#include <stdlib.h>

#define LOG_SIZE            sizeof(struct log_s)
#define LOG_BUFFER_SIZE     300 //PROBLEM IN ALLOCATING MEMORY: CANNOT ALLOCATE BIG BUFFER (50 still safe)

extern struct drone qr;

struct log_s
{
    int timestamp;

    short start;

    short ae1;
    short ae2;
    short ae3;
    short ae4;

    short sax;
    short say;
    short saz;

    short sp;
    short sq;
    short sr;

    short end;
};

void reset_log(void);
char init_log(void);
void add_log(void);
void upload_log(void);


#endif // LOG_H

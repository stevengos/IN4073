/**
@author Gianluca Savaia
*/

#ifndef LOG_H
#define LOG_H

#include "drone.h"
#include "transmitter.h"
#include <stdlib.h>

#define LOG_SIZE            30//sizeof(struct log_s)
#define LOG_BUFFER_SIZE     40 //PROBLEM IN ALLOCATING MEMORY

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

struct log_s*   log_buffer;
int log_size = 0;

char malloc_memory[LOG_SIZE * LOG_BUFFER_SIZE];
int malloc_memory_size = LOG_SIZE * LOG_BUFFER_SIZE;

void reset_log(void);
char init_log(void);
void add_log(void);
void upload_log(void);


#endif // LOG_H

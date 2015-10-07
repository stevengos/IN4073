/**
@author Gianluca Savaia
*/

#ifndef LOG_H
#define LOG_H

#include "drone.h"
#include "transmitter.h"
#include <stdlib.h>

#define LOG_SIZE            sizeof(struct log_s)
#define LOG_BUFFER_SIZE_KB  10240  //10kB

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

    short fp;
    short fq;
    short fr;

    short end;
};

char init_log(void);
void add_log(void);
void upload_log(void);


#endif // LOG_H

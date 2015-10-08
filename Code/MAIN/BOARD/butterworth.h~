/**
  @author SinaValipoor
*/

#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include "drone.h"
#include "fix_board.c"
#include <stdlib.h>



#define a0 329 //Second-Order Butterworth constants in q14
#define a1 658
#define a2 329
#define b1 -25576
#define b2 10508

#define BUTTER_SIZE 10// 6 * char take 10 to be shure
#define BUTTERWORTH_DATA_BUFFER_SIZE   100 // we save 100 x data in buffer for now

extern struct drone qr;
int Sample=0;

struct butter_s  //Structure that will contain the buffered data
{
   short sp;
   short sq;
   short sr;
};

//struct butter_s butter_buffer[BUTTER_SIZE * BUTTERWORTH_DATA_BUFFER_SIZE];

short x[3][3];
short y[3][3];

void butter_second(void);
void reset_sample(void);
void dest_struct(void);

#endif//BUTTERWORTH_H


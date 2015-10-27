/**
  @author SinaValipoor
*/

#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include "drone.h"
#include "fix_board.c"
#include <stdlib.h>


/* Second-Order Butterworth coefficient in Q14 Sampleing Frequentie = 500Hz, Cutoff @100Hz*/
#define a0 3384 
#define a1 6769
#define a2 3384
#define b1 -6054
#define b2 3208

extern struct drone qr;

int Sample=0;

/*Structure that will contain the buffered data*/
struct butter_s  
{
  
   short int sp;
   short int sq;
   short int sr;
};

short int x[3][3];
short int y[3][3];

void butter_second(void);
#endif


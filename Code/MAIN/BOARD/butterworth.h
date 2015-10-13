/**
  @author SinaValipoor
*/

#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include "drone.h"
#include "fix_board.c"
#include <stdlib.h>



#define a0 3384 //Second-Order Butterworth constants in q14 Sampleing Frequentie = 500Hz, Cutoff @100Hz
#define a1 6769
#define a2 3384
#define b1 -6054
#define b2 3208


int Sample=0;

struct butter_s  //Structure that will contain the buffered data
{
  
   short int sp;
   short int sq;
   short int sr;
};

short x[3][3];
short y[3][3];

void butter_second(void);
#endif//BUTTERWORTH_H


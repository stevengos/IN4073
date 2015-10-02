#include <stdio.h>
#include <stdlib.h>
#include "fix_board.c"
#include "x32.h"
#include "assert.h"

/* define some peripheral short hands
 */
#define X32_display	peripherals[PERIPHERAL_DISPLAY]
#define X32_leds	peripherals[PERIPHERAL_LEDS]
#define X32_buttons	peripherals[PERIPHERAL_BUTTONS]
#define X32_clock	peripherals[PERIPHERAL_MS_CLOCK]


#define a0 329 //Second-Order Butterworth constants in q14
#define a1 658 
#define a2 329
#define b1 -25576
#define b2 10508


int data1[97] = {517, 524, 497, 411, 519, 412, 516, 524, 497, 411, 519, 412, 517, 524, 497, 411, 519, 412, 516, 524, 497, 411, 519, 412, 516, 524, 497, 411, 519, 413, 517, 524, 497, 411, 519, 412, 516, 524, 497, 411, 519, 412, 516, 524, 497, 411, 519, 412, 517, 524, 497, 411, 519, 412, 516, 524, 497, 411, 519, 413, 517, 524, 497, 411, 519, 412, 517, 524, 497, 411, 519, 412, 517, 525, 497, 411, 519, 413, 517, 525, 429, 411, 519, 412, 517, 524, 497, 411, 519, 412, 517, 525, 497, 411, 519, 413}; 
int y[97];
int x[97];
int NumbSamples;

void butterworth( int data[],  int y[], int length);
/*Import floattofix function *********/
q14 normal2q ( int x);
int  q2normal(q14 a);
q14 q_multiplication (q14 a, q14 b);

int  main(int argc, char **argv)
{

  
 int i;
 NumbSamples = 0;  
 
  /*if((y = (int *) malloc( 97 * sizeof(int))) == NULL)
    {
        printf(stderr, "Error allocating the buffer for y.\n");
        exit(1);
    }*/
  
  
  

   /****perform butter on sample data */
  butterworth(data1, y, 97);

  for ( i =0; i< 97; i++){
        printf("The data value is equal to %d and the filterd one is %d\n", data1[(96-i)], y[i]);
    }
   printf("In total we have %d samples\n", NumbSamples);

  //free(y);

  return 0;
}












void butterworth( int data[],  int y[], int length){
 
 int i, j; 
 int x0 ,x1;
 int x2, y1, y2;

 if((length == 0))
   {
    printf(stderr, "Data length is zero!\n");
    exit(1);
   }
 NumbSamples = 0;
 

 /*if((x = (int *) malloc( length * sizeof(int))) == NULL)
    {
        printf(stderr, "Error allocating the buffer for x.\n");
        exit(1);
    }*/
 

 /* initialize history
         */
  for (i = 0; i < length; i++) {
		x[i] = 0;
		y[i] = 0;
	}

 
/**Read the data and perform the filtering *****/
  for (i=0; i<length; i++){
  
      /* Update sample history */
     for (j=length-1; j>0; j--){
          x[j]=x[j-1];
          y[j]=y[j-1];
       }
      /*Read the next data */
      x[0]=data[i];

 /***********Second-order Butterworth filter 10Hz Lp @ 200HZ sample rate
  ***********Also using fix point arthimetics***************************/

       x0 = x[0];
       x1 = x[1];
       x2 = x[2];
       y1 = y[1];
       y2 = y[2];
/********Calculate output *******/
/********The q_multi already convert back the values******/
       y[0]  = (q_multiplication(a0,(q14)x0)+q_multiplication(a1,(q14)x1)+q_multiplication(a2,(q14)x2)-q_multiplication(b1,(q14)y1)-q_multiplication(b2,(q14)y2));
     
      NumbSamples++;
  }

  //free(x);
}

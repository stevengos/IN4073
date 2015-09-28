/***********This is second order butterworth filter in c **************************************
 ***********part of the QR design for the course In4073 2015/2016 , group 5 ******************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "fix.c" 

#define  WINLENGTH 20 // correspondens with input sample length

int NumbSamples;

double data1[WINLENGTH] ={0.02, 1.2, 0.3, 1.2, 2.3, 0.65, 0.23, 2.3, 0.75, 0.46, 0.66, 0.32, 0.15, 0.56, 1.33, 0.211, 0.11, 3.1, 0.3, 0.5}; 



void butterworth( double data[],  double *x,  double *y, int length);
/*Import floattofix function *********/
q14 normal2q ( double x);
 double q2normal(q14 a);
q14 q_multiplication (q14 a, q14 b);

int  main(int argc, char **argv)
{

 double *x, *y;
 int i;
 NumbSamples = 0;  
 
 if((x = (double *) malloc( WINLENGTH * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Error allocating the buffer for x.\n");
        exit(1);
    }
  if((y = (double *) malloc( WINLENGTH * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Error allocating the buffer for y.\n");
        exit(1);
    }
  
  /* initialize history
         */
  for (i = 0; i < WINLENGTH; i++) {
		x[i] = 0;
		y[i] = 0;
	}

   /****perform butter on sample data */
  butterworth(data1, x, y, WINLENGTH);

  for ( i =0; i< 20; i++){
        printf("The data value is equal to %f and the filterd one is %f\n", data1[(19-i)], y[i]);
    }
   printf("In total we have %d samples\n", NumbSamples);

  return 1;
}


void butterworth( double data[],  double *x,  double *y, int length){

 int i, j;

 
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

      q14 a0 = normal2q(0.020083372); 
      q14 a1 = normal2q(0.040166743); 
      q14 a2 = normal2q(0.020083372); 
      q14 b1 = normal2q(-1.5610181); 
      q14 b2 = normal2q(0.6413515); 
      q14 x0 = normal2q(x[0]);
      q14 x1 = normal2q(x[1]);
      q14 x2 = normal2q(x[2]);
      q14 y1 = normal2q(y[1]);
      q14 y2 = normal2q(y[2]);
/********Calculate output *******/
      q14 y0 = q_multiplication(a0,x0)+q_multiplication(a1,x1)+q_multiplication(a2,x2)-q_multiplication(b1,y1)-q_multiplication(b2,y2);
      y[0] = q2normal(y0);
      NumbSamples++;
  }

  
}
      







     

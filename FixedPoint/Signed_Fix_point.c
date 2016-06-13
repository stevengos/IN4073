#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Q 14
#define K (1 << (Q - 1))
typedef  int q14;
int normal2q (float x);
q14 q_multiplication (q14 a, q14 b);
float q2normal(q14 a);
q14 q_division (q14 a, q14 b);
q14 q_add(q14 a, q14 b);
q14 q_subtract(q14 a, q14 b);


int main(int argc, char *argv[])
{

  float x,y;
  q14 res;

  if(argc < 3)
 {
 float x = -2.1477;
 float y = 1.533;
 }
   else{
  x = atof(argv[1]);
  y = atof(argv[2]);
 }
 q14 first = normal2q(x);
 q14 second = normal2q(y);
 res = q_multiplication(first,second);
 /*if (strcmp(argv[3],'a') == 0){
        res = q_add(first,second);
    }
 else if(argv[3] == 's'){
        res = q_subtract(first,second);
    }
 else if(argv[3] == "m"){
           res = q_multiplication(first,second);
    }
 else if(argv[3] == "d"){
           res = q_division(first,second);
    }
 else {
          printf(" Op = %s , No operation or wrong operation defined try again\n", argv[3]);
          exit(1);
   } */
        
float result = q2normal(res);

printf("The value float x = %f, float y = %f, x turned to q14 = %d, y turned to q14 =%d, q14 mult result = %d and the result is = %f\n",x ,y , first, second, res, result);

return 1;
}



/***** this function receives a float and 
 turn it to q14 value*********/

q14 normal2q (float x) {   
 int result;
 /******Second edition-board *****
 result = x * (1 << Q);*/

 
 
 /******First edition*/ 
 float y = pow(2,Q); 
 float d = y*x;
 /*r**Rounding to the closest int near the float value*/ 
 int integerPart = floor(d);//return int value smaller than d. 
 if((d-(float)integerPart)>= 0.5){
    result = integerPart+1;
  }
 else {
    result = integerPart;
  } 
   
 return result;
}

/******this function receives a q14 value
 and turn it into floating point real value************/

 float q2normal(q14 a) {
 float result;
/******Second edition-board ** 
 result = ((float) a) / (1 << Q);*/

/***First edition*/
 float y = pow(2,Q);
 float x = a;
 result = x/y; 
 return (result);
}

/*********this function adds two q14 numbers*********/

q14 q_add(q14 a, q14 b){
 q14 result;
 long tmp;
 tmp = (long)a + (long)b;
 //Add saturation
 if (tmp > 0x7FFF){
    tmp = 0x7FFF;
   }
 if (tmp < -1 * 0x7FFF){
    tmp = -1 * 0x7FFF;
   }
 result = (q14) tmp;
 return result;
}

/***********this function subtract two q14 numbers*******/
q14 q_subtract(q14 a, q14 b){
  q14 result;
  result = a - b;
  return result;
}
 
 
 
 /*********this function multiplies 2 x float numbers**/

q14 q_multiplication (q14 a, q14 b){
    q14 result;
    long temp;
    temp = (long)a * (long)b; 
    temp += K;
    // Correct by dividing by base
    result = (q14)(temp >> Q);
    return result;
}

 /***********this function performs the division *************/
q14 q_division (q14 a, q14 b){

	 q14 result;
	 long temp;
	 temp=(long)a << Q;
	// Rounding: mid values are rounded up.
         if ((temp >= 0 && b >= 0) || (temp < 0 && b < 0))
            temp += b / 2;
         else
            temp -= b / 2;
         result = (q14)(temp / b);
	 return result;
}

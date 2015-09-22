#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Q 14
#define K (1 << (Q - 1))
typedef unsigned short int q88;
unsigned short int normal2q (float x);
q88 q_multiplication (q88 a, q88 b);
float q2normal(q88 a);
q88 q_division (q88 a, q88 b);
q88 q_add(q88 a, q88 b);
q88 q_subtract(q88 a, q88 b);


int main(int argc, char *argv[])
{
 //the values 3.75, 3.90625
 float x = 2.1477;
 float y = 1.533;
 q88 first = normal2q(x);
 q88 second = normal2q(y);
 q88 multRes = q_division(first,second);
 float result = q2normal(multRes);

printf("The value float x = %f, float y = %f, x turned to q88 = %d, y turned to q88 =%d, q88 mult result = %d and the result is = %f",x ,y , first, second, multRes, result);

return 1;
}



/***** this function recieves a float and 
 turn it to q88 value*********/

unsigned short int normal2q (float x) {   
 int result;
 float y = pow(2,Q); 
 float d = y*x;
 /*r**Rounding to the closest int near the float value */
 int integerPart = floor(d);//return int value smaller than d. 
 if((d-(float)integerPart)>= 0.5){
    result = integerPart+1;
  }
 else {
    result = integerPart;
  }
   
 return result;
}

/******this function recieves a q88 value
 and turn it into floating point real value************/

float q2normal(q88 a) {
 float y = pow(2,Q);
 float x = a;
 return (x/y);
}

/*********this function adds two q88 numbers*********/

q88 q_add(q88 a, q88 b){
 q88 result;
 unsigned long tmp;
 tmp = (long)a + (long)b;
 //Add saturation
 if (tmp > 0xFFFFFFFF){
    tmp = 0xFFFFFFFF;
   }
 result = (q88) tmp;
 return result;
}

/***********this function subtract two q88 numbers*******/
q88 q_subtract(q88 a, q88 b){
  q88 result;
  result = a - b;
  return result;
}
 
 
 
 /*********this function multiplies 2 x float numbers**/

q88 q_multiplication (q88 a, q88 b){
    q88 result;
    unsigned long temp;
    temp = (unsigned long)a * (unsigned long)b; 
    temp += K;
    // Correct by dividing by base
    result = (q88)(temp >> Q);
    return result;
}

 /***********this function performs the division *************/
q88 q_division (q88 a, q88 b){

	 q88 result;
	 unsigned long temp;
	 temp=(unsigned long)a << Q;
	// Rounding: mid values are rounded up.
	 temp += b / 2;
	 result = (q88)(temp / b);
	 return result;
}


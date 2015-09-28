
#include "fix.h"

q14 normal2q (double x) {   
 int result;
 /******Second edition-board *****
 result = x * (1 << Q);*/

 /******First edition*/ 
 double y = pow(2,Q); 
 double d = y*x;
 /*r**Rounding to the closest int near the double value*/ 
 int integerPart = floor(d);//return int value smaller than d. 
 if((d-(double)integerPart)>= 0.5){
    result = integerPart+1;
  }
 else {
    result = integerPart;
  } 
   
 return result;
}


 double q2normal(q14 a) {
 double result;
/******Second edition-board ** 
 result = ((double) a) / (1 << Q);*/

/***First edition*/
 double y = pow(2,Q);
 double x = a;
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
 
 
 
 /*********this function multiplies 2 x double numbers**/

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



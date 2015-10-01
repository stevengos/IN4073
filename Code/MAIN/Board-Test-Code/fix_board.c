
#include "fix_board.h"

q14 normal2q (int x) {   
 int result;
 /******Second edition-board ******/
 result = x * (1 << Q);
   
 return result;
}


 int q2normal(q14 a) {
 int result;
/******Second edition-board ***/ 
 result =  a / (1 << Q);
 return result;
}

/*********this function adds two q14 numbers*********/

q14 q_add(q14 a, q14 b){
 q14 result;
 int tmp;
 tmp = a + b;
 /*Add saturation
 if (tmp > 0x7FFF){
    tmp = 0x7FFF;
   }
 if (tmp < -1 * 0x7FFF){
    tmp = -1 * 0x7FFF;
   }*/
 result = tmp;
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
    int temp;
    temp = a * b; 
    temp += K;
    // Correct by dividing by base
    result = temp >> Q;
    return result;
}

 /***********this function performs the division *************/
q14 q_division (q14 a, q14 b){

	 q14 result;
	 int temp;
	 temp=a << Q;
	// Rounding: mid values are rounded up.
         if ((temp >= 0 && b >= 0) || (temp < 0 && b < 0))
            temp += b / 2;
         else
            temp -= b / 2;
         result = temp / b;
	 return result;
}

/************************This function calculates Square root ***************/
q14 q_sqrt (q14 a){
  q14 result =0;
  int i;
  for (i =0; i<15; i++){
      result=(1/2)*(result+result/a);
      }
   return  result;
}
          



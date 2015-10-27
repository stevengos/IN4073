/**
  @author SinaValipoor
*/

#include "fix_board.h"
/***This function converts int value to q14 - board side ***/
q14 normal2q (int x) {   
 int result;
 result = x * (1 << Q);
   
 return result;
}

/***this function converts q14 value back to int ***/
 int q2normal(q14 a) {
	 int result;
	 result =  a / (1 << Q);
	 return result;
}

/***this function adds two q14 numbers***/

q14 q_add(q14 a, q14 b){
 q14 result;
 int tmp;
 tmp = (int)a + (int)b;
 /*Add saturation*/
 if (tmp > 0x7FFF){
    tmp = 0x7FFF;
   }
 if (tmp < -1 * 0x7FFF){
    tmp = -1 * 0x7FFF;
   }
 result = (q14) tmp;
 return result;
}

/***this function subtracts two q14 numbers***/
q14 q_subtract(q14 a, q14 b){
  q14 result;
  result = a - b;
  return result;
}
 
 
 
 /***this function multiplies two q14 numbers***/

q14 q_multiplication (q14 a, q14 b){
    q14 result;
    int temp;
    temp = a * b; 
    temp += K;
    /* Correct by dividing by base*/
    result = (q14)(temp >> Q);
    return result;
}

 /***this function performs the division between two q14 numbers ***/
q14 q_division (q14 a, q14 b){

	 q14 result;
	 int temp;
	 temp=(int)a << Q;
	/*Rounding: mid values are rounded up*/
         if ((temp >= 0 && b >= 0) || (temp < 0 && b < 0))
            temp += b / 2;
         else
            temp -= b / 2;
         result = (q14)(temp / b);
	 return result;
}

        



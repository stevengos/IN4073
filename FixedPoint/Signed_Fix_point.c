#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Q 14
#define K (1 << (Q - 1))
typedef  int q88;
int normal2q (float x);
q88 q_multiplication (q88 a, q88 b);
float q2normal(q88 a);
q88 q_division (q88 a, q88 b);
q88 q_add(q88 a, q88 b);
q88 q_subtract(q88 a, q88 b);


int main(int argc, char *argv[])
{

  if(argc < 3)
 {
   printf("error usage %s float1    float2  ", argv[0]);
   exit(1);
 }
  q88 res;
 /*the values 3.75, 3.90625
 float x = -2.1477;
 float y = 1.533; */
 float x = atof(argv[1]);
 float y = atof(argv[2]);
 q88 first = normal2q(x);
 q88 second = normal2q(y);
 
 //here you can define the operation type
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

printf("The value float x = %f, float y = %f, x turned to q88 = %d, y turned to q88 =%d, q88 mult result = %d and the result is = %f\n",x ,y , first, second, res, result);

return 1;
}



/***** this function recieves a float and 
 turn it to q88 value*********/

q88 normal2q (float x) {   
 int result;
 
 /***********X32Version**************
 long d = (long) x << Q;
 result = (int) d
 */
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
 /************X32Version************
 long x = (long) a >> Q;
 return x;
 */
 float y = pow(2,Q);
 float x = a;
 return (x/y);
}

/*********this function adds two q88 numbers*********/

q88 q_add(q88 a, q88 b){
 q88 result;
 long tmp;
 tmp = (long)a + (long)b;
 //Add saturation
 if (tmp > 0x7FFF){
    tmp = 0x7FFF;
   }
 if (tmp < -1 * 0x7FFF){
    tmp = -1 * 0x7FFF;
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
    long temp;
    temp = (long)a * (long)b; 
    temp += K;
    // Correct by dividing by base
    result = (q88)(temp >> Q);
    return result;
}

 /***********this function performs the division *************/
q88 q_division (q88 a, q88 b){

	 q88 result;
	 long temp;
	 temp=(long)a << Q;
	// Rounding: mid values are rounded up.
         if ((temp >= 0 && b >= 0) || (temp < 0 && b < 0))
            temp += b / 2;
         else
            temp -= b / 2;
         result = (q88)(temp / b);
	 return result;
}


/**
  @author SinaValipoor
*/


#include "butterworth.h"

short i, j;
short x0_1, x0_2, x0_3;
short x1_1, x1_2, x1_3;
short x2_1, x2_2, x2_3;
short y1_1, y1_2, y1_3;
short y2_1, y2_2, y2_3;

void butter_second()
{
    struct butter_s new_data;

    /**update the history**/

    for (j=2; j>0; j--){
          for (i=0;i<3;i++){
              x[j][i]=x[j-1][i];
              y[j][i]=y[j-1][i];
            }
       }
    /**read data ******/
    x[0][0] = qr.sp;
    x[0][1] = qr.sq;
    x[0][2] = qr.sr;

    /*readin the variables ***/
    x0_1 = x[0][0];
    x0_2 = x[0][1];
    x0_3 = x[0][2];
    x1_1 = x[1][0];
    x1_2 = x[1][1];
    x1_3 = x[1][2];
    x2_1 = x[2][0];
    x2_2 = x[2][1];
    x2_3 = x[2][2];
    y1_1 = y[1][0];
    y1_2 = y[1][1];
    y1_3 = y[1][2];
    y2_1 = y[2][0];
    y2_2 = y[2][1];
    y2_3 = y[2][2];
    /********Calculate the values *****/

    y[0][0] = (q_multiplication(a0,(q14)x0_1)+q_multiplication(a1,(q14)x1_1)+q_multiplication(a2,(q14)x2_1)-q_multiplication(b1,(q14)y1_1)-q_multiplication(b2,(q14)y2_1));
    y[0][1] = (q_multiplication(a0,(q14)x0_2)+q_multiplication(a1,(q14)x1_2)+q_multiplication(a2,(q14)x2_2)-q_multiplication(b1,(q14)y1_2)-q_multiplication(b2,(q14)y2_2));
    y[0][2] = (q_multiplication(a0,(q14)x0_3)+q_multiplication(a1,(q14)x1_3)+q_multiplication(a2,(q14)x2_3)-q_multiplication(b1,(q14)y1_3)-q_multiplication(b2,(q14)y2_3));


    /********Write the results to structure ******/
    new_data.sp = y[0][0];
    new_data.sq = y[0][1];
    new_data.sr = y[0][2];

//    /**Write the data in buffer */
//    butter_buffer[Sample] = new_data;
//    Sample++;

    qr.fp = new_data.sp;
    qr.fq = new_data.sq;
    qr.fr = new_data.sr;
}


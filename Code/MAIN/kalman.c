/**
  @author SinaValipoor
*/

/*************************kalman filter for QR ********************************/
#include "Kalman.h"




void kalman_setvar_init (struct kalman filter_coefficient){

  filter_coefficient.Q_angle = 0;
  filter_coefficient.Q_bias = 0;
  filter_coefficient.R_measure = 0;

  filter_coefficient.angle =0; //Reset angle
  filter_coefficient.bias = 0; //Reset bias

  /* fil initial values **/
 filter_coefficient.p[0][0] = 0; //initial angle
 filter_coefficient.p[0][1] = 0;
 filter_coefficient.p[1][0] = 0;
 filter_coefficient.p[1][1] = 0; //initial gyroscope bias
};

int kalmanfilter_filter(struct kalman filter_coefficient, int new_angle, int new_rate, int dt){

 /**Step 1***/
 /***predict current state variable ***/
 /***Angle and gyroscope bias***/
 filter_coefficient.rate = newRate - filter_coefficient.bias;
 filter_coefficient.angle += q_multiplication( dt , filter_coefficient.rate);

 /***Step 2***/
 /****calculate/update the error covariance matrix P ****/
  filter_coefficient.P[0][0] += q_multiplication(dt, (   q_multiplication(dt,filter_coefficient.P[1][1]) - filter_coefficient.P[0][1] - filter_coefficient.P[1][0] + filter_coefficient.Q_angle) );
  filter_coefficient.P[0][1] -= q_multiplication(dt ,filter_coefficient.P[1][1]);
  filter_coefficient.P[1][0] -= q_multiplication(dt , filter_coefficient.P[1][1]);
  filter_coefficient.P[1][1] += q_multiplication(filter_coefficient.Q_bias, dt);

 /***Step 3**/
 /***calculate the inovation (difference between the angles)*******/
  int y = newAngle - filter_coefficient.angle; // Angle difference

 /***Step 4***/
 /**Estimate the error ****/
  int S = filter_coefficient.P[0][0] + filter_coefficient.R_measure;

 /***Step 5***/
 /***Calculate the kalman gain 2x1 ***/
  int K[2];
  K[0] = filter_coefficient.P[0][0] / S;
  K[1] = filter_coefficient.P[1][0] / S;

 /***Step 6***/
 /***Calculate/Update the state variable ***/
  filter_coefficient.angle += K[0] * y;
  filter_coefficient.bias += K[1] * y;

 /***Step 7***/
 /***Update the error covariance P ***/
  int P00_temp = filter_coefficient.P[0][0];
  int P01_temp = filter_coefficient.P[0][1];
  filter_coefficient.P[0][0] -= K[0] * P00_temp;
  filter_coefficient.P[0][1] -= K[0] * P01_temp;
  filter_coefficient.P[1][0] -= K[1] * P00_temp;
  filter_coefficient.P[1][1] -= K[1] * P01_temp;

 /**Done return the angle ***/
  return filter_coefficient.angle;
}

/**Setup starting angle and bias ****/
void kalman_setAngle(kalman kalman, int angle ){
 kalman.angle = angle;
 angleSet=true;
}
void kalman_setBias(kalman kalman, int bias){
 kalman.bias = bias;
 biasSet=true;
}
int kalman_getRate(kalman kalman){
  return kalman.rate;
 }

/**Tune the filter****/
void kalman_setQangle(kalman kalman, int Qangle){
 kalman.Q_angle = Qangle;
 Q_angleSet=true;
}
void kalman_setQbias(kalman kalman, int Qbias){
 kalman.Q_bias = Qbias;
 Q_biasSet=true;
}
void kalman_setRmeasure(kalman kalman, int Rmeasure){
 kalman.R_measure = Rmeasure;
 R_measureSet=true;
}





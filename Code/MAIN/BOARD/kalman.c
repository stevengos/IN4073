/**
  @author SinaValipoor
*/

/*************************kalman filter for QR ********************************/
#include "kalman.h"




void kalman_setvar_init (kalman filter_coefficient){
  = (day == WEEKEND) ? 12 : 9;
  filter_coefficient.Q_angle = (Q_angleSet==true) ? filter_coefficient.Q_angle : 0.001f;
  filter_coefficient.Q_bias = (Q_biasSet==true) ? filter_coefficient.Q_bias : 0.003f;
  filter_coefficient.R_measure = (R_measureSet==true) ? filter_coefficient.R_measure : 0.03f;

  filter_coefficient.angle = (angleSet==true) ? filter_coefficient.angle : 0.0f; //Reset angle
  filter_coefficient.bias = (biasSet==true) ? filter_coefficient.bias : 0.0f; //Reset bias 

  /* fil initial values **/
 filter_coefficient.p[0][0] = 0.0f; //initial angle
 filter_coefficient.p[0][1] = 0.0f;
 filter_coefficient.p[1][0] = 0.0f;
 filter_coefficient.p[1][1] = 0.0f; //initial gyroscope bias
};

float kalmanfilter_filter(kalman filter_coefficient, float new_angle, float new_rate, float dt){

 /**Step 1***/
 /***predict current state variable ***/
 /***Angle and gyroscope bias***/
 filter_coefficient.rate = newRate - filter_coefficient.bias;
 filter_coefficient.angle += dt * filter_coefficient.rate;

 /***Step 2***/
 /****calculate/update the error covariance matrix P ****/
  filter_coefficient.P[0][0] += dt * (dt*filter_coefficient.P[1][1] - filter_coefficient.P[0][1] - filter_coefficient.P[1][0] + filter_coefficient.Q_angle);
  filter_coefficient.P[0][1] -= dt * filter_coefficient.P[1][1];
  filter_coefficient.P[1][0] -= dt * filter_coefficient.P[1][1];
  filter_coefficient.P[1][1] += filter_coefficient.Q_bias * dt;

 /***Step 3**/
 /***calculate the inovation (difference between the angles)*******/
  float y = newAngle - filter_coefficient.angle; // Angle difference

 /***Step 4***/
 /**Estimate the error ****/
  float S = filter_coefficient.P[0][0] + filter_coefficient.R_measure;

 /***Step 5***/
 /***Calculate the kalman gain 2x1 ***/
  float K[2];
  K[0] = filter_coefficient.P[0][0] / S;
  K[1] = filter_coefficient.P[1][0] / S;

 /***Step 6***/
 /***Calculate/Update the state variable ***/
  filter_coefficient.angle += K[0] * y;
  filter_coefficient.bias += K[1] * y;

 /***Step 7***/
 /***Update the error covariance P ***/
  float P00_temp = filter_coefficient.P[0][0];
  float P01_temp = filter_coefficient.P[0][1];
  filter_coefficient.P[0][0] -= K[0] * P00_temp;
  filter_coefficient.P[0][1] -= K[0] * P01_temp;
  filter_coefficient.P[1][0] -= K[1] * P00_temp;
  filter_coefficient.P[1][1] -= K[1] * P01_temp;

 /**Done return the angle ***/
  return filter_coefficient.angle;
}

/**Setup starting angle and bias ****/
void kalman_setAngle(kalman kalman, float angle ){
 kalman.angle = angle;
 angleSet=true;
}
void kalman_setBias(kalman kalman, float bias){
 kalman.bias = bias;
 biasSet=true;
}
float kalman_getRate(kalman kalman){
  return kalman.rate;
 }

/**Tune the filter****/
void kalman_setQangle(kalman kalman, float Qangle){
 kalman.Q_angle = Qangle;
 Q_angleSet=true;
}
void kalman_setQbias(kalman kalman, float Qbias){
 kalman.Q_bias = Qbias;
 Q_biasSet=true;
}
void kalman_setRmeasure(kalman kalman, float Rmeasure){
 kalman.R_measure = Rmeasure;
 R_measureSet=true;
}


 
 

/**
  @author SinaValipoor
*/

#ifndef KALMAN_H
#define KALMAN_H

/*#include "drone.h"
#include "fix_board.c"*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool Q_angleSet=false;
bool Q_biasSet=false;
bool R_measureSet=false;
bool biasSet=false;
bool angleSet=false;
struct kalman //structure that keeps the tune variables 
{
 
 float Q_angle;
 float Q_bias;
 float R_measure; // Measurement noise variance - this is actually the variance of the measurement noise
 float angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
 float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
 float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate
 float P[2][2]; // Error covariance matrix - This is a 2x2 matrix
};



#endif

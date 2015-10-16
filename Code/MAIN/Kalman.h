/**
  @author SinaValipoor
*/

#ifndef KALMAN_H
#define KALMAN_H

/*#include "drone.h"
#include "fix_board.c"*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct kalman //structure that keeps the tune variables
{

 int Q_angle;
 int Q_bias;
 int R_measure; // Measurement noise variance - this is actually the variance of the measurement noise
 int angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
 int bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
 int rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate
 int P[2][2]; // Error covariance matrix - This is a 2x2 matrix
};



#endif

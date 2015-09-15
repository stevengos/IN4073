/**
@author Gianluca Savaia
@last update 2015-09-15
*/

#ifndef DRONE_H
#define DRONE_H

#include "x32.h"
#include "static.h"

#define REFRESH_TIME 1

#define STEP_PITCH      10
#define STEP_ROLL       10
#define STEP_YAW        10
#define STEP_LIFT       10
#define STEP_YAWRATE    10

#define MAX_RPM     0x03ff
#define MIN_RPM     0

#define PANIC_RPM 50
#define PANIC_TIME 5000

struct drone
{
    //setpoints for motor
    char ae1;
    char ae2;
    char ae3;
    char ae4;

    //setpoints for configuration
    char pitch;
    char roll;
    char yawrate;
    char lift;

    //sensed data
    char sax;
    char say;
    char saz;
    char sp;
    char sq;
    char sr;

    //flags
    char current_mode;
    char flag_mode;
    char exit;
};

void run_drone();
void clear_drone();
void delay(int);

void safe_mode(void);
void panic_mode(void);
void manual_mode(void);
void calibration_mode(void);
void yaw_mode(void);
void full_mode(void);

#endif // DRONE_H

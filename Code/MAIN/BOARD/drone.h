/**
@author Gianluca Savaia
*/

#ifndef DRONE_H
#define DRONE_H

#include "x32.h"
#include "static.h"
#include "utility.h"
#include "../interface/packet.h"

#define MAX_RPM         0x3ff
#define MIN_RPM         0x00a
#define STEP_RPM        0x00f
#define MOTOR_REFRESH   200

#define MAX_PITCH   127
#define MAX_ROLL    127
#define MAX_YAWRATE 127
#define MAX_LIFT    255

#define MIN_PITCH   -127
#define MIN_ROLL    -127
#define MIN_YAWRATE -127
#define MIN_LIFT    0

#define STEP_PITCH  5
#define STEP_ROLL   5
#define STEP_YAW    10
#define STEP_LIFT   5

#define STEP_SCALE_PARAMETER 250

#define PANIC_RPM   300
#define PANIC_TIME  2000

struct drone
{
    //setpoints for motor
    short ae1;
    short ae2;
    short ae3;
    short ae4;

    //openloop control input
    int pitch_momentum;
    int roll_momentum;
    int yaw_momentum;
    int lift_force;

    //references for control mode
    short pitch_ref;
    short roll_ref;
    short yawrate_ref;

    //parameters Manual Mode
    short scale_pitch;
    short scale_roll;
    short scale_yaw;
    short scale_lift;

    short off_pitch;
    short off_roll;
    short off_yawrate;
    short off_lift;

    //Parameter Yaw Mode
    int controller_pitch;
    int controller_roll;
    int controller_yaw;

    //sensed data
    short sax;
    short say;
    short saz;
    short sp;
    short sq;
    short sr;

    //filtered data
    short fax;
    short fay;
    short faz;
    short fp;
    short fq;
    short fr;

    //sensor offsets
    short off_ax;
    short off_ay;
    short off_az;
    short off_p;
    short off_q;
    short off_r;

    //flags
    char current_mode;
    char flag_mode;
    char exit;
    char link_down;

    char log;
    char log_full;
};

void run_drone();
void clear_drone();

void safe_mode(void);
void panic_mode(void);
void manual_mode(void);
void calibration_mode(void);
void yaw_mode(void);
void full_mode(void);

void stop_motors(void);

#endif // DRONE_H

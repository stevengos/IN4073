/**
@author Gianluca Savaia
*/

#ifndef DRONE_H
#define DRONE_H

#include "x32.h"
#include "static.h"
#include "utility.h"
#include "../interface/packet.h"

#define MAX_RPM     0x3ff
#define MIN_RPM     0x00a
#define STEP_RPM    0x010
#define MOTOR_REFRESH   200

#define MAX_PITCH   255
#define MAX_ROLL    255
#define MAX_YAWRATE 255
#define MAX_LIFT    255

#define MIN_PITCH   -255
#define MIN_ROLL    -255
#define MIN_YAWRATE -255
#define MIN_LIFT    0

#define STEP_SCALE_PARAMETER 1000

#define PANIC_RPM   200
#define PANIC_TIME  5000

struct drone
{
    //setpoints for motor
    short ae1;
    short ae2;
    short ae3;
    short ae4;

    //openloop control input
    short pitch_momentum;
    short roll_momentum;
    short yaw_momentum;
    short lift_force;

    //references for control mode
    short pitch_ref;
    short roll_ref;
    short yawrate_ref;
    short lift_ref;

    //parameters Manual Mode
    short scale_pitch;
    short scale_roll;
    short scale_yaw;
    short scale_lift;

    short step_pitch;
    short step_roll;
    short step_yawrate;
    short step_lift;

    //Parameter Yaw Mode
    short controller_pitch;
    short controller_roll;
    short controller_yaw;

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

/**
@author Gianluca Savaia
*/

#ifndef DRONE_H
#define DRONE_H

#include "x32.h"
#include "static.h"

#define REFRESH_TIME 10

#define STEP_PITCH      10
#define STEP_ROLL       10
#define STEP_YAW        10
#define STEP_LIFT       10
#define STEP_YAWRATE    10

#define MAX_RPM     0x3ff
#define MIN_RPM     0x000
#define STEP_RPM    0x010

#define MAX_PITCH   200000
#define MAX_ROLL    200000
#define MAX_YAWRATE 200000
#define MAX_LIFT    200000

#define MIN_PITCH   -200000
#define MIN_ROLL    -200000
#define MIN_YAWRATE -200000
#define MIN_LIFT    0

#define PANIC_RPM 50
#define PANIC_TIME 5000

struct drone
{
    //setpoints for motor
    short ae1;
    short ae2;
    short ae3;
    short ae4;

    //setpoints for configuration
    short pitch;
    short roll;
    short yawrate;
    short lift;

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
    char link_down;
    char log;

    //logs buffer
    int log_buffer[LOG_BUFFER_SIZE];
    int log_size;
};

void run_drone();
void clear_drone();
void print_drone();
void add_log();

void safe_mode(void);
void panic_mode(void);
void manual_mode(void);
void calibration_mode(void);
void yaw_mode(void);
void full_mode(void);

void stop_motors(void);

#endif // DRONE_H

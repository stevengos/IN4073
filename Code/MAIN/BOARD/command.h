/**
@author Gianluca Savaia
*/

#ifndef COMMAND_H
#define COMMAND_H

#include "../interface/packet.h"
#include "../interface/hamming.h"

#include "drone.h"
#include "isr.h"
#include "transmitter.h"
#include "log.h"

void perform_command(char,char);
void acknowledge(char);

//machine settings
void set_mode(char);
void stop();
void set_log(char);
void upload_log();

//parameters setting
void set_scale_pitch(char);
void set_scale_roll(char);
void set_scale_yaw(char);
void set_scale_lift(char);

void set_controller_pitch(char command);
void set_controller_roll(char command);
void set_controller_yaw(char command);

// interface used by joystick
void set_pitch(char);
void set_roll(char);
void set_lift(unsigned char);
void set_yawrate(char);

// interface used by keyboard
void d_pitch(char);
void d_roll(char);
void d_lift(char);
void d_yawrate(char);

void set_led(char);

#endif // COMMAND_H

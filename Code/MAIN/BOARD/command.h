#ifndef COMMAND_H
#define COMMAND_H

void perform_command(char,char);

void set_mode(char);

// interface used by joystick
void set_pitch(char);
void set_roll(char);
void set_yaw(char);
void set_lift(char);
void set_yawrate(char);

// interface used by keyboard
void inc_pitch();
void inc_roll();
void inc_yaw();
void inc_lift();
void inc_yawrate();

void set_led(char);

#endif // COMMAND_H

#ifndef COMMANDS_H
#define COMMANDS_H

#define PANIC_RPM 50
#define PANIC_TIME 5000

void perform_command(char,char);

void set_led(char);

void set_mode(char);
void set_yawrate(char);
void set_roll(char);
void set_pitch(char);
void set_lift(char);

#endif // COMMANDS_H

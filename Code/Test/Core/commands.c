#include "commands.h"
#include "packet.h"
#include "drone.h"

extern struct drone qr;

void perform_command(char header, char command)
{
    switch( header )
    {
        //Set Mode
        case SET_MODE:
                    set_mode(command);
                    break;
        //Set Pitch
        case SET_PITCH:
                    set_pitch(command);
                    break;
        //Set Roll
        case SET_ROLL:
                    set_roll(command);
                    break;
        //Set Yaw
        case SET_YAWRATE:
                    set_yawrate(command);
                    break;
        case SET_LIFT:
                    set_lift(command);
                    break;
        case SET_LED:
                    set_led(command);
                    break;
    };
}

/*
    set_mode changes the operating mode of the drone.
    It performs some checking in order to avoid unsafe behaviour
*/
void set_mode(char command)
{
    /* It is not allowed to switch between modes unless either you are currently in SAFE_MODE or the command is SAFE/PANIC_MODE */
    if( qr.current_mode != SAFE_MODE && ( command != SAFE_MODE || command != PANIC_MODE ) )
        return; // ignore command

    switch(command)
    {
        case SAFE_MODE:
            set_lift(0x0);
            qr.current_mode = SAFE_MODE;
            break;

        case PANIC_MODE:
            set_lift(PANIC_RPM);
            qr.current_mode = PANIC_MODE;
            break;

        case MANUAL_MODE:
            qr.current_mode = MANUAL_MODE;
            break;

        case CALIBRATION_MODE:
            qr.current_mode = CALIBRATION_MODE;
            break;

        case YAW_MODE:
            qr.current_mode = YAW_MODE;
            break;

        case FULL_MODE:
            qr.current_mode = FULL_MODE;
            break;
    };

    qr.flag_mode = 1;
}

void set_lift(char command)
{
    if( qr.current_mode == SAFE_MODE )
        return;

    qr.lift = command;
}

void set_pitch(char command){}
void set_roll(char command){}

void set_yawrate(char command)
{
    if( qr.current_mode == SAFE_MODE )
        return;

    qr.yawrate = command;
}

void set_led(char command)
{
	X32_leds = command;
}

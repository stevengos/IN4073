/**
@author Gianluca Savaia
*/

#include "command.h"

extern struct drone qr;

void perform_command(char header, char command)
{
    if( qr.log )
        add_log();

    switch( header )
    {
        case ALIVE:
                    return;

        case SET_MODE:
                    set_mode(command);
                    break;

        /* set setpoints */ //{
        case SET_PITCH:
                    set_pitch(command);
                    break;

        case SET_ROLL:
                    set_roll(command);
                    break;

        case SET_LIFT:
                    set_lift(command);
                    break;

        case SET_YAWRATE:
                    set_yawrate(command);
                    break;
        //}

        /* tune setpoints */ //{
        case D_PITCH:
                    d_pitch(command);
                    break;

        case D_ROLL:
                    d_roll(command);
                    break;

        case D_LIFT:
                    d_lift(command);
                    break;

        case D_YAWRATE:
                    d_yawrate(command);
                    break;
        //}

        /* tune scales */ //{
        case SET_SCALE_PITCH:
                    set_scale_pitch(command);
                    break;
        case SET_SCALE_ROLL:
                    set_scale_roll(command);
                    break;
        case SET_SCALE_YAW:
                    set_scale_yaw(command);
                    break;
        case SET_SCALE_LIFT:
                    set_scale_lift(command);
                    break;
        //}

        case SET_CONTROLLER_PITCH:
                    set_controller_pitch(command);
                    break;
        case SET_CONTROLLER_ROLL:
                    set_controller_roll(command);
                    break;
        case SET_CONTROLLER_YAW:
                    set_controller_yaw(command);
                    break;
        case OPEN_LOOP:
                    qr.controller_pitch = 0;
                    qr.controller_roll = 0;
                    qr.controller_yaw = 0;
                    break;

        case LOG:
                    set_log(command);
                    break;

        case STOP:
                    stop();
                    break;
        default:
                    acknowledge(ACK_INVALID);
    };
}

void stop()
{
    if( qr.current_mode != SAFE_MODE ) //machine can be stopped only in SAFE_MODE
    {
        acknowledge(ACK_INVALID);
        return;
    }

    qr.exit = 1;
    qr.flag_mode = 1;

    acknowledge(ACK_POSITIVE);
}

//set_mode changes the operating mode of the drone. It performs some checking in order to avoid unsafe behaviour.
void set_mode(char command)
{
    ENABLE_INTERRUPT(INTERRUPT_TIMER1);

    // It is not allowed to switch between modes unless either you are currently in SAFE_MODE or the command is SAFE/PANIC_MODE
    if( qr.current_mode != SAFE_MODE && ( command != SAFE_MODE && command != PANIC_MODE ) )
    {
        acknowledge(ACK_INVALID);
        return;
    }

    if( qr.lift_force && ( command != SAFE_MODE && command != PANIC_MODE ) )
    {
        acknowledge(ACK_INVALID);
        return;
    }

    switch(command)
    {
        case SAFE_MODE:
            qr.current_mode = SAFE_MODE;
            break;

        case PANIC_MODE:
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

    qr.flag_mode = 1; //signal a change in mode

    acknowledge(ACK_POSITIVE);
}

/* Setting parameters */
//{
void set_scale_pitch(char command)
{
    if(command == INCREASE)
        qr.scale_pitch += STEP_SCALE_PARAMETER;
    else
        qr.scale_pitch -= STEP_SCALE_PARAMETER;
}

void set_scale_roll(char command)
{
    if(command == INCREASE)
        qr.scale_roll += STEP_SCALE_PARAMETER;
    else
        qr.scale_roll -= STEP_SCALE_PARAMETER;
}

void set_scale_yaw(char command)
{
    if(command == INCREASE)
        qr.scale_yaw += STEP_SCALE_PARAMETER;
    else
        qr.scale_yaw -= STEP_SCALE_PARAMETER;
}

void set_scale_lift(char command)
{
    if(command == INCREASE)
        qr.scale_lift += STEP_SCALE_PARAMETER;
    else
        qr.scale_lift -= STEP_SCALE_PARAMETER;
}
//}

/* Set controller parameters */
//{
void set_controller_pitch(char command)
{
    if(command == INCREASE)
        qr.controller_pitch += 1;
    else
        qr.controller_pitch = qr.controller_pitch-1 >= 0 ? qr.controller_pitch-1 : 0;
}
void set_controller_roll(char command)
{
    if(command == INCREASE)
        qr.controller_roll += 1;
    else
        qr.controller_roll = qr.controller_roll-1 >= 0 ? qr.controller_roll-1 : 0;
}
void set_controller_yaw(char command)
{
    if(command == INCREASE)
        qr.controller_yaw += 1;
    else
        qr.controller_yaw = qr.controller_yaw-1 >= 0 ? qr.controller_yaw-1 : 0;
}
//}

void set_log(char command)
{
    switch(command)
    {
        case LOG_START:
                if( init_log() )
                    qr.log = 1, acknowledge(ACK_POSITIVE);
                else
                    acknowledge(ACK_NEGATIVE);
                break;
        case LOG_STOP:
                qr.log = 0;
                qr.log_full = 1;
                acknowledge(ACK_POSITIVE);
                break;
        case LOG_GET:
                upload_log();
                break;
        default:
                acknowledge(ACK_INVALID);
                return;
    }
}

/* JOYSTICK SECTION */
//{
void set_pitch(char command)
{
    short value = command + qr.off_pitch;

    if( qr.current_mode == FULL_MODE )
        qr.pitch_ref = value > MAX_PITCH ? MAX_PITCH : value < MIN_PITCH ? MIN_PITCH : value;
    else
        qr.pitch_momentum = value > MAX_PITCH ? MAX_PITCH : value < MIN_PITCH ? MIN_PITCH : value;

    acknowledge(ACK_POSITIVE);
}

void set_roll(char command)
{
    short value = command + qr.off_roll;

    if( qr.current_mode == FULL_MODE )
        qr.roll_ref = value > MAX_ROLL ? MAX_ROLL : value < MIN_ROLL ? MIN_ROLL : value;
    else
        qr.roll_momentum = value > MAX_ROLL ? MAX_ROLL : value < MIN_ROLL ? MIN_ROLL : value;

    acknowledge(ACK_POSITIVE);
}

void set_lift(unsigned char command)
{
    short value = command + qr.off_lift;

    qr.lift_force = value > MAX_LIFT ? MAX_LIFT : value < MIN_LIFT ? MIN_LIFT : value;

    acknowledge(ACK_POSITIVE);
}

void set_yawrate(char command)
{
    short value = command + qr.off_yawrate;

    if( qr.current_mode == YAW_MODE || qr.current_mode == FULL_MODE)
        qr.yawrate_ref = value > MAX_YAWRATE ? MAX_YAWRATE : value < MIN_YAWRATE ? MIN_YAWRATE : value;
    else
        qr.yaw_momentum = value > MAX_YAWRATE ? MAX_YAWRATE : value < MIN_YAWRATE ? MIN_YAWRATE : value;

    acknowledge(ACK_POSITIVE);
}
//}

/* KEYBOARD SECTION */
//{

void d_pitch(char command)
{
    qr.off_pitch = command == INCREASE ? qr.off_pitch + STEP_PITCH : qr.off_pitch - STEP_PITCH;

    acknowledge(ACK_POSITIVE);
}

void d_roll(char command)
{
    qr.off_roll = command == INCREASE ? qr.off_roll + STEP_ROLL : qr.off_roll - STEP_ROLL;

    acknowledge(ACK_POSITIVE);
}

void d_yawrate(char command)
{
    qr.off_yawrate = command == INCREASE ? qr.off_yawrate + STEP_YAW : qr.off_yawrate - STEP_YAW;

    acknowledge(ACK_POSITIVE);
}

void d_lift(char command)
{
    qr.off_lift = command == INCREASE ? qr.off_lift + STEP_LIFT : qr.off_lift - STEP_LIFT;

    acknowledge(ACK_POSITIVE);
}

//}

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

        case SET_CONTROLLER_YAW:
                    set_controller_yaw(command);
                    break;

        case ALIVE:
                    return;

        case BLINK_LED:
                    break;

        case SET_LED:
                    set_led(command);
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

void set_controller_yaw(char command)
{
    if(command == INCREASE)
        qr.controller_yaw += 1;
    else
        qr.controller_yaw = qr.controller_yaw-1 >= 0 ? qr.controller_yaw-1 : 0;
}

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
    qr.pitch_momentum = command > MAX_PITCH ? MAX_PITCH : command < MIN_PITCH ? MIN_PITCH : command;

    if( qr.current_mode == FULL_MODE )
        qr.pitch_ref = command > MAX_PITCH ? MAX_PITCH : command < MIN_PITCH ? MIN_PITCH : command;

    acknowledge(ACK_POSITIVE);
}

void set_roll(char command)
{
    qr.roll_momentum = command > MAX_ROLL ? MAX_ROLL : command < MIN_ROLL ? MIN_ROLL : command;

    if( qr.current_mode == FULL_MODE )
        qr.roll_ref = command > MAX_ROLL ? MAX_ROLL : command < MIN_ROLL ? MIN_ROLL : command;

    acknowledge(ACK_POSITIVE);
}

void set_lift(unsigned char command)
{
    qr.lift_force = command > MAX_LIFT ? MAX_LIFT : command < MIN_LIFT ? MIN_LIFT : command;

    if( qr.current_mode == FULL_MODE )
        qr.lift_ref = command > MAX_LIFT ? MAX_LIFT : command < MIN_LIFT ? MIN_LIFT : command;

    acknowledge(ACK_POSITIVE);
}

void set_yawrate(char command)
{
    qr.yaw_momentum = command > MAX_YAWRATE ? MAX_YAWRATE : command < MIN_YAWRATE ? MIN_YAWRATE : command;

    if( qr.current_mode == FULL_MODE )
        qr.yawrate_ref = command > MAX_YAWRATE ? MAX_YAWRATE : command < MIN_YAWRATE ? MIN_YAWRATE : command;

    acknowledge(ACK_POSITIVE);
}
//}

/* KEYBOARD SESSION */
//{

void d_pitch(char command)
{
    if( qr.current_mode == FULL_MODE )

        if( command == INCREASE )
            if( qr.pitch_ref + qr.step_pitch > MAX_PITCH )
                acknowledge(ACK_INVALID);
            else
                qr.pitch_ref += qr.step_pitch, acknowledge(ACK_POSITIVE);
        else
            if( qr.pitch_ref - qr.step_pitch < MIN_PITCH )
                acknowledge(ACK_INVALID);
            else
                qr.pitch_ref -= qr.step_pitch, acknowledge(ACK_POSITIVE);
    else
        if( command == INCREASE )
            if( qr.pitch_momentum + qr.step_pitch > MAX_PITCH )
                acknowledge(ACK_INVALID);
            else
                qr.pitch_momentum += qr.step_pitch, acknowledge(ACK_POSITIVE);
        else
            if( qr.pitch_momentum - qr.step_pitch < MIN_PITCH )
                acknowledge(ACK_INVALID);
            else
                qr.pitch_momentum -= qr.step_pitch, acknowledge(ACK_POSITIVE);
}

void d_roll(char command)
{
    if( qr.current_mode == FULL_MODE )

        if( command == INCREASE )
            if( qr.roll_ref + qr.step_roll > MAX_ROLL )
                acknowledge(ACK_INVALID);
            else
                qr.roll_ref += qr.step_roll, acknowledge(ACK_POSITIVE);
        else
            if( qr.roll_ref - qr.step_roll < MIN_ROLL )
                acknowledge(ACK_INVALID);
            else
                qr.roll_ref -= qr.step_roll, acknowledge(ACK_POSITIVE);
    else

        if( command == INCREASE )
            if( qr.roll_momentum + qr.step_roll > MAX_ROLL )
                acknowledge(ACK_INVALID);
            else
                qr.roll_momentum += qr.step_roll, acknowledge(ACK_POSITIVE);
        else
            if( qr.roll_momentum - qr.step_roll < MIN_ROLL )
                acknowledge(ACK_INVALID);
            else
                qr.roll_momentum -= qr.step_roll, acknowledge(ACK_POSITIVE);
}

void d_yawrate(char command)
{
    if( qr.current_mode == FULL_MODE )

        if( command == INCREASE )
            if( qr.yawrate_ref + qr.step_yawrate > MAX_YAWRATE )
                acknowledge(ACK_INVALID);
            else
                qr.yawrate_ref += qr.step_yawrate, acknowledge(ACK_POSITIVE);
        else
            if( qr.yawrate_ref - qr.step_yawrate < MIN_YAWRATE )
                acknowledge(ACK_INVALID);
            else
                qr.yawrate_ref -= qr.step_yawrate, acknowledge(ACK_POSITIVE);
    else

        if( command == INCREASE )
            if( qr.yaw_momentum + qr.step_yawrate > MAX_YAWRATE )
                acknowledge(ACK_INVALID);
            else
                qr.yaw_momentum += qr.step_yawrate, acknowledge(ACK_POSITIVE);
        else
            if( qr.yaw_momentum - qr.step_yawrate < MIN_YAWRATE )
                acknowledge(ACK_INVALID);
            else
                qr.yaw_momentum -= qr.step_yawrate, acknowledge(ACK_POSITIVE);
}

void d_lift(char command)
{
    if( qr.current_mode == FULL_MODE )

        if( command == INCREASE )
            if( qr.lift_ref + qr.step_lift > MAX_LIFT )
                acknowledge(ACK_INVALID);
            else
                qr.lift_ref += qr.step_lift, acknowledge(ACK_POSITIVE);
        else
            if( qr.lift_ref - qr.step_lift < MIN_LIFT )
                qr.lift_ref = MIN_LIFT, acknowledge(ACK_INVALID);
            else
                qr.lift_ref -= qr.step_lift, acknowledge(ACK_POSITIVE);
    else

        if( command == INCREASE )
            if( qr.lift_force + qr.step_lift > MAX_LIFT )
                acknowledge(ACK_INVALID);
            else
                qr.lift_force += qr.step_lift, acknowledge(ACK_POSITIVE);
        else
            if( qr.lift_force - qr.step_lift < MIN_LIFT )
                qr.lift_force = MIN_LIFT, acknowledge(ACK_INVALID);
            else
                qr.lift_force -= qr.step_lift, acknowledge(ACK_POSITIVE);
}

void set_led(char command)
{
    X32_LEDS = command;
    acknowledge(ACK_POSITIVE);
}

//}

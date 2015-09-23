/**
@author Gianluca Savaia
*/

#include "command.h"

#include "../interface/packet.h"
#include "../interface/hamming.h"

#include "drone.h"
#include "isr.h"

#include <stdio.h>

extern struct drone qr;

void perform_command(char header, char command)
{
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
                    printf("board> Stop signal received.\n");
                    stop();
                    break;
        default:
                    acknowledge(ACK_INVALID);
    };

    //print_drone();

    if(qr.log)
        add_log();
}

void acknowledge(char response)
{
    char counter = 0;
    packet_t packet;

    if( response != ACK_POSITIVE && response != ACK_NEGATIVE && response != ACK_INVALID)
    {
        printf("board> Internal error: call to acknowledge() with input not defined by protocol.\n");
        response = ACK_NEGATIVE;
    }

    while( !X32_RS232_WRITE )
    {
        if( counter++ > TIMEOUT_BUFFER_TX )
            return;
        else
            ucatnap(SLEEP_BUFFER_TX);
    }

    X32_RS232_DATA = ACK;               //header

    while( !X32_RS232_WRITE )
    {
        if( counter++ > TIMEOUT_BUFFER_TX )
            return;
        else
            ucatnap(SLEEP_BUFFER_TX);
    }

    X32_RS232_DATA = response;          //command

    while( !X32_RS232_WRITE )
    {
        if( counter++ > TIMEOUT_BUFFER_TX )
            return;
        else
            ucatnap(SLEEP_BUFFER_TX);
    }

    packet.header = ACK;
    packet.command = response;

    compute_hamming(&packet);

    X32_RS232_DATA = packet.crc;              //checksum
}

void stop()
{
    if( qr.current_mode != SAFE_MODE ) //machine can be stopped only in SAFE_MODE
    {
        acknowledge(ACK_INVALID);
        printf("board> Please, if you want to stop then go in SAFE_MODE first.\n");
        return;
    }

    qr.exit = 1;
    qr.flag_mode = 1;

    printf("board> Machine Stopped\n");

    acknowledge(ACK_POSITIVE);
}

//set_mode changes the operating mode of the drone. It performs some checking in order to avoid unsafe behaviour.
void set_mode(char command)
{
    //prepare timer interrupt
    X32_TIMER_CYCLE = TIMEOUT_TIMER*CLOCKS_PER_MS;
    SET_INTERRUPT_VECTOR(INTERRUPT_TIMER1, &isr_timer);
    SET_INTERRUPT_PRIORITY(INTERRUPT_TIMER1, 1);
    ENABLE_INTERRUPT(INTERRUPT_TIMER1);

    // It is not allowed to switch between modes unless either you are currently in SAFE_MODE or the command is SAFE/PANIC_MODE
    if( qr.current_mode != SAFE_MODE && ( command != SAFE_MODE && command != PANIC_MODE ) )
    {
        acknowledge(ACK_INVALID);
        printf("board> Please go in SAFE_MODE if you want to change mode.\n");
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

void set_log(char command)
{
    switch(command)
    {
        case LOG_START:
                qr.log = 1;
                acknowledge(ACK_POSITIVE);
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

void send_short(short value)
{
    unsigned char c1, c2;

    c1 = value;
    c2 = (value >> 8);

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c1;

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c2;
}

void send_int(int value)
{
    unsigned char c1, c2, c3, c4;

    c1 = value;
    c2 = (value >> 8);
    c3 = (value >> 16);
    c4 = (value >> 24);

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c1;

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c2;

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c3;

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c4;
}

void upload_log()
{
    int counter_timeout = 0;
    int i = 0;

    unsigned char cl, cr;

    if( qr.current_mode != SAFE_MODE )
    {
        acknowledge(ACK_INVALID);
        return;
    }

    while( i < qr.log_size )
    {
        struct log_s outgoing = qr.log_buffer[i];
        short* ptr_head = &outgoing.ae1; //************* FIRST STRUCT ELEMENT
        short* ptr_tail = &outgoing.ae4; //************* LAST STRUCT ELEMENT

        X32_DISPLAY = outgoing.id;

        /* Sending log id */
        counter_timeout = 0;

        while( !X32_RS232_WRITE )
        {
            if( counter_timeout++ > TIMEOUT_BUFFER_TX )
            {
                acknowledge(ACK_NEGATIVE);
                return;
            }
            else
                ucatnap(SLEEP_BUFFER_TX);
        }

        send_int(outgoing.id);

        /* Sending timestamp */
        counter_timeout = 0;

        while( !X32_RS232_WRITE )
        {
            if( counter_timeout++ > TIMEOUT_BUFFER_TX )
            {
                acknowledge(ACK_NEGATIVE);
                return;
            }
            else
                ucatnap(SLEEP_BUFFER_TX);
        }

        send_int( outgoing.timestamp );

        /* Sending log data */
        while( ptr_head != ptr_tail )
        {
            counter_timeout = 0;

            while( !X32_RS232_WRITE )
            {
                if( counter_timeout++ > TIMEOUT_BUFFER_TX )
                {
                    acknowledge(ACK_NEGATIVE);
                    return;
                }
                else
                    ucatnap(SLEEP_BUFFER_TX);
            }

            send_short( *ptr_head );

            ptr_head++;
        }

        send_short(LOG_END);

        i++;
    }

    acknowledge(ACK_POSITIVE);
}

/* JOYSTICK SECTION */
//{
void set_pitch(char command)
{
    if( qr.current_mode == SAFE_MODE )
    {
        acknowledge(ACK_INVALID);
        return;
    }

    if( qr.current_mode == MANUAL_MODE )
        qr.pitch_momentum = command > MAX_PITCH ? MAX_PITCH : command < MIN_PITCH ? MIN_PITCH : command;

    if( qr.current_mode == FULL_MODE )
        qr.pitch_ref = command > MAX_PITCH ? MAX_PITCH : command < MIN_PITCH ? MIN_PITCH : command;
}

void set_roll(char command)
{
    if( qr.current_mode == SAFE_MODE )
    {
        acknowledge(ACK_INVALID);
        return;
    }

    if( qr.current_mode == MANUAL_MODE )
        qr.roll_momentum = command > MAX_ROLL ? MAX_ROLL : command < MIN_ROLL ? MIN_ROLL : command;

    if( qr.current_mode == FULL_MODE )
        qr.roll_ref = command > MAX_ROLL ? MAX_ROLL : command < MIN_ROLL ? MIN_ROLL : command;
}

void set_lift(char command)
{
    if( qr.current_mode == SAFE_MODE )
    {
        acknowledge(ACK_INVALID);
        return;
    }

    qr.lift = command > MAX_LIFT ? MAX_LIFT : command < MIN_LIFT ? MIN_LIFT : command;
}

void set_yawrate(char command)
{
    if( qr.current_mode == SAFE_MODE )
    {
        acknowledge(ACK_INVALID);
        return;
    }

    qr.yawrate = command > MAX_YAWRATE ? MAX_YAWRATE : command < MIN_YAWRATE ? MIN_YAWRATE : command;
}
//}

/* KEYBOARD SESSION */
//{

void d_pitch(char command)
{
    if( qr.current_mode == SAFE_MODE )
        acknowledge(ACK_INVALID);
    else
        if( command == INCREASE )
            if( qr.pitch + qr.step_pitch > MAX_PITCH )
                acknowledge(ACK_INVALID);
            else
                qr.pitch += qr.step_pitch, acknowledge(ACK_POSITIVE);
        else
            if( qr.pitch - qr.step_pitch < MIN_PITCH )
                acknowledge(ACK_INVALID);
            else
                qr.pitch -= qr.step_pitch, acknowledge(ACK_POSITIVE);
}

void d_roll(char command)
{
    if( qr.current_mode == SAFE_MODE )
        acknowledge(ACK_INVALID);
    else
        if( command == INCREASE )
            if( qr.roll + qr.step_roll > MAX_ROLL )
                acknowledge(ACK_INVALID);
            else
                qr.roll += qr.step_roll, acknowledge(ACK_POSITIVE);
        else
            if( qr.roll - qr.step_roll < MIN_ROLL )
                acknowledge(ACK_INVALID);
            else
                qr.roll -= qr.step_roll, acknowledge(ACK_POSITIVE);
}

void d_yawrate(char command)
{
    if( qr.current_mode == SAFE_MODE )
        acknowledge(ACK_INVALID);
    else
        if( command == INCREASE )
            if( qr.yawrate + qr.step_yawrate > MAX_YAWRATE )
                acknowledge(ACK_INVALID);
            else
                qr.yawrate += qr.step_yawrate, acknowledge(ACK_POSITIVE);
        else
            if( qr.yawrate - qr.step_yawrate < MIN_YAWRATE )
                acknowledge(ACK_INVALID);
            else
                qr.yawrate -= qr.step_yawrate, acknowledge(ACK_POSITIVE);
}

void d_lift(char command)
{
    if( qr.current_mode == SAFE_MODE )
        acknowledge(ACK_INVALID);
    else
        if( command == INCREASE )
            if( qr.lift + qr.step_lift > MAX_LIFT )
                acknowledge(ACK_INVALID);
            else
                qr.lift += qr.step_lift, acknowledge(ACK_POSITIVE);
        else
            if( qr.lift - qr.step_lift < MIN_LIFT )
                qr.lift = MIN_LIFT, acknowledge(ACK_INVALID);
            else
                qr.lift -= qr.step_lift, acknowledge(ACK_POSITIVE);
}

void set_led(char command)
{
    X32_LEDS = command;
    acknowledge(ACK_POSITIVE);
}

//}

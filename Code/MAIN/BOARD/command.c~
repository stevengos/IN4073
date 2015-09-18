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

    print_drone();
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
            usleep(SLEEP_BUFFER_TX);
    }

    X32_RS232_DATA = ACK;               //header

    while( !X32_RS232_WRITE )
    {
        if( counter++ > TIMEOUT_BUFFER_TX )
            return;
        else
            usleep(SLEEP_BUFFER_TX);
    }

    X32_RS232_DATA = response;          //command

    while( !X32_RS232_WRITE )
    {
        if( counter++ > TIMEOUT_BUFFER_TX )
            return;
        else
            usleep(SLEEP_BUFFER_TX);
    }

    packet.header = ACK;
    packet.command = response;

    compute_hamming(&packet);

    X32_RS232_DATA = packet.crc;              //checksum
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
    X32_DISPLAY = 0x0000;

    printf("board> Machine Stopped\n");

    acknowledge(ACK_POSITIVE);
}

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

    printf("board> Log %s.\n", qr.log ? "on" : "off");
}

void upload_log()
{
    char counter_timeout = 0;
    char i = 0;

    printf("board> uploading log.\n");

    if( qr.current_mode != SAFE_MODE )
    {
        printf("board> Command discarded: you can upload logs only in SAFE_MODE.\n");
        acknowledge(ACK_INVALID);
        return;
    }

    while( i < qr.log_size )
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
                usleep(SLEEP_BUFFER_TX);
        }

        X32_RS232_DATA = qr.log_buffer[i];

        i++;
    }

    acknowledge(ACK_POSITIVE);
}

/* JOYSTICK SECTION */
void set_pitch(char command)
{
}
void set_roll(char command)
{
}

void set_lift(char command)
{
    if( qr.current_mode == SAFE_MODE )
        return;

    qr.lift = command;
}

void set_yawrate(char command)
{
    if( qr.current_mode == SAFE_MODE )
        return;

    qr.yawrate = command;
}

/* KEYBOARD SESSION */

void d_pitch(char command)
{
    if( qr.current_mode == SAFE_MODE )
        acknowledge(ACK_INVALID);
    else
        if( command == INCREASE )
            if( qr.pitch + STEP_PITCH > MAX_PITCH )
                acknowledge(ACK_INVALID);
            else
                qr.pitch += STEP_PITCH, acknowledge(ACK_POSITIVE);
        else
            if( qr.pitch - STEP_PITCH < MIN_PITCH )
                acknowledge(ACK_INVALID);
            else
                qr.pitch -= STEP_PITCH, acknowledge(ACK_POSITIVE);
}

void d_roll(char command)
{
    if( qr.current_mode == SAFE_MODE )
        acknowledge(ACK_INVALID);
    else
        if( command == INCREASE )
            if( qr.roll + STEP_ROLL > MAX_ROLL )
                acknowledge(ACK_INVALID);
            else
                qr.roll += STEP_ROLL, acknowledge(ACK_POSITIVE);
        else
            if( qr.roll - STEP_ROLL < MIN_ROLL )
                acknowledge(ACK_INVALID);
            else
                qr.roll -= STEP_ROLL, acknowledge(ACK_POSITIVE);
}

void d_yawrate(char command)
{
    if( qr.current_mode == SAFE_MODE )
        acknowledge(ACK_INVALID);
    else
        if( command == INCREASE )
            if( qr.yawrate + STEP_YAWRATE > MAX_YAWRATE )
                acknowledge(ACK_INVALID);
            else
                qr.yawrate += STEP_YAWRATE, acknowledge(ACK_POSITIVE);
        else
            if( qr.yawrate - STEP_YAWRATE < MIN_YAWRATE )
                acknowledge(ACK_INVALID);
            else
                qr.yawrate -= STEP_YAWRATE, acknowledge(ACK_POSITIVE);
}

void d_lift(char command)
{
    if( qr.current_mode == SAFE_MODE )
        acknowledge(ACK_INVALID);
    else
        if( command == INCREASE )
            if( qr.lift + STEP_LIFT > MAX_LIFT )
                acknowledge(ACK_INVALID);
            else
                qr.lift += STEP_LIFT, acknowledge(ACK_POSITIVE);
        else
            if( qr.lift - STEP_LIFT < MIN_LIFT )
                qr.lift = MIN_LIFT, acknowledge(ACK_INVALID);
            else
                qr.lift -= STEP_LIFT, acknowledge(ACK_POSITIVE);
}

void set_led(char command)
{
    X32_LEDS = command;
    acknowledge(ACK_POSITIVE);
}

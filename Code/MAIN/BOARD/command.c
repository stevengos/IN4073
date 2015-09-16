/**
@author Gianluca Savaia
@last update 2015-09-15
*/

#include "command.h"

#include "../interface/packet.h"
#include "../interface/hamming.h"

#include "drone.h"
#include <stdio.h>

extern struct drone qr;

void perform_command(char header, char command)
{
    printf("board> Performing command: %x %x \n", header, command);

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

        case SET_YAW:
                    set_yaw(command);
                    break;

        case SET_LIFT:
                    set_lift(command);
                    break;

        case SET_YAWRATE:
                    set_yawrate(command);
                    break;

        case INC_PITCH:
                    inc_pitch();
                    break;

        case INC_ROLL:
                    inc_roll();
                    break;

        case INC_YAW:
                    inc_yaw();
                    break;

        case INC_LIFT:
                    inc_lift();
                    break;

        case INC_YAWRATE:
                    inc_yawrate();
                    break;

        case ALIVE:
                    break;

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
                    acknowledge(ACK_NEGATIVE);
    };
}

void acknowledge(char response)
{
    char counter = 0;
    packet_t packet;

    if( response != ACK_POSITIVE && response != ACK_NEGATIVE )
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
    // It is not allowed to switch between modes unless either you are currently in SAFE_MODE or the command is SAFE/PANIC_MODE
    if( qr.current_mode != SAFE_MODE && ( command != SAFE_MODE || command != PANIC_MODE ) )
    {
        acknowledge(ACK_NEGATIVE);
        return;
    }

    switch(command)
    {
        case SAFE_MODE:
            qr.current_mode = SAFE_MODE;
            set_led(LED1);
            break;

        case PANIC_MODE:
            qr.current_mode = PANIC_MODE;
            set_led(LED2);
            break;

        case MANUAL_MODE:
            qr.current_mode = MANUAL_MODE;
            set_led(LED3);
            break;

        case CALIBRATION_MODE:
            qr.current_mode = CALIBRATION_MODE;
            set_led(LED4);
            break;

        case YAW_MODE:
            qr.current_mode = YAW_MODE;
            set_led(LED5);
            break;

        case FULL_MODE:
            qr.current_mode = FULL_MODE;
            set_led(LED6);
            break;
    };

    qr.flag_mode = 1; //signal a change in mode
}

void stop()
{
    if( qr.current_mode != SAFE_MODE ) //machine can be stopped only in SAFE_MODE
    {
        acknowledge(ACK_NEGATIVE);
        return;
    }

    qr.exit = 1;
    qr.flag_mode = 1;
    X32_LEDS = ALL_OFF;

    acknowledge(ACK_POSITIVE);

    printf("board> Machine Stopped\n");
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
                acknowledge(ACK_NEGATIVE);
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
        acknowledge(ACK_NEGATIVE);
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

void set_pitch(char command)
{
}
void set_roll(char command)
{
}

void set_yaw(char command)
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

void inc_pitch()
{
    if( qr.current_mode == SAFE_MODE )
        return;

    qr.ae1 = qr.ae1 + STEP_PITCH > MAX_RPM ? MAX_RPM : qr.ae1 + STEP_PITCH;
}

void inc_roll()
{
    if( qr.current_mode == SAFE_MODE )
        return;

    qr.ae4 = qr.ae4 + STEP_ROLL > MAX_RPM ? MAX_RPM : qr.ae4 + STEP_ROLL;
}

void inc_yaw()
{
}

void inc_lift()
{
    if( qr.current_mode == SAFE_MODE )
        return;

    qr.ae1 = qr.ae1 + STEP_LIFT > MAX_RPM ? MAX_RPM : qr.ae1 + STEP_LIFT;
    qr.ae2 = qr.ae1 + STEP_LIFT > MAX_RPM ? MAX_RPM : qr.ae2 + STEP_LIFT;
    qr.ae3 = qr.ae1 + STEP_LIFT > MAX_RPM ? MAX_RPM : qr.ae3 + STEP_LIFT;
    qr.ae4 = qr.ae1 + STEP_LIFT > MAX_RPM ? MAX_RPM : qr.ae4 + STEP_LIFT;
}

void inc_yawrate()
{
    if( qr.current_mode == SAFE_MODE )
        return;

    qr.ae1 = qr.ae1 + STEP_YAWRATE > MAX_RPM ? MAX_RPM : qr.ae1 + STEP_YAWRATE;
}


void set_led(char command)
{
    X32_LEDS = command;
    acknowledge(ACK_POSITIVE);
}


/**
@author Gianluca Savaia
@last update 2015-09-15
*/

#include "drone.h"
#include "../interface/packet.h"

#include <stdio.h>

extern struct drone qr;

void run_drone()
{
    clear_drone();

    while( !qr.exit )
    {
        qr.flag_mode = 0;

        switch(qr.current_mode)
        {
            case SAFE_MODE:
                safe_mode();
                break;

            case PANIC_MODE:
                panic_mode();
                break;

            case MANUAL_MODE:
                manual_mode();
                break;

            case CALIBRATION_MODE:
                calibration_mode();
                break;

            case YAW_MODE:
                yaw_mode();
                break;

            case FULL_MODE:
                full_mode();
                break;

            default:
                qr.current_mode = SAFE_MODE;
                break;
        };
    }
}

void safe_mode()
{
    while(!qr.flag_mode);
}

void panic_mode() {}

void manual_mode()
{
    while(!qr.flag_mode)
    {
        DISABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

//        X32_QR_a1 = qr.ae1;
//        X32_QR_a2 = qr.ae2;
//        X32_QR_a3 = qr.ae3;
//        X32_QR_a4 = qr.ae4;

        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

        sleep(1); //refresh time
    }
}

void calibration_mode(){}

void yaw_mode(){}

void full_mode(){}

void clear_drone()
{
    qr.ae1 = 0;
    qr.ae2 = 0;
    qr.ae3 = 0;
    qr.ae4 = 0;

    qr.pitch = 0;
    qr.roll = 0;
    qr.yawrate = 0;
    qr.lift = 0;

    qr.sax = 0;
    qr.say = 0;
    qr.saz = 0;
    qr.sp = 0;
    qr.sq = 0;
    qr.sr = 0;

    qr.current_mode = SAFE_MODE;
    qr.flag_mode = 0;
    qr.exit = 0;
    qr.link_down = 0;
    qr.log = 0;
    qr.log_size = 0;
}

void add_log()
{
    if( !qr.log )
        return;

    if( qr.log_size >= LOG_BUFFER_SIZE )
    {
        qr.log = 0;
        return;
    }

    printf("board> Adding Log.\n");

    /* write log into buffer */
    qr.log_buffer[qr.log_size] = 'H';

    qr.log_size++;
}


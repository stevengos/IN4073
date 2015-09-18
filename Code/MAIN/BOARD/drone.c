/**
@author Gianluca Savaia
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
        };
    }
}

void safe_mode()
{
    X32_LEDS = LED8;

    while(!qr.flag_mode);

    X32_LEDS = ALL_OFF;
}

void panic_mode() { X32_LEDS = ALL_ON; while(!qr.flag_mode); X32_LEDS = ALL_OFF; }

void manual_mode()
{
    int ae1 = 0;
    int ae2 = 0;
    int ae3 = 0;
    int ae4 = 0;
    int scaling_factor = 100;
    short debug = 0;

    X32_LEDS = LED7;

    while(!qr.flag_mode)
    {
        //X32_DISPLAY = debug++;

        ae1 = scaling_factor*( qr.lift  + 2*qr.pitch                    - qr.yawrate ) / 4;
        ae2 = scaling_factor*( qr.lift                  - 2*qr.roll     + qr.yawrate ) / 4;
        ae3 = scaling_factor*( qr.lift  - 2*qr.pitch                    - qr.yawrate ) / 4;
        ae4 = scaling_factor*( qr.lift                  + 2*qr.roll     + qr.yawrate ) / 4;

        ae1 = ae1 <= 0 ? 0 : float32_to_int32( float32_sqrt( int32_to_float32(ae1) ) );
        ae2 = ae2 <= 0 ? 0 : float32_to_int32( float32_sqrt( int32_to_float32(ae2) ) );
        ae3 = ae3 <= 0 ? 0 : float32_to_int32( float32_sqrt( int32_to_float32(ae3) ) );
        ae4 = ae4 <= 0 ? 0 : float32_to_int32( float32_sqrt( int32_to_float32(ae4) ) );

        qr.ae1 = ae1 > MAX_RPM ? MAX_RPM : (short)ae1;
        qr.ae2 = ae2 > MAX_RPM ? MAX_RPM : (short)ae2;
        qr.ae3 = ae3 > MAX_RPM ? MAX_RPM : (short)ae3;
        qr.ae4 = ae4 > MAX_RPM ? MAX_RPM : (short)ae4;

        DISABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

//        X32_QR_a1 = qr.ae1;
//        X32_QR_a2 = qr.ae2;
//        X32_QR_a3 = qr.ae3;
//        X32_QR_a4 = qr.ae4;

        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

        usleep(REFRESH_TIME); //refresh time
    }

    stop_motors();

    X32_LEDS = ALL_OFF;
}

void calibration_mode(){ X32_LEDS = LED6; while(!qr.flag_mode); X32_LEDS = ALL_OFF; }

void yaw_mode(){ X32_LEDS = LED5; while(!qr.flag_mode); X32_LEDS = ALL_OFF; }

void full_mode(){ X32_LEDS = LED4; while(!qr.flag_mode); X32_LEDS = ALL_OFF; }

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

void stop_motors()
{
    X32_LEDS = ALL_ON;

    while(qr.ae1 || qr.ae2 || qr.ae3 || qr.ae4)
    {
        qr.ae1 = qr.ae1 - STEP_RPM < 0 ? 0 : qr.ae1 - STEP_RPM;
        qr.ae2 = qr.ae2 - STEP_RPM < 0 ? 0 : qr.ae2 - STEP_RPM;
        qr.ae3 = qr.ae3 - STEP_RPM < 0 ? 0 : qr.ae3 - STEP_RPM;
        qr.ae4 = qr.ae4 - STEP_RPM < 0 ? 0 : qr.ae4 - STEP_RPM;

//        X32_QR_A1 = qr.ae1;
//        X32_QR_A2 = qr.ae2;
//        X32_QR_A3 = qr.ae3;
//        X32_QR_A4 = qr.ae4;

        usleep(REFRESH_TIME);
    }

    qr.lift = 0;
    qr.pitch = 0;
    qr.roll = 0;
    qr.yawrate = 0;
}

void print_drone() //PRINT DRONE STATUS
{
    printf("board> printing drone status:\n");
    printf("mode=%d ", qr.current_mode);
    printf("log=%d ", qr.log);
    printf("flag=%d ", qr.flag_mode);
    printf("link=%d ", qr.link_down);
    printf("exit=%d\n", qr.exit);
    printf("ae1=%d ae2=%d ae3=%d ae4=%d\n", qr.ae1, qr.ae2, qr.ae3, qr.ae4);
    printf("pitch=%d roll=%d yawrate=%d lift=%d\n", qr.pitch, qr.roll, qr.yawrate, qr.lift);
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


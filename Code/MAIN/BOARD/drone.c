/**
@author Gianluca Savaia
*/

#include "drone.h"
#include "../interface/packet.h"
#include "../interface/log.h"

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
    short debug = 0;
    X32_LEDS = LED8;

    stop_motors();

    while(!qr.flag_mode);// X32_DISPLAY = debug++;

    X32_LEDS = ALL_OFF;
}

void panic_mode()
{
    X32_LEDS = LED7;

    DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

    if( qr.ae1 && qr.ae2 && qr.ae3 && qr.ae4 )

        qr.ae1 = PANIC_RPM,
        qr.ae2 = PANIC_RPM,
        qr.ae3 = PANIC_RPM,
        qr.ae3 = PANIC_RPM;

    #ifdef PERIPHERAL_XUFO_A0

    X32_QR_A1 = qr.ae1;
    X32_QR_A2 = qr.ae2;
    X32_QR_A3 = qr.ae3;
    X32_QR_A4 = qr.ae4;

    #endif // PERIPHERAL_XUFO_A0

    qr.lift_force       = qr.scale_lift*( qr.ae1*qr.ae1 + qr.ae2*qr.ae2 + qr.ae3*qr.ae3 + qr.ae4*qr.ae4 );
    qr.pitch_momentum   =  0;
    qr.roll_momentum    =  0;
    qr.yaw_momentum     =  0;

    catnap(PANIC_TIME);

    qr.current_mode = SAFE_MODE;
    qr.flag_mode = 1;
    X32_LEDS = ALL_OFF;

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL);
}

void manual_mode()
{
    int ae1 = 0;
    int ae2 = 0;
    int ae3 = 0;
    int ae4 = 0;
    short debug = 0;

    X32_LEDS = LED6;

    while(!qr.flag_mode)
    {
        X32_DISPLAY = debug++;

        ae1 = ( qr.scale_lift*qr.lift_force  + 2*qr.scale_pitch*qr.pitch_momentum                                           - qr.scale_yaw*qr.yaw_momentum ) / 4;
        ae2 = ( qr.scale_lift*qr.lift_force                                         - 2*qr.scale_roll*qr.roll_momentum      + qr.scale_yaw*qr.yaw_momentum ) / 4;
        ae3 = ( qr.scale_lift*qr.lift_force  - 2*qr.scale_pitch*qr.pitch_momentum                                           - qr.scale_yaw*qr.yaw_momentum ) / 4;
        ae4 = ( qr.scale_lift*qr.lift_force                                         + 2*qr.scale_roll*qr.roll_momentum      + qr.scale_yaw*qr.yaw_momentum ) / 4;

        ae1 = ae1 <= MIN_RPM ? MIN_RPM : float32_to_int32( float32_sqrt( int32_to_float32(ae1) ) );
        ae2 = ae2 <= MIN_RPM ? MIN_RPM : float32_to_int32( float32_sqrt( int32_to_float32(ae2) ) );
        ae3 = ae3 <= MIN_RPM ? MIN_RPM : float32_to_int32( float32_sqrt( int32_to_float32(ae3) ) );
        ae4 = ae4 <= MIN_RPM ? MIN_RPM : float32_to_int32( float32_sqrt( int32_to_float32(ae4) ) );

        qr.ae1 = ae1 > MAX_RPM ? MAX_RPM : (short)ae1;
        qr.ae2 = ae2 > MAX_RPM ? MAX_RPM : (short)ae2;
        qr.ae3 = ae3 > MAX_RPM ? MAX_RPM : (short)ae3;
        qr.ae4 = ae4 > MAX_RPM ? MAX_RPM : (short)ae4;

        DISABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

        #ifdef PERIPHERAL_XUFO_A0

        X32_QR_A1 = qr.ae1;
        X32_QR_A2 = qr.ae2;
        X32_QR_A3 = qr.ae3;
        X32_QR_A4 = qr.ae4;

        #endif

        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
    }

    //X32_DISPLAY = debug++;

    stop_motors();

    X32_LEDS = ALL_OFF;
}

void calibration_mode()
{
    X32_LEDS = LED6;

    catnap(1);

    DISABLE_INTERRUPT(INTERRUPT_XUFO);

    qr.off_ax = X32_QR_S1;
    qr.off_ay = X32_QR_S2;
    qr.off_az = X32_QR_S3;
    qr.off_p = X32_QR_S4;
    qr.off_q = X32_QR_S5;
    qr.off_r = X32_QR_S6;

    ENABLE_INTERRUPT(INTERRUPT_XUFO);

    X32_LEDS = ALL_OFF;
}

void yaw_mode()
{
    short e;

    X32_LEDS = LED5;

    while(!qr.flag_mode)
    {
        //e = qr.sr - qr.yawrate;


    }

    X32_LEDS = ALL_OFF;
}

void full_mode(){ X32_LEDS = LED4; while(!qr.flag_mode); X32_LEDS = ALL_OFF; }

void clear_drone()
{
    qr.ae1 = 0;
    qr.ae2 = 0;
    qr.ae3 = 0;
    qr.ae4 = 0;

    qr.pitch_momentum = 0;
    qr.roll_momentum = 0;
    qr.yaw_momentum = 0;
    qr.lift_force = 0;

    qr.pitch_ref = 0;
    qr.roll_ref = 0;
    qr.yawrate_ref = 0;
    qr.lift_ref = 0;

    qr.scale_pitch = 8240;
    qr.scale_roll = 8240;
    qr.scale_yaw = 16400;
    qr.scale_lift = 16400;

    qr.step_pitch = 5;
    qr.step_roll = 5;
    qr.step_yawrate = 10;
    qr.step_lift = 5;

    qr.sax = 0;
    qr.say = 0;
    qr.saz = 0;
    qr.sp = 0;
    qr.sq = 0;
    qr.sr = 0;

    qr.off_ax = 0;
    qr.off_ay = 0;
    qr.off_az = 0;
    qr.off_p = 0;
    qr.off_q = 0;
    qr.off_r = 0;

    qr.current_mode = SAFE_MODE;
    qr.flag_mode = 0;
    qr.exit = 0;
    qr.link_down = 0;
    qr.log = 0;
    qr.log_size = 0;
}

void stop_motors()
{
    while(qr.ae1 || qr.ae2 || qr.ae3 || qr.ae4)
    {
        qr.ae1 = qr.ae1 - STEP_RPM < 0 ? 0 : qr.ae1 - STEP_RPM;
        qr.ae2 = qr.ae2 - STEP_RPM < 0 ? 0 : qr.ae2 - STEP_RPM;
        qr.ae3 = qr.ae3 - STEP_RPM < 0 ? 0 : qr.ae3 - STEP_RPM;
        qr.ae4 = qr.ae4 - STEP_RPM < 0 ? 0 : qr.ae4 - STEP_RPM;

        #ifdef PERIPHERAL_XUFO_A0

        X32_QR_A1 = qr.ae1;
        X32_QR_A2 = qr.ae2;
        X32_QR_A3 = qr.ae3;
        X32_QR_A4 = qr.ae4;

        #endif // PERIPHERAL_XUFO_A0

        ucatnap(500);
    }

    qr.lift_force = 0;
    qr.pitch_momentum = 0;
    qr.roll_momentum = 0;
    qr.yaw_momentum = 0;

    qr.lift_ref = 0;
    qr.pitch_ref = 0;
    qr.roll_ref = 0;
    qr.yawrate_ref = 0;
}

void print_drone() //PRINT DRONE STATUS
{
    printf("board> printing drone status:\n");
    printf("mode=%d ", qr.current_mode);
    printf("log=%d ", qr.log);
    printf("flag=%d ", qr.flag_mode);
    printf("link=%d ", qr.link_down);
    printf("exit=%d\n", qr.exit);
    printf("scales: pitch=%d roll=%d yawrate=%d lift=%d\n", qr.scale_pitch, qr.scale_roll, qr.scale_yaw, qr.scale_lift);
    printf("ae1=%d ae2=%d ae3=%d ae4=%d\n", qr.ae1, qr.ae2, qr.ae3, qr.ae4);
    printf("pitch=%d roll=%d yawrate=%d lift=%d\n", qr.pitch_momentum, qr.roll_momentum, qr.yaw_momentum, qr.lift_force);
}

void catnap(int ms)
{
    int now = X32_CLOCK_MS;

    while( X32_CLOCK_MS < now + ms);
}

void ucatnap(int us)
{
    int now = X32_CLOCK_US;

    while( X32_CLOCK_US < now + us);
}

void add_log()
{
    struct log_s new_log;

    if( !qr.log )
        return;

    if( qr.log_size >= LOG_BUFFER_SIZE )
    {
        qr.log = 0;
        return;
    }

    printf("board> Adding Log.\n");

    DISABLE_INTERRUPT(INTERRUPT_GLOBAL); //SAVE LOG ATOMICALLY

    new_log.id = qr.log_size;

    #ifdef X32_XUFO_TIMESTAMP
    new_log.timestamp = X32_XUFO_TIMESTAMP;
    #else
    new_log.timestamp = X32_CLOCK_MS;
    #endif

    new_log.ae1 = qr.ae1;
    new_log.ae2 = qr.ae2;
    new_log.ae3 = qr.ae3;
    new_log.ae4 = qr.ae4;

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL); //END LOG

    qr.log_buffer[qr.log_size] = new_log; // write log into buffer

    qr.log_size++;

    printf("board> Log added %d %d %d %d %d %d.\n", new_log.id, new_log.timestamp, new_log.ae1, new_log.ae2, new_log.ae3, new_log.ae4);
}


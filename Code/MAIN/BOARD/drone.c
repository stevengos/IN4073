/**
@author Gianluca Savaia
*/

#include "drone.h"
#include "mafilter.h"

extern struct drone qr;

/**
    State Machine for the Quad-Rotor
*/
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

    stop_motors();

    TURNON_LED(LED8);

    while(!qr.flag_mode) BLINK_LED(LED1);

    TURNOFF_LED(LED8);
}

void panic_mode()
{
    int time;

    DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

    #ifdef PERIPHERAL_XUFO_A0
    X32_QR_A1 = qr.ae1;
    X32_QR_A2 = qr.ae1;
    X32_QR_A3 = qr.ae1;
    X32_QR_A4 = qr.ae1;
    #endif

    while( qr.ae1 > PANIC_RPM )
    {
        qr.ae1 = qr.ae1 - STEP_RPM;
        qr.ae2 = qr.ae1 - STEP_RPM;
        qr.ae3 = qr.ae1 - STEP_RPM;
        qr.ae4 = qr.ae1 - STEP_RPM;

        #ifdef PERIPHERAL_XUFO_A0
        X32_QR_A1 = qr.ae1;
        X32_QR_A2 = qr.ae2;
        X32_QR_A3 = qr.ae3;
        X32_QR_A4 = qr.ae4;
        #endif

        ucatnap(500);
    }

    time = X32_CLOCK_MS + PANIC_TIME;

    while( X32_CLOCK_MS < time )
    {
        #ifdef PERIPHERAL_XUFO_A0
        X32_QR_A1 = qr.ae1;
        X32_QR_A2 = qr.ae2;
        X32_QR_A3 = qr.ae3;
        X32_QR_A4 = qr.ae4;
        #endif

        catnap(200);
        BLINK_LED(ALL_ON);
    }

    qr.lift_force       =  qr.scale_lift*( qr.ae1*qr.ae1 + qr.ae2*qr.ae2 + qr.ae3*qr.ae3 + qr.ae4*qr.ae4 );
    qr.pitch_momentum   =  0;
    qr.roll_momentum    =  0;
    qr.yaw_momentum     =  0;

    stop_motors();

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

    TURNON_LED(LED7);

    while(!qr.flag_mode)
    {
        BLINK_LED(LED1);

        if( qr.lift_force )
        {
            DISABLE_INTERRUPT(INTERRUPT_GLOBAL);
            ae1 = ( qr.scale_lift*qr.lift_force  - 2*qr.scale_pitch*qr.pitch_momentum                                           + qr.scale_yaw*qr.yaw_momentum ) / 4;
            ae2 = ( qr.scale_lift*qr.lift_force                                         + 2*qr.scale_roll*qr.roll_momentum      - qr.scale_yaw*qr.yaw_momentum ) / 4;
            ae3 = ( qr.scale_lift*qr.lift_force  + 2*qr.scale_pitch*qr.pitch_momentum                                           + qr.scale_yaw*qr.yaw_momentum ) / 4;
            ae4 = ( qr.scale_lift*qr.lift_force                                         - 2*qr.scale_roll*qr.roll_momentum      - qr.scale_yaw*qr.yaw_momentum ) / 4;
            ENABLE_INTERRUPT(INTERRUPT_GLOBAL);

            ae1 = ae1 <= MIN_RPM ? MIN_RPM : sqrt(ae1);
            ae2 = ae2 <= MIN_RPM ? MIN_RPM : sqrt(ae2);
            ae3 = ae3 <= MIN_RPM ? MIN_RPM : sqrt(ae3);
            ae4 = ae4 <= MIN_RPM ? MIN_RPM : sqrt(ae4);

            ae1 = ae1 > MAX_RPM ? MAX_RPM : ae1;
            ae2 = ae2 > MAX_RPM ? MAX_RPM : ae2;
            ae3 = ae3 > MAX_RPM ? MAX_RPM : ae3;
            ae4 = ae4 > MAX_RPM ? MAX_RPM : ae4;

            qr.ae1 = ae1 - qr.ae1 > STEP_RPM ? qr.ae1 + STEP_RPM : qr.ae1 - ae1 > STEP_RPM ? qr.ae1 - STEP_RPM : ae1;
            qr.ae2 = ae2 - qr.ae2 > STEP_RPM ? qr.ae2 + STEP_RPM : qr.ae2 - ae2 > STEP_RPM ? qr.ae2 - STEP_RPM : ae2;
            qr.ae3 = ae3 - qr.ae3 > STEP_RPM ? qr.ae3 + STEP_RPM : qr.ae3 - ae3 > STEP_RPM ? qr.ae3 - STEP_RPM : ae3;
            qr.ae4 = ae4 - qr.ae4 > STEP_RPM ? qr.ae4 + STEP_RPM : qr.ae4 - ae4 > STEP_RPM ? qr.ae4 - STEP_RPM : ae4;

            DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

            #ifdef PERIPHERAL_XUFO_A0

            X32_QR_A1 = qr.ae1;
            X32_QR_A2 = qr.ae2;
            X32_QR_A3 = qr.ae3;
            X32_QR_A4 = qr.ae4;

            #endif

            ENABLE_INTERRUPT(INTERRUPT_GLOBAL);
        }
        else
            stop_motors();
    }

    TURNOFF_LED(LED7);
}

void calibration_mode()
{
    int samples = 100;
    int i = 0;
    int average = 0;

    DISABLE_INTERRUPT(INTERRUPT_XUFO);

    panic_blink(2);

/****************************************************/
    for(i=0, average=0; i<samples; i++, catnap(1))
        average += X32_QR_S1;

    average = average/samples;
    qr.off_ax = average;

/****************************************************/
    for(i=0, average=0; i<samples; i++, catnap(1))
        average += X32_QR_S2;

    average = average/samples;
    qr.off_ay = average;

/****************************************************/
    for(i=0, average=0; i<samples; i++, catnap(1))
        average += X32_QR_S3;

    average = average/samples;
    qr.off_az = average;

/****************************************************/
    for(i=0, average=0; i<samples; i++, catnap(1))
        average += X32_QR_S4;

    average = average/samples;
    qr.off_p = average;

/****************************************************/
    for(i=0, average=0; i<samples; i++, catnap(1))
        average += X32_QR_S5;

    average = average/samples;
    qr.off_q = average;

/****************************************************/
    for(i=0, average=0; i<samples; i++, catnap(1))
        average += X32_QR_S6;

    average = average/samples;
    qr.off_r = average;

    ENABLE_INTERRUPT(INTERRUPT_XUFO);

    qr.current_mode = SAFE_MODE;
    qr.flag_mode = 1;
}

void yaw_mode()
{
    short e = 0;
    int ae1 = 0;
    int ae2 = 0;
    int ae3 = 0;
    int ae4 = 0;

    TURNON_LED(LED6);

    qr.scale_pitch  = 8240/4;
    qr.scale_roll   = 8240/4;
    qr.scale_yaw    = 2000;
    qr.scale_lift   = 16400/2;

    qr.controller_pitch = 0;
    qr.controller_roll = 0;
    qr.controller_yaw = 7;

    while(!qr.flag_mode)
    {
        BLINK_LED(LED1);

        if(qr.lift_force)
        {
            DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

            e = qr.yawrate_ref + qr.fr;
            qr.yaw_momentum = qr.controller_yaw * e;

            ae1 = ( qr.scale_lift*qr.lift_force  - 2*qr.scale_pitch*qr.pitch_momentum                                           + qr.scale_yaw*qr.yaw_momentum ) / 4;
            ae2 = ( qr.scale_lift*qr.lift_force                                         + 2*qr.scale_roll*qr.roll_momentum      - qr.scale_yaw*qr.yaw_momentum ) / 4;
            ae3 = ( qr.scale_lift*qr.lift_force  + 2*qr.scale_pitch*qr.pitch_momentum                                           + qr.scale_yaw*qr.yaw_momentum ) / 4;
            ae4 = ( qr.scale_lift*qr.lift_force                                         - 2*qr.scale_roll*qr.roll_momentum      - qr.scale_yaw*qr.yaw_momentum ) / 4;

            ENABLE_INTERRUPT(INTERRUPT_GLOBAL);

            ae1 = ae1 <= MIN_RPM ? MIN_RPM : sqrt(ae1);
            ae2 = ae2 <= MIN_RPM ? MIN_RPM : sqrt(ae2);
            ae3 = ae3 <= MIN_RPM ? MIN_RPM : sqrt(ae3);
            ae4 = ae4 <= MIN_RPM ? MIN_RPM : sqrt(ae4);

            ae1 = ae1 > MAX_RPM ? MAX_RPM : ae1;
            ae2 = ae2 > MAX_RPM ? MAX_RPM : ae2;
            ae3 = ae3 > MAX_RPM ? MAX_RPM : ae3;
            ae4 = ae4 > MAX_RPM ? MAX_RPM : ae4;

            qr.ae1 = ae1 - qr.ae1 > STEP_RPM ? qr.ae1 + STEP_RPM : qr.ae1 - ae1 > STEP_RPM ? qr.ae1 - STEP_RPM : ae1;
            qr.ae2 = ae2 - qr.ae2 > STEP_RPM ? qr.ae2 + STEP_RPM : qr.ae2 - ae2 > STEP_RPM ? qr.ae2 - STEP_RPM : ae2;
            qr.ae3 = ae3 - qr.ae3 > STEP_RPM ? qr.ae3 + STEP_RPM : qr.ae3 - ae3 > STEP_RPM ? qr.ae3 - STEP_RPM : ae3;
            qr.ae4 = ae4 - qr.ae4 > STEP_RPM ? qr.ae4 + STEP_RPM : qr.ae4 - ae4 > STEP_RPM ? qr.ae4 - STEP_RPM : ae4;

            DISABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

            #ifdef PERIPHERAL_XUFO_A0

            X32_QR_A1 = qr.ae1;
            X32_QR_A2 = qr.ae2;
            X32_QR_A3 = qr.ae3;
            X32_QR_A4 = qr.ae4;

            #endif

            ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
        }
        else
            stop_motors();
    }

    TURNOFF_LED(LED6);
}

/**
@Profiling: 540us for the main loop
*/
void full_mode()
{
    int e_p = 0;
    int e_q = 0;
    int e_r = 0;

    int ae1 = 0;
    int ae2 = 0;
    int ae3 = 0;
    int ae4 = 0;

    TURNON_LED(LED5);

    qr.controller_pitch = 60*1000;
    qr.controller_roll = 60*1000;
    qr.controller_yaw = 240*1000;

    qr.scale_pitch  = 1;
    qr.scale_roll   = 1;
    qr.scale_yaw    = 1;
    qr.scale_lift   = 4500;

    while(!qr.flag_mode)
    {
        BLINK_LED(LED1);

        if(qr.lift_force)
        {
            DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

/* RATE CONTROL */
            e_p = qr.pitch_ref/10 - qr.fq;
            e_p += (qr.fax/7);
            qr.pitch_momentum = qr.controller_pitch * e_p;

            e_q = qr.roll_ref/10 - qr.fp;
            e_q +=  - (qr.fay/8);
            qr.roll_momentum = qr.controller_roll * e_q;
/* END RATE CONTROL*/

            e_r = qr.yawrate_ref - qr.fr;
            qr.yaw_momentum = qr.controller_yaw * e_r;

            ae1 = ( qr.scale_lift*qr.lift_force  - 2*qr.scale_pitch*qr.pitch_momentum                                           + qr.scale_yaw*qr.yaw_momentum ) / 4;
            ae2 = ( qr.scale_lift*qr.lift_force                                         + 2*qr.scale_roll*qr.roll_momentum      - qr.scale_yaw*qr.yaw_momentum ) / 4;
            ae3 = ( qr.scale_lift*qr.lift_force  + 2*qr.scale_pitch*qr.pitch_momentum                                           + qr.scale_yaw*qr.yaw_momentum ) / 4;
            ae4 = ( qr.scale_lift*qr.lift_force                                         - 2*qr.scale_roll*qr.roll_momentum      - qr.scale_yaw*qr.yaw_momentum ) / 4;

            ENABLE_INTERRUPT(INTERRUPT_GLOBAL);

            ae1 = ae1 <= MIN_RPM ? MIN_RPM : sqrt(ae1);
            ae2 = ae2 <= MIN_RPM ? MIN_RPM : sqrt(ae2);
            ae3 = ae3 <= MIN_RPM ? MIN_RPM : sqrt(ae3);
            ae4 = ae4 <= MIN_RPM ? MIN_RPM : sqrt(ae4);

            ae1 = ae1 > MAX_RPM ? MAX_RPM : ae1;
            ae2 = ae2 > MAX_RPM ? MAX_RPM : ae2;
            ae3 = ae3 > MAX_RPM ? MAX_RPM : ae3;
            ae4 = ae4 > MAX_RPM ? MAX_RPM : ae4;

            qr.ae1 = ae1 - qr.ae1 > STEP_RPM ? qr.ae1 + STEP_RPM : qr.ae1 - ae1 > STEP_RPM ? qr.ae1 - STEP_RPM : ae1;
            qr.ae2 = ae2 - qr.ae2 > STEP_RPM ? qr.ae2 + STEP_RPM : qr.ae2 - ae2 > STEP_RPM ? qr.ae2 - STEP_RPM : ae2;
            qr.ae3 = ae3 - qr.ae3 > STEP_RPM ? qr.ae3 + STEP_RPM : qr.ae3 - ae3 > STEP_RPM ? qr.ae3 - STEP_RPM : ae3;
            qr.ae4 = ae4 - qr.ae4 > STEP_RPM ? qr.ae4 + STEP_RPM : qr.ae4 - ae4 > STEP_RPM ? qr.ae4 - STEP_RPM : ae4;

            DISABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

            #ifdef PERIPHERAL_XUFO_A0

            X32_QR_A1 = qr.ae1;
            X32_QR_A2 = qr.ae2;
            X32_QR_A3 = qr.ae3;
            X32_QR_A4 = qr.ae4;

            #endif

            ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
        }
        else
            stop_motors();
    }

    TURNOFF_LED(LED5);
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

    qr.pitch_ref = 0;
    qr.roll_ref = 0;
    qr.yawrate_ref = 0;
}

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

    //values before the slash are the ones which allow to have full output power
    qr.scale_pitch  = 8240/4;
    qr.scale_roll   = 8240/4;
    qr.scale_yaw    = 16400/2;
    qr.scale_lift   = 16400/2;

    qr.controller_pitch = 0;
    qr.controller_roll = 0;
    qr.controller_yaw = 0;

    qr.off_pitch = 0;
    qr.off_roll = 0;
    qr.off_yawrate = 0;
    qr.off_lift = 0;

    qr.sax = 0;
    qr.say = 0;
    qr.saz = 0;
    qr.sp = 0;
    qr.sq = 0;
    qr.sr = 0;

    qr.fax = 0;
    qr.fay = 0;
    qr.faz = 0;
    qr.fp = 0;
    qr.fq = 0;
    qr.fr = 0;

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
    qr.log_full = 0;

    initfilter();
    X32_LEDS = ALL_OFF;
}

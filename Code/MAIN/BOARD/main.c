/**
@author Gianluca Savaia
@version 1.0
@last update 2015-09-11
*/

#include <stdio.h>
#include "x32.h"
#include "static.h"

#include "../interface/packet.h"
#include "commands.h"
#include "drone.h"
#include "isr.h"
#include "mode.h"

struct drone qr; // OUR DRONE

void delay(int ms)
{
	int time = X32_clock;
	while(X32_clock - time < ms);
}

int main()
{
    char c;

    while (X32_rs232_ready)
        c = X32_rs232_data; // empty buffer

    //set ISR vector, buttons
    SET_INTERRUPT_VECTOR(INTERRUPT_BUTTONS, &isr_buttons);
    SET_INTERRUPT_PRIORITY(INTERRUPT_BUTTONS, 10);
    ENABLE_INTERRUPT(INTERRUPT_BUTTONS);

    //prepare rs232 rx interrupt
    SET_INTERRUPT_VECTOR(INTERRUPT_PRIMARY_RX, &isr_rs232_rx);
    SET_INTERRUPT_PRIORITY(INTERRUPT_PRIMARY_RX, 20);
    ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

    //reset output
    X32_leds = 0x0000;
    X32_display = 0x0000;

    //reset drone properties
    qr.current_mode = SAFE_MODE;
    qr.flag_mode = 0;
    qr.exit = 0;
    qr.rpm1 = 0;
    qr.rpm2 = 0;
    qr.rpm3 = 0;
    qr.rpm4 = 0;
    qr.pitch = 0;
    qr.roll = 0;
    qr.yawrate = 0;
    qr.lift = 0;

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL);

    printf("setup complete. ");

    //Main process
    while( !qr.exit )
    {
        qr.flag_mode = 0;

        switch(qr.current_mode)
        {
            case SAFE_MODE:
                while(!qr.flag_mode);
                break;
            case PANIC_MODE:
                delay(PANIC_TIME);
                break;

            case MANUAL_MODE:
                //disable interrupts from sensors
                manual_mode();
                break;

            case CALIBRATION_MODE:
                //calibrate_sensors();
                break;

            case YAW_MODE:
                yaw_mode(); break;

            case FULL_MODE:
                //full_control();
                break;
        };
    }

    X32_display = 0xC1A0;
    X32_leds = 0x0000;

    return 0;
}


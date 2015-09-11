/**
@author Gianluca Savaia
@version 1.0
@last update 2015-09-11
*/

#include <stdio.h>
#include "x32.h"

#include "../interface/packet.h"
#include "commands.h"
#include "drone.h"

#define X32_display	peripherals[PERIPHERAL_DISPLAY]
#define X32_leds	peripherals[PERIPHERAL_LEDS]
#define X32_buttons	peripherals[PERIPHERAL_BUTTONS]
#define X32_clock	peripherals[PERIPHERAL_MS_CLOCK]

#define X32_rs232_data	peripherals[PERIPHERAL_PRIMARY_DATA]
#define X32_rs232_stat	peripherals[PERIPHERAL_PRIMARY_STATUS]
#define X32_rs232_ready (X32_rs232_stat & 0x02)

struct drone qr; // OUR DRONE

void delay(int ms)
{
	int time = X32_clock;
	while(X32_clock - time < ms);
}

void isr_buttons()
{
    printf("board> EXIT PROGRAM\n");
    qr.exit = 1;
    qr.flag_mode = 1;
}

void isr_rs232_rx(void)
{
    char header;
    char command;

    printf("board> inside isr_rs232_rx\n");

    DISABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

    if( !X32_rs232_ready )
        return;

    header = X32_rs232_data; //read first byte (HEADER)

    //if( !X32_rs232_ready )
    //    return;

    while(!X32_rs232_ready);

    command = X32_rs232_data; //read second byte (COMMAND)

    perform_command(header, command);

    ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
}

void manual_control()
{
    while(!qr.flag_mode)
    {
        //read data from struct and apply to motors
    }
}

void yaw_control()
{
    char eps; //error signal
    char regulator = 49;

    while(!qr.flag_mode)
    {
        //atomic
        eps = qr.yawrate - qr.syawrate;

        /*
            ae1 = regulator * eps;
            ...
        */
    }
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
            case SAFE_MODE:             printf("entered safe mode. ");
                while(!qr.flag_mode);
                break;
            case PANIC_MODE:
                delay(PANIC_TIME);
                break;

            case MANUAL_MODE:
                //disable interrupts from sensors
                manual_control();
                break;

            case CALIBRATION_MODE:
                //calibrate_sensors();
                break;

            case YAW_MODE:
                yaw_control(); break;

            case FULL_MODE:
                //full_control();
                break;
        };
    }

    X32_display = 0xC1A0;
    X32_leds = 0x0000;

    return 0;
}

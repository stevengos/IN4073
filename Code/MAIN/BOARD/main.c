/**
@author Gianluca Savaia
@last update 2015-09-15
*/

#include <stdio.h>
#include "x32.h"
#include "static.h"

#include "drone.h"
#include "isr.h"


struct drone qr; // OUR DRONE

int main()
{
    char c;

    while (X32_RS232_READ)
        c = X32_RS232_DATA; // empty buffer

    //set ISR vector, buttons
    SET_INTERRUPT_VECTOR(INTERRUPT_BUTTONS, &isr_buttons);
    SET_INTERRUPT_PRIORITY(INTERRUPT_BUTTONS, 100);
    ENABLE_INTERRUPT(INTERRUPT_BUTTONS);

    //prepare rs232 rx interrupt
    SET_INTERRUPT_VECTOR(INTERRUPT_PRIMARY_RX, &isr_rs232_rx);
    SET_INTERRUPT_PRIORITY(INTERRUPT_PRIMARY_RX, 20);
    ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

    //prepare rs232 rx interrupt
    X32_TIMER_CYCLE = TIMEOUT_TIMER*CLOCKS_PER_MS;
    SET_INTERRUPT_VECTOR(INTERRUPT_TIMER1, &isr_timer);
    SET_INTERRUPT_PRIORITY(INTERRUPT_TIMER1, 10);
    ENABLE_INTERRUPT(INTERRUPT_TIMER1);

    //reset output
    X32_LEDS = 0x0000;
    X32_DISPLAY = 0x0000;

    //reset drone properties
    clear_drone();

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL);

    run_drone();

    X32_DISPLAY = 0xC1A0;

    return 0;
}

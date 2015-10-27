/**
@author Gianluca Savaia

This is the main program which is launched at startup.
The board is initialized (interrupts, buffers, leds, ...) and the main loop is called.
*/

#include "x32.h"
#include "static.h"

#include "drone.h"
#include "isr.h"

struct drone qr; /* This struct contains all the information regarding the status of the QR */

int main()
{
    flush_buffer();

    //enable interrupt for buttons
    SET_INTERRUPT_VECTOR(INTERRUPT_BUTTONS, &isr_buttons);
    SET_INTERRUPT_PRIORITY(INTERRUPT_BUTTONS, 100);
    ENABLE_INTERRUPT(INTERRUPT_BUTTONS);

    //enable interrupt for sensors
    SET_INTERRUPT_VECTOR(INTERRUPT_XUFO, &isr_sensors);
    SET_INTERRUPT_PRIORITY(INTERRUPT_XUFO, 90);
    ENABLE_INTERRUPT(INTERRUPT_XUFO);

    //enable interrupt for rs232 rx
    SET_INTERRUPT_VECTOR(INTERRUPT_PRIMARY_RX, &isr_rs232_rx);
    SET_INTERRUPT_PRIORITY(INTERRUPT_PRIMARY_RX, 80);
    ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

    //prepare timer interrupt
    X32_TIMER_CYCLE = TIMEOUT_TIMER*CLOCKS_PER_MS;
    SET_INTERRUPT_VECTOR(INTERRUPT_TIMER1, &isr_timer);
    SET_INTERRUPT_PRIORITY(INTERRUPT_TIMER1, 1);

    #ifdef PERIPHERAL_DISPLAY
    X32_DISPLAY = 0xAAAA;
    #endif

    clear_drone(); //reset drone properties

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL);

    run_drone(); /* Launching main loop (state machine) */

    #ifdef PERIPHERAL_DISPLAY
    X32_DISPLAY = 0xC1A0;
    #endif

    X32_LEDS = ALL_OFF;

    DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

    return 0;
}


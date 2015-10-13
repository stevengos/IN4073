#include <stdio.h>
#include "x32.h"

#include "packet.h"
#include "commands.h"

#define X32_display	peripherals[PERIPHERAL_DISPLAY]
#define X32_leds	peripherals[PERIPHERAL_LEDS]
#define X32_buttons	peripherals[PERIPHERAL_BUTTONS]
#define X32_clock	peripherals[PERIPHERAL_MS_CLOCK]

#define X32_rs232_data	peripherals[PERIPHERAL_PRIMARY_DATA]
#define X32_rs232_stat	peripherals[PERIPHERAL_PRIMARY_STATUS]
#define X32_rs232_ready (X32_rs232_stat & 0x02)

extern int lol;

int done;

void delay(int ms)
{
	int time = X32_clock;
	while(X32_clock - time < ms);
}

void turnon_led(char command)
{
    X32_leds = command;
}

void perform_command(char label, char command)
{
    X32_display = 0xDDDD; delay(2000);

    switch(label)
    {
        case 0x0: break;
        case 0x1: break;
        case 0x2: break;
        case 0x3: break;
        case 0x4: break;
        case 0x5: break;
        case 0x6: break;
        case 0x7: break;
        case 0x8: break;
        case 0x9: turnon_led(command); break;
        default: return;
    };
}


void isr_rs232_rx(void)
{
    char label;
    char command;

        X32_display = 0xAAAA; delay(2000);


    DISABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

    if( !X32_rs232_ready )
        return;

    label = X32_rs232_data; //read first byte (LABEL)

    if( !X32_rs232_ready )
        return;

    command = X32_rs232_data; //read second byte (COMMAND)

    perform_command(label, command);

    ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
}

void isr_buttons(void)
{
    X32_display = 0x8B8B;
    X32_leds = 0x0000;
    done = 1;
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

        X32_leds = 0x0000;
        X32_display = 0x0000;
        done = 0;
        c = 0;

        ENABLE_INTERRUPT(INTERRUPT_GLOBAL);

        while(!done)
        {
            X32_display = c++;
            delay(1000);
        }

        return 0;
}

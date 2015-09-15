/**
@author Gianluca Savaia
@last update 2015-09-15
*/

#include "isr.h"

#include "drone.h"
#include <stdio.h>

extern struct drone qr;

void isr_buttons(void)
{
    printf("board> Button Pressed: stopping machine.\n");
    qr.exit = 1;
    qr.flag_mode = 1;
}

void isr_rs232_rx(void)
{
    char header;
    char command;
    char crc;

    char counter = 0;

    DISABLE_INTERRUPT( INTERRUPT_PRIMARY_RX );

    if( !X32_RS232_READ )
    {
        printf("board> Interrupt from RS232 arrived, but buffer was empty.\n");
        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
        return;
    }

    header = X32_RS232_DATA; //read first byte (HEADER)

    while( !X32_RS232_READ )
    {
        if( counter++ > TIMEOUT_BUFFER_RX )
        {
            printf("board> Waiting for payload...\n");
            ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
            return;
        }
        else
            usleep(SLEEP_BUFFER_RX);
    }

    command = X32_RS232_DATA; //read second byte (COMMAND)

    while( !X32_RS232_READ );
    {
        if( counter++ > TIMEOUT_BUFFER_RX )
        {
            printf("board> Waiting for checksum...\n");
            ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
            return;
        }
        else
            usleep(SLEEP_BUFFER_RX);
    }

    crc = X32_RS232_DATA; //read third byte (CRC)

    printf("board> Packet received.\n");

    perform_command(header, command);

    //ACKNOWLEDGEMENT ???

    ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
}

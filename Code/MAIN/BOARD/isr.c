/**
@author Gianluca Savaia
@last update 2015-09-15
*/

#include "isr.h"

#include "drone.h"

#include "../interface/packet.h"
#include "../interface/hamming.h"

#include <stdio.h>

extern struct drone qr;

void isr_buttons(void)
{
    add_log();
}

void isr_rs232_rx(void)
{
    packet_t incoming;
    char counter = 0;

    qr.link_down = 0;

    DISABLE_INTERRUPT( INTERRUPT_PRIMARY_RX );

    if( !X32_RS232_READ )
    {
        printf("board> Interrupt from RS232 arrived, but buffer was empty.\n");
        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
        return;
    }

    incoming.header = X32_RS232_DATA; //read first byte (HEADER)

    while( !X32_RS232_READ )
    {
        if( counter++ > TIMEOUT_BUFFER_RX )
        {
            printf("board> Packet incomplete.\n");
            ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
            acknowledge(ACK_NEGATIVE);
            return;
        }
        else
            usleep(SLEEP_BUFFER_RX);
    }

    incoming.command = X32_RS232_DATA; //read second byte (COMMAND)

    while( !X32_RS232_READ );
    {
        if( counter++ > TIMEOUT_BUFFER_RX )
        {
            printf("board> Waiting for checksum...\n");
            ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
            acknowledge(ACK_NEGATIVE);
            return;
        }
        else
            usleep(SLEEP_BUFFER_RX);
    }

    incoming.crc = X32_RS232_DATA; //read third byte (CRC)

    printf("board> Packet received.\n");

    if( check_hamming(incoming) )
    {
        printf("board> Checksum verified.\n");
        perform_command(incoming.header, incoming.command);
    }
    else
    {
        printf("board> Packet is corrupt.\n");
        acknowledge(ACK_NEGATIVE);
    }

    ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
}


void isr_timer(void)
{
    if( qr.link_down )
    {
        printf("board> PC Link is down! SAFE_MODE set.\n");
        qr.current_mode = SAFE_MODE;
        qr.exit = 1;
        qr.flag_mode = 1;
    }

    qr.link_down = 1;
}

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
short debug = 0;

void isr_buttons(void)
{
    stop();
}

void isr_sensors(void)
{
    qr.sax = X32_QR_S1;
    qr.say = X32_QR_S2;
    qr.saz = X32_QR_S3;
    qr.sp = X32_QR_S4;
    qr.sq = X32_QR_S5;
    qr.sr = X32_QR_S6;
}

void isr_rs232_rx(void)
{
    packet_t incoming;
    char counter = 0;

    qr.link_down = 0;

    //X32_DISPLAY = debug++;

    if( !X32_RS232_READ )
    {
        return;
    }

    incoming.header = X32_RS232_DATA; //read first byte (HEADER)

    while( !X32_RS232_READ )
    {
        if( counter++ > TIMEOUT_BUFFER_RX )
        {
            acknowledge(ACK_NEGATIVE);
            return;
        }
        else
            ucatnap(SLEEP_BUFFER_RX);
    }

    incoming.command = X32_RS232_DATA; //read second byte (COMMAND)

    while( !X32_RS232_READ );
    {
        if( counter++ > TIMEOUT_BUFFER_RX )
        {
            acknowledge(ACK_NEGATIVE);
            return;
        }
        else
            ucatnap(SLEEP_BUFFER_RX);
    }

    incoming.crc = X32_RS232_DATA; //read third byte (CRC)

    if( check_hamming(incoming) )
        perform_command(incoming.header, incoming.command);
    else
        acknowledge(ACK_NEGATIVE);
}


void isr_timer(void)
{
    if( qr.link_down )
    {
        printf("board> PC Link is down! SAFE_MODE set.\n");
        qr.current_mode = SAFE_MODE;
        stop_motors();
        qr.exit = 1;
        qr.flag_mode = 1;
        X32_LEDS = LED1;
    }

    qr.link_down = 1;
}

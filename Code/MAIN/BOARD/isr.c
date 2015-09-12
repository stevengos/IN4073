/**
@author Gianluca Savaia
@last update 2015-09-12
*/

#include "isr.h"

#include "drone.h"
#include <stdio.h>

extern struct drone qr;

void isr_buttons(void)
{
    printf("board> EXIT PROGRAM\n");
    qr.exit = 1;
    qr.flag_mode = 1;
}

void isr_rs232_rx(void)
{
    char header;
    char command;
    char crc;

    printf("board> inside isr_rs232_rx\n");

    DISABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

    if( !X32_rs232_ready )
        return;

    header = X32_rs232_data; //read first byte (HEADER)

    if( !X32_rs232_ready )
        return;

    command = X32_rs232_data; //read second byte (COMMAND)

    if( !X32_rs232_ready )
        return;

    crc = X32_rs232_data; //read third byte (CRC)

    perform_command(header, command);

    //ACKNOWLEDGEMENT ???

    ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);
}

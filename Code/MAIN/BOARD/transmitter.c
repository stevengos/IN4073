/**
@author Gianluca Savaia

This module is in charge of the data transmission with the PC terminal.
Every other module on-board which need to send data has to pass through this one.
*/

#include "transmitter.h"

/**
@author Gianluca Savaia
This method is used to acknowledge the PC terminal, sending back a feedback which can be evaluated PC side
*/
void acknowledge(char response)
{
    char counter = 0;
    packet_t packet;

    // check that 'response' is a valid acknowledge (look at packet.h)
    if( response != ACK_POSITIVE && response != ACK_NEGATIVE && response != ACK_INVALID && response != ACK_HAMMING)
        response = ACK_NEGATIVE;

    while( !X32_RS232_WRITE )
    {
        if( counter++ > TIMEOUT_BUFFER_TX )
            return;
        else
            ucatnap(SLEEP_BUFFER_TX);
    }

    X32_RS232_DATA = ACK;               //header

    while( !X32_RS232_WRITE )
    {
        if( counter++ > TIMEOUT_BUFFER_TX )
            return;
        else
            ucatnap(SLEEP_BUFFER_TX);
    }

    X32_RS232_DATA = response;          //command

    while( !X32_RS232_WRITE )
    {
        if( counter++ > TIMEOUT_BUFFER_TX )
            return;
        else
            ucatnap(SLEEP_BUFFER_TX);
    }

    packet.header = ACK;
    packet.command = response;

    compute_hamming(&packet);

    X32_RS232_DATA = packet.crc;              //checksum
}

/**
@author Gianluca Savaia
The UART allows to send only one byte at the time. This function allows to send a short (2 bytes)
over the link by splitting it into two parts and send them consequently
*/
void send_short(short value)
{
    unsigned char c1, c2;

    c1 = value;
    c2 = (value >> 8);

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c1;

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c2;
}

/**
@author Gianluca Savaia
The UART allows to send only one byte at the time. This function allows to send an integer (4 bytes)
over the link by splitting it into four parts and send them consequently
*/
void send_int(int value)
{
    unsigned char c1, c2, c3, c4;

    c1 = value;
    c2 = (value >> 8);
    c3 = (value >> 16);
    c4 = (value >> 24);

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c1;

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c2;

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c3;

    while(!X32_RS232_WRITE);
    X32_RS232_DATA = c4;
}



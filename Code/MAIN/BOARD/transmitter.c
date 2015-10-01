/**
@author Gianluca Savaia
*/

#include "transmitter.h"

void acknowledge(char response)
{
    char counter = 0;
    packet_t packet;

    if( response != ACK_POSITIVE && response != ACK_NEGATIVE && response != ACK_INVALID)
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



/**
@author Gianluca Savaia
*/

#include "hamming.h"

/**
@author Gianluca Savaia
Computes the hamming code for struct packet_s with 16bits data and 5bits parity (encapsulated in one byte).
The implementation is optimized so to be as fast as possible.
As a further work it can be implemented the error correction (allowed by the hamming code).
*/
void compute_hamming(packet_t* packet)
{
    char code = 0;
    char sum = 0;
    char header = packet->header;
    char command = packet->command;

    //1BIT PARITY
    sum = 0;

    sum += (header & MSB1) >> 7;
    sum += (header & MSB2) >> 6;
    sum += (header & MSB4) >> 4;
    sum += (header & MSB5) >> 3;
    sum += (header & MSB7) >> 1;

    sum += (command & MSB1) >> 7;
    sum += (command & MSB3) >> 5;
    sum += (command & MSB4) >> 4;
    sum += (command & MSB6) >> 2;
    sum += (command & MSB8);

    code |= (sum % 2);

    //2BIT PARITY
    sum = 0;

    sum += (header & MSB1) >> 7;
    sum += (header & MSB3) >> 5;
    sum += (header & MSB4) >> 4;
    sum += (header & MSB6) >> 2;
    sum += (header & MSB7) >> 1;

    sum += (command & MSB2) >> 6;
    sum += (command & MSB3) >> 5;
    sum += (command & MSB5) >> 3;
    sum += (command & MSB6) >> 2;

    code |= ( (sum % 2) << 1 );

    //3BIT PARITY
    sum = 0;

    sum += (header & MSB2) >> 6;
    sum += (header & MSB3) >> 5;
    sum += (header & MSB4) >> 4;
    sum += (header & MSB8);

    sum += (command & MSB1) >> 7;
    sum += (command & MSB2) >> 6;
    sum += (command & MSB3) >> 5;
    sum += (command & MSB7) >> 1;
    sum += (command & MSB8);

    code |= ( (sum % 2) << 2);

    //4BIT PARITY
    sum = 0;

    sum += (header & MSB5) >> 3;
    sum += (header & MSB6) >> 2;
    sum += (header & MSB7) >> 1;
    sum += (header & MSB8);

    sum += (command & MSB1) >> 7;
    sum += (command & MSB2) >> 6;
    sum += (command & MSB3) >> 5;

    code |= ( (sum % 2) << 3);

    //5BIT PARITY
    sum = 0;

    sum += (command & MSB4) >> 4;
    sum += (command & MSB5) >> 3;
    sum += (command & MSB6) >> 2;
    sum += (command & MSB7) >> 1;
    sum += (command & MSB8);

    code |= ( (sum % 2) << 4);

    //Copy computed code in struct
    packet->crc = code;
}

/**
@author Gianluca Savaia
Return zero if packet contains errors
*/
char check_hamming(packet_t tobe_checked)
{
    packet_t temp = tobe_checked;

    compute_hamming(&temp);

    return tobe_checked.crc == temp.crc ? 1 : 0;
}

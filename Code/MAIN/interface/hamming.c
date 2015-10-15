/**
@author Gianluca Savaia
*/

#include "hamming.h"

/* Computes the hamming code for struct packet_s with 16bits data and 5bits parity (encapsulated in one byte) */
void compute_hamming(packet_t* packet)
{
    char code = 0;
    char sum = 0;
    char header = packet->header;
    char command = packet->command;

    //1BIT PARITY
    sum = 0;

    sum += (header & MSB1);
    sum += (header & MSB2) >> 1;
    sum += (header & MSB4) >> 3;
    sum += (header & MSB5) >> 4;
    sum += (header & MSB7) >> 6;

    sum += (command & MSB1);
    sum += (command & MSB3) >> 2;
    sum += (command & MSB4) >> 3;
    sum += (command & MSB6) >> 5;
    sum += (command & MSB8) >> 7;

    code |= (sum % 2);

    //2BIT PARITY
    sum = 0;

    sum += header & MSB1;
    sum += (header & MSB3) >> 2;
    sum += (header & MSB4) >> 3;
    sum += (header & MSB6) >> 5;
    sum += (header & MSB7) >> 6;

    sum += (command & MSB2) >> 1;
    sum += (command & MSB3) >> 2;
    sum += (command & MSB5) >> 4;
    sum += (command & MSB6) >> 5;

    code |= ( (sum % 2) << 1 );

    //3BIT PARITY
    sum = 0;

    sum += (header & MSB2) >> 1;
    sum += (header & MSB3) >> 2;
    sum += (header & MSB4) >> 3;
    sum += (header & MSB8) >> 7;

    sum += command & MSB1;
    sum += (command & MSB2) >> 1;
    sum += (command & MSB3) >> 2;
    sum += (command & MSB7) >> 6;
    sum += (command & MSB8) >> 7;

    code |= ( (sum % 2) << 2);

    //4BIT PARITY
    sum = 0;

    sum += (header & MSB5) >> 4;
    sum += (header & MSB6) >> 5;
    sum += (header & MSB7) >> 6;
    sum += (header & MSB8) >> 7;

    sum += (command & MSB1);
    sum += (command & MSB2) >> 1;
    sum += (command & MSB3) >> 2;

    code |= ( (sum % 2) << 3);

    //5BIT PARITY
    sum = 0;

    sum += (command & MSB4) >> 3;
    sum += (command & MSB5) >> 4;
    sum += (command & MSB6) >> 5;
    sum += (command & MSB7) >> 6;
    sum += (command & MSB8) >> 7;

    code |= ( (sum % 2) << 4);

    //Copy computed code in struct
    packet->crc = code;
}

/* Return zero if packet contains errors */
char check_hamming(packet_t tobe_checked)
{
    packet_t temp = tobe_checked;

    compute_hamming(&temp);

    return tobe_checked.crc == temp.crc ? 1 : 0;
}

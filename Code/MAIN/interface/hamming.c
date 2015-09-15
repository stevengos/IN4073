/**
@author Gianluca Savaia
@last update 2015-09-15
*/

#include "hamming.h"

/* Computes the hamming code for struct packet_s with 16bits data and 5bits parity (encapsulated in one byte) */
void compute_hamming(packet_t* packet)
{
    char code = 0;

    //1BIT PARITY
    char sum = 0;

    sum += (packet->header & MSB1) ? 1 : 0;
    sum += (packet->header & MSB2) ? 1 : 0;
    sum += (packet->header & MSB4) ? 1 : 0;
    sum += (packet->header & MSB5) ? 1 : 0;
    sum += (packet->header & MSB7) ? 1 : 0;

    sum += (packet->command & MSB1) ? 1 : 0;
    sum += (packet->command & MSB3) ? 1 : 0;
    sum += (packet->command & MSB4) ? 1 : 0;
    sum += (packet->command & MSB6) ? 1 : 0;
    sum += (packet->command & MSB8) ? 1 : 0;

    code |= (sum % 2);

    //2BIT PARITY
    sum = 0;

    sum += packet->header & MSB1 ? 1 : 0;
    sum += packet->header & MSB3 ? 1 : 0;
    sum += packet->header & MSB4 ? 1 : 0;
    sum += packet->header & MSB6 ? 1 : 0;
    sum += packet->header & MSB7 ? 1 : 0;

    sum += packet->command & MSB2 ? 1 : 0;
    sum += packet->command & MSB3 ? 1 : 0;
    sum += packet->command & MSB5 ? 1 : 0;
    sum += packet->command & MSB6 ? 1 : 0;

    code |= ( (sum % 2) << 1 );

    //3BIT PARITY
    sum = 0;

    sum += packet->header & MSB2 ? 1 : 0;
    sum += packet->header & MSB3 ? 1 : 0;
    sum += packet->header & MSB4 ? 1 : 0;
    sum += packet->header & MSB8 ? 1 : 0;

    sum += packet->command & MSB1 ? 1 : 0;
    sum += packet->command & MSB2 ? 1 : 0;
    sum += packet->command & MSB3 ? 1 : 0;
    sum += packet->command & MSB7 ? 1 : 0;
    sum += packet->command & MSB8 ? 1 : 0;

    code |= ( (sum % 2) << 2);

    //4BIT PARITY
    sum = 0;

    sum += packet->header & MSB5 ? 1 : 0;
    sum += packet->header & MSB6 ? 1 : 0;
    sum += packet->header & MSB7 ? 1 : 0;
    sum += packet->header & MSB8 ? 1 : 0;

    sum += packet->command & MSB1 ? 1 : 0;
    sum += packet->command & MSB2 ? 1 : 0;
    sum += packet->command & MSB3 ? 1 : 0;

    code |= ( (sum % 2) << 3);

    //5BIT PARITY
    sum = 0;

    sum += packet->command & MSB4 ? 1 : 0;
    sum += packet->command & MSB5 ? 1 : 0;
    sum += packet->command & MSB6 ? 1 : 0;
    sum += packet->command & MSB7 ? 1 : 0;
    sum += packet->command & MSB8 ? 1 : 0;

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

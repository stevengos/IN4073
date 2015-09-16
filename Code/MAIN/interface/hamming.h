/**
@author Gianluca Savaia
@last update 2015-09-15
*/

#ifndef HAMMING_H
#define HAMMING_H

#include "packet.h"

#define MSB1    0x80
#define MSB2    0x40
#define MSB3    0x20
#define MSB4    0x10
#define MSB5    0x08
#define MSB6    0x04
#define MSB7    0x02
#define MSB8    0x01

void compute_hamming(struct packet_s*);
char check_hamming(struct packet_s);

#endif // HAMMING_H

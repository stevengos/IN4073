/**
@author Gianluca Savaia
*/
#ifndef TRANSMITTER_H
#define TRANSMITETR_H

#include "x32.h"
#include "static.h"

#include "utility.h"
#include "../interface/hamming.h"
#include "../interface/packet.h"

void acknowledge(char);
void send_short(short);
void send_int(int);

#endif

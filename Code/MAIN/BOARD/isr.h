/**
@author Gianluca Savaia
*/

#ifndef ISR_H
#define ISR_H

#include "x32.h"
#include "static.h"
#include "command.h"

#include "drone.h"
#include "../interface/packet.h"
#include "../interface/hamming.h"

#define TIMEOUT_TIMER       250 //ms

void isr_buttons(void);
void isr_rs232_rx(void);
void isr_timer(void);
void isr_sensors(void);


#endif // ISR_H

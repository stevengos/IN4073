/**
@author Gianluca Savaia
*/

#ifndef ISR_H
#define ISR_H

#include "x32.h"
#include "static.h"
#include "command.h"

#define TIMEOUT_BUFFER_RX   10   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define SLEEP_BUFFER_RX     10

#define TIMEOUT_TIMER       100 //100ms

void isr_buttons(void);
void isr_rs232_rx(void);
void isr_timer(void);


#endif // ISR_H

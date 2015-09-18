/**
@author Gianluca Savaia
*/

#ifndef ISR_H
#define ISR_H

#include "x32.h"
#include "static.h"
#include "command.h"

#define TIMEOUT_BUFFER_RX   100
#define SLEEP_BUFFER_RX     10

#define TIMEOUT_TIMER       6000 //6sec

void isr_buttons(void);
void isr_rs232_rx(void);
void isr_timer(void);


#endif // ISR_H

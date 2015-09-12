/**
@author Gianluca Savaia
@version 1.0
@last update 2015-09-11
*/

#ifndef ISR_H
#define ISR_H

#include "x32.h"
#include "static.h"
#include "command.h"

void isr_buttons(void);
void isr_rs232_rx(void);



#endif // ISR_H

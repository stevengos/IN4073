/**
@author Gianluca Savaia
@last update 2015-09-15
*/

#ifndef STATIC_H
#define STATIC_H

#define X32_DISPLAY	    peripherals[PERIPHERAL_DISPLAY]
#define X32_LEDS	    peripherals[PERIPHERAL_LEDS]
#define X32_BUTTONS	    peripherals[PERIPHERAL_BUTTONS]
#define X32_CLOCK_MS    peripherals[PERIPHERAL_MS_CLOCK]
#define X32_CLOCK_US    peripherals[PERIPHERAL_US_CLOCK]
#define X32_TIMER_CYCLE peripherals[PERIPHERAL_TIMER1_PERIOD]

#define X32_QR_A1 		peripherals[PERIPHERAL_XUFO_A0]
#define X32_QR_A2 		peripherals[PERIPHERAL_XUFO_A1]
#define X32_QR_A3 		peripherals[PERIPHERAL_XUFO_A2]
#define X32_QR_A4 		peripherals[PERIPHERAL_XUFO_A3]

#define X32_RS232_DATA	    peripherals[PERIPHERAL_PRIMARY_DATA]
#define X32_RS232_STATUS	peripherals[PERIPHERAL_PRIMARY_STATUS]
#define X32_RS232_WRITE     (X32_RS232_STATUS & 0x01)
#define X32_RS232_READ      (X32_RS232_STATUS & 0x02)

#endif // STATIC_H

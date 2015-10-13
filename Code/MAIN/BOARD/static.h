/**
@author Gianluca Savaia
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

#define X32_QR_S1 		peripherals[PERIPHERAL_XUFO_S0]
#define X32_QR_S2 		peripherals[PERIPHERAL_XUFO_S1]
#define X32_QR_S3 		peripherals[PERIPHERAL_XUFO_S2]
#define X32_QR_S4 		peripherals[PERIPHERAL_XUFO_S3]
#define X32_QR_S5 		peripherals[PERIPHERAL_XUFO_S4]
#define X32_QR_S6 		peripherals[PERIPHERAL_XUFO_S5]
#define X32_QR_TIME 	peripherals[PERIPHERAL_XUFO_TIMESTAMP]

#define X32_RS232_DATA	    peripherals[PERIPHERAL_PRIMARY_DATA]
#define X32_RS232_STATUS	peripherals[PERIPHERAL_PRIMARY_STATUS]
#define X32_RS232_WRITE     (X32_RS232_STATUS & 0x01)
#define X32_RS232_READ      (X32_RS232_STATUS & 0x02)

#define TIMEOUT_BUFFER_TX   200  //counter
#define SLEEP_BUFFER_TX     200  //usec

#define TIMEOUT_BUFFER_RX   20  //counter
#define SLEEP_BUFFER_RX     20  //usec

#endif // STATIC_H

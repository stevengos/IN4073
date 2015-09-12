/**
@author Gianluca Savaia
@last update 2015-09-12
*/

#ifndef STATIC_H
#define STATIC_H

#define X32_display	peripherals[PERIPHERAL_DISPLAY]
#define X32_leds	peripherals[PERIPHERAL_LEDS]
#define X32_buttons	peripherals[PERIPHERAL_BUTTONS]
#define X32_clock	peripherals[PERIPHERAL_MS_CLOCK]

#define X32_QR_a1 		peripherals[PERIPHERAL_XUFO_A0]
#define X32_QR_a2 		peripherals[PERIPHERAL_XUFO_A1]
#define X32_QR_a3 		peripherals[PERIPHERAL_XUFO_A2]
#define X32_QR_a4 		peripherals[PERIPHERAL_XUFO_A3]

#define X32_rs232_data	peripherals[PERIPHERAL_PRIMARY_DATA]
#define X32_rs232_stat	peripherals[PERIPHERAL_PRIMARY_STATUS]
#define X32_rs232_ready (X32_rs232_stat & 0x02)

#endif // STATIC_H

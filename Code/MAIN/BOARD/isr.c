/**
@author Gianluca Savaia
This module contains all the interrupt service routines.
*/

#include "isr.h"
#include "mafilter.h"
#include "butterworth.h"

extern struct drone qr;

/**
@author Gianluca Savaia
In case of stall it is possible to push a button to exit the program on-board
and avoiding to make use of hard reset.
*/
void isr_buttons(void)
{
    char i;

    DISABLE_INTERRUPT(INTERRUPT_BUTTONS);

    for(i=0; i < 10; i++, catnap(500))

        X32_LEDS = ALL_ON, catnap(500), X32_LEDS = ALL_OFF;

    stop();
}

/**
@author Gianluca Savaia
Read sensors data into the main struct and calls the filtering function
*/
void isr_sensors(void)
{
    TURNON_LED(LED3);

    DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

    qr.sax = X32_QR_S1 - qr.off_ax;
    qr.say = X32_QR_S2 - qr.off_ay;
    qr.saz = X32_QR_S3 - qr.off_az;
    qr.sp = X32_QR_S4 - qr.off_p;
    qr.sq = X32_QR_S5 - qr.off_q;
    qr.sr = X32_QR_S6 - qr.off_r;

    //mafilter();
    butter_second();

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL);

    TURNOFF_LED(LED3);
}


/**
@author Gianluca Savaia
Receive messages from the PC Link and checks for CRC errors.
@Profiling: 240us to read 3 bytes, 430us for checksum, 50us to perform the command
*/
void isr_rs232_rx(void)
{
    packet_t incoming;
    char counter = 0;

    qr.link_down = 0;
    TURNON_LED(LED2);

    if( !X32_RS232_READ )
        return;

    incoming.header = X32_RS232_DATA; //read first byte (HEADER)

    while( !X32_RS232_READ )
    {
        if( counter++ > TIMEOUT_BUFFER_RX )
        {
            acknowledge(ACK_NEGATIVE);
            flush_buffer();
            return;
        }
        else
            ucatnap(SLEEP_BUFFER_RX);
    }

    incoming.command = X32_RS232_DATA; //read second byte (COMMAND)

    while( !X32_RS232_READ );
    {
        if( counter++ > TIMEOUT_BUFFER_RX )
        {
            acknowledge(ACK_NEGATIVE);
            flush_buffer();
            return;
        }
        else
            ucatnap(SLEEP_BUFFER_RX);
    }

    incoming.crc = X32_RS232_DATA; //read third byte (CRC)

    if( check_hamming(incoming) )
        perform_command(incoming.header, incoming.command);
    else
    {
        acknowledge(ACK_HAMMING);
        flush_buffer();
    }
}

/**
@author Gianluca Savaia
Timeout which allows to notice if the PC link is down.
This is done by checking if we have received a message whithin a certain time window.
*/
void isr_timer(void)
{
    if( qr.link_down || X32_QR_BATTERY < BATTERY_LOW ) // if the link is down or battery drained, then panic and exit
    {
        unsigned char i;

        TURNOFF_LED(LED2);

        panic_mode();

        qr.current_mode = SAFE_MODE;
        stop_motors();
        qr.exit = 1;
        qr.flag_mode = 1;

        #ifdef PERIPHERAL_DISPLAY
            X32_DISPLAY = 0xf1fa;
        #endif

        DISABLE_INTERRUPT(INTERRUPT_TIMER1);
    }

    qr.link_down = 1; // this variable is set to zero whenever I receive a new packet
    TURNOFF_LED(LED2);
}

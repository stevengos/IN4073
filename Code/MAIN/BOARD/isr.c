/**
@author Gianluca Savaia
*/

#include "isr.h"
#include "mafilter.h"

extern struct drone qr;

void isr_buttons(void)
{
    char i;

    DISABLE_INTERRUPT(INTERRUPT_BUTTONS);

    for(i=0; i < 10; i++, catnap(500))

        X32_LEDS = ALL_ON, catnap(500), X32_LEDS = ALL_OFF;

    stop();
}

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

    mafilter();
    //butter_second();

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL);

    TURNOFF_LED(LED3);
}

/**
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


void isr_timer(void)
{
    if( qr.link_down || 1000 < BATTERY_LOW )  //X32_QR_BATTERY FIX
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

    qr.link_down = 1;
    TURNOFF_LED(LED2);
}

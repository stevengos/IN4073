/**
@author Gianluca Savaia
*/

#include "log.h"

void add_log()
{
    struct log_s new_log;

    if( !qr.log )
        return;

    if( log_size >= LOG_BUFFER_SIZE )
    {
        X32_DISPLAY = 0xffff;
        qr.log = 0;
        return;
    }

    X32_DISPLAY = log_size;

    DISABLE_INTERRUPT(INTERRUPT_GLOBAL); //SAVE LOG ATOMICALLY

    #ifdef PERIPHERAL_XUFO_TIMESTAMP
    new_log.timestamp = X32_QR_TIME;
    #else
    new_log.timestamp = X32_CLOCK_MS;
    #endif

    new_log.ae1 = qr.ae1;
    new_log.ae2 = qr.ae2;
    new_log.ae3 = qr.ae3;
    new_log.ae4 = qr.ae4;

    new_log.sax = qr.sax;
    new_log.say = qr.say;
    new_log.saz = qr.saz;

    new_log.sp = qr.sp;
    new_log.sq = qr.sq;
    new_log.sr = qr.sr;

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL); //END LOG

    log_buffer[log_size] = new_log; // write log into buffer

    log_size++;
}


void upload_log()
{
    int counter_timeout = 0;
    int i = 0;

    if( qr.current_mode != SAFE_MODE )
    {
        acknowledge(ACK_INVALID);
        return;
    }

    /***** Sending log size **********/
    counter_timeout = 0;

    while( !X32_RS232_WRITE )
    {
        if( counter_timeout++ > TIMEOUT_BUFFER_TX )
        {
            acknowledge(ACK_NEGATIVE);
            return;
        }
        else
            ucatnap(SLEEP_BUFFER_TX);
    }

    if( log_size < 0 )
            log_size = 0;

    send_int( log_size - 1);

    while( i < log_size - 1 )
    {
        struct log_s outgoing = log_buffer[i];
        short* ptr_head = &outgoing.start; //************* FIRST STRUCT ELEMENT
        short* ptr_tail = &outgoing.end; //************* LAST STRUCT ELEMENT

        i++;

        #ifdef PERIPHERAL_DISPLAY
        X32_DISPLAY = i;
        #endif

        /***** Sending timestamp **********/
        counter_timeout = 0;

        while( !X32_RS232_WRITE )
        {
            if( counter_timeout++ > TIMEOUT_BUFFER_TX )
            {
                acknowledge(ACK_NEGATIVE);
                return;
            }
            else
                ucatnap(SLEEP_BUFFER_TX);
        }

        send_int( outgoing.timestamp );

        /************** Sending log data ***********/
        ptr_head++; //goto first datum

        while( ptr_head != ptr_tail )
        {
            counter_timeout = 0;

            while( !X32_RS232_WRITE )
            {
                if( counter_timeout++ > TIMEOUT_BUFFER_TX )
                {
                    acknowledge(ACK_NEGATIVE);
                    return;
                }
                else
                    ucatnap(SLEEP_BUFFER_TX);
            }

            send_short( *ptr_head );

            ptr_head++;
        }

        send_short(LOG_NEWLINE);
    }
}


void reset_log(void)
{
    log_size = 0;
}

char init_log(void)
{
    int decrease;

    free(log_buffer);
    log_buffer = 0;
    log_size = 0;

    for(decrease=0; !log_buffer && decrease < LOG_BUFFER_SIZE; decrease+=1)

        log_buffer = (struct log_s*)malloc( LOG_SIZE * (LOG_BUFFER_SIZE - decrease) );

    return log_buffer ? 1 : 0;
}

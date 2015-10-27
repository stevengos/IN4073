/**
@author Gianluca Savaia
This module gathers all functions involved in the data logging.
*/

#include "log.h"

struct log_s* log_buffer;
int log_size;
int log_buffer_size;

malloc_memory_size = LOG_BUFFER_SIZE_KB;
malloc_memory[LOG_BUFFER_SIZE_KB];

/**
@author Gianluca Savaia
Add a new log to the buffer.
*/
void add_log()
{
    struct log_s new_log;

    if( !qr.log )
    {
        TURNOFF_LED(LED4);
        return;
    }

    if( log_size >= log_buffer_size )
    {
        qr.log = 0;
        qr.log_full = 1;
        TURNOFF_LED(LED4);

        return;
    }

    TURNON_LED(LED4);

    #ifdef PERIPHERAL_DISPLAY
    X32_DISPLAY = log_size;
    #endif

    DISABLE_INTERRUPT(INTERRUPT_GLOBAL); //SAVE LOG ATOMICALLY

    #ifdef PERIPHERAL_XUFO_TIMESTAMP
    new_log.timestamp = X32_QR_TIME;
    #else
    new_log.timestamp = X32_CLOCK_MS;
    #endif

    new_log.battery = X32_QR_BATTERY;

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

    new_log.fax = qr.fax;
    new_log.fay = qr.fay;
    new_log.faz = qr.faz;

    new_log.fp = qr.fp;
    new_log.fq = qr.fq;
    new_log.fr = qr.fr;

    log_buffer[log_size] = new_log; // write log into buffer

    if( !check_sanity(&new_log, &log_buffer[log_size]) )
    {
        qr.log = 0;
        qr.log_full = 1;
        TURNOFF_LED(LED4);

        return;
    }

    log_size++;

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL); //END LOG
}

/**
@author Gianluca Savaia
Send all logs to the PC terminal.
I implemented a way of unrolling the log which is independent of the log size which makes use of pointers.
*/
void upload_log()
{
    int counter_timeout = 0;
    int i = 0;

    if( qr.current_mode != SAFE_MODE ) //upload is possible only in SAFE MODE
    {
        acknowledge(ACK_INVALID);
        return;
    }

    if( !qr.log_full ) //if buffer is empty do not go on
    {
        acknowledge(ACK_INVALID);
        return;
    }

    acknowledge(ACK_POSITIVE);

    DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

    ucatnap(500);

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

    send_int( log_size );

    #ifdef PERIPHERAL_DISPLAY
    X32_DISPLAY = log_size;
    #endif

    while( i < log_size-1 )
    {
        struct log_s outgoing = log_buffer[i];
        short* ptr_head = &outgoing.start; //************* FIRST STRUCT ELEMENT
        short* ptr_tail = &outgoing.end;   //************* LAST  STRUCT ELEMENT

        i++;

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

            #ifdef PERIPHERAL_DISPLAY
            X32_DISPLAY = i;
            #endif
        }

        send_short(LOG_NEWLINE);
    }

    //synchro();
    ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

    catnap(1000);

    ENABLE_INTERRUPT(INTERRUPT_GLOBAL);
}

/**
@author Gianluca Savaia
Init log and allocate buffer in RAM.
*/
char init_log(void)
{
    int decrease_kb;

    if( qr.log_full )
    {
        log_size = 0;
        qr.log_full = 0;

        return 1;
    }

    log_buffer = 0;
    log_size = 0;

    for(decrease_kb=0; !log_buffer && decrease_kb < LOG_BUFFER_SIZE_KB; decrease_kb+=100)

        log_buffer = (struct log_s*)malloc( LOG_BUFFER_SIZE_KB - decrease_kb );


    log_buffer_size = ( LOG_BUFFER_SIZE_KB - decrease_kb )/LOG_SIZE;

    return log_buffer ? 1 : 0;
}

/**
@author Gianluca Savaia
Check if the log has been saved correctly into RAM
*/
char check_sanity(struct log_s* log1, struct log_s* log2)
{
    short* ptr_1   = &(log1->start);
    short* ptr_2   = &(log2->start);
    short* ptr_end = &(log1->end);

    if( log1->timestamp != log2->timestamp )
        return 0;

    ptr_1++;
    ptr_2++;

    while( ptr_1 != ptr_end )
        if( *ptr_1 != *ptr_2 )
            return 0;
        else
            ptr_1++, ptr_2++;

    return 1;
}

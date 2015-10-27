/**
@author Gianluca Savaia
This is the MOVING AVERAGE FILTER designed at first when the more complex
Butterworth was not ready
*/

#include "mafilter.h"

#include "drone.h"

extern struct drone qr;

/* FIFO QUEUE: new data comes from head of buffer (index=0) and exits from tail (index=MEMORY-1) */
int ax_buffer[MEMORY];
int ay_buffer[MEMORY];
int az_buffer[MEMORY];

int p_buffer[MEMORY];
int q_buffer[MEMORY];
int r_buffer[MEMORY];

/**
@author Gianluca Savaia
Zeroing the buffers.
*/
void initfilter()
{
    int i=0;

    for(i=0; i<MEMORY; i++)
        ax_buffer[i] = 0;
    for(i=0; i<MEMORY; i++)
        ay_buffer[i] = 0;
    for(i=0; i<MEMORY; i++)
        az_buffer[i] = 0;

    for(i=0; i<MEMORY; i++)
        p_buffer[i] = 0;
    for(i=0; i<MEMORY; i++)
        q_buffer[i] = 0;
    for(i=0; i<MEMORY; i++)
        r_buffer[i] = 0;
}

/**
@author Gianluca Savaia
This method is called whenever a new set of data from the sensors is ready.
This newcome set is processed and filtered.
*/
void mafilter()
{
    int i = 0;
    int sum = 0;

    /********************* GET NEWCOME DATA ********************************/
    for(i=0; i<MEMORY-1; i++)
        ax_buffer[i+1] = ax_buffer[i];
    for(i=0; i<MEMORY-1; i++)
        ay_buffer[i+1] = ay_buffer[i];
    for(i=0; i<MEMORY-1; i++)
        az_buffer[i+1] = az_buffer[i];

    ax_buffer[0] = qr.sax;
    ay_buffer[0] = qr.say;
    az_buffer[0] = qr.saz;


    for(i=0; i<MEMORY-1; i++)
        p_buffer[i+1] = p_buffer[i];
    for(i=0; i<MEMORY-1; i++)
        q_buffer[i+1] = q_buffer[i];
    for(i=0; i<MEMORY-1; i++)
        r_buffer[i+1] = r_buffer[i];

    p_buffer[0] = qr.sp;
    q_buffer[0] = qr.sq;
    r_buffer[0] = qr.sr;

    /********************* COMPUTE FILTERED DATA *********************************/

    for(i=0, sum=0; i<MEMORY; i++)
        sum += ax_buffer[i];
    sum = sum/MEMORY;
    qr.fax = (short)sum;

    for(i=0, sum=0; i<MEMORY; i++)
        sum += ay_buffer[i];
    sum = sum/MEMORY;
    qr.fay = (short)sum;

    for(i=0, sum=0; i<MEMORY; i++)
        sum += az_buffer[i];
    sum = sum/MEMORY;
    qr.faz = (short)sum;


    for(i=0, sum=0; i<MEMORY; i++)
        sum += p_buffer[i];
    sum = sum/MEMORY;
    qr.fp = (short)sum;

    for(i=0, sum=0; i<MEMORY; i++)
        sum += q_buffer[i];
    sum = sum/MEMORY;
    qr.fq = (short)sum;

    for(i=0, sum=0; i<MEMORY; i++)
        sum += r_buffer[i];
    sum = sum/MEMORY;
    qr.fr = (short)sum;
}

#include "mode.h"
#include "x32.h"
#include "static.h"
#include "drone.h"

extern struct drone qr;

void manual_mode()
{
    while(!qr.flag_mode)
    {
        //read data from struct and apply to motors
    }
}

void yaw_mode()
{
    char eps; //error signal
    char regulator = 49;

    while(!qr.flag_mode)
    {
        //atomic
        eps = qr.yawrate - qr.syawrate;

        /*
            ae1 = regulator * eps;
            ...
        */
    }
}

/**
@author Gianluca Savaia
*/

#ifndef LOG_H
#define LOG_H

#define LOG_SIZE sizeof(struct log_s)
#define LOG_END 0x7fff

struct log_s
{
    int id;
    int timestamp;

    short ae1;
    short ae2;
    short ae3;
    short ae4;

    short sax;
    short say;
    short saz;

    short sp;
    short sq;
    short sr;
};

#endif // LOG_H

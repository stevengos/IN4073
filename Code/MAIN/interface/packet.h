/**
@author Gianluca Savaia
@last update 2015-09-15
*/

/* In the following we list the admissable values for the fields of the struct packet_s */

#ifndef PACKET_H
#define PACKET_H

/* ########### HEADER ############ */
#define SET_MODE        0x0

#define SET_PITCH       0x1
#define SET_ROLL        0x2
#define SET_YAW         0x3
#define SET_LIFT        0x4
#define SET_YAWRATE     0x5

#define INC_PITCH       0x6
#define INC_ROLL        0x7
#define INC_YAW         0x8
#define INC_LIFT        0x9
#define INC_YAWRATE     0xA

//...
#define ALIVE           0x10
#define ACK             0x11
#define BLINK_LED       0x12
#define SET_LED         0x13


/* ############ COMMAND ############# */

/* Eligible values when the header is "SET_MODE" */
#define SAFE_MODE   0x0
#define PANIC_MODE  0x1
#define MANUAL_MODE 0x2
#define CALIBRATION_MODE 0x3
#define YAW_MODE    0x4
#define FULL_MODE   0x5

/* Eligible values files when the header is "ACK" */
#define ACK_POSITIVE 0x0
#define ACK_NEGATIVE 0xF

/* Eligible values field when the header is "SET_LED" (use bitwise to operate on more than one led at once */
#define ALL_OFF 0x00
#define ALL_ON  0xFF
#define LED1 0x01 //0b00000001
#define LED2 0x02 //0b00000010
#define LED3 0x04 //0b00000100
#define LED4 0x08 //0b00001000
#define LED5 0x10 //0b00010000
#define LED6 0x20 //0b00100000
#define LED7 0x40 //0b01000000
#define LED8 0x80 //0b10000000

struct packet_s
{
    char header;
    char command;
    char crc;
};

typedef struct packet_s packet_t;

#endif


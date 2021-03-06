/* In the following we list the admissable values for the fields of the struct packet_s */

/* Eligible values for "label" field */
#define SET_MODE 0x0
#define SET_PITCH 0x1
#define SET_ROLL 0x2
#define SET_YAWRATE 0x3
#define SET_LIFT 0x4

//...
#define SET_LED 0x80

/* Eligible values for "command" field when the label is "SET_MODE" */
#define SAFE_MODE 0x0
#define PANIC_MODE 0x1
#define MANUAL_MODE 0x2
#define CALIBRATION_MODE 0x3
#define YAW_MODE 0x4
#define FULL_MODE 0x5

/* the rates */

#define YAW_RATE 0x6
#define LIFT_RATE 0X7
#define ROLL_RATE 0x8
#define PITCH_RATE 0x9 

/* Eligible values for "command" field when the label is "SET_LED". Use OR bitwise to operate on more than one led at once */
#define LED_OFF 0x0
#define LED_ON  0xF
#define LED_BLINK 0x0E 
#define LED1 0b00000001
#define LED2 0b00000010
#define LED3 0b00000100
#define LED4 0b00001000
#define LED5 0b00010000
#define LED6 0b00100000
#define LED7 0b01000000
#define LED8 0b10000000

struct packet_s
{
    char label;
    char command;
};

typedef struct packet_s packet;


/**
@author Gianluca Savaia
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H


#include <termios.h>
#include <stdio.h>
#include "../interface/packet.h"

#define INVALID     126

#define ESC 27

#define ZERO        48
#define ONE         49
#define TWO         50
#define THREE       51
#define FOUR        52
#define FIVE        53
#define SIX         54
#define SEVEN       55
#define EIGHT       56
#define NINE        57

/*lowercase*/
#define A           97
#define Z           122
#define Q           113
#define W           119
#define Y           121
#define H           104
#define U           117
#define J           106
#define I           105
#define K           107
#define O           111
#define L           108

#define ARROW_U     65
#define ARROW_D     66
#define ARROW_R     67
#define ARROW_L     68

inline void open_keyboard(struct termios*, struct termios*);
inline void close_keyboard(struct termios*);

packet_t encapsulate(char);

#endif // KEYBOARD_H

/**
@author Gianluca Savaia
*/

#include "keyboard.h"
#include "../interface/hamming.h"

inline void open_keyboard(struct termios* oldTerminalSettings, struct termios* newTerminalSettings)
{
    tcgetattr(0, oldTerminalSettings);

    *newTerminalSettings = *oldTerminalSettings;

    newTerminalSettings->c_lflag &= (~ICANON & ~ECHO); //dont wait for enter, dont print to terminal

    tcsetattr(0, TCSANOW, newTerminalSettings);
}

inline void close_keyboard(struct termios* oldTerminalSettings)
{
    tcsetattr(0, TCSANOW, oldTerminalSettings);
}

char getchar_keyboard()
{
    char ch = getchar();

    printf("%d ", ch);

    if( ch == ESC )
    {
        ch = getchar();
        printf("%d ", ch);

        if( ch == ESC )
        {
            printf("\n");
            return ESC;
        }
        if( ch != 91 )
        {
            printf("\n");
            return INVALID;
        }

        ch = getchar();
        printf("\n");

        switch(ch)
        {
            case ARROW_U:
                return ARROW_U;
            case ARROW_D:
                return ARROW_D;
            case ARROW_L:
                return ARROW_L;
            case ARROW_R:
                return ARROW_R;
            default:
                return INVALID;
        };
    }

    printf("\n");

    return ch;
}

packet_t encapsulate(char command)
{
    packet_t outgoing;

    switch(command)
    {
        case ESC:
            outgoing.header     = STOP;
            outgoing.command    = EMPTY;
            break;

        /* MODES */
        case ZERO:
            outgoing.header     = SET_MODE;
            outgoing.command    = SAFE_MODE;
            break;
        case ONE:
            outgoing.header     = SET_MODE;
            outgoing.command    = PANIC_MODE;
            break;
        case TWO:
            outgoing.header     = SET_MODE;
            outgoing.command    = MANUAL_MODE;
            break;
        case THREE:
            outgoing.header     = SET_MODE;
            outgoing.command    = CALIBRATION_MODE;
            break;
        case FOUR:
            outgoing.header     = SET_MODE;
            outgoing.command    = YAW_MODE;
            break;
        case FIVE:
            outgoing.header     = SET_MODE;
            outgoing.command    = FULL_MODE;
            break;

        /* MANUAL CONTROL */
        case A:
            outgoing.header     = D_LIFT;
            outgoing.command    = INCREASE;
            break;
        case Z:
            outgoing.header     = D_LIFT;
            outgoing.command    = DECREASE;
            break;
        case Q:
            outgoing.header     = D_YAWRATE;
            outgoing.command    = DECREASE;
            break;
        case W:
            outgoing.header     = D_YAWRATE;
            outgoing.command    = INCREASE;
            break;
        case ARROW_U:
            outgoing.header     = D_PITCH;
            outgoing.command    = DECREASE;
            break;
        case ARROW_D:
            outgoing.header     = D_PITCH;
            outgoing.command    = INCREASE;
            break;
        case ARROW_R:
            outgoing.header     = D_ROLL;
            outgoing.command    = DECREASE;
            break;
        case ARROW_L:
            outgoing.header     = D_ROLL;
            outgoing.command    = INCREASE;
            break;

        /* CONTROLLER SETTINGS */
        case U:
            outgoing.header     = EMPTY;
            outgoing.command    = DECREASE;
            break;
        case J:
            outgoing.header     = EMPTY;
            outgoing.command    = EMPTY;
            break;
        case K:
            outgoing.header     = EMPTY;
            outgoing.command    = EMPTY;
            break;
        case O:
            outgoing.header     = EMPTY;
            outgoing.command    = EMPTY;
            break;
        case L:
            outgoing.header     = EMPTY;
            outgoing.command    = EMPTY;
            break;

        /* NOT RECOGNIZED */
        default:
            outgoing.header     = EMPTY;
            outgoing.command    = EMPTY;
            break;
    };

    compute_hamming(&outgoing);

    return outgoing;
}
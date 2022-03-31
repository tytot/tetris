#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

struct position {
    int x;
    int y;
}

struct tetrimino {
    position pos;
    int size;
    position states[4][4];
    int color;
}

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

#endif

#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

typedef struct {
    int y;
    int x;
} position;

typedef struct {
    position pos;
    int size;
    const position (*states)[4][4];
    int state;
    u16 color;
} tetrimino;

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

#include "main.h"

#ifndef BOARD_H
#define BOARD_H

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define BORDER_WIDTH 2

#define BLOCK_SIZE 6

#define TETRIMINO_I 0
#define TETRIMINO_J 1
#define TETRIMINO_L 2
#define TETRIMINO_O 3
#define TETRIMINO_S 4
#define TETRIMINO_T 5
#define TETRIMINO_Z 6

#define STAR_THRESHOLD 100
#define MAX_STARS 5

typedef enum {
    LEFT,
    RIGHT,
    DOWN,
    COUNTERCLOCKWISE,
    CLOCKWISE
} direction;

void spawnTetrimino(void);
void dropTetrimino(void);
void moveTetrimino(direction direction);
void undrawTetrimino(void);
void drawTetrimino(void);

void drawBoard(void);
void resetBoard(void);

extern int gameOver;

#endif
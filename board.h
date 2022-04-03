#include "main.h"

#ifndef TETRIMINOS_H
#define TETRIMINOS_H

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

tetrimino newTetrimino(int type, position pos);
void clearTetrimino(tetrimino tetrimino);
void drawTetrimino(tetrimino tetrimino);

void drawBoard(void);

#endif
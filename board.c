#include "board.h"
#include "main.h"

const int boardWidth = BLOCK_SIZE * BOARD_WIDTH;
const int boardCol = (WIDTH - (BLOCK_SIZE * BOARD_WIDTH)) / 2;
const int boardHeight = BLOCK_SIZE * BOARD_HEIGHT;
const int boardRow = (HEIGHT - (BLOCK_SIZE * BOARD_HEIGHT)) / 2;

int board[BOARD_HEIGHT][BOARD_WIDTH];
tetrimino activeTetrimino;

const int tetriminoSizes[7] = { 4, 3, 3, 2, 3, 3, 3 };
const position tetriminoStates[7][4][4] = { 
    {
        { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 } },
        { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 } },
        { { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } },
    },
    {
        { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 0, 2 } },
        { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
        { { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
    },
    {
        { { 1, 0 }, { 1, 1 }, { 0, 2 }, { 1, 2 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
        { { 1, 0 }, { 2, 0 }, { 1, 1 }, { 1, 2 } },
        { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
    },
    {
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
    },
    {
        { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 } },
        { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
        { { 2, 0 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
        { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
    },
    {
        { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
        { { 1, 0 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
        { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
    },
    {
        { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
        { { 1, 1 }, { 2, 1 }, { 0, 2 }, { 1, 2 } },
        { { 1, 0 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
        { { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
    }
};
const u16 tetriminoColors[7] = {
    CYAN, BLUE, ORANGE, YELLOW, GREEN, MAGENTA, RED
};

tetrimino newTetrimino(int type) {
    tetrimino tetrimino;
    tetrimino.size = tetriminoSizes[type];
    position pos;
    pos.x = (BOARD_WIDTH - tetrimino.size) / 2;
    pos.y = 0;
    tetrimino.pos = pos;
    tetrimino.states = &tetriminoStates[type];
    tetrimino.state = 0;
    tetrimino.color = tetriminoColors[type];
    return tetrimino;
}

void boundTetrimino(void) {
    int tetriminoStartX = (*activeTetrimino.states)[activeTetrimino.state][0].x;
    int tetriminoEndX = (*activeTetrimino.states)[activeTetrimino.state][3].x;
    int leftOverflow = -activeTetrimino.pos.x - tetriminoStartX;
    int rightOverflow = activeTetrimino.pos.x + tetriminoEndX + 1 - BOARD_WIDTH;
    if (leftOverflow > 0) {
        activeTetrimino.pos.x += leftOverflow;
    } else if (rightOverflow > 0) {
        activeTetrimino.pos.x -= rightOverflow;
    }
}

int isTetriminoSet(void) {
    for (int i = 0; i < 4; i++) {
        position pos = (*activeTetrimino.states)[activeTetrimino.state][i];
        if (pos.y + 1 == BOARD_HEIGHT || board[pos.y + 1][pos.x]) {
            return 1;
        }
    }
    return 0;
}

void moveTetrimino(direction direction) {
    if (direction == LEFT) {
        activeTetrimino.pos.x--;
    } else if (direction == RIGHT) {
        activeTetrimino.pos.x++;
    } else if (direction == DOWN) {
        activeTetrimino.pos.y++;
    } else if (direction == COUNTERCLOCKWISE) {
        activeTetrimino.state--;
        if (activeTetrimino.state == -1) {
            activeTetrimino.state = 3;
        }
    } else if (direction == CLOCKWISE) {
        activeTetrimino.state++;
        if (activeTetrimino.state == 4) {
            activeTetrimino.state = 0;
        }
    }
    boundTetrimino();
}

void undrawTetrimino(void) {
    int tetriminoX = (*activeTetrimino.states)[activeTetrimino.state][0].x;
    int tetriminoWidth = ((*activeTetrimino.states)[activeTetrimino.state][3].x - tetriminoX) + 1;
    drawRectDMA(BLOCK_SIZE * activeTetrimino.pos.y + boardRow, BLOCK_SIZE * (activeTetrimino.pos.x + tetriminoX) + boardCol, BLOCK_SIZE * tetriminoWidth, BLOCK_SIZE * activeTetrimino.size, BLACK);
}

void drawTetrimino(void) {
    for (int i = 0; i < 4; i++) {
        position pos = (*activeTetrimino.states)[activeTetrimino.state][i];
        drawRectDMA(BLOCK_SIZE * (activeTetrimino.pos.y + pos.y) + boardRow, BLOCK_SIZE * (activeTetrimino.pos.x + pos.x) + boardCol, BLOCK_SIZE, BLOCK_SIZE, activeTetrimino.color);
    }
}

void drawBoard(void) {
    drawRectDMA(boardRow - BORDER_WIDTH, boardCol - BORDER_WIDTH, boardWidth + 2 * BORDER_WIDTH, boardHeight + 2 * BORDER_WIDTH, WHITE);
    drawRectDMA(boardRow, boardCol, boardWidth, boardHeight, BLACK);
}
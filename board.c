#include "board.h"
#include "main.h"

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
        { { 0, 1 }, { 0, 2 }, { 1, 1 }, { 2, 1 } },
        { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
        { { 0, 1 }, { 1, 1 }, { 2, 0 }, { 2, 1 } },
    },
    {
        { { 0, 2 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
        { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 0 } },
        { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
    },
    {
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
    },
    {
        { { 0, 1 }, { 0, 2 }, { 1, 0 }, { 1, 1 } },
        { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
        { { 1, 1 }, { 1, 2 }, { 2, 0 }, { 2, 1 } },
        { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
    },
    {
        { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 2, 1 } },
        { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 1 } },
        { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
    },
    {
        { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
        { { 0, 2 }, { 1, 1 }, { 1, 2 }, { 2, 1 } },
        { { 1, 0 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
        { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 2, 0 } },
    }
};
const u16 tetriminoColors[7] = {
    CYAN, BLUE, ORANGE, YELLOW, GREEN, MAGENTA, RED
};

tetrimino newTetrimino(int type, position pos) {
    tetrimino tetrimino;
    tetrimino.pos = pos;
    tetrimino.size = tetriminoSizes[type];
    tetrimino.states = &tetriminoStates[type];
    tetrimino.state = 0;
    tetrimino.color = tetriminoColors[type];
    return tetrimino;
}

void clearTetrimino(tetrimino tetrimino) {
    drawRectDMA(BLOCK_SIZE * tetrimino.pos.y, BLOCK_SIZE * tetrimino.pos.x, BLOCK_SIZE * tetrimino.size, BLOCK_SIZE * tetrimino.size, BLACK);
}

void drawTetrimino(tetrimino tetrimino) {
    for (int i = 0; i < 4; i++) {
        position pos = (*tetrimino.states)[tetrimino.state][i];
        drawRectDMA(BLOCK_SIZE * (tetrimino.pos.y + pos.y), BLOCK_SIZE * (tetrimino.pos.x + pos.x), BLOCK_SIZE, BLOCK_SIZE, tetrimino.color);
    }
}

void drawBoard(void) {
    int boardWidth = BLOCK_SIZE * BOARD_WIDTH;
    int boardCol = (WIDTH - boardWidth) / 2;
    int boardHeight = BLOCK_SIZE * BOARD_HEIGHT;
    int boardRow = (HEIGHT - boardHeight) / 2;
    drawRectDMA(boardRow - BORDER_WIDTH, boardCol - BORDER_WIDTH, boardWidth + 2 * BORDER_WIDTH, boardHeight + 2 * BORDER_WIDTH, WHITE);
    drawRectDMA(boardRow, boardCol, boardWidth, boardHeight, BLACK);
}
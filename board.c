#include <stdio.h>

#include "board.h"
#include "main.h"
#include "images/star.h"

const int boardWidth = BLOCK_SIZE * BOARD_WIDTH;
const int boardCol = (WIDTH - (BLOCK_SIZE * BOARD_WIDTH)) / 2;
const int boardHeight = BLOCK_SIZE * BOARD_HEIGHT;
const int boardRow = (HEIGHT - (BLOCK_SIZE * BOARD_HEIGHT)) / 2;

int board[BOARD_HEIGHT][BOARD_WIDTH];
int gameOver = 0;
int score = 0;
int numStars = 0;

tetrimino activeTetrimino;
tetrimino ghostTetrimino;

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
const int tetriminoHeights[7][4] = { 
    { 2, 4, 3, 4 },
    { 2, 3, 3, 3 },
    { 2, 3, 3, 3 },
    { 2, 2, 2, 2 },
    { 2, 3, 3, 3 },
    { 2, 3, 3, 3 },
    { 2, 3, 3, 3 }
};
const u16 tetriminoColors[7] = {
    CYAN, BLUE, ORANGE, YELLOW, GREEN, MAGENTA, RED
};

void drawScore(void) {
    char scoreBuffer[8];
    snprintf(scoreBuffer, 9, "%08d", score);
    drawRectDMA(boardRow + 10, boardWidth + boardCol + 10, 50, 10, BLACK);
    drawString(boardRow + 10, boardWidth + boardCol + 10, scoreBuffer, WHITE);
}

void drawStar(void) {
    drawImageDMA(boardRow + 20, boardWidth + boardCol + 10 + (10 * numStars), 11, 11, star);
    numStars++;
}

void drawBoard(void) {
    fillScreenDMA(BLACK);
    drawRectDMA(boardRow - BORDER_WIDTH, boardCol - BORDER_WIDTH, boardWidth + 2 * BORDER_WIDTH, boardHeight + 2 * BORDER_WIDTH, WHITE);
    drawRectDMA(boardRow, boardCol, boardWidth, boardHeight, BLACK);
    char scoreLabel[] = "Score:";
    drawString(boardRow, boardWidth + boardCol + 10, scoreLabel, WHITE);
    drawScore();
}

void resetBoard(void) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            board[y][x] = 0;
        }
    }
    gameOver = 0;
    score = 0;
    numStars = 0;
    drawBoard();
    spawnTetrimino();
}

void addPoints(int points) {
    score += points;
    drawScore();
    int newNumStars = score / STAR_THRESHOLD;
    if (newNumStars > MAX_STARS) {
        newNumStars = MAX_STARS;
    }
    if (newNumStars > numStars) {
        for (int i = numStars; i < newNumStars; i++) {
            drawStar();
        }
    }
}

void clearLines(void) {
    int linesCleared = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        int lineClear = 1;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (linesCleared) {
                board[y + linesCleared][x] = board[y][x];
                drawRectDMA(BLOCK_SIZE * (y + linesCleared) + boardRow, BLOCK_SIZE * x + boardCol, BLOCK_SIZE, BLOCK_SIZE, board[y][x]);
            }
            if (!board[y][x]) {
                lineClear = 0;
            }
        }
        if (lineClear) {
            linesCleared++;
        }
    }
    if (linesCleared) {
        for (int y = 0; y < linesCleared; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board[y][x] = 0;
            }
        }
        if (linesCleared == 1) {
            addPoints(40);
        } else if (linesCleared == 2) {
            addPoints(100);
        } else if (linesCleared == 3) {
            addPoints(300);
        } else {
            addPoints(1200);
        }
    }
}

tetrimino newTetrimino(int type) {
    tetrimino tetrimino;
    tetrimino.size = tetriminoSizes[type];
    position pos;
    pos.x = (BOARD_WIDTH - tetrimino.size) / 2;
    pos.y = 0;
    tetrimino.pos = pos;
    tetrimino.states = &tetriminoStates[type];
    tetrimino.heights = &tetriminoHeights[type];
    tetrimino.state = 0;
    tetrimino.color = tetriminoColors[type];
    return tetrimino;
}

void spawnTetrimino(void) {
    int type = randint(0, 6);
    activeTetrimino = newTetrimino(type);
}

int isTetriminoSet(void) {
    if (activeTetrimino.pos.y + (*activeTetrimino.heights)[activeTetrimino.state] >= BOARD_HEIGHT) {
        return 1;
    }
    for (int i = 0; i < 4; i++) {
        position pos = (*activeTetrimino.states)[activeTetrimino.state][i];
        if (board[activeTetrimino.pos.y + pos.y + 1][activeTetrimino.pos.x + pos.x]) {
            return 1;
        }
    }
    return 0;
}

int isTetriminoColliding(tetrimino tetrimino) {
    if (tetrimino.pos.y + (*tetrimino.heights)[tetrimino.state] > BOARD_HEIGHT) {
        return 1;
    }
    if (tetrimino.pos.x + (*tetrimino.states)[tetrimino.state][0].x < 0 || tetrimino.pos.x + (*tetrimino.states)[tetrimino.state][3].x >= BOARD_WIDTH) {
        return 1;
    }    
    for (int i = 0; i < 4; i++) {
        position pos = (*tetrimino.states)[tetrimino.state][i];
        if (board[tetrimino.pos.y + pos.y][tetrimino.pos.x + pos.x]) {
            return 1;
        }
    }
    return 0;
}

int testXOffsets[5] = { 0, -1, 1, -2, 2 };
void rotateTetrimino(int state) {
    tetrimino testTetrimino = activeTetrimino;
    testTetrimino.state = state;
    int initialX = testTetrimino.pos.x;
    for (int i = 0; i < 5; i++) {
        testTetrimino.pos.x = initialX + testXOffsets[i];
        if (!isTetriminoColliding(testTetrimino)) {
            activeTetrimino = testTetrimino;
            break;
        }
    }
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

int L(void) {
    for (int i = 0; i < 4; i++) {
        position pos = (*activeTetrimino.states)[activeTetrimino.state][i];
        if (activeTetrimino.pos.y + pos.y < 0) {
            return 1;
        }
    }
    return 0;
}

void placeTetrimino(void) {
    drawTetrimino();
    for (int i = 0; i < 4; i++) {
        position pos = (*activeTetrimino.states)[activeTetrimino.state][i];
        board[activeTetrimino.pos.y + pos.y][activeTetrimino.pos.x + pos.x] = activeTetrimino.color;
    }
    clearLines();
    if (L()) {
        gameOver = 1;
    } else {
        spawnTetrimino();
        if (L() || isTetriminoColliding(activeTetrimino)) {
            gameOver = 1;
        }
    }
}

void dropTetrimino(void) {
    activeTetrimino = ghostTetrimino;
    placeTetrimino();
}

void moveTetrimino(direction direction) {
    if (direction == DOWN) {
        if (isTetriminoSet()) {
            placeTetrimino();
        } else {
            activeTetrimino.pos.y++;
        }
    } else {
        if (direction == LEFT) {
            activeTetrimino.pos.x--;
            boundTetrimino();
        } else if (direction == RIGHT) {
            activeTetrimino.pos.x++;
            boundTetrimino();
        } else {
            int newState = activeTetrimino.state;
            if (direction == COUNTERCLOCKWISE) {
                newState--;
                if (newState == -1) {
                    newState = 3;
                }
            } else if (direction == CLOCKWISE) {
                newState++;
                if (newState == 4) {
                    newState = 0;
                }
            }
            rotateTetrimino(newState);
        }
    }
}

void undrawTetrimino(void) {
    for (int i = 0; i < 4; i++) {
        position pos = (*activeTetrimino.states)[activeTetrimino.state][i];
        drawRectDMA(BLOCK_SIZE * (ghostTetrimino.pos.y + pos.y) + boardRow, BLOCK_SIZE * (ghostTetrimino.pos.x + pos.x) + boardCol, BLOCK_SIZE, BLOCK_SIZE, BLACK);
        drawRectDMA(BLOCK_SIZE * (activeTetrimino.pos.y + pos.y) + boardRow, BLOCK_SIZE * (activeTetrimino.pos.x + pos.x) + boardCol, BLOCK_SIZE, BLOCK_SIZE, BLACK);
    }
}

void drawTetrimino(void) {
    ghostTetrimino = activeTetrimino;
    while (!isTetriminoColliding(ghostTetrimino)) {
        ghostTetrimino.pos.y++;
    }
    ghostTetrimino.pos.y--;
    for (int i = 0; i < 4; i++) {
        position pos = (*activeTetrimino.states)[activeTetrimino.state][i];
        drawRectDMA(BLOCK_SIZE * (ghostTetrimino.pos.y + pos.y) + boardRow, BLOCK_SIZE * (ghostTetrimino.pos.x + pos.x) + boardCol, BLOCK_SIZE, BLOCK_SIZE, GRAY);
        drawRectDMA(BLOCK_SIZE * (activeTetrimino.pos.y + pos.y) + boardRow, BLOCK_SIZE * (activeTetrimino.pos.x + pos.x) + boardCol, BLOCK_SIZE, BLOCK_SIZE, activeTetrimino.color);
    }
}
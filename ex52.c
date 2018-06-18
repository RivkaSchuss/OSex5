//Rivka Schuss 340903129


#include <signal.h>
#include <zconf.h>
#include <stdlib.h>
#include <stdio.h>


#define BOARD_SIZE 20
#define HORIZ 0
#define VERTICAL 1
#define TRUE 1
#define STD_ERROR 2
#define ERROR "Error in system call.\n"

/**
 * the game board struct
 */
typedef struct Board {
    char gameBoard[BOARD_SIZE][BOARD_SIZE];
    int x_position;
    int y_position;
    int shape;
} Board;

Board board;

void eraseBoard(Board* board);
void updatePos(Board* board, int x, int y, int rotate);
void initializeBoard(Board* board);
void printBoard(Board* board);
void movePiece(int sig);
void rotateShape(Board *board);
void getKey(int sig);

int main() {
    initializeBoard(&board);
    printBoard(&board);
    //signals to the move piece function
    signal(SIGALRM, movePiece);
    //sends an alarms
    alarm(TRUE);
    //signals to the get key function
    signal(SIGUSR2, getKey);
    while (TRUE) {
        //wait until signal from a process
        pause();
    }
}

/**
 * writes an error
 */
void error() {
    write(STD_ERROR, ERROR, sizeof(ERROR));
    exit(-1);
}

/**
 * initializes the game board
 * @param board reference to the game board
 */
void initializeBoard(Board* board) {
    board->shape = HORIZ;
    board->x_position = 0;
    board->y_position = BOARD_SIZE / 2 - 1;
    eraseBoard(board);
    updatePos(board, board->x_position, board->y_position, 0);
}

/**
 * prints the game board
 * @param board reference to the game board
 */
void printBoard(Board* board) {
    //clears the board
    if (system("clear") < 0) {
        error();
    }

    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            printf("%c", board->gameBoard[i][j]);
        }
        printf("\n");
    }
}

/**
 * clears the game board
 * @param board reference to the game board
 */
void eraseBoard(Board* board) {
    int i, j;
    for (i = 0; i < BOARD_SIZE - 1; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (j == 0 || j == BOARD_SIZE - 1) {
                board->gameBoard[i][j] = '*';
                continue;
            }
            board->gameBoard[i][j] = ' ';
        }
    }
    for (j = 0; j < BOARD_SIZE; j++) {
        board->gameBoard[BOARD_SIZE - 1][j] = '*';
    }
}

/**
 * updates the piece's position
 * @param board reference to the game board
 * @param x the current x position
 * @param y the current y position
 * @param rotate whether or not the piece is rotated
 */
void updatePos(Board* board, int x, int y, int rotate) {

    if (board->shape == HORIZ) {
        if (y <= 0)
            return;
        if (y + 2 >= BOARD_SIZE -1 )
            return;
    }
    if (board->shape == VERTICAL ) {
        if (y <= 0)
            return;
        if (y >= BOARD_SIZE - 1)
            return;
    }
    if (!rotate) {
        if (board->shape == HORIZ) {
            board->gameBoard[board->x_position][board->y_position] = ' ';
            board->gameBoard[board->x_position][board->y_position + 1] = ' ';
            board->gameBoard[board->x_position][board->y_position + 2] = ' ';
        } else {
            board->gameBoard[board->x_position][board->y_position] = ' ';
            board->gameBoard[board->x_position + 1][board->y_position] = ' ';
            board->gameBoard[board->x_position + 2][board->y_position] = ' ';
        }
    }

    board->x_position = x;
    board->y_position = y;
    if (board->shape == HORIZ) {
        board->gameBoard[x][y] = '-';
        board->gameBoard[x][y + 1] = '-';
        board->gameBoard[x][y + 2] = '-';

        if (x >= BOARD_SIZE - 1) {
            initializeBoard(board);
        }

    } else {
        board->gameBoard[x][y] = '-';
        board->gameBoard[x + 1][y] = '-';
        board->gameBoard[x + 2][y] = '-';

        if (x + 2 >= BOARD_SIZE - 1) {
            initializeBoard(board);
        }
    }
}

/**
 * moves a piece
 * @param sig the signal
 */
void movePiece(int sig) {
    //sends a signal
    signal(SIGALRM, movePiece);
    alarm(TRUE);
    updatePos(&board, board.x_position + 1, board.y_position, 0);
    printBoard(&board);
}


/**
 * rotates the game piece
 * @param board reference to the game board
 */
void rotateShape(Board *board) {
    if (board->shape == HORIZ) {
        board->gameBoard[board->x_position][board->y_position] = ' ';
        board->gameBoard[board->x_position][board->y_position + 1] = ' ';
        board->gameBoard[board->x_position][board->y_position + 2] = ' ';
    } else {
        board->gameBoard[board->x_position][board->y_position] = ' ';
        board->gameBoard[board->x_position + 1][board->y_position] = ' ';
        board->gameBoard[board->x_position + 2][board->y_position] = ' ';
    }
    int prevXPos = board->x_position;
    int prevYPos = board->y_position;
    if (board->shape == HORIZ) {
        board->shape = VERTICAL;
        updatePos(board , prevXPos - 1, prevYPos + 1, 1);
    } else  {
        int x = prevXPos + 1;
        int y = prevYPos - 1;

        if (y + 2 >= BOARD_SIZE - 1) {
            y = BOARD_SIZE - 4;
        }
        board->shape = HORIZ;
        updatePos(board, x, y, 1);
    }
}


/**
 * gets the key entered to progress
 * @param sig the signal
 */
void getKey(int sig) {
    //sends a signal
    signal(SIGUSR2, getKey);
    char c;
    c = getchar();
    switch (c) {
        case 's':
            updatePos(&board, board.x_position + 1, board.y_position, 0);
            break;
        case 'd':
            updatePos(&board, board.x_position, board.y_position + 1, 0);
            break;
        case 'a':
            updatePos(&board, board.x_position, board.y_position - 1, 0);
            break;
        case 'w':
            rotateShape(&board);
            break;
        case 'q':
            exit(TRUE);
        default:
            break;
    }
    printBoard(&board);
}




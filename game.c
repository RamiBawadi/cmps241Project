#include <stdio.h>

#define ROWS 6
#define COLS 7

void insert(char board[ROWS][COLS], int row, int col, char symbol) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        if (board[row][col] == '*') {
            board[row][col] = symbol;
        } else {
            printf("That spot is already taken!\n");
        }
    } else {
        printf("Invalid position!\n");
    }
}

void printBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char board[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '*';
        }
    }

    printf("Initial board:\n");
    printBoard(board);

    printf("\nBoard after insertions:\n");
    printBoard(board);

    return 0;
}

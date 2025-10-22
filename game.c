#include <stdio.h>
#include "game.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


char getChar(char symbol, char dif) {
    bool validIN = false;
    char sym;

    while (!validIN) { 
        
        printf("\n");
        printf("Player %c, please select a character : ", symbol);
        clear_input_buffer();
        if (scanf("%c", &sym) != 1) {
            printf("Invalid character.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (sym == '\n' ||  sym == '\0'|| sym == ' ' || sym == '*' || sym == '#'||sym == dif) {
            printf("Can't select this char, please select another char.\n");
        } else {
            validIN = true;
            return sym;
        }
    }
    return '*';
}

char boardAt(char board[ROWS][COLS], int row, int col) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        return board[row][col];
    }
    return '\0';
}

void insertAt(char board[ROWS][COLS], int row, int col, char symbol) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        if (boardAt(board, row, col) == '*') {
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
    printf("1 2 3 4 5 6 7\n");
}

int checkWinCondition(char board[ROWS][COLS]) {
    int directions[4][2] = {
        {0, 1}, {1, 0}, {1, 1}, {-1, 1}
    };

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            char symbol = boardAt(board, row, col);
            if (symbol == '*' || symbol == '\0') continue;

            for (int dir = 0; dir < 4; dir++) {
                int rowStep = directions[dir][0];
                int colStep = directions[dir][1];
                int count = 1;

                for (int k = 1; k < 4; k++) {
                    int newRow = row + rowStep * k;
                    int newCol = col + colStep * k;
                    if (newRow < 0 || newRow >= ROWS || newCol < 0 || newCol >= COLS)
                        break;

                    char next = boardAt(board, newRow, newCol);
                    if (next == symbol)
                        count++;
                    else
                        break;
                }

                if (count >= 4)
                    return 1;
            }
        }
    }

    return 0;
}

int getAvailbleY(char board[ROWS][COLS], int x) {
    for (int i = ROWS - 1; i >= 0; i--) {
        if (boardAt(board, i, x) == '*') {
            return i;
        }
    }
    return -1;
}

void ValidateInput(char board[ROWS][COLS], char symbol) {
    int col, y;
    bool validIN = false;

    while (!validIN) { 
        printf("\nIt's %c's turn. Please enter the column number: ", symbol);
        if (scanf("%d", &col) != 1) {
            clear_input_buffer();
            printf("Invalid column number. Column number between 1-7.\n");
            continue;
        }

        clear_input_buffer();
        col--;
       
        if (col < 0 || col >= COLS) {
            printf("Column out of range! Column number between 1-7.\n");
            continue;
        }

        y = getAvailbleY(board, col);
        if (y == -1) {
            printf("Column %d is full! Try a different one.\n", col + 1);
        } else {
            validIN = true;
            insertAt(board, y, col, symbol);
        }
    }
}

bool canContinueGame(char board[ROWS][COLS]) {
    for (int i = 0; i < COLS; i++) {
        if (getAvailbleY(board, i) != -1)
            return true;
    }
    return false;
}

bool playAgain() {
    bool validIN = false;
    char res;

    while (!validIN) { 
        printf("\nPlease select Y for yes or N for no (Y/n): ");
        if (scanf(" %c", &res) != 1) {
            printf("Invalid character.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (res == 'Y' || res == 'y') return true;
        if (res == 'N' || res == 'n') return false;

        printf("That was not a valid input.\n");
    }

    return false;
}

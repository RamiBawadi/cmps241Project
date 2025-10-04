#include <stdio.h>

#define ROWS 6
#define COLS 7

char BoardAt(char board[ROWS][COLS], int row, int col) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        return board[row][col];
    } else {
        printf("Invalid position!\n");
    }
}

void insertAt(char board[ROWS][COLS], int row, int col, char symbol) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        if (BoardAt(board, row, col) == '*') {
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

int CheckWinCondition(char board[ROWS][COLS]) {
    
    int directions[4][2] = {
        {0, 1},   
        {1, 0},   
        {1, 1},   
        {-1, 1}   
    };

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            char symbol = BoardAt(board, row, col);

            if (symbol == '*' || symbol == '\0')
                continue; 

            
            for (int dir = 0; dir < 4; dir++) {
                int rowStep = directions[dir][0];
                int colStep = directions[dir][1];
                int count = 1;

                
                for (int k = 1; k < 4; k++) {
                    int newRow = row + rowStep * k;
                    int newCol = col + colStep * k;

                    char next = BoardAt(board, newRow, newCol);
                    if (next == symbol)
                        count++;
                    else
                        break;
                }

                if (count == 4)
                    return 1; 
            }
        }
    }

    return 0; 
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

    insertAt(board,1,1,'A');
    
    printf("\nBoard after insertions:\n");
    printBoard(board);

    return 0;
}



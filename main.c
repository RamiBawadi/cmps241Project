#include <stdio.h>
#include <stdbool.h>
#include "game.h"

int main() {
    char board[ROWS][COLS];
    bool turnA = true;
    bool endGame = false;
    char playerAChar = 'A'
    char playerBChar = 'B';
    int countAWon = 0
    int countBWon = 0;

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            board[i][j] = '*';
        }
    }

    printf("Welcome to Connect 4!");

    playerAChar = getChar('A', '*');
    playerBChar = getChar('B', playerAChar);

    while (!endGame) {
        if (!canContinueGame(board)) {

            printf("Board is full! No one won.\n");

            printf("\nDo you want to play again?");
            if (playAgain()) {
                for (int i = 0; i < ROWS; i++)
                    for (int j = 0; j < COLS; j++)
                        board[i][j] = '*';

                printf("\n\nWelcome to a new game of Connect 4!");
                turnA = true;
                continue;
            } else {
                printf("Thanks for playing. Games won are %d-%d\n", countAWon, countBWon);
                break;
            }
        }

        if (turnA){
            ValidateInput(board, playerAChar);
        }
        else{
            ValidateInput(board, playerBChar);
        }

        printBoard(board);

        if (checkWinCondition(board)) {
            if (turnA) {
                printf("\nPlayer %c wins!\n", playerBChar);
                countBWon++;
            } else {
                printf("\nPlayer %c wins!\n", playerAChar);
                countAWon++;
            }

            printf("\nDo you want to play again?");
            if (playAgain()) {
                for (int i = 0; i < ROWS; i++)
                    for (int j = 0; j < COLS; j++)
                        board[i][j] = '*';

                printf("\n\nWelcome to a new game of Connect 4!");
                turnA = true;
                continue;
            } else {
                printf("Thanks for playing. Games won are %d-%d\n", countAWon, countBWon);
                break;
            }
        }

        turnA = !turnA; //switch player
    }

    return 0;
}

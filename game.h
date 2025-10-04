#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define ROWS 6
#define COLS 7

// Input handling
void clear_input_buffer();
char getChar(char symbol, char dif);
bool playAgain();

// Board manipulation
char boardAt(char board[ROWS][COLS], int row, int col);
void insertAt(char board[ROWS][COLS], int row, int col, char symbol);
void printBoard(char board[ROWS][COLS]);

// Game logic
int checkWinCondition(char board[ROWS][COLS]);
int getAvailbleY(char board[ROWS][COLS], int x);
void ValidateInput(char board[ROWS][COLS], char symbol);
bool canContinueGame(char board[ROWS][COLS]);

#endif

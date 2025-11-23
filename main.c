#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "ai.h"
#include "server.h"

int main()
{
    char board[ROWS][COLS];
    bool turnA = true;
    bool endGame = false;

    char playerAChar = 'A';
    char playerBChar = 'B';
    int countAWon = 0;
    int countBWon = 0;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = '*';
        }
    }

    printf("Welcome to Connect 4!");

    playerAChar = getChar('A', '*');

    bool validinputIN = false;
    bool isPlayerB_AI = false;
    bool isPlayerB_Online = false;
    bool isHost = false;

    while (!validinputIN)
    {
        printf("\n");
        int res;
        printf("Do you want to play agaist:\n\t0 : AI\n\t1 : Another Player Locally\n\t2 : Another Player Online\n\nPlease select a number (0,1,2) : ");
        if (scanf("%d", &res) != 1)
        {
            printf("Invalid number, please try again.\n");
            clear_input_buffer();
            continue;
        }

        if (res == 0)
        {
            validinputIN = true;
            isPlayerB_AI = true;
        }
        else if (res == 1)
        {
            validinputIN = true;
            isPlayerB_AI = false;
        }
        else if (res == 2)
        {
            clear_input_buffer();
            bool validinputIN_2 = false;
            while (!validinputIN_2)
            {
                int res2;
                printf("\n\nPlaying Online!\n Please select:\n\t0 : Host Game\n\t1 : Join Game\n\t2 : Go Back\n\nPlease select a number (0,1,2) : ");

                if (scanf("%d", &res2) != 1)
                {
                    printf("Invalid number, please try again.\n");
                    clear_input_buffer();
                    continue;
                }

                if (res2 == 0)
                {
                    isHost = true;
                    isPlayerB_Online = true;
                    validinputIN_2 = true;
                    validinputIN = true;
                    hostGame();
                }
                else if (res2 == 1)
                {
                    isHost = false;
                    isPlayerB_Online = true;
                    validinputIN_2 = true;
                    validinputIN = true;
                    joinGame();
                }
                else if (res2 == 2)
                {
                    validinputIN_2 = true; // to break inner loop
                    validinputIN = false;  // to continue outer loop
                }
                else
                {
                    printf("Invalid number, please try again.\n");
                    clear_input_buffer();
                }
            }
        }

        else
        {
            printf("Invalid number, please try again.\n");
            clear_input_buffer();
        }
    }

    if (isPlayerB_AI)
    {
        playerBChar = '#';
        reportStrategyComplexity();
    }
    else
    {
        playerBChar = getChar('B', playerAChar);
    }

    while (!endGame)
    {
        if (!canContinueGame(board))
        {

            printf("Board is full! No one won.\n");

            printf("\nDo you want to play again?");
            bool _playAgain = playAgain();
            printf("\nPlay again status : %d", _playAgain);
            if (_playAgain)
            {
                for (int i = 0; i < ROWS; i++)
                    for (int j = 0; j < COLS; j++)
                        board[i][j] = '*';

                printf("\n\nWelcome to a new game of Connect 4!");
                turnA = true;
                continue;
            }
            else
            {
                printf("Thanks for playing. Games won are %d-%d\n", countAWon, countBWon);
                break;
            }
        }

        if (turnA)
        {
            ValidateInput(board, playerAChar);
        }
        else
        {
            if (isPlayerB_AI)
            {
                printf("\n\n");
                ValidateInput_Ai(board);
            }
            else
            {
                ValidateInput(board, playerBChar);
            }
        }

        printBoard(board);

        if (checkWinCondition(board))
        {
            if (turnA)
            {
                printf("\nPlayer %c wins!\n", playerAChar);
                countAWon++;
            }
            else
            {
                if (isPlayerB_AI)
                {
                    printf("\nAI wins!\n");
                }
                else
                {
                    printf("\nPlayer %c wins!\n", playerBChar);
                }
                countBWon++;
            }

            printf("\nDo you want to play again?");
            if (playAgain())
            {
                for (int i = 0; i < ROWS; i++)
                    for (int j = 0; j < COLS; j++)
                        board[i][j] = '*';

                printf("\n\nWelcome to a new game of Connect 4!");
                turnA = true;
                continue;
            }
            else
            {
                printf("Thanks for playing. Games won are %d-%d\n", countAWon, countBWon);
                break;
            }
        }

        turnA = !turnA; // switch player
    }

    return 0;
}

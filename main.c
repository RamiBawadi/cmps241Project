#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "ai.h"
#include "server.h"
#include "client.h"

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
                }
                else if (res2 == 1)
                {
                    isHost = false;
                    isPlayerB_Online = true;
                    validinputIN_2 = true;
                    validinputIN = true;
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
    else if (isPlayerB_Online)
    {
        if (isHost)
        {
            // Host starts the server and waits for client
            printf("\nStarting server... waiting for another player..\n");
            server_init();
            server_send_char(playerAChar);
            playerBChar = server_receive_char();
            printf("Another player connected with character %c.\n\n", playerBChar);
        }
        else
        {
            // Client connects to the host
            char ip[64];
            printf("Enter host IP (eg: 127.0.0.1): ");
            scanf("%63s", ip);

            client_init(ip);

            playerBChar = client_receive_char();
            client_send_char(playerAChar);
        }
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

            if (isPlayerB_Online && isHost)
            {

                printf("\n");
                ValidateInput_Server(board, playerAChar);
                server_send_board(board);
            }
            else if (isPlayerB_Online && !isHost)
            {
                printf("\nIt's Player %c's turn. Waiting for their move...\n\n", playerBChar);
                client_receive_board(board);
            }
            else
            {
                ValidateInput(board, playerAChar);
            }
        }
        else
        {

            if (isPlayerB_AI)
            {
                ValidateInput_Ai(board);
            }
            else if (isPlayerB_Online && isHost)
            {
                // Server receives B's moves
                printf("\nIt's Player %c's turn. Waiting for their move...\n\n", playerBChar);
                int col = server_receive_int();
                int y = getAvailbleY(board, col);
                insertAt(board, y, col, playerBChar);
                server_send_board(board);
            }
            else if (isPlayerB_Online && !isHost)
            {

                printf("\n");
                int col = ValidateInput_Client(board, playerAChar);
                client_send_int(col);
                client_receive_board(board);
            }
            else
            {
                // LOCAL PvP
                ValidateInput(board, playerBChar);
            }
        }

        printBoard(board);
        printf("\n");
        bool winner = checkWinCondition(board);

        if (!winner && isPlayerB_Online && isHost)
        {
            server_send_message("No Win");
        }

        if (isPlayerB_Online && !isHost)
        {
            char *msg = client_receive_message();

            if (strncmp(msg, "WIN:", 4) == 0)
            {
                char winner = msg[4];
                if (winner != playerAChar)
                {
                    printf("\nPlayer %c wins! Better luck next time\n", winner);
                }
                else
                {
                    printf("\nYou WON! Congratulations!\n");
                }
                endGame = true;
                client_close();
            }
        }

        if (winner && (!isPlayerB_Online || (isPlayerB_Online && isHost)))
        {
            if (turnA)
            {

                if (isPlayerB_Online)
                {

                    char msg[16];
                    printf("\nYou WON! Congratulations!\n");
                    sprintf(msg, "WIN:%c", playerAChar);
                    server_send_message(msg);
                    endGame = true;
                    break;
                }
                else
                {
                    printf("\nPlayer %c wins!\n", playerAChar);
                    countAWon++;
                }
            }
            else
            {
                if (isPlayerB_AI)
                {
                    printf("\nAI wins!\n");
                }
                else if (isPlayerB_Online)
                {
                    printf("\nPlayer %c wins! Better luck next \n", playerBChar);
                    char msg[16];
                    sprintf(msg, "WIN:%c", playerBChar); // format WIN:A or WIN:B
                    server_send_message(msg);
                    endGame = true;
                    break;
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

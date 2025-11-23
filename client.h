#ifndef CLIENT_H
#define CLIENT_H
#include "game.h"

int client_init(const char *ip);
char *client_receive_message();
void client_receive_board(char board[ROWS][COLS]);
void client_send_int(int col);
void client_close();

#endif

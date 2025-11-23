#ifndef SERVER_H
#define SERVER_H
#include "game.h"

int server_init();
void server_send_message(const char *msg);
void server_send_board(char board[ROWS][COLS]);
int server_receive_int();
char server_receive_char();
void server_send_char(char c);
void server_close();

#endif

#include "server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 8080

static int client_fd;
static int server_fd;

int server_init()
{
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        return 0;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind");
        return 0;
    }

    if (listen(server_fd, 1) < 0)
    {
        perror("listen");
        return 0;
    }

    printf("Waiting for player B...\n");

    client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (client_fd < 0)
    {
        perror("accept");
        return 0;
    }
}

void server_send_message(const char *msg)
{
    write(client_fd, msg, strlen(msg));
    write(client_fd, "\n", 1);
}

void server_send_board(char board[ROWS][COLS])
{
    for (int r = 0; r < ROWS; r++)
    {
        write(client_fd, board[r], COLS);
        write(client_fd, "\n", 1);
    }
}

int server_receive_int()
{
    char buffer[16];
    int n = read(client_fd, buffer, sizeof(buffer) - 1);
    if (n <= 0)
        return -1;

    buffer[n] = '\0';
    return atoi(buffer);
}

char server_receive_char()
{
    char c;
    int n = read(client_fd, &c, 1);
    if (n <= 0)
        return '\0';
    return c;
}

void server_send_char(char c)
{
    write(client_fd, &c, 1);
}

void server_close()
{
    close(client_fd);
    close(server_fd);
}

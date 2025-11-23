#include "client.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 8080

static int sockfd;

int client_init(const char *ip)
{
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return 0;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        return 0;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        return 0;
    }

    printf("Connected to server at %s:%d\n", ip, PORT);
    return 1;
}

char *client_receive_message()
{
    static char buffer[128];
    int n = read(sockfd, buffer, sizeof(buffer) - 1);

    if (n <= 0)
    {
        buffer[0] = '\0';
        return buffer;
    }

    buffer[n] = '\0';
    return buffer;
}

void client_receive_board(char board[ROWS][COLS])
{
    char c;
    char line[COLS + 2];

    for (int r = 0; r < ROWS; r++)
    {
        int i = 0;

        while (1)
        {
            int n = read(sockfd, &c, 1);

            if (n <= 0)
                return; // disconnection

            if (c == '\n')
                break;

            if (i < COLS)
                line[i++] = c;
        }

        line[i] = '\0';

        for (int col = 0; col < COLS; col++)
            board[r][col] = line[col];
    }
}

void client_send_int(int col)
{
    char buf[16];
    sprintf(buf, "%d\n", col); // newline for server parsing
    write(sockfd, buf, strlen(buf));
}

char client_receive_char()
{
    char c;
    int n = read(sockfd, &c, 1);

    if (n <= 0)
        return '\0';

    return c;
}

void client_send_char(char c)
{
    write(sockfd, &c, 1);
}

void client_close()
{
    close(sockfd);
}

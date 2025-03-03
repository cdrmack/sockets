#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/foo_socket"
#define SOCKET_ERROR (-1)
#define BACKLOG 4
#define BUFFER_SIZE 256

typedef struct sockaddr_un SA_UN;
typedef struct sockaddr SA;

void handle_connection(int client_socket);
void check(int result, const char *fn_name);

int main(int argc, char *argv[])
{
    unlink(SOCKET_NAME);

    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    check(server_socket, "socket()");

    SA_UN server_addr = {0};
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_NAME,
            sizeof(server_addr.sun_path) - 1);

    check(bind(server_socket, (SA *)&server_addr, sizeof(SA_UN)), "bind()");
    check(listen(server_socket, BACKLOG), "listen()");

    int client_socket = 0;

    while (true)
    {
        printf("[server] waiting for connection\n");
        client_socket = accept(server_socket, NULL, NULL);
        check(client_socket, "accept");

        handle_connection(client_socket);
    }

    return EXIT_SUCCESS;
}

void handle_connection(int client_socket)
{
    char buffer[BUFFER_SIZE] = {0};
    int received_data = 0;
    int result = 0;

    while (true)
    {
        check(read(client_socket, buffer, BUFFER_SIZE), "read()");
        memcpy(&received_data, buffer, sizeof(int));
        printf("[server] received %d\n", received_data);

        if (received_data == 0)
        {
            break;
        }

        result += received_data;
    }

    memset(buffer, 0, BUFFER_SIZE);
    sprintf(buffer, "result = %d", result);

    check(write(client_socket, buffer, BUFFER_SIZE), "write()");
    check(close(client_socket), "close()");
}

void check(int result, const char *fn_name)
{
    if (result == SOCKET_ERROR)
    {
        perror(fn_name);
        exit(EXIT_FAILURE);
    }

    printf("[server] %s\n", fn_name);
}

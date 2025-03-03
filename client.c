#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/foo_socket"
#define SOCKET_ERROR (-1)
#define BUFFER_SIZE 256

typedef struct sockaddr_un SA_UN;
typedef struct sockaddr SA;

void check(int result, const char *fn_name);

int main(int artgc, char *argv[])
{
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    check(client_socket, "socket()");

    SA_UN address = {0};
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_NAME, sizeof(address.sun_path) - 1);

    check(connect(client_socket, (SA *)&address, sizeof(SA_UN)), "connect()");

    int number;
    do
    {
        printf("[client] enter number to be sent to the server: ");
        scanf("%d", &number);

        check(write(client_socket, &number, sizeof(int)), "write()");

        printf("[client] data sent = %d\n", number);
    } while (number);

    puts("[client] waiting for the result");
    char buffer[BUFFER_SIZE] = {0};
    check(read(client_socket, buffer, BUFFER_SIZE), "read()");

    printf("[client] received result: %s\n", buffer);

    close(client_socket);

    return EXIT_SUCCESS;
}

void check(int result, const char *fn_name)
{
    if (result == SOCKET_ERROR)
    {
        perror(fn_name);
        exit(EXIT_FAILURE);
    }

    printf("[client] %s\n", fn_name);
}

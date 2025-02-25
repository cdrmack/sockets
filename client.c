#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/foo_socket"

int main(int artgc, char *argv[])
{
    int data_socket;
    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (data_socket == -1)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    puts("socket() call succeeded");

    struct sockaddr_un address;
    memset(&address, 0, sizeof(struct sockaddr_un));

    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_NAME, sizeof(address.sun_path) - 1);
    // address.sun_len is a legacy feature

    int ret = connect(data_socket, (struct sockaddr *)&address,
                      sizeof(struct sockaddr_un));

    if (ret == -1)
    {
        perror("connect() failed");
        exit(EXIT_FAILURE);
    }

    puts("connect() call succeeded");

    return EXIT_SUCCESS;
}

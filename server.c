#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/foo_socket"

int main(int artgc, char *argv[])
{
    unlink(SOCKET_NAME);

    int socket_descriptor;
    socket_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);

    if (socket_descriptor == -1)
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

    int ret;
    ret = bind(socket_descriptor, (struct sockaddr *)&address,
               sizeof(struct sockaddr_un));

    if (ret == -1)
    {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }

    puts("bind() call succeeded");

    const int backlog = 20;
    ret = listen(socket_descriptor, backlog);

    if (ret == -1)
    {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    puts("listen() call succeeded");

    int data_socket;
    for (;;)
    {
        puts("waiting for accept()");

        // blocking operation
        data_socket = accept(socket_descriptor, NULL, NULL);
        if (data_socket == -1)
        {
            perror("accept() failed");
            exit(EXIT_FAILURE);
        }

        puts("connection accepted");

        for (;;)
        {
            // TODO
        }
    }

    close(socket_descriptor);

    return EXIT_SUCCESS;
}

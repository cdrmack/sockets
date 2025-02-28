#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/foo_socket"
#define BUFFER_SIZE 256

int main(int artgc, char *argv[])
{
    unlink(SOCKET_NAME);

    int socket_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);

    if (socket_descriptor == -1)
    {
        perror("[server] socket() failed");
        exit(EXIT_FAILURE);
    }

    puts("[server] socket() call succeeded");

    struct sockaddr_un address;
    memset(&address, 0, sizeof(struct sockaddr_un));

    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_NAME, sizeof(address.sun_path) - 1);
    // address.sun_len is a legacy feature

    int ret = bind(socket_descriptor, (struct sockaddr *)&address,
                   sizeof(struct sockaddr_un));

    if (ret == -1)
    {
        perror("[server] bind() failed");
        exit(EXIT_FAILURE);
    }

    puts("[server] bind() call succeeded");

    const int backlog = 20;
    ret = listen(socket_descriptor, backlog);

    if (ret == -1)
    {
        perror("[server] listen() failed");
        exit(EXIT_FAILURE);
    }

    puts("[server] listen() call succeeded");

    int data_socket = 0;
    char buffer[BUFFER_SIZE];

    for (;;)
    {
        puts("[server] waiting for accept()");

        // blocking operation
        data_socket = accept(socket_descriptor, NULL, NULL);
        if (data_socket == -1)
        {
            perror("[server] accept() failed");
            exit(EXIT_FAILURE);
        }

        puts("[server] connection accepted");

        int result = 0;
        for (;;)
        {
            memset(buffer, 0, BUFFER_SIZE);

            puts("[server] waiting for read()");
            // blocking operation
            ret = read(data_socket, buffer, BUFFER_SIZE);

            if (ret == -1)
            {
                perror("[server] read() failed");
                exit(EXIT_FAILURE);
            }

            int read_value = 0;
            memcpy(&read_value, buffer, sizeof(int));
            printf("[server] received: %d\n", read_value);

            if (read_value == 0)
            {
                break;
            }

            result += read_value;
        }

        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, "Result = %d", result);

        puts("[server] sending result");
        ret = write(data_socket, buffer, BUFFER_SIZE);

        if (ret == -1)
        {
            perror("[server] write() failed");
            exit(EXIT_FAILURE);
        }

        close(data_socket);
        puts("[server] connection closed");
    }

    close(socket_descriptor);
    unlink(SOCKET_NAME);

    return EXIT_SUCCESS;
}

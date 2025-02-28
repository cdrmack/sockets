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
    int data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (data_socket == -1)
    {
        perror("[client] socket() failed");
        exit(EXIT_FAILURE);
    }

    puts("[client] socket() call succeeded");

    struct sockaddr_un address;
    memset(&address, 0, sizeof(struct sockaddr_un));

    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_NAME, sizeof(address.sun_path) - 1);
    // address.sun_len is a legacy feature

    int ret = connect(data_socket, (struct sockaddr *)&address,
                      sizeof(struct sockaddr_un));

    if (ret == -1)
    {
        perror("[client] connect() failed");
        exit(EXIT_FAILURE);
    }

    puts("[client] connect() call succeeded");

    int number;
    do
    {
        printf("[client] enter number to send to server: ");
        scanf("%d", &number);

        ret = write(data_socket, &number, sizeof(int));
        if (ret == -1)
        {
            perror("[client] write() failed");
            exit(EXIT_FAILURE);
        }

        printf("[client] sent %d bytes, data sent = %d\n", ret, number);
    } while (number);

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    puts("[client] waiting for the result");
    ret = read(data_socket, buffer, BUFFER_SIZE);

    if (ret == -1)
    {
        perror("[client] read() failed");
        exit(EXIT_FAILURE);
    }

    printf("[client] received result: %s\n", buffer);

    close(data_socket);

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define REQUEST_FIFO "client_to_server"
#define RESPONSE_FIFO "server_to_client"

#define BUFFER_SIZE 256

int main()
{
    int request_fd;
    int response_fd;

    char buffer[BUFFER_SIZE];

    // Create request FIFO
    if (mkfifo(REQUEST_FIFO, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo request");
            exit(1);
        }
    }

    // Create response FIFO
    if (mkfifo(RESPONSE_FIFO, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo response");
            exit(1);
        }
    }

    printf("====================================\n");
    printf("        FIFO SERVER STARTED         \n");
    printf("====================================\n");

    printf("Waiting for client...\n");

    // Open request FIFO for reading
    request_fd = open(REQUEST_FIFO, O_RDONLY);

    if (request_fd == -1)
    {
        perror("Error opening request FIFO");
        exit(1);
    }

    // Open response FIFO for writing
    response_fd = open(RESPONSE_FIFO, O_WRONLY);

    if (response_fd == -1)
    {
        perror("Error opening response FIFO");
        close(request_fd);
        exit(1);
    }

    printf("Client connected.\n");

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        // Read message from client
        ssize_t bytes_read =
            read(request_fd, buffer, sizeof(buffer) - 1);

        if (bytes_read < 0)
        {
            perror("read");
            break;
        }

        if (bytes_read == 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        buffer[bytes_read] = '\0';

        printf("Client sent: %s\n", buffer);

        // Exit command
        if (strcmp(buffer, "exit") == 0)
        {
            char response[] = "Server shutting down.";

            write(response_fd,
                  response,
                  strlen(response));

            break;
        }

        // Process the message
        char response[BUFFER_SIZE + 32];

        snprintf(response,
                 sizeof(response),
                 "Server processed: %s",
                 buffer);

        // Send response
        write(response_fd,
              response,
              strlen(response));

        printf("Response sent: %s\n", response);
    }

    close(request_fd);
    close(response_fd);

    // Remove FIFO files
    unlink(REQUEST_FIFO);
    unlink(RESPONSE_FIFO);

    printf("\nServer terminated.\n");

    return 0;
}

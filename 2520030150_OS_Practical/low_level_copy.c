#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define BUFFER_SIZE 4096

double get_time_seconds()
{
    struct timespec start;

    clock_gettime(CLOCK_MONOTONIC, &start);

    return start.tv_sec +
           start.tv_nsec / 1000000000.0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    const char *source = argv[1];
    const char *destination = argv[2];

    int source_fd;
    int destination_fd;

    char buffer[BUFFER_SIZE];

    ssize_t bytes_read;
    ssize_t bytes_written;

    source_fd = open(source, O_RDONLY);

    if (source_fd == -1)
    {
        perror("open source");
        return 1;
    }

    destination_fd = open(
        destination,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (destination_fd == -1)
    {
        perror("open destination");
        close(source_fd);
        return 1;
    }

    double start_time = get_time_seconds();

    while ((bytes_read = read(
                source_fd,
                buffer,
                BUFFER_SIZE)) > 0)
    {
        ssize_t total_written = 0;

        while (total_written < bytes_read)
        {
            bytes_written = write(
                destination_fd,
                buffer + total_written,
                bytes_read - total_written
            );

            if (bytes_written == -1)
            {
                perror("write");
                close(source_fd);
                close(destination_fd);
                return 1;
            }

            total_written += bytes_written;
        }
    }

    if (bytes_read == -1)
    {
        perror("read");
        close(source_fd);
        close(destination_fd);
        return 1;
    }

    double end_time = get_time_seconds();

    close(source_fd);
    close(destination_fd);

    printf("Low-level copy completed.\n");
    printf("Source      : %s\n", source);
    printf("Destination : %s\n", destination);
    printf("Time        : %.6f seconds\n",
           end_time - start_time);

    return 0;
}

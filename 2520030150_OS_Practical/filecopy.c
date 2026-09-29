#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
    int source_fd, destination_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Open source file for reading
    source_fd = open("sample.txt", O_RDONLY);

    if (source_fd == -1)
    {
        perror("Error opening source file");
        return 1;
    }

    // Open/create destination file for writing
    destination_fd = open("copy.txt",
                          O_WRONLY | O_CREAT | O_TRUNC,
                          0644);

    if (destination_fd == -1)
    {
        perror("Error opening destination file");
        close(source_fd);
        return 1;
    }

    // Read from source and write to destination
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0)
    {
        ssize_t total_written = 0;

        while (total_written < bytes_read)
        {
            ssize_t bytes_written = write(
                destination_fd,
                buffer + total_written,
                bytes_read - total_written
            );

            if (bytes_written == -1)
            {
                perror("Error writing to destination file");
                close(source_fd);
                close(destination_fd);
                return 1;
            }

            total_written += bytes_written;
        }
    }

    if (bytes_read == -1)
    {
        perror("Error reading source file");
    }

    // Close both files
    close(source_fd);
    close(destination_fd);

    printf("File copied successfully.\n");

    return 0;
}

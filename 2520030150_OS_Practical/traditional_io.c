#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main()
{
    const char *filename = "traditional_test.txt";

    int fd;

    char buffer[BUFFER_SIZE];

    const char *data =
        "Hello from traditional read/write I/O!\n";

    fd = open(
        filename,
        O_RDWR | O_CREAT | O_TRUNC,
        0644
    );

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    /*
     * Write initial data.
     */
    if (write(fd, data, strlen(data)) == -1)
    {
        perror("write");
        close(fd);
        return 1;
    }

    /*
     * Move file offset back to beginning.
     */
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        return 1;
    }

    /*
     * Read data.
     */
    ssize_t bytes_read = read(
        fd,
        buffer,
        sizeof(buffer) - 1
    );

    if (bytes_read == -1)
    {
        perror("read");
        close(fd);
        return 1;
    }

    buffer[bytes_read] = '\0';

    printf("Original content:\n");
    printf("%s", buffer);

    /*
     * Move back to beginning.
     */
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        return 1;
    }

    const char *replacement =
        "Modified using read/write!\n";

    if (write(
            fd,
            replacement,
            strlen(replacement)) == -1)
    {
        perror("write");
        close(fd);
        return 1;
    }

    printf("\nModified content:\n");
    printf("%s", replacement);

    close(fd);

    return 0;
}

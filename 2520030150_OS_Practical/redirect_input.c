#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char buffer[100];

    fd = open("input.txt", O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return 1;
    }

    close(fd);

    printf("Reading from redirected stdin:\n");

    ssize_t bytes_read;

    while ((bytes_read =
                read(STDIN_FILENO,
                     buffer,
                     sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    return 0;
}

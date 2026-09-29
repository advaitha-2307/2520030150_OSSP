#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
    const char *filename = "mmap_test.txt";

    int fd;

    struct stat file_info;

    char *mapped_data;

    fd = open(
        filename,
        O_RDWR | O_CREAT,
        0644
    );

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    const char *initial_data =
        "Hello from memory mapped I/O!\n";

    /*
     * Set file size before mapping.
     */
    if (ftruncate(fd, strlen(initial_data)) == -1)
    {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    if (write(fd, initial_data, strlen(initial_data)) == -1)
    {
        perror("write");
        close(fd);
        return 1;
    }

    if (fstat(fd, &file_info) == -1)
    {
        perror("fstat");
        close(fd);
        return 1;
    }

    size_t file_size = file_info.st_size;

    /*
     * Map file into memory.
     */
    mapped_data = mmap(
        NULL,
        file_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    if (mapped_data == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    printf("Original file content:\n");
    printf("%.*s", (int)file_size, mapped_data);

    /*
     * Modify file through mapped memory.
     */
    const char *replacement =
        "Modified using mmap()!\n";

    size_t replacement_size = strlen(replacement);

    if (replacement_size <= file_size)
    {
        memcpy(
            mapped_data,
            replacement,
            replacement_size
        );
    }

    /*
     * Synchronize memory changes with file.
     */
    if (msync(
            mapped_data,
            file_size,
            MS_SYNC) == -1)
    {
        perror("msync");
    }

    printf("\nModified content:\n");
    printf("%.*s", (int)file_size, mapped_data);

    /*
     * Remove mapping.
     */
    if (munmap(mapped_data, file_size) == -1)
    {
        perror("munmap");
    }

    close(fd);

    printf("\nChanges written to %s\n", filename);

    return 0;
}

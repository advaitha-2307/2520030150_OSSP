#include <stdio.h>
#include <stdlib.h>
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

    FILE *source;
    FILE *destination;

    char buffer[BUFFER_SIZE];

    size_t bytes_read;

    source = fopen(argv[1], "rb");

    if (source == NULL)
    {
        perror("fopen source");
        return 1;
    }

    destination = fopen(argv[2], "wb");

    if (destination == NULL)
    {
        perror("fopen destination");
        fclose(source);
        return 1;
    }

    double start_time = get_time_seconds();

    while ((bytes_read =
                fread(buffer, 1, BUFFER_SIZE, source)) > 0)
    {
        size_t bytes_written =
            fwrite(buffer, 1, bytes_read, destination);

        if (bytes_written != bytes_read)
        {
            perror("fwrite");
            fclose(source);
            fclose(destination);
            return 1;
        }
    }

    if (ferror(source))
    {
        perror("fread");
        fclose(source);
        fclose(destination);
        return 1;
    }

    double end_time = get_time_seconds();

    fclose(source);
    fclose(destination);

    printf("Standard library copy completed.\n");
    printf("Source      : %s\n", argv[1]);
    printf("Destination : %s\n", argv[2]);
    printf("Time        : %.6f seconds\n",
           end_time - start_time);

    return 0;
}

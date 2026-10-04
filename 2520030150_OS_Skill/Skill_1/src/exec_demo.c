#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Before exec()\n");

    execlp("ls", "ls", "-l", NULL);

    perror("exec failed");

    return 1;
}

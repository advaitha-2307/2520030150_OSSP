#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;

    printf("Parent process started\n");

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child process created\n");
        printf("Child PID: %d\n", getpid());

        execlp("ls", "ls", "-l", NULL);

        perror("exec failed");
        return 1;
    }
    else
    {
        printf("Parent waiting for child...\n");

        waitpid(pid, NULL, 0);

        printf("Child completed\n");
    }

    return 0;
}

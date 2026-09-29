#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    int fd;

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        /*
         * Child process
         */

        fd = open(
            "ls_output.txt",
            O_WRONLY | O_CREAT | O_TRUNC,
            0644
        );

        if (fd == -1)
        {
            perror("open");
            exit(1);
        }

        /*
         * Redirect stdout
         */
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            exit(1);
        }

        close(fd);

        /*
         * Replace child process with ls
         */
        execlp("ls", "ls", "-l", NULL);

        perror("execlp");
        exit(1);
    }

    /*
     * Parent waits for child
     */

    waitpid(pid, NULL, 0);

    printf("Command completed.\n");
    printf("Check ls_output.txt\n");

    return 0;
}

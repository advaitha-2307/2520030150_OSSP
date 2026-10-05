#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    char *args[] = {"ls", "-l", NULL};

    printf("Parent Process PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        /* Child process */
        printf("Child Process PID: %d\n", getpid());
        printf("Executing: ls -l\n");

        execvp(args[0], args);

        /* execvp returns only if execution fails */
        perror("execvp failed");
        exit(1);
    }

    /* Parent process */
    printf("Parent waiting for child...\n");

    int status;

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        return 1;
    }

    if (WIFEXITED(status)) {
        printf("Child exited with status: %d\n",
               WEXITSTATUS(status));
    } else {
        printf("Child did not exit normally.\n");
    }

    printf("Parent process finished.\n");

    return 0;
}

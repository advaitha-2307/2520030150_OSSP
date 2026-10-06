#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    printf("Parent Process PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        /* Child process */
        printf("Child Process Started\n");
        printf("Child PID: %d\n", getpid());

        for (int i = 1; i <= 5; i++) {
            printf("Child working... %d/5\n", i);
            sleep(1);
        }

        printf("Child Process Completed\n");
        exit(42);
    }

    /* Parent process */
    printf("Parent is monitoring child PID: %d\n", pid);
    printf("Parent waiting using waitpid()...\n");

    int status;

    pid_t result = waitpid(pid, &status, 0);

    if (result == -1) {
        perror("waitpid failed");
        return 1;
    }

    printf("\nChild monitoring completed.\n");

    if (WIFEXITED(status)) {
        printf("Child exited normally.\n");
        printf("Child exit status: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child was terminated by signal: %d\n",
               WTERMSIG(status));
    }

    printf("Parent Process Completed.\n");

    return 0;
}

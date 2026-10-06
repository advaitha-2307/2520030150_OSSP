#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_PATH 4096

int main(void) {
    char command[256];

    printf("PATH Command Resolver\n");
    printf("Enter command name: ");

    if (fgets(command, sizeof(command), stdin) == NULL) {
        return 1;
    }

    command[strcspn(command, "\n")] = '\0';

    if (strlen(command) == 0) {
        printf("Error: Empty command.\n");
        return 1;
    }

    /* If command contains '/', check it directly */
    if (strchr(command, '/') != NULL) {
        if (access(command, X_OK) == 0) {
            printf("Executable found: %s\n", command);
        } else {
            printf("Command not executable or not found: %s\n", command);
        }

        return 0;
    }

    char *path = getenv("PATH");

    if (path == NULL) {
        printf("Error: PATH variable not found.\n");
        return 1;
    }

    printf("\nPATH directories:\n");

    char path_copy[MAX_PATH];

    strncpy(path_copy, path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    char *directory = strtok(path_copy, ":");

    int found = 0;

    while (directory != NULL) {
        char full_path[MAX_PATH];

        snprintf(full_path,
                 sizeof(full_path),
                 "%s/%s",
                 directory,
                 command);

        printf("Checking: %s\n", full_path);

        if (access(full_path, X_OK) == 0) {
            struct stat file_info;

            if (stat(full_path, &file_info) == 0 &&
                S_ISREG(file_info.st_mode)) {

                printf("\nCommand found!\n");
                printf("Executable: %s\n", full_path);

                found = 1;
                break;
            }
        }

        directory = strtok(NULL, ":");
    }

    if (!found) {
        printf("\nCommand not found: %s\n", command);
    }

    return 0;
}

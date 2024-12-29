#include <stdio.h>
#include <stdlib.h>
#include "permissions.h"
#include <sys/stat.h>
#include "logger.h"
#include <string.h>

//Function to change the file/folder permissions
void chperm(const char *path) {
    char input_buffer[16];
    char *endptr;
    unsigned int mode_input;
    mode_t mode;

    struct stat pathStat;

    //Prints error if there anything wrong with the folder
    if (stat(path, &pathStat) == -1) {
        perror("Error opening directory");
        logger("chperm", path, 0, NULL);
        return;
    }

    //Takes the permissions from user and prints any error
    printf("Enter the new permissions for '%s' in octal (e.g., 755): ", path);

    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Failed to read input\n");
        logger("chperm", path, 0, NULL);
        return;
    }

    //Remove newline character if present
    char *newline = strchr(input_buffer, '\n');
    if (newline) {
        *newline = '\0';
    }

    //Convert string to an unsigned integer (octal)
    mode_input = strtoul(input_buffer, &endptr, 8);

    //Check if the input was valid or contained non-numeric characters
    if (*endptr != '\0') {
        printf("Invalid input: not a valid octal number\n");
        logger("chperm", path, 0, NULL);
        return;
    }

    mode = (mode_t)mode_input;

    //Prints if there is any error while changing permissions
    if (chmod(path, mode) == -1) {
        perror("Error changing permissions");
        logger("chperm", path, 0, NULL);
        return;
    }

    logger("chperm", path, 1, NULL);
}
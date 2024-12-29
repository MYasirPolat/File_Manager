#include "logger.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

//Function to log operations to log.txt. Writes the time stamp, operation name, the path(s) affected and if it is succesfful or not
void logger(const char* operation, const char* firstPath,const bool status, const char* secondPath) {

    //Opens the log file and returns error if there is any
    FILE* file = fopen("./static/log.txt", "a");

    if (file == NULL) {
        perror("Error opening log file\n");
        return;
    }

    //Takes the successful status a bool and turns it to a string accordingly
    const char* result = status ? "successfully." : "unsuccessfully.";

    //Gets the current time 
    time_t current_time = time(NULL);
    char* time_str = ctime(&current_time);
    time_str[strlen(time_str) - 1] = '\0'; //Replace the newline character with null character

    //Logs according to the operation
    if (strncmp(operation, "slist", 5) == 0) {
        fprintf(file, "%s : User has listed the folder at %s %s\n", time_str, firstPath, result);

    } else if (strncmp(operation, "crtdir", 6) == 0) {
        fprintf(file, "%s : User has created the folder at %s %s\n", time_str, firstPath, result);

    } else if (strncmp(operation, "deldir", 6) == 0) {
        fprintf(file, "%s : User has deleted folder at %s %s\n", time_str, firstPath, result);

    } else if (strncmp(operation, "crtfile", 7) == 0) {
        fprintf(file, "%s : User has created a file at  %s %s\n", time_str, firstPath, result);

    } else if (strncmp(operation, "delfile", 7) == 0) {
        fprintf(file, "%s : User has deleted a file at %s %s\n", time_str, firstPath, result);

    } else if (strncmp(operation, "cpfile", 6) == 0) {
        fprintf(file, "%s : User has copied a file from %s to %s %s\n", time_str, firstPath, secondPath, result);

    } else if (strncmp(operation, "mvfile", 6) == 0) {
        fprintf(file, "%s : User has moved a file from %s to %s %s\n", time_str, firstPath, secondPath, result);

    } else if (strncmp(operation, "dpfile", 6) == 0) {
        fprintf(file, "%s : User has displayed the contents of a file at %s %s\n", time_str, firstPath, result);

    } else if (strncmp(operation, "cpdir", 5) == 0) {
        fprintf(file, "%s : User has copied a directory from %s to %s %s\n", time_str, firstPath,secondPath, result);

    } else if (strncmp(operation, "srcfile", 7) == 0) {
        fprintf(file, "%s : User has searched a file at %s %s\n", time_str, firstPath, result);

    } else if (strncmp(operation, "chperm", 6) == 0) {
        fprintf(file, "%s : User has changed the permissons of %s %s\n", time_str, firstPath, result);

    }else if (strncmp(operation, "help", 4) == 0) {
        fprintf(file, "%s : User has used the help command %s %s\n", time_str, firstPath, result);

    }
    fclose(file);
}

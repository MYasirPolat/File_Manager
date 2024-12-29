#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "file_operations.h"
#include "directory_ops.h"
#include "permissions.h"

int main(void) {

    char input[256];
    char command[16];
    char firstPath[128];
    char secondPath[128];

    //System works in an infinite loop untill exit command is entered or ctrl + c is pressed
    while(true) {
        printf("Available Commands:\n"
               "1. slist <path>\n"
               "2. crtdir <path>\n"
               "3. deldir <path>\n"
               "4. crtfile <path>\n"
               "5. delfile <path>\n"
               "6. cpfile <source path> <destination path>\n"
               "7. mvfile <source path> <destination path>\n"
               "8. dpfile <path>\n"
               "9. srcfile <path>\n"
               "10. chperm <path>\n"
               "11. exit\n");

        //Prints if there is any error while getting inout from user
        printf("Enter command: ");
        if (fgets(input, 256, stdin) == NULL) {
            perror("Error reading input");
            continue;
        }

        //Initialize variables to empty strings
        command[0] = firstPath[0] = secondPath[0] = '\0';

        //Parse the input
        sscanf(input, "%s %s %s", command, firstPath, secondPath);

        //Runs a function according to the user's input
        if (strncmp(command, "slist", 5)==0){
            slist(firstPath);
        }
        else if (strncmp(command, "crtdir", 6) == 0) {
            crtdir(firstPath);
        } else if (strncmp(command, "deldir", 6) == 0) {
            deldir(firstPath);
        } else if (strncmp(command, "crtfile", 7) == 0) {
            crtfile(firstPath);
        } else if (strncmp(command, "delfile", 6) == 0) {
            delfile(firstPath);
        } else if (strncmp(command, "cpfile", 6) == 0) {
            cpfile(firstPath, secondPath);
        } else if (strncmp(command, "mvfile", 6) == 0) {
            mvfile(firstPath, secondPath);
        } else if (strncmp(command, "dpfile", 6) == 0) {
            dpfile(firstPath);
        } else if (strncmp(command, "srcfile", 7) == 0) {
            srcfile(firstPath);
        } else if (strncmp(command, "chperm", 6) == 0) {
            chperm(firstPath);
        } else if(strncmp(command, "exit", 5) == 0){
            return 0;
        }else {
            printf("Unknown command.\n");
        }
        
        //Stops the program for 2 seconds to user to read the outputs
        sleep(2);
    }
    return 0;
}

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include "logger.h"
#include "file_operations.h"

//Function to create a file. First takes the directory then asks the user the file name seperatly.
void crtfile( const char* path) {

    struct stat pathStat;
    
    //Prints error if there anything wrong with the folder
    if (stat(path, &pathStat) == -1) {
        perror("Error opening directory");
        logger("crtfile",path,0,NULL);

        return;
    }

    //Takes the name of the file from user
    printf("Please enter the name of the file you want to create: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) != NULL) {
        // Remove trailing newline, if present
        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n') {
            filename[len - 1] = '\0';
        }
    }
    char fullPath[512];
    strcpy(fullPath,path);
    strcat(fullPath,"/");
    strcat(fullPath,filename);

    //Creates the file and prints error if there are any
    if (creat(fullPath, 0775) == -1) {
        perror("File creation failed");
        logger("crtfile",path,0,NULL);
        return;

    }
    logger("crtfile",path,1,NULL);

}

//Function to delete a file
void delfile(const char* path) {

    struct stat pathStat;

    //Prints error if there anything wrong with the folder
    if (stat(path, &pathStat) == -1) {
        perror("Error opening folder");
        logger("delfile",path,0,NULL);

        return;
    }

    //Check if path is a directory
    if (S_ISDIR(pathStat.st_mode)) {
        printf("Path is a directory.\n");
        logger("delfile", path, 0, NULL);
        return;
    }

    //Delete the file
    if (unlink(path) == 0) {
        printf("File '%s' successfully deleted.\n", path);
        logger("delfile",path,1,NULL);

    } else {
        perror("Error deleting file");
        logger("delfile",path,0,NULL);

    }
}

//Function to copy a file from a path to another. Checks the directories, creates a new file at destination, reads and writes the files, deletes old file.
void cpfile(const char *sourcePath, const char *destinationPath) {
    int source_fd, destination_fd;
    int BUFFER_SIZE = 4096;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    struct stat sourceStat, destStat;

    //Check if the source path is a directory
    if (stat(sourcePath, &sourceStat) == 0 && S_ISDIR(sourceStat.st_mode)) {
        printf("Source path is a directory.\n");
        logger("mvfile", sourcePath, 0, destinationPath);
        return;
    }

    //Check if the destination path exists and is a directory
    if (stat(destinationPath, &destStat) == 0 && S_ISDIR(destStat.st_mode)) {
        printf("Destination path is a directory.\n");
        logger("mvfile", sourcePath, 0, destinationPath);
        return;
    }

    //Open the source file for reading
    source_fd = open(sourcePath, O_RDONLY);
    if (source_fd == -1) {
        perror("Error opening source file");
        logger("cpfile",sourcePath,0,destinationPath);

        return ;
    }

    //Open/create the destination file for writing
    destination_fd = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destination_fd == -1) {
        perror("Error opening/creating destination file");
        logger("cpfile",sourcePath,0,destinationPath);

        close(source_fd);
        return;
    }

    //Read from source and write to destination
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(destination_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Error writing to destination file");
            logger("cpfile",sourcePath,0,destinationPath);

            close(source_fd);
            close(destination_fd);
            return;
        }
    }

    //Prints if there is an error while reading file 
    if (bytes_read == -1) {
        perror("Error reading from source file");
        logger("cpfile",sourcePath,0,destinationPath);

        close(source_fd);
        close(destination_fd);
        return;
    }

    //Close the file descriptors
    close(source_fd);
    close(destination_fd);

    logger("cpfile",sourcePath,1,destinationPath);
}

//Function to move a file from a path to another path
void mvfile(const char *sourcePath, const char *destinationPath) {

    struct stat sourceStat, destStat;

    //Check if the source path is a directory
    if (stat(sourcePath, &sourceStat) == 0 && S_ISDIR(sourceStat.st_mode)) {
        printf("Source path is a directory.\n");
        logger("mvfile", sourcePath, 0, destinationPath);
        return;
    }

    //Check if the destination is a directory
    if (stat(destinationPath, &destStat) == 0 && S_ISDIR(destStat.st_mode)) {
        printf("Destination path is a directory.\n");
        logger("mvfile", sourcePath, 0, destinationPath);
        return;
    }
    //Move the file
    if (rename(sourcePath, destinationPath) < 0) {
        perror("Error moving file");
        logger("mvfile",sourcePath,0,destinationPath);

        return;
    }

    logger("mvfile",sourcePath,1,destinationPath);

}

//Function that displays the contents of a file
void dpfile(const char *path) {
    const int BUFFER_SIZE = 1024;

    //Opens the file and prints error if there is any
    int file_descriptor = open(path, O_RDONLY);
    if (file_descriptor == -1) {
        perror("Error opening file");
        logger("dpfile",path,0,NULL);

        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    //Reads and writes to files
    while ((bytes_read = read(file_descriptor, buffer, BUFFER_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
            perror("Error writing file contents to console");
            logger("dpfile",path,0,NULL);

            close(file_descriptor);
            return;
        }
    }

    //Returns error if there is an error reading file
    if (bytes_read == -1) {
        perror("Error reading file");
        logger("dpfile",path,0,NULL);

    }

    //Close the file descriptor
    close(file_descriptor);

    logger("dpfile",path,1,NULL);

}


//Function to search for files containing the search term in their names
void srcfile(const char *path) {

    //Opens the path and returns if there is an error
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        logger("srcfile",path,0,NULL);

        return;
    }

    char search_term[256];

    //Asks for the search from the user
    printf("Enter the search term: ");

     if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        // Remove trailing newline, if present
        size_t len = strlen(search_term);
        if (len > 0 && search_term[len - 1] == '\n') {
            search_term[len - 1] = '\0';
        }
    }

    struct dirent *entry;

    printf("Searching for '%s' in directory: %s\n", search_term, path);
    bool isFound = 0;

    //Read entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        //Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        //Check if the search term is in the file/directory name
        if (strstr(entry->d_name, search_term) != NULL) {
            printf("Found: %s\n", entry->d_name);
            logger("srcfile",path,1,NULL);
            isFound= 1;
        }

    }

    //Prints not found if it can't find any result
    if(isFound==0)printf("Not Found\n");

    //Close the directory
    closedir(dir);
    return;
}




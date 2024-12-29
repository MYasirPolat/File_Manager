#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include "logger.h"
#include "directory_ops.h"
#include <string.h>
#include "file_operations.h"

//Function that creates a folder
void crtdir(const char* path) {
    if (mkdir(path, 0775) == -1) {
        perror("Error creating folder: ");
        logger("deldir",path,0,NULL);
        return;
    }
    logger("crtdir",path,1,NULL);

}

//Function that deletes a folder
void deldir(const char* path) {

    struct stat pathStat;

    //Prints error if there anything wrong with the folder
    if (stat(path, &pathStat) == -1) {
        perror("Error opening folder\n"); 
        logger("deldir",path,0,NULL);

        return;
    }

    //Return error if the path is not a directory
    if (!S_ISDIR(pathStat.st_mode)) {
        printf("Path is not a directory");
        logger("deldir",path,0,NULL);

        return;
    }

    //Open the folder
    DIR *dir = opendir(path);
    
    struct dirent *entry;
    int isEmpty = 1;

    //Check if the directory is empty and process entries
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            isEmpty = 0;
            break;
        }
    }

    //If the directory is not empty, warn the user and deletes if user requests.
    if (!isEmpty) {
        printf("%s is not empty. Do you still want to delete it? (y/N): ",path);
        char decision[3];
        if (fgets(decision, sizeof(decision), stdin) != NULL) {
            size_t len = strlen(decision);
            if (len > 0 && decision[len - 1] == '\n') {
                decision[len - 1] = '\0';
            }
        }

        if (decision[0] == 'n' || decision[0] == 'N') {
            logger("deldir", path, 0, NULL);
            closedir(dir);
            return;
        } 
        
        else if (decision[0] != 'y' && decision[0] != 'Y') {
            logger("deldir", path, 0, NULL);
            closedir(dir);
            return;
        }

       closedir(dir); //Close the current directory handle

        //Reopen the directory to process entries for deletion
        dir = opendir(path);
        if (dir == NULL) {
            perror("Error reopening directory");
            logger("deldir", path, 0, NULL);
            return;
        }

        //Delete the files
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char childPath[1024];
                snprintf(childPath, sizeof(childPath), "%s/%s", path, entry->d_name);

                struct stat childStat;
                if (stat(childPath, &childStat) == -1) {
                    perror("Error reading file");
                    continue;
                }

                if (S_ISDIR(childStat.st_mode)) {
                    deldir(childPath); //Recursive call for subfolder that are may or may not be empty
                } else {
                    if (unlink(childPath) == -1) {
                        perror("Error deleting file");
                    }
                }
            }
        }
    }
    
    if(rmdir(path)==-1){
        perror("Error deleting folder");
        logger("deldir",path,0,NULL); 
    }

    logger("deldir",path,1,NULL); 
    closedir(dir);
}

//Function to list the contents of a folder (Basically ls -la command) 
void slist(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("Error opening directory");
        logger("slist",path,0,NULL);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s", path, entry->d_name);

        struct stat pathStat;
        if (stat(filePath, &pathStat) == -1) {
            perror("stat");
            logger("slist",path,0,NULL);
            return;
        }

        //Print file type and permissions
        char perms[11] = "----------";
        if (S_ISDIR(pathStat.st_mode)) perms[0] = 'd';
        if (pathStat.st_mode & S_IRUSR) perms[1] = 'r';
        if (pathStat.st_mode & S_IWUSR) perms[2] = 'w';
        if (pathStat.st_mode & S_IXUSR) perms[3] = 'x';
        if (pathStat.st_mode & S_IRGRP) perms[4] = 'r';
        if (pathStat.st_mode & S_IWGRP) perms[5] = 'w';
        if (pathStat.st_mode & S_IXGRP) perms[6] = 'x';
        if (pathStat.st_mode & S_IROTH) perms[7] = 'r';
        if (pathStat.st_mode & S_IWOTH) perms[8] = 'w';
        if (pathStat.st_mode & S_IXOTH) perms[9] = 'x';
        printf("%s ", perms);

        //Number of hard links
        printf("%ld ", pathStat.st_nlink);

        //Owner and group name
        struct passwd *pwd = getpwuid(pathStat.st_uid);
        struct group *grp = getgrgid(pathStat.st_gid);
        if (pwd) {
            printf("%s ", pwd->pw_name);
        } else {
            printf("%d ", pathStat.st_uid);
        }

        if (grp) {
            printf("%s ", grp->gr_name);
        } else {
            printf("%d ", pathStat.st_gid);
        }

        //File size
        printf("%lld ", (long long)pathStat.st_size);

        //Last modification time
        char time_buf[256];
        struct tm *time_info = localtime(&pathStat.st_mtime);
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", time_info);
        printf("%s ", time_buf);

        //File name
        printf("%s\n", entry->d_name);
    }
    logger("slist",path,1,NULL);
    closedir(dir);
}
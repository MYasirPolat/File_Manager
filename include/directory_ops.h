#ifndef DIRECTORY_OPS_H
#define DIRECTORY_OPS_H

#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void crtdir(const char *path);   
void deldir(const char *path);   
void slist(const char *path);    

#endif 

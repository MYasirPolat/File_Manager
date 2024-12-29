#ifndef FILE_OPERATONS_H
#define FILE_OPERATONS_H

#include "logger.h"  

void crtfile(const char* path);
void delfile(const char* path);
void cpfile(const char *sourcePath, const char *destinationPath);
void mvfile(const char *sourcePath, const char *destinationPath);
void dpfile(const char *path);
void srcfile(const char *path);

#endif
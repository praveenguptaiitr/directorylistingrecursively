#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

#include "dirlistlogdef.h"

void listDir(char *filepath);
int isTxtFile(char *filename);

long int read_file(char* file);
void removeNonAlphaNumFromStr(char* buf);



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>

#define MAX_PATH_SIZE 1024
#define FILE_COLOR "\033[0m"
#define EXEC_COLOR "\033[32m"
#define DIR_COLOR "\033[34m"
#define HIDDEN_COLOR "\033[90m"
#define RESET_COLOR "\033[0m"

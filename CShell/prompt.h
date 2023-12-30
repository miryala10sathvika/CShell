#ifndef __PROMPT_H
#define __PROMPT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shellw.c"
#include "signalsfile.c"
#define MAX_COMMAND_LENGTH 100
#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_BLUE "\033[34m"
void prompt();

#endif

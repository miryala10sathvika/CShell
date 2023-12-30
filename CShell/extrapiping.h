#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#define MAX_CMD_LEN 512
void piping(char *args[],int argCount,char *home_directory);
int checkRedirection(char *command,char *home_directory);
void shellworking2(int argc, char *argv,char *home_directory,int input_pipe, int output_pipe);

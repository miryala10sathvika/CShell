#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include "prompt.c"
#include "warp.c"
#include "peek.c"
#include "pastevent.c"
#include "proclore.c"
#include "seek.c"
#include "activities.c"
#include "fgandbg.c"
#include "bg.c"
#include "extrapiping.c"
#include "redirect.c"
#include "neonate.c"
#include "iman.c"
#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64
#define MAX_BACKGROUND_PROCESSES 10

#endif
void piping(char *args[],int argCount,char *home_directory);
void peek(int argc, char *argv[],char *prev_dir,char *home_directory,int input_pipe, int output_pipe);
void shellworking(int argc, char *argv,char *home_directory);
void shellworking2(int argc, char *argv,char *home_directory,int input_pipe, int output_pipe);
void ping(int argCount, char *args[], char *homedirectory, int input_pipe, int output_pipe);
void warp(char *path, char *prev_dir, const char *home_directory, int input_pipe, int output_pipe);
void seekCommand(const char *search, const char *targetDir, bool isFile, bool isDir, bool isexec, int topLevel,int input_pipe, int output_pipe,int echeck);
void procloreCommand(pid_t pid,int input_pipe, int output_pipe);
void activities(int input_pipe, int output_pipe);
int neonate(int argc, char *argv[],int input_pipe,int output_pipe) ;
void iman(int argc, char *command_name, int input_pipe, int output_pipe);
void bringToForeground(int pid,int input_pipe, int output_pipe);
void changeToRunning(int pid,int input_pipe, int output_pipe);
void addCommand(char *command,  int numArgs);
void loadPastEventsFromFile() ;
void pastevent(int numArgs, char *args[],char *home_directory,int input_pipe, int output_pipe);
void executeCommand(char **args, bool background, char *command);
void displayEndedBackgroundProcesses();
int check_piping(char *command);
void implementPiping(char *command,char *home_directory);
int checkRedirection(char *command,char *home_directory);
void createBackGroundProcess(char *args, pid_t pid, int isRunning,int flag);
void removeLeadingTrailingSpaces(char *str) ;
void killall();
void remove_quotes(char *str);
#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64
#define MAX_BACKGROUND_PROCESSES 10

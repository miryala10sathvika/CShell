#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
int check_process(int id,int flags);
void del_process(int id);
void setforegroundpid(int pid);

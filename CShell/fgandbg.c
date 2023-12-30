#include "fgandbg.h"
void bringToForeground(int pid,int input_pipe, int output_pipe) {
   if (check_process(pid,1)){
   int status;
   signal(SIGTTIN, SIG_IGN);
   signal(SIGTTOU,SIG_IGN);
   tcsetpgrp(STDIN_FILENO,pid);
   setforegroundpid(pid);
   del_process(pid); 
   if (kill(pid, SIGCONT) == -1) {
        perror("Error bringing process to foreground");
    } else {
        printf("Bringing process %d to foreground...\n", pid);
        if (waitpid(-1, &status, WUNTRACED) == -1) {
            perror("Error waiting for process");
        } else {
            tcsetpgrp(STDIN_FILENO,getpgrp());
            signal(SIGTTIN,SIG_DFL);
            signal(SIGTTOU,SIG_DFL);
            if (WIFEXITED(status)) {
                printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Process %d terminated by signal %d\n", pid, WTERMSIG(status));
            }
        }
    }
   }
if (output_pipe != -1) {
        dup2(output_pipe, STDOUT_FILENO);
        close(output_pipe);
    }
    // Read from the input pipe if there is a valid input pipe
    if (input_pipe != -1) {
        dup2(input_pipe, STDIN_FILENO);
        close(input_pipe);
    }
}
void changeToRunning(int pid,int input_pipe, int output_pipe) {
	if (check_process(pid,1)){
	if (pid <= 0) {
		printf("Invalid process ID\n");
     } else{
     	kill(pid, SIGTTIN);  
    	if (kill(pid, SIGCONT) == -1) {
        	perror("Error changing process state to running");
    	} else {
        	printf("Changing process %d state to Running (in the background)...\n", pid);
   		 }
    	}
    }
    if (output_pipe != -1) {
        dup2(output_pipe, STDOUT_FILENO);
        close(output_pipe);
    }
    // Read from the input pipe if there is a valid input pipe
    if (input_pipe != -1) {
        dup2(input_pipe, STDIN_FILENO);
        close(input_pipe);
    }
}
            

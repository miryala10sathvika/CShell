#include "bg.h"
struct BackgroundProcess {
    pid_t pid;
    char command[1024];
    int is_running;
};
pid_t foreground_pid = -1;
char* foreground_name;
struct timeval start_time, end_time;
struct BackgroundProcess background_processes[10];
int background_process_count = 0;
void createBackGroundProcess(char *args, pid_t pid, int isRunning,int flag) {
    background_processes[background_process_count].pid = pid;
    if (flag==1){snprintf(background_processes[background_process_count].command, sizeof(background_processes[background_process_count].command), "%s", foreground_name);}
    else
    {snprintf(background_processes[background_process_count].command, sizeof(background_processes[background_process_count].command), "%s", args);}
    background_processes[background_process_count].is_running = isRunning;
    background_process_count++;
}

int getforegroundpid() {
    return foreground_pid;
}

void setforegroundpid(int pid) {
    foreground_pid = pid;
}
void executeCommand(char **args, bool background, char *command) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args[0], args);
        printf("ERROR: %s is not a valid command\n", args[0]);
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        if (!background) {
            foreground_pid = pid;
            foreground_name=command;
            gettimeofday(&start_time, NULL);
            int status;
            waitpid(pid, &status, 0);
            gettimeofday(&end_time, NULL);
            double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1e6;
            if (elapsed_time > 2.0) {
                printf("<%s : %.0f>", command, elapsed_time);
            }
        }
        if (background) {
            if (background_process_count < 10) {
                createBackGroundProcess(command, pid, 1,0);
                printf("%d\n", pid);
            } else {
                printf("Maximum background processes reached.\n");
            }
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    } else {
        perror("Forking failed");
    }
}

void displayEndedBackgroundProcesses() {
    int status;
    for (int i = 0; i < background_process_count; i++) {
        pid_t pid = background_processes[i].pid;
        if (waitpid(pid, &status, WNOHANG) == pid) {
            if (WIFEXITED(status)) {
                printf("%s exited normally.(%d)\n", background_processes[i].command, pid);
                del_process(pid);
            } else if (WIFSIGNALED(status)) {
                printf("%s exited abnormally.(%d)\n", background_processes[i].command, pid);
                del_process(pid);
            }

        }
    }
}

void del_process(pid_t id) { // Change the parameter type to pid_t
    int flag = 0;
        for (int i = 0; i <= background_process_count; i++) {
            if (background_processes[i].pid == id) {
                flag = 1;
                for (int j = i; j < background_process_count; j++)
                    background_processes[j] = background_processes[j + 1];
                background_process_count--;
            }
        }
    if (flag == 0 && id != -1)
        printf("Error: no such process found\n");
}
void killall() { 
	for (int i = 0; i <= background_process_count; i++) {
            kill(background_processes[i].pid,SIGKILL);
        }
	background_process_count = 0;
	printf("killed all background process\n");
}
int check_process(int id,int flags) { // Change the parameter type to pid_t
    int flag = 0;
    for (int i = 0; i <= background_process_count; i++) {
    	if (background_processes[i].pid == id && flags==1) {
            background_processes[i].is_running = flags;
        }
        if (background_processes[i].pid == id) {
            return 1;
        }
    }
    if (flag == 0 && id != -1)
        printf("Error: no such process found\n");
    return 0;
}
int compareProcesses(const void *a, const void *b) {
    return ((struct BackgroundProcess *)a)->pid - ((struct BackgroundProcess *)b)->pid;
}

void activities(int input_pipe, int output_pipe) {
    // Sort the background_processes array based on PID
    qsort(background_processes, background_process_count, sizeof(struct BackgroundProcess), compareProcesses);
    // Print the sorted list
    for (int i = 0; i < background_process_count; i++) {
        printf("%d : %s - %s\n", background_processes[i].pid, background_processes[i].command,
               background_processes[i].is_running ? "Running" : "Stopped");
    }
}


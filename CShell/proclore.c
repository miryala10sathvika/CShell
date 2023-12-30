#include "proclore.h"
void procloreCommand(pid_t pid,int input_pipe, int output_pipe) {
    char stat_file_path[256];
    snprintf(stat_file_path, sizeof(stat_file_path), "/proc/%d/stat", pid);

    FILE *stat_file = fopen(stat_file_path, "r");
    if (stat_file == NULL) {
        perror("Error opening stat file");
        return;
    }
    int proc_pid;
    char status;
    int pgrp;
    unsigned long vsize;
    char executable_path[1024];
    fscanf(stat_file, "%d %*s %c %d", &proc_pid, &status, &pgrp);
    for (int i = 0; i < 19; i++) {
        fscanf(stat_file, "%*d");
    }
    fscanf(stat_file, "%lu", &vsize);
    char exe_file_path[256];
    snprintf(exe_file_path, sizeof(exe_file_path), "/proc/%d/exe", pid);
    ssize_t len = readlink(exe_file_path, executable_path, sizeof(executable_path) - 1);
    if (len != -1) {
        executable_path[len] = '\0';
    }
    fclose(stat_file);
    //pid_t pgid = getpgid(pid);
    //if (pgid == fg_pgid) {       for other terminals
    //if (pgrp != getpgid(0)) {     //for user defined
    // pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);
    //pid_t console_pid = tcgetpgrp(STDOUT_FILENO);
	//pid_t my_pid = getpgrp()
    printf("pid : %d\n", proc_pid);
    printf("Process status : %c", status);
    if (status == 'R' || status == 'S' || status == 'Z') {
        if (pgrp != getpgid(0)) {
            printf("+\n");
        } else {
            printf("\n");
        }
    }
    printf("Process Group : %d\n", pgrp);
    printf("Virtual memory : %lu\n", vsize);
    printf("Executable path : %s\n", executable_path);
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

#include "signalsfile.h"
// Function declarations
void send_signal(int pid, int signal_num,int input_pipe, int output_pipe) {
    if (kill(pid, signal_num) == 0) {
        printf("Sent signal %d to process with PID %d\n", signal_num, pid);
    } else {
        perror("Error sending signal");
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
void ping(int argCount, char *args[], char *homedirectory, int input_pipe, int output_pipe) {
    if (argCount != 3) {
        printf("Invalid Command!\n");
        return;
    }
    int signal_num = atoi(args[2]);
    if (signal_num < 0) {
        printf("Invalid Signal Number\n");
        return;
    }
    int pid = atoi(args[1]);
    if (check_process(pid,0)) {
        send_signal(pid, signal_num % 32,input_pipe,output_pipe);
    } else {
        perror("No such Process not found");
        return ;
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


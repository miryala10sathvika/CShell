#include "activities.h"
// Struct to store process information
/*typedef struct {
    char pid[20];
    char cmd[MAX_PROCESS_NAME];
    char state;
} ProcessInfo;

// Comparison function for sorting by command name
int compareByCmdName(const void *a, const void *b) {
    return strcmp(((ProcessInfo *)a)->cmd, ((ProcessInfo *)b)->cmd);
}

// Function to list all child processes of the current process
void listChildProcesses(int parentPID,int input_pipe, int output_pipe) {
    DIR *procDir;
    struct dirent *entry;

    char parentPIDStr[20];
    snprintf(parentPIDStr, sizeof(parentPIDStr), "%d", parentPID);

    if ((procDir = opendir("/proc")) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    // Create an array to store process information
    ProcessInfo processInfoArray[256]; // Adjust the size as needed
    int numProcesses = 0;

    while ((entry = readdir(procDir)) != NULL) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
            char statusPath[MAX_PROCESS_NAME];
            char cmd[MAX_PROCESS_NAME];
            char state;
            snprintf(statusPath, sizeof(statusPath), "/proc/%s/status", entry->d_name);
            FILE *statusFile = fopen(statusPath, "r");
            if (statusFile) {
                char line[128];
                while (fgets(line, sizeof(line), statusFile)) {
                    if (strncmp(line, "Name:", 5) == 0) {
                        // Extract the command name
                        sscanf(line, "Name:\t%s", cmd);
                    }
                    if (strncmp(line, "PPid:", 5) == 0) {
                        int ppid = atoi(line + 6);
                        if (ppid == parentPID) {
                            fscanf(statusFile, "%*d %s %c", cmd, &state);
                            strcpy(processInfoArray[numProcesses].pid, entry->d_name);
                            strcpy(processInfoArray[numProcesses].cmd, cmd);
                            processInfoArray[numProcesses].state = state;
                            numProcesses++;
                            break;
                        }
                    }
                }
                fclose(statusFile);
            }
        }
    }

    closedir(procDir);

    // Sort the array by command name
    qsort(processInfoArray, numProcesses, sizeof(ProcessInfo), compareByCmdName);

    // Print the sorted process information
    for (int i = 0; i < numProcesses; i++) {
        printf("[%s] : %s - %s\n", processInfoArray[i].pid, processInfoArray[i].cmd,
               (processInfoArray[i].state == 'R') ? "Running" : "Stopped");
    }
    // Redirect output to the next command if there is a valid output pipe
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

void activities(int input_pipe, int output_pipe) {
    pid_t parentPID = getpid();
    listChildProcesses(parentPID,input_pipe, output_pipe);
}*/


#include "redirect.h"
int status;
int checkRedirection(char *command,char *home_directory){
int app=0;
	if ((strstr(command, ">>") != NULL || strstr(command, "<") != NULL || strstr(command, ">") != NULL)){
		if (strstr(command, ">>")!=NULL)
			app=1;
		redirection(command, app, home_directory);
		return 1;
	}
	return 0;
}
void redirection(char *command, int app, char *home_directory) {
    char *temp1[100000], *temp2[100000], *temp3[100000], i_file[100000], o_file[100000];
    strcpy(o_file, "");
    strcpy(i_file, "");
    int fds[4];
    for (int i = 0; i < 4; i++)
        fds[i] = -1;
    char delim[3];
    if (app == 1)
        strcpy(delim, ">>");
    else
        strcpy(delim, ">");
    char *com_list1[1000];
    int k = 0;
    char *arg = strtok(command, delim);
    while (arg != NULL) {
        com_list1[k++] = arg;
        arg = strtok(NULL, delim);
        removeLeadingTrailingTabs(com_list1[k-1]);
    }
    if (k == 2) {
        strcpy(o_file, com_list1[1]);
        com_list1[1] = strtok(o_file, " \t\n\r");
        strcpy(o_file, com_list1[1]);
        removeLeadingTrailingSpaces(o_file);
        removeLeadingTrailingTabs(o_file);
    }
    k = 0;
    char *com_list2[1000];
    arg = strtok(command, "<");
    while (arg != NULL) {
        com_list2[k++] = arg;
        removeLeadingTrailingTabs(com_list2[k-1]);
        arg = strtok(NULL, "<");
    }
    if (k == 2) {
        strcpy(i_file, com_list2[1]);
        com_list2[1] = strtok(i_file, " \t\n\r");
        strcpy(i_file, com_list2[1]);
        removeLeadingTrailingSpaces(i_file);
        removeLeadingTrailingTabs(i_file);
    }
    if (strcmp(i_file, "") != 0 && strcmp(o_file, "") == 0) {
        int oldin = dup(STDIN_FILENO);
        int pid = fork();
        fds[0] = open(i_file, O_RDONLY);
        if (fds[0] < 0) {
            perror("No such input file found!\n");
            return;
        }
        if (pid == 0 && !(fds[0] < 0)) {
            close(0); // Close stdin
            dup(fds[0]); // Redirect stdin to the input file
            close(fds[0]); // Close the file descriptor
            shellworking2(0, com_list2[0], home_directory, -1, oldin);
            dup2(oldin, 0); // Restore stdin
            exit(0);
        } else {
            while (wait(&status) != pid);
        }
    } else if (strcmp(o_file, "") != 0 && strcmp(i_file, "") == 0) {
        int oldout = dup(STDOUT_FILENO);
        int pid = fork();
        if (app == 0)
            fds[0] = open(o_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        else if (app == 1)
            fds[0] = open(o_file, O_APPEND | O_WRONLY | O_CREAT, 0644);
        if (fds[0] < 0) {
            perror("Failed to open output file!\n");
            return;
        }
        dup2(fds[0], 1);
        if (pid == 0) {
            close(fds[0]); // Close the file descriptor
            shellworking2(0, com_list1[0], home_directory, oldout, -1);
            exit(0);
        } else {
            while (wait(&status) != pid);
            dup2(oldout, 1); // Restore stdout
            close(fds[0]); // Close the file descriptor
        }
    } 
else {
    int oldin = dup(STDIN_FILENO);
    int oldout = dup(STDOUT_FILENO);
    int pid = fork();
    fds[0] = open(i_file, O_RDONLY);
    if (app == 0)
        fds[1] = open(o_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    else if (app == 1)
        fds[1] = open(o_file, O_APPEND | O_WRONLY | O_CREAT, 0644);
    if (fds[0] < 0 || fds[1] < 0) {
        perror("Failed to open input or output file!\n");
        return;
    }
    if (pid == 0) {
        dup2(fds[0], 0); // Redirect stdin to input file
        close(fds[0]); // Close the file descriptor
        dup2(fds[1], 1); // Redirect stdout to output file
        close(fds[1]); // Close the file descriptor
        shellworking2(0, com_list2[0], home_directory, oldout, oldin);
        dup2(oldout, 1); // Restore stdout
        dup2(oldin, 0); // Restore stdin
        exit(0);
    } else {
        close(fds[0]); // Close the file descriptor in the parent process
        close(fds[1]); // Close the file descriptor in the parent process
        while (wait(&status) != pid);
        dup2(oldout, 1); // Restore stdout
    }
}
}


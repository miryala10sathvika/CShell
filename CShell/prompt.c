#include "prompt.h"
void sigint_handler(int signum) ;
void sigtstp_handler(int signum);
void addSemicolonAfterAmpersand(char *input) {
    char result[1000];  // Adjust the size as needed
    int resultIndex = 0;

    for (int i = 0; i < strlen(input); i++) {
        result[resultIndex++] = input[i];
        
        if (input[i] == '&' && (i + 1 < strlen(input) && input[i + 1] != ';')) {
            result[resultIndex++] = ';';
        }   
    }

    result[resultIndex] = '\0';
    strcpy(input, result);
}
// Function to handle Ctrl-D (EOF) signal
void ctrl_d_handler() {
    killall();
    printf("\nCtrl-D pressed. Logging out of the shell...\n");
    exit(0);  // Exit the shell
}
void sigint_handler(int signum) {
    int foreground_pid = getforegroundpid();
    if (foreground_pid != -1) {
        kill(foreground_pid, SIGINT);
        prompt();
    } else {
        printf("No foreground process to interrupt\n");
        prompt();
    }
}
// Function to handle Ctrl+Z (SIGTSTP) signal
void sigtstp_handler(int signum) {
    int foreground_pid = getforegroundpid();
    if (foreground_pid != -1) {
        kill(foreground_pid, SIGTTIN);
    	kill(foreground_pid, SIGTSTP);
        createBackGroundProcess("", foreground_pid, 0,1);
        prompt();
    } else {
        printf("No foreground process to stop\n");
        prompt();
    }
}
void prompt(){
void getCurrentDirectory(char *buffer, size_t size) {
    if (getcwd(buffer, size) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
}
    struct passwd *pw;
    uid_t uid = geteuid();
    pw = getpwuid(uid);

    if (pw == NULL) {
        perror("getpwuid");
       	exit(1);
    }
    char home_directory[4096];
    getCurrentDirectory(home_directory, sizeof(home_directory));
    char hostname[MAX_COMMAND_LENGTH];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("gethostname");
        exit(1);
    }
    char cwd[4096];
    char cwd2[4096];
    struct sigaction sa_int, sa_tstp, sa_eof;
    sa_int.sa_handler = sigint_handler;
    sa_eof.sa_handler = ctrl_d_handler;
    sa_tstp.sa_handler = sigtstp_handler;
    sigemptyset(&sa_eof.sa_mask);
    sigemptyset(&sa_int.sa_mask);
    sa_eof.sa_flags = 0;
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    sigaction(SIGTSTP, &sa_tstp, NULL);
    while (1) {
    	displayEndedBackgroundProcesses();
        getCurrentDirectory(cwd, sizeof(cwd));
        // Replace home directory with '~'
        if (strncmp(cwd, home_directory, strlen(home_directory)) == 0 && strlen(home_directory) == strlen(cwd)) {
               strcpy(cwd, "~");
        } else if (strncmp(cwd, home_directory, strlen(home_directory)) == 0) {
               memmove(cwd, "~", 1);
               memmove(cwd + 1, cwd + strlen(home_directory), strlen(cwd) - strlen(home_directory) + 1);
        }
        printf("<"COLOR_GREEN "%s@"COLOR_BLUE "%s:"COLOR_RESET "%s> ", pw->pw_name, hostname, cwd);
        char command[MAX_COMMAND_LENGTH];
        if (fgets(command, sizeof(command), stdin) == NULL) {
        	ctrl_d_handler();
        }
        if (strncmp(command," ",strlen(" "))==0 || strncmp(command,"\t",strlen("\t"))==0){continue;}
        command[strlen(command) - 1] = '\0'; 
        addSemicolonAfterAmpersand(command);
        //printf("%s",command);
        shellworking(strlen(command),command,home_directory);
    }
}

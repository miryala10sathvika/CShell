#include "neonate.h"
void die(const char *s) {
    perror(s);
    exit(1);
}
struct termios orig_termios;
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
int getMostRecentPID() {
    int mostRecentPID = -1;
    struct dirent *entry;
    DIR *dp = opendir("/proc");

    if (dp == NULL) {
        perror("opendir");
        return mostRecentPID;
    }

    while ((entry = readdir(dp))) {
        if (isdigit(entry->d_name[0])) {
            int pid = atoi(entry->d_name);
            if (pid > mostRecentPID) {
                mostRecentPID = pid;
            }
        }
    }

    closedir(dp);
    return mostRecentPID;
}
bool isInteger(const char *str) {
    int len = strlen(str);
    int decimalPointCount = 0;

    for (int i = 0; i < len; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            continue; // Digit
        } else if (str[i] == '.') {
            decimalPointCount++;
            if (decimalPointCount > 1) {
                return false; // More than one decimal point
            }
        } else if (str[i] == '-' && i == 0) {
            continue; // Negative sign at the beginning
        } else {
            return false; // Invalid character
        }
    }

    return true;
}
int neonate(int argc, char *argv[],int input_pipe,int output_pipe) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [time_arg]\n", argv[0]);
        return 1;
    }
    int time_arg = atoi(argv[2]);
    char *endptr1;
    long intResult = strtol(argv[2], &endptr1, 10);
    if (*endptr1 == '\0'){
    if (time_arg <= 0) {
        fprintf(stderr, "Invalid time_arg: %s\n", argv[2]);
        return 1;
    }
    }
    else{fprintf(stderr, "Invalid time_arg: %s\n", argv[2]);
        return 1;}

    enableRawMode();

    char inputBuffer[MAXINPUTLENGTHN];
    int inputLength = 0;
    bool printPID = true;

    struct timeval lastPrintTime;
    gettimeofday(&lastPrintTime, NULL);

    int recentPID = -1; // Initialize with an invalid PID

    while (1) {
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);

        if (printPID && (currentTime.tv_sec - lastPrintTime.tv_sec) >= time_arg) {
            recentPID = getMostRecentPID();
            printf("Most Recently Created PID: %d\n", recentPID);
            fflush(stdout);
            lastPrintTime = currentTime;
        }

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int ready = select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout);
        if (ready == -1) {
            perror("select");
            break;
        } else if (ready > 0) {
            char c;
            if (read(STDIN_FILENO, &c, 1) == 1) {
                if (c == 'x') {
                    break;
                }
            }
        }
    }

    disableRawMode();
    printf("Exiting...\n");
    if (output_pipe != -1) {
        dup2(output_pipe, STDOUT_FILENO);
        close(output_pipe);
    }
    // Read from the input pipe if there is a valid input pipe
    if (input_pipe != -1) {
        dup2(input_pipe, STDIN_FILENO);
        close(input_pipe);
    }
    return 1;
}


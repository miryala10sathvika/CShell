#include "seek.h"
void seekCommand(const char *search, const char *targetDir, bool isFile, bool isDir, bool isexec, int topLevel,int input_pipe, int output_pipe,int echeck) {
    DIR *dir = opendir(targetDir);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }
    struct dirent *entry;
    int found = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {  // Exclude hidden files/dirs
            int isMatching = 0;
            if ((isFile && entry->d_type == DT_REG) || (isDir && entry->d_type == DT_DIR)) {
                char str[200];
                strcpy(str, entry->d_name);
                char *dotPosition = strchr(str, '.');
                if (dotPosition != NULL) {
                    *dotPosition = '\0';
                }
                if (strcmp(str, search) == 0) {
                    echeck+=1;
                    if (entry->d_type == DT_DIR) {
                        printf(ANSI_COLOR_BLUE "%s/%s\n" ANSI_COLOR_RESET, targetDir, entry->d_name);
                        char nextPath[1024];
                	snprintf(nextPath, sizeof(nextPath), "%s/%s", targetDir, entry->d_name);
                	seekCommand(search, nextPath, isFile, isDir, isexec, false,input_pipe, output_pipe,echeck);
                    } else {
                        printf(ANSI_COLOR_GREEN "%s/%s\n" ANSI_COLOR_RESET, targetDir, entry->d_name);
                    }
                    isMatching = 1;
                    found += 1;
                    char cwd[1024];
                    getcwd(cwd, sizeof(cwd));
                    //chdir(targetDir);
                    if (isexec && entry->d_type == DT_DIR && echeck==1) {
                        chdir(targetDir);
                        if (access(entry->d_name, X_OK) == 0) {
                            chdir(entry->d_name);
                            printf("Changed working directory to %s\n", entry->d_name);
                            closedir(dir);
                            return;
                        } else {
                            printf("Missing permissions for task!\n");
                        }
                    }
                    if (isexec && entry->d_type == DT_REG && echeck==1) {
                        // Print content of the file
                        chdir(targetDir);
                        FILE *file = fopen(entry->d_name, "r");
                        if (file != NULL) {
                            char buffer[1024];
                            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                                printf("%s", buffer);
                            }
                            fclose(file);
                            closedir(dir);
                            chdir(cwd);
                            return;
                        } else {
                            printf("Error opening file!\n");
                        }
                    }
                }
            }

            if (!isMatching && entry->d_type == DT_DIR) {
                char nextPath[1024];
                snprintf(nextPath, sizeof(nextPath), "%s/%s", targetDir, entry->d_name);
                seekCommand(search, nextPath, isFile, isDir, isexec, false,input_pipe,output_pipe,echeck);
            }
        }
    }

    closedir(dir);

    if (topLevel && !found) {
        printf("No match found!\n");
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


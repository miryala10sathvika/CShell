#include "warp.h"
void warp(char *path, char *prev_dir, const char *home_directory, int input_pipe, int output_pipe) {
    int flag=1;
    char current_dir[MAX_INPUT_SIZE];
    char new_dir[MAX_INPUT_SIZE];
    char previ_dir[MAX_INPUT_SIZE];
    // Save the current working directory
    getcwd(previ_dir, sizeof(previ_dir));
    // Determine the new directory
    if (path[0] == '~') {
        strcpy(new_dir, home_directory);
        if (strncmp(path, "~/", 2) == 0) {
            strcpy(new_dir, home_directory);
            char path2[MAX_INPUT_SIZE];
            if (strlen(path) >= 2) {
                strcpy(path2, path + 1);
            }
            strcat(new_dir, path2);
            
        } 
    } else if (path[0] == '-') {
        strcpy(new_dir, prev_dir);
        if (strlen(new_dir)==0){strcpy(new_dir, home_directory);
        	printf("OLDPWD not set\n");
        	flag=0;
        }
    } else if (strcmp(path, ".") == 0) {
        strcpy(new_dir, previ_dir);  
        
        // Use the previous directory
        if (strncmp(path, "./", 2) == 0) {
            char path2[MAX_INPUT_SIZE];
            if (strlen(path) >= 2) {
                strcpy(path2, path + 1);
            }
            strcat(new_dir, path2);
            
        }
    } else if (strcmp(path, "..") == 0) {
        if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
            perror("getcwd");
            return;
        }
        char *last_slash = strrchr(current_dir, '/');
        if (last_slash != NULL) {
            *last_slash = '\0';
        } else {
            fprintf(stderr, "Cannot go up from root directory.\n");
            return;
        }
        strcpy(new_dir, current_dir);
        if (strncmp(path, "../", 3) == 0) {
            char path2[MAX_INPUT_SIZE];
            if (strlen(path) >= 2) {
                strcpy(path2, path + 1);
            }
            strcat(new_dir, path2);
            
        }
    } else {
        if (realpath(path, new_dir) == NULL) {
            perror("realpath");
            return;
        }
    }
    // Change the directory
    if (chdir(new_dir) == 0) {
        printf("%s\n", new_dir);
        if (flag){
        strcpy(prev_dir, previ_dir);}
    } else {
        perror("chdir");
        return;
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


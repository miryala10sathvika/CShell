#include "extrapiping.h"
int check_piping(char *command){
	if (strstr(command, "|") != NULL){
		return 1;
	}
	return 0;
}
void removeLeadingTrailingSpaces(char *str) {
    int len = strlen(str);
    // Remove leading spaces and tabs
    int start = 0;
    while (isspace(str[start])) {
        start++;
    }
    // Remove trailing spaces and tabs
    int end = len - 1;
    while (end >= 0 && isspace(str[end])) {
        end--;
    }
    // Shift the non-space part of the string to the beginning
    int length = end - start + 1;
    memmove(str, str + start, length);
    str[length] = '\0';
}
void removeLeadingTrailingTabs(char *str) {
    int len = strlen(str);
    
    // Remove leading tabs
    int start = 0;
    while (str[start] == '\t') {
        start++;
    }

    // Remove trailing tabs
    int end = len - 1;
    while (end >= 0 && str[end] == '\t') {
        end--;
    }

    // Shift the non-tab part of the string to the beginning
    int length = end - start + 1;
    memmove(str, str + start, length);
    str[length] = '\0';
}
void implementPiping(char *command,char *home_directory){
	char *args[MAX_ARGS];
        int argCount = 0;
        char *arg = strtok(command, "|");
        if (arg == NULL) {
        printf("Error: No command before pipe\n");
        return;
    }
        while (arg != NULL) {
            args[argCount++] = arg;
            //printf("%s 1 in piping\n",args[argCount-1]);
            removeLeadingTrailingSpaces(args[argCount-1]);
            arg = strtok(NULL, "|");
        }
        if (argCount < 2 || args[argCount - 1] == NULL) {
        printf("Error: No command after pipe\n");
        return;
    }
        removeLeadingTrailingSpaces(args[argCount-1]);
        piping(args,argCount,home_directory);
}
bool is_quoted(char c) {
    return (c == '"' || c == '\'');
}
void remove_quotes(char *str) {
    int len = strlen(str);
    // Remove leading spaces and tabs
    int start = 0;
    while (str[start] == '\'' && str[start] == '"') {
        start++;
    }
    // Remove trailing spaces and tabs
    int end = len - 1;
    while (end >= 0 && str[end] == '\'' && str[end] == '"') {
        end--;
    }
    // Shift the non-space part of the string to the beginning
    int length = end - start + 1;
    memmove(str, str + start, length);
    str[length] = '\0';
}
void piping(char *args[],int argCount,char *home_directory)
{
    int pipingflag = 1;
    int mypipe[2], mypipe2[2], inp = 0, outp = 0;
    char *temp[10000], command[10000], tempo[10000];
    for (int i = 0; i < argCount; i++)
    {
        strcpy(command, args[i]);
        int oldout, oldin, ofd, ifd;
        bool inside_quotes = false;
        int c = 0;
        int token_start = 0;
        for (int j = 0; command[j] != '\0'; j++) {
            if (is_quoted(command[j])) {
                inside_quotes = !inside_quotes;
            }
            if (!inside_quotes && (command[j] == ' ' || command[j] == '\t' || command[j] == '\n' || command[j] == '\r')) {
                command[j] = '\0';
                remove_quotes(&command[token_start]);
                temp[c++] = &command[token_start];
                token_start = j + 1;
            }
        }
        temp[c++] = &command[token_start];
        remove_quotes(&command[token_start]);
        temp[c] = NULL;  
        if (i % 2 != 0)
        {
            if (pipe(mypipe2) ==-1)
                perror("Error: pipe could not be created\n");
        }
        else{
            if (pipe(mypipe)==-1)
                perror("Error: pipe could not be created\n");
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            if (i == 0)
            {
                dup2(mypipe[1], 1); // output to pipe
                close(mypipe[0]);   //closing input end of pipe
                strcpy(tempo, args[i]);
            }
            else if (i == argCount - 1)
            {
                if (i % 2 == 1)
                {
                    // printf("came into odd end\n");
                    dup2(mypipe[0], 0); //Input from pipe
                }
                else
                {
                    // printf("came into even end\n");
                    dup2(mypipe2[0], 0); //input from transit
                }

            }
            else if (i % 2 == 0)
            {
                // printf("came into even middle\n");
                dup2(mypipe2[0], 0); //input from transit
                close(mypipe[0]);    // close input end of pipe
                dup2(mypipe[1], 1);  // output to pipe
            }
            else if (i % 2 == 1)
            {
                // printf("came into odd middle\n");
                dup2(mypipe[0], 0);  //input from pipe
                close(mypipe[1]);    // close output end of pipe
                dup2(mypipe2[1], 1); // output to transit
            }
            strcpy(tempo, args[i]);
            int r = checkRedirection(tempo,home_directory);
            	shellworking2(0, args[i], home_directory,mypipe[0], mypipe2[1]);
            	if(r==0){
                    exit(0);
                }
             /*int z = execvp(temp[0], temp); // exec
                if (z < 0)
                    perror("Error: command not found\n");*/
                
                // Execute other commands as usual 
            exit(0);
        }
        else
        {
            wait(NULL);
            if (i == 0)
            {
                close(mypipe[1]);
                if (inp == 1)
                {
                    dup2(oldin, 0);
                    close(ifd);
                }
            }
            else if (i == argCount - 1)
            {
                if (i % 2 == 0)
                    close(mypipe2[0]);
                else
                    close(mypipe[0]);

                if (outp == 1)
                {
                    dup2(oldout, 1);
                    close(ofd);
                }
            }
            else if (i % 2 == 0)
            {
                close(mypipe2[0]);
                close(mypipe[1]);
            }
            else if (i % 2 == 1)
            {
                close(mypipe[0]);
                close(mypipe2[1]);
            }
        }
    }
}

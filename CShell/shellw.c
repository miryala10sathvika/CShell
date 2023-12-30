#include "headers.h"
void addCommand(char *command, int numArgs);
struct timeval start_time, end_time;
char prev_dir[MAX_INPUT_LENGTH];
bool background = false;
void shellworking2(int argc, char *argv,char *home_directory,int input_pipe, int output_pipe) {
        char *args[MAX_ARGS];
        char* saveptr2;
        int argCount = 0;
        removeLeadingTrailingSpaces(argv);
        //printf("%s 1",argv);
        char *arg = strtok_r(argv, " \t\n",&saveptr2);
        while (arg != NULL) {
            args[argCount++] = arg;
            //printf("%s inside\n",args[argCount-1]);
            removeLeadingTrailingSpaces(args[argCount-1]);
            remove_quotes(args[argCount-1]);
            arg = strtok_r(NULL, " \t\n",&saveptr2);
        }
        args[argCount] = NULL;
        //for (int i = 0; i < argCount; i++) {printf("%s shellworking2\n",args[i]);}
        if (argCount > 0) {
            if (strcmp(args[0], "warp") == 0) {
                if (argCount == 1) {
                    warp("~", prev_dir,home_directory,input_pipe,output_pipe);
                } else {
                    for (int i = 1; i < argCount; i++) {
                        if (strncmp(args[i], ";", 1) == 0 || strncmp(args[i], "|", 1) == 0) 
                        	break;
                        warp(args[i], prev_dir,home_directory,input_pipe,output_pipe);
                    }
                }
            } 
    else if (strcmp(args[0], "seek") == 0) {
    bool searchFiles = true;
    bool searchDirectories = true;
    bool isExecute = false;
    char *search = NULL;
    char *targetDir = "."; // Default target directory is current directory
    if (argCount < 2) {
        printf("Usage: seek [-d | -f | -e] <search> [target_directory]\n");
        return ;
}
    for (int i = 1; i < argCount; i++) {
        if (strstr(args[i], "-d") != NULL) {
            searchFiles = false;
        } else if (strstr(args[i], "-f") != NULL) {
            searchDirectories = false;
        } else if (strstr(args[i], "-e") != NULL) {
            isExecute = true;
        } else {
            if (search == NULL) {
                search = args[i];
                continue;
            } else if (search!=NULL) {
                targetDir = args[i];
                if (strcmp(targetDir,"~")==0){strcpy(targetDir,home_directory);}
            }
        }
        if (searchFiles==false && searchDirectories==false) {printf("Invalid Flags\n");return ;}
    }
       seekCommand(search, targetDir, searchFiles, searchDirectories, isExecute, 1,input_pipe, output_pipe,0);
        }
            else if (strcmp(args[0], "proclore") == 0) {
            if (argCount == 1) {
                pid_t shell_pid = getpid();
                procloreCommand(shell_pid,input_pipe, output_pipe);
            } else if (argCount == 2) {
                pid_t pid = atoi(args[1]);
                procloreCommand(pid,input_pipe, output_pipe);
            } else {
                printf("Usage: proclore [pid]\n");
            }
        } else if (strcmp(args[0], "peek") == 0) {
                peek(argCount, args,prev_dir,home_directory,input_pipe, output_pipe);
            }
            else if (strcmp(args[0], "ping") == 0) {
                ping(argCount,args,home_directory,input_pipe, output_pipe);
            }
            else if (strcmp(args[0], "activities") == 0) {
                activities(input_pipe, output_pipe);
            }
            else if ((strcmp(args[0], "neonate") == 0) && (strcmp(args[1], "-n") == 0)) {
                neonate(argCount,args,input_pipe,output_pipe);
            }
            else if (strcmp(args[0], "iMan") == 0) {
                iman(argCount,args[1],input_pipe,output_pipe);
            }
            else if (strcmp(args[0], "fg") == 0) {
                bringToForeground(atoi(args[1]),input_pipe, output_pipe);
            }
            else if (strcmp(args[0], "bg") == 0) {
                changeToRunning(atoi(args[1]),input_pipe, output_pipe);
            } 
            else if (strcmp(args[0], "pastevents") == 0) {
                pastevent(argCount, args,home_directory,input_pipe, output_pipe);
            } else if (strcmp(args[argCount - 1], "&") == 0) {
                background = true;
                args[argCount - 1] = NULL; // Remove the '&' from args
            }
            else{
            executeCommand(args, background,argv);}
            if (background) {
                executeCommand(args, background,argv);
                background=false;
            }
        } 
        return ;
    }
void shellworking(int argc, char *argv,char *home_directory) {
    char* saveptr1;
    //printf("%s arging 1\n",argv);
    addCommand(argv, argc);
    int f=0;
    //printf("%s arging\n",argv);
    char *com_list[MAX_ARGS];
    int comCount = 0;
    char *command = strtok_r(argv, ";",&saveptr1);
    while (command != NULL) {
    	com_list[comCount++] = command;
    	if (strncmp(com_list[comCount-1]," ",strlen(com_list[comCount-1]))==0 ||strncmp(com_list[comCount-1],"	",strlen(com_list[comCount-1]))==0){comCount--;}
    	//printf("%s 1 in strtok_r\n",com_list[comCount-1]);
        command = strtok_r(NULL, ";",&saveptr1);
        removeLeadingTrailingSpaces(com_list[comCount-1]);
        //printf("%s 2 in strtok_r\n",com_list[comCount-1]);
    }
    com_list[comCount]=NULL;
    for(int i=0; i<comCount; i++) {
    	displayEndedBackgroundProcesses();// printed when prompt
    	//printf("%s 1\n",com_list[i]);
        if (check_piping(com_list[i])){
        	implementPiping(com_list[i],home_directory);
        	//printf("%s 2\n",com_list[i]);
        }
        else{
		char argv_copy[MAX_INPUT_LENGTH]; // Create a copy of the command
		//printf("%s 3\n",com_list[i]);
		strcpy(argv_copy, com_list[i]); // Copy the command to the new buffer
		int r = 0;
		r = checkRedirection(argv_copy, home_directory); // Pass the copy to checkRedirection
		if (r == 0) {
    		shellworking2(0, com_list[i], home_directory, -1, -1);
    		//printf("%s 4\n",com_list[i]);
		}

        }
}
	return ;
}


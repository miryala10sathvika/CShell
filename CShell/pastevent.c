#include "pastevent.h"
void shellworking(int argc,char *argv,char *home_directory);
// Data structure to store a command and its arguments
int countDigits(int number) {
    int count = 0;
    if (number == 0) {
        return 1; // Special case for handling zero
    }
    while (number != 0) {
        number /= 10;
        count++;
    }
    return count;
}

typedef struct {
    char command[MAXI_COMMAND_LENGTH];
    int numArgs;
} Command;
Command pastEvents[MAXI_COMMANDS];
int numPastEvents = 0;
void replacePastEvents(char *input) {
    char *ptr = input;
    char modifiedCommand[MAXI_COMMAND_LENGTH];
    while ((ptr = strstr(ptr, "pastevents execute")) != NULL) {
        //printf("%s arging pastevents replacing inside\n", ptr);
        int index;
        if (sscanf(ptr, "pastevents execute %d", &index) == 1) {
            //printf("%d index \n", index);
            if (index >= 1 && index <= numPastEvents) {
                strcpy(modifiedCommand, pastEvents[numPastEvents - index].command);
                // Calculate the length of the prefix "pastevents execute <index>"
                int prefixLength = strlen("pastevents execute") + countDigits(index) + 1;
                // Calculate the length of the replacement command
                int replacementLength = strlen(modifiedCommand);

                // Copy the modified command back to the input
                strcpy(ptr, modifiedCommand);
                // Move the pointer past the replaced command
                ptr += replacementLength;
                //printf("%s  replace inside pastevents\n", modifiedCommand);
            } else {
                ptr += strlen("pastevents execute");
            }
        } else {
            ptr += strlen("pastevents execute");
        }
    }
}

bool isSameAsPrevious(char *command) {
    if (numPastEvents > 0 ){
    for (int i=0 ;i<=numPastEvents;i++){
     if (strcmp(pastEvents[numPastEvents - i].command, command) == 0) {
        return true;
    }}}
    return false;
}
// Function to add a command to past events
void addCommand(char *command,  int numArgs) {
    if (((strstr(command, "pastevents")!=NULL) && (strstr(command, "execute")==NULL))|| (strstr(command, "pastevents purge")!=NULL)){return ;}
    if (strncmp(command," ",strlen(command))==0){return ;}
    if (numPastEvents >= MAXI_COMMANDS) {
        // Remove the oldest command
        for (int i = 0; i < numPastEvents - 1; ++i) {
            strcpy(pastEvents[i].command, pastEvents[i + 1].command);
            pastEvents[i].numArgs = pastEvents[i + 1].numArgs;
        }
        --numPastEvents;
    }
    if (strstr(command,"pastevents")!=NULL){
    	 //printf("%s arging pastevents\n",command);
         replacePastEvents(command);
         //printf("%s arging pastevents replace\n",command);
    }
    if (!isSameAsPrevious(command)) {
        // Add the new command to the end
        //printf("%s same\n",command);
        strcpy(pastEvents[numPastEvents].command, command);
        pastEvents[numPastEvents].numArgs = numArgs;
        ++numPastEvents;
        savePastEventsToFile();
    }
}
void savePastEventsToFile() {
    FILE *file = fopen("past_events.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < numPastEvents; i++) {
        fprintf(file, "%s\n", pastEvents[i].command);
    }
    fprintf(file, "%s\n", "exit");
    fclose(file);
}

void loadPastEventsFromFile() {
    FILE *file = fopen("past_events.txt", "r");
    if (file == NULL) {
        return;
    }

    char line[MAXI_COMMAND_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        addCommand(line, 0);  // Assuming numArgs is not used for loading
    }

    fclose(file);
}
void printpastevents(int input_pipe, int output_pipe) {
    for (int i = 0; i < numPastEvents; i++) {
        printf("%s\n", pastEvents[i].command);
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
void pastevent(int numArgs, char *args[],char *home_directory,int input_pipe, int output_pipe) {
    if (numArgs >= 3 && strcmp(args[1], "execute") == 0) {
        int index = atoi(args[2]);
        if (index >= 1 && index <= numPastEvents) {
            //printf("%s",pastEvents[numPastEvents - index].command);
            shellworking2(pastEvents[numPastEvents - index-1].numArgs, pastEvents[numPastEvents - index-1].command, home_directory, input_pipe, output_pipe);
        } else {
            printf("Invalid index\n");
        }
    } else if (numArgs == 2 && strcmp(args[1], "purge") == 0) {
        numPastEvents = 0;
        remove("past_events.txt");
        for (int i = 0; i < MAXI_COMMANDS; ++i) {
            pastEvents[i].command[0] = '\0';
            pastEvents[i].numArgs = 0;
        }
    }
    else if (numArgs == 1 && strcmp(args[0], "pastevents") == 0) {
        printpastevents(input_pipe, output_pipe);
    } else {
        printf("Invalid pastevents command\n");
    }
}


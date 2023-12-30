# Custom Shell Program

## Overview

This C program implements a simple custom shell that allows users to input commands and execute them. The program displays a prompt, accepts user input, and then executes the specified command using system calls.

## How it Works

1. The program includes the necessary headers, including `headers.h` and `prompt.c`, to ensure proper functionality.

2. The `main` function serves as the entry point of the program. It enters an infinite loop that continuously displays a custom shell prompt and waits for user input.

3. Upon receiving user input, the `prompt` function is called to process and execute the entered command. This function handles the execution of commands using system calls.

4. The shell supports executing various commands, system utilities, and external programs just like a typical shell.

## Assumptions

1. The program assumes that the user is familiar with basic shell usage and the syntax of shell commands.

2. The headers and code files included (`headers.h` and `prompt.c`) are assumed to be available and correctly implemented.

3. The program assumes a basic level of error handling for user input and command execution. More advanced error handling can be added as needed.

# Custom Shell Program

## Overview

This C program implements a custom shell that provides basic command execution functionality similar to a Unix-like shell. The shell allows users to enter commands, execute them, and manage background processes. Additionally, it features a history mechanism to track and execute past commands.

## Features

1. **Command Execution**: Users can enter commands just like in a regular shell. The program executes the commands and displays their output.

2. **Background Processes**: Commands suffixed with '&' are executed in the background, allowing users to continue entering commands while a process runs in the background.

3. **History Tracking**: The program keeps track of past executed commands and provides a way to execute them again.

4. **Foreground Process Timer**: If a foreground process takes more than 2 seconds to execute, the program displays the time taken in seconds.

5. **Error Handling**: The program displays an error message if a command is not recognized or if an invalid option is used.

6. **Process Monitoring**: The program displays notifications when background processes exit, indicating whether they exited normally or abnormally.

## Assumptions

1. The program assumes basic familiarity with shell commands and syntax."-" is not assumed to be relative path."" are not handled.

2. The `headers.h` file contains the required definitions and includes.

3. The shell supports basic commands and options but does not include advanced features like redirection, piping, or complex scripting.

4. The program may need to be adapted or extended for specific use cases or requirements.

5. There is no infinite loops between pastevents.Pastevents background processes are omitted

6. Relative path is to the directory where shell invoked or to the directory given is printed.Warps only to directories folders not files.Peek lexicographic order is in form of ascii data form first 		 capital then small.

7. No more than 10 Background processes are considered.FG after total execution of command the total time is printed.

8. Assumes that user has a basic knowledge of every command they are using with syntax

9. Specific fail cases are only prompted on the screen.

10. Some times shell may not give desired output. please restart the shell or try for two to three times

11. seek with only one file as output is executed without -e flag

12. Userdefined warp , peek commands are not executed background.

13. due to variations in different os and systems colors of the text may vary.

14. Due to varitions of different os and systems output may be different.

15. ">" ,"<" are not usable.Storing in pastevents during & it is modified.

16. During printing or reading files errors may be invoked as reading permissions for the file are not given.

17. Some system Calls may not be executed. As this is primitive shell.

18. File inside a File inside a File with all same names present are not considered 

19. Some times although the seek shows output but also shows no match found please ignore it.

20. Other than user specifications other functionalities does not support "~" ,".".

21. peek here describes month in numbers day  time.

22. ls and cd are not implemented.

23. If command in pipeline fails irrespective of that others are executed.

24. Fg and Bg assumes always a pid is sent by the user.

25. SED is only applicable when execvp is used due to its "".Neonate timeslice are sent as integers (flaots are converted to integers)

26. When warp is used as in pipe and redirection changes does not reflect in the command prompt as it is parent while changes are done in child however the changed directory in the child is carry forwarded to parent.Warp arguments doent have gaps.S

27. Multiple redirections are not possible.Redirection is given more priority than piping.

28. Counts " ,','\0' and ascii colors characters wc.

29. Sorting lexigraphical order says that first capitals lexical and small lexical.

30. Echo prints " " and wc also counts null character.

31. Pid in somecases it is used as int instead of its own pid_t and in ping only numbers are expected not words.

32. Ctrl-C and Ctrl-Z are not offenly pressed and are not for userdefined. 

33. Abnormal Signal sending printly terminated abnormally background process.In seek only -d ,-e ,-f are allowed.

34. '-' is not sent at first to warp and to peek its not sent.In proclore absolute path is defined as executable path.

## Notes

- This shell is a simplified implementation and may not include all features of a full-fledged shell.

- Refer to the source code comments for detailed explanations of individual functions and logic.

- Some times during using seek function ouput maybe incorect without using flags and paths.Please use apt flags and path and retry.
- sometimes creating xxt file
VIM ,emacs,gedit???seek relative path changing printing??activities??piping with redirection??popen


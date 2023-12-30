/*
void seekCommand(const char *search, const char *targetDir, bool isFile, bool isDir, bool isexec, int topLevel) {
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
                if (strcmp(entry->d_name, search) == 0) {
                    printf("%s/%s\n", targetDir, entry->d_name);
                    isMatching = 1;
                    found += 1;

                    if (isexec && entry->d_type == DT_DIR) {
                        if (access(entry->d_name, X_OK) == 0) {
                            chdir(entry->d_name);
                            printf("Changed working directory to %s\n", entry->d_name);
                        } else {
                            printf("Missing permissions for task!\n");
                        }
                    }
                }
            }

            if (!isMatching && entry->d_type == DT_DIR) {
                char nextPath[1024]; // Increased buffer size
                snprintf(nextPath, sizeof(nextPath), "%s/%s", targetDir, entry->d_name);
                seekCommand(search, nextPath, isFile, isDir, isexec, false);
            }
        }
    }

    closedir(dir);

    if (topLevel && !found) {
        // Check without extension
        char *ext = strrchr(search, '.');
        if (ext == NULL) {
            ext = (char *)search + strlen(search);
        }

        dir = opendir(targetDir);
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_name[0] != '.') {
                    if ((isFile && entry->d_type == DT_REG) || (isDir && entry->d_type == DT_DIR)) {
                        if (strcmp(ext, entry->d_name) == 0) {
                            printf("%s/%s\n", targetDir, entry->d_name);
                            found += 1;

                            if (isexec && entry->d_type == DT_DIR) {
                                if (access(entry->d_name, X_OK) == 0) {
                                    chdir(entry->d_name);
                                    printf("Changed working directory to %s\n", entry->d_name);
                                } else {
                                    printf("Missing permissions for task!\n");
                                }
                            }
                        }
                    }
                }
            }
            closedir(dir);
        }

        if (topLevel && !found) {
            printf("No match found!\n");
        }
    }
}





bool searchFiles = true;
            bool searchDirectories = true;
            bool executeFlag = false;
            for (int i = 1; i < argCount; i++) {
                if (strcmp(args[i], "-d") == 0) {
                    searchFiles = false;
                } else if (strcmp(args[i], "-f") == 0) {
                    searchDirectories = false;
                } else if (strcmp(args[i], "-e") == 0) {
                    executeFlag = true;
                } else if (searchFiles == false && searchDirectories == false) {
                    printf("Invalid flags!\n");
                    return;
                }
            }
            
            // Check if search argument and target directory are provided
            if (argCount == 2) {
                seekCommand(args[argCount - 1], ".", searchFiles, searchDirectories, executeFlag,f);
            } else if (argCount >3 && (searchFiles == true && executeFlag == true) && (executeFlag == true && searchDirectories == true)) {
                seekCommand(args[argCount - 1], args[argCount - 2], searchFiles, searchDirectories, executeFlag,f);
            } else {
                // Invalid usage
                printf("Usage: seek [-d | -f | -e] <search> [target_directory]\n");
            }*/
            
            
            
            
            
            
            #include "seek.h"

void seekCommand(const char *search, const char *targetDir, bool isFile, bool isDir, bool isexec, int topLevel) {
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
                //printf("%s\n",entry->d_name);
                char str[200];
                strcpy(str,entry->d_name);
                char *dotPosition = strchr(str, '.');
    if (dotPosition != NULL) {
        *dotPosition = '\0';
    }
                if (strcmp(str, search) == 0  ) {
                    printf("%s/%s\n", targetDir, entry->d_name);
                    isMatching = 1;
                    found += 1;

                    if (isexec && entry->d_type == DT_DIR) {
                        if (access(entry->d_name, X_OK) == 0) {
                            chdir(entry->d_name);
                            printf("Changed working directory to %s\n", entry->d_name);
                        } else {
                            printf("Missing permissions for task!\n");
                        }
                    }
                    if (isexec && entry->d_type == DT_REG) {
                                // Print content of the file
                                FILE *file = fopen(entry->d_name, "r");
                                if (file != NULL) {
                                    char buffer[1024];
                                    while (fgets(buffer, sizeof(buffer), file) != NULL) {
                                        printf("%s", buffer);
                                    }
                                    fclose(file);
                                } else {
                                    printf("Error opening file!\n");
                                }
                            }
                }
            }

            if (!isMatching && entry->d_type == DT_DIR) {
                char nextPath[1024]; // Increased buffer size
                snprintf(nextPath, sizeof(nextPath), "%s/%s", targetDir, entry->d_name);
                seekCommand(search, nextPath, isFile, isDir, isexec, false);
            }
        }
    }

    closedir(dir);

    if (topLevel && !found) {
        // Check without extension
        char *ext = strrchr(search, '.');
        if (ext == NULL) {
            ext = (char *)search + strlen(search);
        }

        dir = opendir(targetDir);
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_name[0] != '.') {
                    if ((isFile && entry->d_type == DT_REG) || (isDir && entry->d_type == DT_DIR)) {
                        if (strcmp(ext, entry->d_name) == 0) {
                            printf("%s/%s\n", targetDir, entry->d_name);
                            found += 1;

                            if (isexec && entry->d_type == DT_DIR) {
                                if (access(entry->d_name, X_OK) == 0) {
                                    chdir(entry->d_name);
                                    printf("Changed working directory to %s\n", entry->d_name);
                                } else {
                                    printf("Missing permissions for task!\n");
                                }
                            }
                            if (isexec && entry->d_type == DT_REG) {
                                // Print content of the file
                                FILE *file = fopen(entry->d_name, "r");
                                if (file != NULL) {
                                    char buffer[1024];
                                    while (fgets(buffer, sizeof(buffer), file) != NULL) {
                                        printf("%s", buffer);
                                    }
                                    fclose(file);
                                } else {
                                    printf("Error opening file!\n");
                                }
                            }
                        }
                    }
                }
            }
            closedir(dir);
        }

        if (topLevel && !found) {
            printf("No match found!\n");
        }
    }
}

            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            #include "seek.h"
#include <string.h>

void seekCommand(const char *search, const char *targetDir, bool isFile, bool isDir, bool isexec, int topLevel) {
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
                    printf("%s/%s\n", targetDir, entry->d_name);
                    isMatching = 1;
                    found += 1;

                    if (isexec && entry->d_type == DT_DIR) {
                        if (access(entry->d_name, X_OK) == 0) {
                            chdir(entry->d_name);
                            printf("Changed working directory to %s\n", entry->d_name);
                            return ;
                        } else {
                            printf("Missing permissions for task!\n");
                        }
                    }
                    if (isexec && entry->d_type == DT_REG) {
                        // Print content of the file
                        printf("%s",entry->d_name);
                        
                        FILE *file = fopen(entry->d_name, "r");
                        if (file != NULL) {
                            char buffer[1024];
                            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                                printf("%s", buffer);
                            }
                            fclose(file);
                            return ;
                        } else {
                            printf("Error opening file!\n");
                        }
                    }
                }
            }

            if (!isMatching && entry->d_type == DT_DIR) {
                char nextPath[1024];
                snprintf(nextPath, sizeof(nextPath), "%s/%s", targetDir, entry->d_name);
                seekCommand(search, nextPath, isFile, isDir, isexec, false);
            }
        }
    }

    closedir(dir);

   if (topLevel && !found) {
        // Check without extension
        char *ext = strrchr(search, '.');
        if (ext == NULL) {
            ext = (char *)search + strlen(search);
        }

        dir = opendir(targetDir);
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_name[0] != '.') {
                    if ((isFile && entry->d_type == DT_REG) || (isDir && entry->d_type == DT_DIR)) {
                        if (strcmp(ext, entry->d_name) == 0) {
                            printf("%s/%s\n", targetDir, entry->d_name);
                            found += 1;

                            if (isexec && entry->d_type == DT_DIR) {
                                if (access(entry->d_name, X_OK) == 0) {
                                    chdir(entry->d_name);
                                    printf("Changed working directory to %s\n", entry->d_name);
                                    return ;
                                } else {
                                    printf("Missing permissions for task!\n");
                                }
                            }
                            if (isexec && entry->d_type == DT_REG) {
                                // Print content of the file
                                FILE *file = fopen(entry->d_name, "r");
                     
                                if (file != NULL) {
                                    char buffer[1024];
                                    while (fgets(buffer, sizeof(buffer), file) != NULL) {
                                        printf("%s", buffer);
                                    }
                                    fclose(file);
                                    return ;
                                } else {
                                    printf("Error opening file!\n");
                                }
                            }
                        }
                    }
                }
            }
            closedir(dir);
        }

        
    }
    if (topLevel && !found) {
            printf("No match found!\n");
        }
}

            
            
            
            

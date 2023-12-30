#include "peek.h"
typedef struct {
    char name[MAX_PATH_SIZE];
    struct stat stat_buf;
} FileInfo;

int fileInfoComparator(const void *a, const void *b) {
    return strcmp(((FileInfo *)a)->name, ((FileInfo *)b)->name);
}

void list_directory(char *path, int show_hidden, int show_details,int input_pipe, int output_pipe) {
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }
    FileInfo files[MAX_PATH_SIZE];
    int fileCount = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue; // Skip hidden files and directories
        }
        char full_path[MAX_PATH_SIZE];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        struct stat stat_buf;
        if (stat(full_path, &stat_buf) == -1) {
            perror("stat");
            closedir(dir);
            return;
        }

        strcpy(files[fileCount].name, entry->d_name);
        files[fileCount].stat_buf = stat_buf;
        fileCount++;
    }
	
    closedir(dir);

    // Sort the files array
    qsort(files, fileCount, sizeof(FileInfo), fileInfoComparator);

    // Print the sorted files
    for (int i = 0; i < fileCount; i++) {
        const char *color_code = FILE_COLOR;
            struct stat *stat_buf = &files[i].stat_buf;
            if (S_ISREG(stat_buf->st_mode)) {
                if (stat_buf->st_mode & S_IXUSR) {
                    color_code = EXEC_COLOR;
                } else {
                    color_code = FILE_COLOR;
                }
            } else if (S_ISDIR(stat_buf->st_mode)) {
                color_code = DIR_COLOR;
            } else if (S_ISLNK(stat_buf->st_mode)) {
                color_code = FILE_COLOR;
            } else {
                color_code = FILE_COLOR;
            }
        
        if (files[i].name[0] == '.') {
            color_code = HIDDEN_COLOR;
        }

        if (show_details) {
            struct passwd *pw = getpwuid(files[i].stat_buf.st_uid);
            struct group *gr = getgrgid(files[i].stat_buf.st_gid);
            struct tm *mod_time = localtime(&files[i].stat_buf.st_mtime);S_ISREG

            char type = '?';
            if (S_ISREG(files[i].stat_buf.st_mode)) {
                type = '-';
            } else if (S_ISDIR(files[i].stat_buf.st_mode)) {
                type = 'd';
            } else if (S_ISLNK(files[i].stat_buf.st_mode)) {
                type = 'l';
            }

            char time_str[80];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", mod_time);

            printf("%c%c%c%c%c%c%c%c%c%c %lu %s %s %lu %s %s%s%s\n",
                   type,
                   (files[i].stat_buf.st_mode & S_IRUSR) ? 'r' : '-',
                   (files[i].stat_buf.st_mode & S_IWUSR) ? 'w' : '-',
                   (files[i].stat_buf.st_mode & S_IXUSR) ? 'x' : '-',
                   (files[i].stat_buf.st_mode & S_IRGRP) ? 'r' : '-',
                   (files[i].stat_buf.st_mode & S_IWGRP) ? 'w' : '-',
                   (files[i].stat_buf.st_mode & S_IXGRP) ? 'x' : '-',
                   (files[i].stat_buf.st_mode & S_IROTH) ? 'r' : '-',
                   (files[i].stat_buf.st_mode & S_IWOTH) ? 'w' : '-',
                   (files[i].stat_buf.st_mode & S_IXOTH) ? 'x' : '-',
                   (unsigned long)files[i].stat_buf.st_nlink,
                   pw ? pw->pw_name : "",
                   gr ? gr->gr_name : "",
                   (unsigned long)files[i].stat_buf.st_size,
                   time_str,
                   color_code,
                   files[i].name,
                   RESET_COLOR);
        } else {
            printf("%s%s%s\n", color_code, files[i].name, RESET_COLOR);
        }
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

void peek(int argc, char *argv[],char *prev_dir,char *home_directory,int input_pipe, int output_pipe) {
    int show_hidden = 0;
    int show_details = 0;
    char *path;
    if (argc==1){
      path=".";
    }
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-a" ,strlen(argv[i])) == 0 || strncmp(argv[i], "-la" ,strlen(argv[i])) == 0  || strncmp(argv[i], "-al" ,strlen(argv[i])) == 0 ) {
            show_hidden = 1;
        }
        if (strncmp(argv[i], "-l" ,strlen(argv[i])) == 0  || strncmp(argv[i], "-al" ,strlen(argv[i])) == 0 || strncmp(argv[i], "-la" ,strlen(argv[i])) == 0 ) {
            show_details = 1;
        }
        else{
        	printf("ERROR \nsuch falgs are not allowed\n");
        	return ;
        }
    }
    if (argc>1){
    path = argv[argc-1];}
    char current_dir[MAX_INPUT_SIZE];
    char new_dir[MAX_INPUT_SIZE];
    char previ_dir[MAX_INPUT_SIZE];
    getcwd(previ_dir, sizeof(previ_dir));
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
    list_directory(path, show_hidden, show_details,input_pipe, output_pipe);
    return;
}


#include "peek.h"
void list_directory(char *path, int show_hidden, int show_details) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

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

        const char *color_code = FILE_COLOR;

        if (show_details) {
            if (S_ISREG(stat_buf.st_mode)) {
                if (stat_buf.st_mode & S_IXUSR) {
                    color_code = EXEC_COLOR;
                } else {
                    color_code = FILE_COLOR;
                }
            } else if (S_ISDIR(stat_buf.st_mode)) {
                color_code = DIR_COLOR;
            } else if (S_ISLNK(stat_buf.st_mode)) {
                color_code = FILE_COLOR;
            } else {
                color_code = FILE_COLOR;
            }
        }

        if (entry->d_name[0] == '.') {
            color_code = HIDDEN_COLOR;
        }

        if (show_details) {
            struct passwd *pw = getpwuid(stat_buf.st_uid);
            struct group *gr = getgrgid(stat_buf.st_gid);
            struct tm *mod_time = localtime(&stat_buf.st_mtime);

            char perms[11];
            perms[0] = (S_ISDIR(stat_buf.st_mode)) ? 'd' : '-';
            perms[1] = (stat_buf.st_mode & S_IRUSR) ? 'r' : '-';
            perms[2] = (stat_buf.st_mode & S_IWUSR) ? 'w' : '-';
            perms[3] = (stat_buf.st_mode & S_IXUSR) ? 'x' : '-';
            perms[4] = (stat_buf.st_mode & S_IRGRP) ? 'r' : '-';
            perms[5] = (stat_buf.st_mode & S_IWGRP) ? 'w' : '-';
            perms[6] = (stat_buf.st_mode & S_IXGRP) ? 'x' : '-';
            perms[7] = (stat_buf.st_mode & S_IROTH) ? 'r' : '-';
            perms[8] = (stat_buf.st_mode & S_IWOTH) ? 'w' : '-';
            perms[9] = (stat_buf.st_mode & S_IXOTH) ? 'x' : '-';
            perms[10] = '\0';

            char time_str[80];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", mod_time);

            printf("%s%s %lu %s %s %lu %s %s %s%s%s\n",
                   color_code,
                   perms,
                   (unsigned long)stat_buf.st_nlink,
                   pw ? pw->pw_name : "",
                   gr ? gr->gr_name : "",
                   (unsigned long)stat_buf.st_size,
                   time_str,
                   entry->d_name,
                   RESET_COLOR);
        } else {
            printf("%s%s%s\n", color_code, entry->d_name, RESET_COLOR);
        }
    }

    closedir(dir);
}

void peek(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "Usage: %s [-a] [-l] <path/name>\n", argv[0]);
        return ;
    }

    int show_hidden = 0;
    int show_details = 0;
    char *path = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "-la") == 0 || strcmp(argv[i], "-al") == 0) {
            show_hidden = 1;
        } else if (strcmp(argv[i], "-l") == 0) {
            show_details = 1;
        } else {
            path = argv[i];
        }
    }

    // Handle special symbols in the path
    if (path) {
        if (path[0] == '~') {
            struct passwd *pw = getpwuid(getuid());
            char home_path[MAX_PATH_SIZE];
            snprintf(home_path, sizeof(home_path), "%s%s", pw->pw_dir, path + 1);
            strcpy(path, home_path);
        } else if (path[0] == '-') {
            // Handle previous directory shortcut
            char prev_path[MAX_PATH_SIZE];
            if (chdir("..") == 0) {
                if (getcwd(prev_path, sizeof(prev_path)) != NULL) {
                    strcpy(path, prev_path);
                }
                chdir(path);
            } else {
                perror("chdir");
                return ;
            }
        }
    }

    if (!path) {
        // If path is not provided, use current directory
        char current_path[MAX_PATH_SIZE];
        if (getcwd(current_path, sizeof(current_path)) != NULL) {
            path = current_path;
        } else {
            perror("getcwd");
            return ;
        }
    }

    list_directory(path, show_hidden, show_details);

    return ;
}


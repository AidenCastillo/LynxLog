// linxlog.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#ifdef _WIN32
#define REPO_DIR ".linxlog"
#define STAGING_DIR ".linxlog\\staging"
#define OBJECTS_DIR ".linxlog\\objects"
#define LOG_FILE ".linxlog\\log"
#else
#define REPO_DIR ".linxlog"
#define STAGING_DIR ".linxlog/staging"
#define OBJECTS_DIR ".linxlog/objects"
#define LOG_FILE ".linxlog/log"
#endif



// Function to initialize the version control system
void init_linxlog() {
    // Initialization code (e.g., creating necessary files or directories)
    printf("Initializing LynxLog...\n");
    mkdir(REPO_DIR);
    mkdir(STAGING_DIR);
    mkdir(OBJECTS_DIR);
    mkdir(LOG_FILE);
    FILE* log_file = fopen(LOG_FILE, "w");
    if (log_file) {
        fclose(log_file);
    }
}

// Function to add a file to version control
void add_file(const char *path) {
    // Code to add the file to version control (e.g., staging the file)


    struct stat st;
    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            DIR *dir = opendir(path);
            struct dirent *entry;
            char new_path[256];

            if (dir) {
                while ((entry = readdir(dir)) != NULL) {
                    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                        continue;
                    }
                    snprintf(new_path, sizeof(new_path), "%s\%s", path, entry->d_name);
                    add_file(new_path);
                }
                closedir(dir);
            } else {
                fprintf(stderr, "Error opening directory: %s\n", path);
            }
        } else if (S_ISREG(st.st_mode)) {
            printf("Adding file: %s\n", path);

            char staging_path[256];

            if (path[0] == '.') {
                path+= 2;
            }

            snprintf(staging_path, sizeof(staging_path), "%s\\%s", STAGING_DIR, path);

            char *dir_path = strdup(staging_path);
            char *last_seperator = strrchr(dir_path, '\\');

#ifdef _WIN32
            if (last_seperator) {
                *last_seperator = '\0';
                char command[256];
                snprintf(command, sizeof(command), "mkdir %s", dir_path);
                system(command);
            }

            free(dir_path);

            char command[256];
            snprintf(command, sizeof(command), "copy %s %s", path, staging_path);
            printf(command);
            system(command);
#else
            char command[256];
            snprintf(command, sizeof(command), "cp %s %s/", filename, STAGING_DIR);
            system(command);
#endif
        }
    } else {
        printf("File not found: %s\n", path);
        return;
    }
}

// Function to commit changes with a message
void commit(const char *message) {
    // Code to commit changes (e.g., saving changes to a repository)
    printf("Committing changes with message: %s\n", message);
#ifdef _WIN32
    char command[256];
    snprintf(command, sizeof(command), "copy %s\\* %s\\", STAGING_DIR, OBJECTS_DIR);
    system(command);
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        fprintf(log_file, "Commit: %s\n", message);
        fclose(log_file);
    }
#else
    char command[256];
    snprintf(command, sizeof(command), "cp -r %s/* %s/", STAGING_DIR, OBJECTS_DIR);
    system(command);
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        fprintf(log_file, "Commit: %s\n", message);
        fclose(log_file);
    }
#endif
}

// Function to show commit log
void show_log() {
    printf("Commit Log:\n");
    char line[256];
    FILE* log_file = fopen(LOG_FILE, "r");
    if (log_file) {
        while (fgets(line, sizeof(line), log_file)) {
            printf("%s", line);
        }
        fclose(log_file);
    }
}



// Compare files content
int compare_files(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");
    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 0;
    }


    int ch1 = 0, ch2 = 0;
    while ((ch1 = fgetc(f1)) != EOF && (ch2 = fgetc(f2)) != EOF) {
        if (ch1 != ch2) {
            fclose(f1);
            fclose(f2);
            return 0;
        }
    }

    fclose(f1);
    fclose(f2);
    return 1;
}
int check_ignore(const struct dirent *entry) {
    // Read .linxignore file
    FILE *ignore_file = fopen(".linxignore", "r");
    if (!ignore_file) {
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), ignore_file)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(entry->d_name, line) == 0) {
            fclose(ignore_file);
            return 1;
        }
    }

    fclose(ignore_file);

    // Default ignore files
    const char *ignore_files[] = {REPO_DIR, STAGING_DIR, OBJECTS_DIR, LOG_FILE, ".linxignore"};

    for (int i = 0; i < sizeof(ignore_files) / sizeof(ignore_files[0]); i++) {
        if (strcmp(entry->d_name, ignore_files[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to show status
void check_status(const char *working_dir_path, const char *staging_dir_path) {
    DIR *working_dir = opendir(working_dir_path);
    DIR *staging_dir = opendir(staging_dir_path);
    struct dirent *entry;
    struct stat st;
    char working_file_path[256];
    char staging_file_path[256];

    if (working_dir && staging_dir) {
        // List files in working directory
        while ((entry = readdir(working_dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || check_ignore(entry)) {
                continue;
            }
            snprintf(working_file_path, sizeof(working_file_path), "%s\\%s", working_dir_path, entry->d_name);
            snprintf(staging_file_path, sizeof(staging_file_path), "%s\\%s", staging_dir_path, entry->d_name);

            if (stat(working_file_path, &st) == 0) {
                if (S_ISDIR(st.st_mode)) {
                    // Recursively check subdirectories
                    check_status(working_file_path, staging_file_path);
                } else if (S_ISREG(st.st_mode)) {
                    if (access(staging_file_path, F_OK) == -1) {
                        printf("Untracked file: %s\n", working_file_path);
                    } else if (!compare_files(working_file_path, staging_file_path)) {
                        printf("Modified file: %s\n", working_file_path);
                    }
                }
            }
        }

        // List files in staging directory
        while ((entry = readdir(staging_dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            snprintf(staging_file_path, sizeof(staging_file_path), "%s\\%s", staging_dir_path, entry->d_name);
            snprintf(working_file_path, sizeof(working_file_path), "%s\\%s", working_dir_path, entry->d_name);

            if (stat(staging_file_path, &st) == 0 && S_ISREG(st.st_mode)) {
                if (access(working_file_path, F_OK) == -1) {
                    printf("Deleted file: %s\n", staging_file_path);
                }
            }
        }

        closedir(working_dir);
        closedir(staging_dir);
    } else {
        printf("Error opening directories: %s, %s\n", working_dir_path, staging_dir_path);
    }
}

void status() {
    printf("Status:\n");
    check_status(".", STAGING_DIR);
}

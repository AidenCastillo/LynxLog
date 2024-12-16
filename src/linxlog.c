// linxlog.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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
void add_file(const char *filename) {
    // Code to add the file to version control (e.g., staging the file)
    printf("Adding file: %s\n", filename);
#ifdef _WIN32
    char command[256];
    snprintf(command, sizeof(command), "copy .\\%s .\\%s\\", filename, STAGING_DIR);
    printf(command);
    system(command);
#else
    char command[256];
    snprintf(command, sizeof(command), "cp %s %s/", filename, STAGING_DIR);
    system(command);
#endif
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

// Function to show status
void status() {
    // Compare working directory with staging and repo
    printf("Status:\n");
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "linxlog.h"

void init() {
    const char *linxlog_dir = ".linxlog";

    // Check if .linxlog directory exists
    struct stat st = {0};
    if (stat(linxlog_dir, &st) == -1) {
        // Create the directory
        if (mkdir(linxlog_dir) == 0) {
            printf("Initalized empty LinxLog repository in %s\n", linxlog_dir);
        } else {
            perror("Failed to create .linxlog directory");
            exit(EXIT_FAILURE);
        }
    }

}

int main(int argc, char *argv[])
{
    // Check if the number of arguments is correct
    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "init") == 0) {
        init_linxlog();
    }
    else if (strcmp(argv[1], "add") == 0 && argc == 3) {
        add_file(argv[2]);
    }
    else if (strcmp(argv[1], "commit") == 0 && argc == 3) {
        commit(argv[2]);
    }
    else if (strcmp(argv[1], "log") == 0) {
        show_log();
    }
    else if (strcmp(argv[1], "status") == 0) {
        status();
    }
    else {
        printf("Unknown comamnd: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return 0;
}

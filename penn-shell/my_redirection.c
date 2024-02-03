#include "redirection.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Returns 0 on success, -1 on failure.
int handleInputRedirection(char *inputFile) {
    if (inputFile == NULL) {
        fprintf(stderr, "Error: No input file specified for redirection.\n");
        return -1;
    }

    int fd = open(inputFile, O_RDONLY);
    if (fd < 0) {
        perror("Error opening input file");
        return -1;
    }

    if (dup2(fd, STDIN_FILENO) < 0) {
        perror("Error redirecting standard input");
        close(fd); 
        return -1;
    }

    close(fd);
    return 0;
}

// Returns 0 on success, -1 on failure.
int handleOutputRedirection(char *outputFile) {
    if (outputFile == NULL) {
        fprintf(stderr, "Error: No output file specified for redirection.\n");
        return -1;
    }

    int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error opening output file");
        return -1;
    }

    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("Error redirecting standard output");
        close(fd);  
        return -1;
    }

    close(fd);
    return 0;
}

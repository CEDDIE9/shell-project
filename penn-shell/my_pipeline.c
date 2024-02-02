#include "my_pipeline.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void execute_command(char **cmd) {
    // This is where you'd call execvp() to execute a single command.
    if (execvp(cmd[0], cmd) == -1) {
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
}

void execute_pipeline(char *commands[], int num_commands) {
    int i;
    int in_fd = 0; // The input for the first command is STDIN.
    int fd[2];

    for (i = 0; i < num_commands; i++) {
        // Create a pipe for all but the last command.
        if (i < num_commands - 1) {
            if (pipe(fd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            if (in_fd != STDIN_FILENO) {
                if (dup2(in_fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(in_fd);
            }
            // For all but the last command, set stdout to the pipe's write end.
            if (i < num_commands - 1) {
                close(fd[0]); // Close read end of the pipe in the child.
                if (dup2(fd[1], STDOUT_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(fd[1]);
            }
            execute_command(commands[i]);
        } else { // Parent process
            wait(NULL); // Wait for the child to finish.
            if (in_fd != STDIN_FILENO) {
                close(in_fd); // Close the previous read end.
            }
            // For all but the last command, the next command's input is the read end of the pipe.
            if (i < num_commands - 1) {
                close(fd[1]); // Close write end of the pipe in the parent.
                in_fd = fd[0]; // Save the read end to be the input for the next command.
            }
        }
    }
}

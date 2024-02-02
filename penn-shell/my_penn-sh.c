#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// Forward declaration of your shell's main loop function
void shell_main_loop(void);

void setupSignalHandlers() {
    struct sigaction sa;

    // Setup the handler function
    sa.sa_handler = &sigintHandler;
    // Block every signal during the handler
    sigfillset(&sa.sa_mask);
    // Remove any flag from sa.sa_flags
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error: Cannot handle SIGINT");
        exit(EXIT_FAILURE);
    }
}

void sigintHandler(int unused) {
    // Do not terminate the program. Instead, print a new line and the prompt again.
    printf("\nmy_penn-sh> ");
    fflush(stdout);
}

int main() {
    setupSignalHandlers();
    shell_main_loop(); // Your shell's main loop
    return 0;
}

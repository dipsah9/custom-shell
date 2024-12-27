#include "../includes/shell.h"

void display_prompt() {
    printf("custom_shell> ");
}

int main() {
    char *input = NULL;  // Pointer to hold the input line
    size_t len = 0;      // Size of the buffer (set dynamically by `getline`)

    while (1) {
        printf("custom_shell> ");

        // Use getline to read user input
        ssize_t nread = getline(&input, &len, stdin);

        // Check for EOF (Ctrl+D)
        if (nread == -1) {
            printf("\nExiting custom shell...\n");
            break;
        }

        // Remove the newline character, if present
        input[strcspn(input, "\n")] = '\0';

        // If input is not empty, execute the command
        if (strlen(input) > 0) {
            if (execute_command(input) == 0) {
                break; // Exit if execute_command returns 0 (e.g., for "exit")
            }
        }
    }

    free(input); // Free dynamically allocated memory
    return 0;

}

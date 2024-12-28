#include "../includes/shell.h"
#include <readline/readline.h>
#include <readline/history.h>


void display_prompt() {
    printf("😍custom_shell> ");
}


int main() {
    rl_attempted_completion_function = custom_completion; // Enable custom tab completion
    const char *manual_path = "manual.txt";
    display_manual(manual_path);
    char *input;

    while (1) {
        input = readline("custom_shell> "); // Read user input with history and completion

        // Exit on EOF (Ctrl+D)
        if (input == NULL) {
            printf("\nExiting custom shell...\n");
            break;
        }

        // Add to history if the input is not empty
        if (strlen(input) > 0) {
            add_history(input);
        }

        // Execute the command
        if (execute_command(input) == 0) {
            free(input); // Free memory before exiting
            break;
        }

        free(input); // Free memory allocated by readline
    }

    return 0;

}

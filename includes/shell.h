#ifndef SHELL_H
#define SHELL_H

// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>


// Constants
#define MAX_CMD_LEN 1024
#define MAX_ARGS 128

// Function Declarations
void display_prompt();
int execute_command(char *input);
void make_folder(const char *folder_name); // Example custom command
void whoami();
void list_file_info(const char *file);
void list_dir(const char *dir_path);
void print_basename(const char *path);
void list_matching_files(const char *dir_path, const char *pattern);
void convert_to_long(const char *str);
void regex_match(const char *str, const char *pattern);


#endif // SHELL_H

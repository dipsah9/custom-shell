#include "../includes/shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <limits.h>
#include <regex.h>
#include <readline/readline.h>
#include <readline/history.h>
//Function to make new folder. 
void make_folder(const char *folder_name) {
    if (mkdir(folder_name, 0755) == -1) {
       printf("Error creating folder '%s': %s\n", folder_name, strerror(errno));
    } else {
        perror("Folder '%s' created successfully.\n");
    }   
}
// Function to execute commands
int execute_command(char *input) {
    char *args[128];
    char *token = strtok(input, " ");
    int i = 0;

    while (token != NULL && i < 128) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (i == 0) {
        return 1; // Empty input, do nothing
    }

    // Check for built-in commands
    if (strcmp(args[0], "exit") == 0) {
        return 0; // Exit the shell
    } else if (strcmp(args[0], "make_folder") == 0) {
        if (args[1] != NULL) {
            make_folder(args[1]);
        } else {
            printf("Usage: make_folder <folder_name>\n");
        }
        return 1;
    } else if (strcmp(args[0], "list_info") == 0) {
        if (args[1] != NULL) {
            list_file_info(args[1]);
        } else {
            printf("Usage: list_info <file_name>\n");
        }
        return 1;
    }else if(strcmp(args[0], "list_dir") == 0){
        if(args[1] != NULL){
            list_dir(args[1]);
        }else{
            printf("Usage: list_dir <path>\n");
        }
        return 1;
    }else if(strcmp(args[0], "print_basename") == 0){
        if(args[1] != NULL){
            print_basename(args[1]);
        }else{
            printf("There is no such function, maybe its 'print_basename'\n");
        }
        return 1;
    }else if(strcmp(args[0], "list_matching_files") == 0){
        if( args[1] != NULL && args[2] != NULL){
            list_matching_files(args[1],args[2]);
        }else{
            printf("Usage: list_matching_files <dir_path> <pattern>\n");
        }
        return 1;
    }else if(strcmp(args[0], "convert_to_long") == 0){
        if(args[1] != NULL){
            convert_to_long(args[1]);
        }else{
            printf("Usage: convert_to_long <string> \n");
        }
        return 1;
    }else if(strcmp(args[0], "regex_match") == 0){
        if(args[1] != NULL && args[2] != NULL){
            regex_match(args[1], args[2]);
        }else{
            printf("Usage: regex_match <str> <patter>");
        }
        return 1;
    }

    // External command execution
    pid_t pid = fork();
    if (pid == -1) {
        printf("Error: Unable to fork a new process: %s\n", strerror(errno));
        return 1;
    } else if (pid == 0) {
        execvp(args[0], args);
        printf("Error: Command '%s' failed: %s\n", args[0], strerror(errno));
        _exit(1);
    } else {
        wait(NULL);
    }
    return 1;
}
//function who am i implementation 
void whoami(){
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if(pw){
        printf("Current: %s\n", pw->pw_name);
    }else{
        perror("Unable to fetch username");
    }
}

//function to list the file information

void list_file_info(const char *file){
    struct stat file_stat;
    if(lstat(file, &file_stat) == -1){
        perror("lstat failed");
        return;
    }

    printf("File: %s\n", file);
    printf("Size: %lld bytes\n", file_stat.st_size);
    printf("Type: ");

    if (S_ISREG(file_stat.st_mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(file_stat.st_mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(file_stat.st_mode)) {
        printf("Symbolic link\n");
    } else {
        printf("Other\n");
    }

}

void list_dir(const char * dir_path){
    DIR *dir = opendir(dir_path);
    if(!dir){
        perror("opendir failed");
        return;
    }

    struct dirent *entry;
   
    printf("Content of directory: %s\n", dir_path);
    while((entry = readdir(dir)) != NULL){
        if((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)){
            continue;
        }
        printf("%s\n", entry->d_name);
    } 
    closedir(dir);
}

void print_basename(const char *path){
    char *base = basename((char*)path);
    printf("Basename: %s\n", base);
}

void list_matching_files(const char *dir_path, const char *pattern) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir failed");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (fnmatch(pattern, entry->d_name, 0) == 0) {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

void convert_to_long(const char *str){
    char *endptr;
    long int result;

    //convert strin to integer usng strtoi
    result = strtol(str, &endptr, 10);

    if(*endptr != '\0'){
        printf("Error: '%s' is not a valid number. \n",str);
        return;
    }

    if((result == LONG_MIN || result == LONG_MAX) && errno == ERANGE){

        perror("Error: Number out of range");
        return;
    }

    printf("Converted number %ld\n", result);
}

// Function to perform regex matching
void regex_match(const char *str, const char *pattern) {
    regex_t regex;
    int ret;

    // Compile the regular expression
    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        printf("Error: Could not compile regex\n");
        return;
    }

    // Execute the regex match
    ret = regexec(&regex, str, 0, NULL, 0);
    if (!ret) {
        printf("Match found for pattern '%s' in string '%s'\n", pattern, str);
    } else if (ret == REG_NOMATCH) {
        printf("No match found for pattern '%s' in string '%s'\n", pattern, str);
    } else {
        char errbuf[100];
        regerror(ret, &regex, errbuf, sizeof(errbuf));
        printf("Regex match failed: %s\n", errbuf);
    }

    // Free the compiled regex
    regfree(&regex);
}

char *command_generator(const char *text, int state){
    static int list_index , len;

    char *commands[]  = {"make_folder", "whoami", "list_file_info", "list_dir", "print_basename",
    "list_matching_files", "convert_to_long", "regex_match"};

    while(commands[list_index]){
        if(strncmp(commands[list_index], text, len) == 0){
            return strdup(commands[list_index++]);
        }
        list_index++;
    }

    return NULL;
}

char **custom_completion(const char * text, int start, int end){
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, command_generator);
}


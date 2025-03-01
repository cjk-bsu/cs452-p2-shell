#include "lab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <getopt.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>

char *get_prompt(const char *env) {
    const char *temp_prompt = getenv(env);
    if (!temp_prompt) {
        temp_prompt = "shell>";
    }

    char *prompt = malloc(strlen(temp_prompt) + 1);
    if (!prompt) {
        perror("get_prompt: malloc failure");
        exit(EXIT_FAILURE);
    }

    strcpy(prompt, temp_prompt);
    return prompt;
}

char **cmd_parse(const char *line) {
    const long arg_max = sysconf(_SC_ARG_MAX);
    char *line_copy = strdup(line); // Duplicate input line for modification
    char *save = NULL;

    if (!line_copy) {
        perror("cmd_parse: strdup failed");
        exit(EXIT_FAILURE);
    }

    char **args = (char **)calloc(arg_max, sizeof(char *)); // Allocate memory for argument list

    if (!args) {
        perror("cmd_parse: calloc failed");
        free(line_copy);
        exit(EXIT_FAILURE);
    }

    char *token = strtok_r(line_copy, " ", &save); // Tokenize the input line

    if (!token) {
        perror("cmd_parse: strtok failed");
        cmd_free(args);
        free(line_copy);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; token && i < arg_max; i++) {
        args[i] = token;
        token = strtok_r(NULL, " ", &save);
    }

    return args;
}

void cmd_free(char **args) {
    char *temp_args = *args;
    free(temp_args);
    free((void *)args);
}

char *trim_white(char *line) {
    if (!line) return NULL;

    while (*line && isspace((unsigned char)*line)) {
        line++;
    }

    if (*line == '\0') return line;

    char *end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) {
        *end-- = '\0';
    }

    return line;
}

void parse_args(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                printf("Shell Version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
                cmd_free(argv);
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
                cmd_free(argv);
                exit(EXIT_FAILURE);
        }
    }
}

int change_dir(char **dir) {
    if (dir[1] == NULL) {
        struct passwd *pw = getpwuid(getuid());
        if (!pw || !pw->pw_dir) {
            perror("getpwuid");
            return -1;
        }
        return chdir(pw->pw_dir);
    }
    return chdir(dir[1]);
}

bool do_builtin(struct shell *sh, char **argv) {
    if (!argv || !argv[0]) { // If no command is given, exit
        cmd_free(argv);
        sh_destroy(sh);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[0], "exit") == 0) { // Handle 'exit' command
        cmd_free(argv);
        sh_destroy(sh);
        return true;
    }

    if (strcmp(argv[0], "cd") == 0) { // Handle 'cd' command
        if (change_dir(argv) != 0) {
            cmd_free(argv);
            sh_destroy(sh);
            exit(EXIT_FAILURE);
        }

        return true;
    }

    if (strcmp(argv[0], "history") == 0) { // Handle 'history' command
        HIST_ENTRY **history = history_list();
        if(history != NULL) {
            for (int i = 0; history[i] != NULL; i++) {
                printf("%d %s\n", i, history[i]->line);
            }
        }

        return true;
    }

    pid_t pid = fork(); // Create a new process
    if (pid == -1) {
        perror("do_builtin: fork failure");
        cmd_free(argv);
        sh_destroy(sh);
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execvp(argv[0], argv); // Execute command in child process
        perror("do_builtin: execvp failure"); 
    } else {
        int status;
        waitpid(pid, &status, 0); // Parent process waits for child to finish
        if (status < 0) {
            perror("do_builtin: waitpid failure");
            cmd_free(argv);
            sh_destroy(sh);
            exit(EXIT_FAILURE);
        }
    }

    return false;
}

void sh_init(struct shell *sh) {

    using_history();

    // Ignore common terminal signals to prevent unintended interruptions
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    sh->prompt = get_prompt("SHELL_PROMPT");

    sh->shell_terminal = STDIN_FILENO;
    sh->shell_is_interactive = isatty(sh->shell_terminal);

    sh->shell_pgid = getpgrp();

    if (!sh->shell_is_interactive) { // Check if shell is interactive
        while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) kill (- sh->shell_pgid, SIGTTIN); // Ensure the shell is in the foreground or suspend the process until it is

        sh->shell_pgid = getpid(); // Set the shell's process group ID to its own PID
        if (setpgid(sh->shell_pgid, sh->shell_pgid) < 0) {
            perror("sh_init: setpgid failure");
            sh_destroy(sh);
            exit(EXIT_FAILURE);
        }

        tcsetpgrp(sh->shell_terminal, sh->shell_pgid); // Set the shell as the foreground process group

        tcgetattr(sh->shell_terminal, &sh->shell_tmodes); // Save the current terminal settings
    }
}

void sh_destroy(struct shell *sh) {
    free(sh->prompt);
    free(sh);
    clear_history();
}
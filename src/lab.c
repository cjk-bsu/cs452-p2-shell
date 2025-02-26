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
    char *prompt = getenv(env);
    if (!prompt) {
        prompt = "shell>";
    }
    return strdup(prompt);
}

char **cmd_parse(const char *line) {
    if (!line) return NULL;

    long arg_max = sysconf(_SC_ARG_MAX);
    if (arg_max <= 0) arg_max = 4096;

    char **args = malloc((arg_max / 2) * sizeof(char *));
    if (!args) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *token;
    char *line_copy = strdup(line);
    if (!line_copy) {
        perror("strdup");
        cmd_free(args);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    token = strtok(line_copy, " ");
    while (token != NULL && i < (arg_max / 2) - 1) {
        if (token) {
            args[i] = strdup(token);
            if (!args[i]) {
                perror("strdup");
                cmd_free(args);
                free(line_copy);
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;
    free(line_copy);
    return args;
}

void cmd_free(char **args) {
    if (!args) return;

    for (int i = 0; args[i] != NULL; i++) {
        free(args[i]);
    }
    free(args);
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
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
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

// void add_history(const char *line) {
//     if (line && *line) {
//         add_history(line); 
//     }
// }

// void print_history() {
//     HIST_ENTRY **history_entries = history_list();  
//     if (history_entries) {
//         for (int i = 0; history_entries[i] != NULL; i++) {
//             printf("%d: %s\n", i + 1, history_entries[i]->line);
//         }
//     } else {
//         printf("No history available.\n");
//     }
// }

bool do_builtin(struct shell *sh, char **argv) {
    if (!argv || !argv[0]) {
        return false;
    }

    if (strcmp(argv[0], "exit") == 0) {
        cmd_free(argv);
        sh_destroy(sh); 
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[0], "cd") == 0) {
        if (change_dir(argv) != 0) {
            perror("cd");
            cmd_free(argv);
            sh_destroy(sh);
            exit(EXIT_FAILURE);
        }
        cmd_free(argv); 
        return true;
    }

    // if (strcmp(argv[0], "history") == 0) {
    //     print_history(); 
    //     cmd_free(argv);
    //     return true;
    // }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return false; 
    } else if (pid == 0) {
        execvp(argv[0], argv); 
        perror("execvp");
        exit(EXIT_FAILURE); 
    } else {
        int status;
        waitpid(pid, &status, 0); 
    }

    cmd_free(argv);
    return true;
}

void sh_init(struct shell *sh) {
    sh->shell_terminal = STDIN_FILENO;
    sh->shell_is_interactive = isatty(sh->shell_terminal);

    if (sh->shell_is_interactive) {
        while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
            kill(-sh->shell_pgid, SIGTTIN);
        }

        sh->shell_pgid = getpid();
        if (setpgid(sh->shell_pgid, sh->shell_pgid) < 0) {
            perror("setpgid");
            exit(EXIT_FAILURE);
        }

        tcsetpgrp(sh->shell_terminal, sh->shell_pgid);
        tcgetattr(sh->shell_terminal, &sh->shell_tmodes);
    }

    sh->prompt = get_prompt("SHELL_PROMPT");
}

void sh_destroy(struct shell *sh) {
    if (sh != NULL) {
        if (sh->prompt != NULL) {
            free(sh->prompt);
        }
        free(sh);
    }
}

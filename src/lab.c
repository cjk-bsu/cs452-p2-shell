// #include "lab.h"
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <pwd.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// #include <signal.h>

// char *get_prompt(const char *env) {
//     char *prompt = getenv(env);
//     if (!prompt) {
//         prompt = "shell>";
//     }
//     return strdup(prompt);
// }

// int change_dir(char **dir) {
//     const char *target_dir = NULL;

//     // Use provided arg location or go to home directory
//     if (dir[1] == NULL) {
//         target_dir = getenv("HOME");

//         // Fallback on getpwuid if getenv fails
//         if (!target_dir) { 
//             struct passwd *pw = getpwuid(getuid());
//             if (!pw) {
//                 perror("getpwuid");
//                 return -1;
//             }
//             target_dir = pw->pw_dir;
//         }
//     } else {
//         target_dir = dir[1];
//     }

//     // Attempt to change directory
//     if (chdir(target_dir) != 0) {
//         perror("cd");
//         return -1;
//     }

//     return 0;
// }

// char **cmd_parse(const char *line) {
//     if (!line) return NULL;
//     char **args = malloc((sysconf(_SC_ARG_MAX) / 2) * sizeof(char *));
//     if (!args) return NULL;

//     char *line_copy = strdup(line);
//     if (!line_copy) {
//         free(args);
//         return NULL;
//     }

//     char *token = strtok(line_copy, " ");
//     int i = 0;
//     while (token) {
//         args[i++] = strdup(token);
//         token = strtok(NULL, " ");
//     }
//     args[i] = NULL;
//     free(line_copy);

//     if (i == 0) {
//         free(args);
//         return NULL;
//     }
//     return args;
// }

// void cmd_free(char **cmd) {
//     if (!cmd) return;
//     for (int i = 0; cmd[i]; i++) {
//         free(cmd[i]);
//     }
//     free(cmd);
// }

// char *trim_white(char *line) {
//     // if (!line) return NULL;
//     // while (isspace(*line)) line++;
//     // if (*line == 0) return line;
//     // char *end = line + strlen(line) - 1;
//     // while (end > line && isspace(*end)) end--;
//     // *(end + 1) = '\0';
//     // return line;

//         if (!line) return NULL;

//     while (isspace((unsigned char)*line)) line++;
//     // if (*line == 0) return strdup("");
//     if (*line == '\0') return line;


//     char *end = line + strlen(line) - 1;
//     while (end > line && isspace((unsigned char)*end)) end--;
//     *(end + 1) = '\0';

//     return strdup(line);
// }

// bool do_builtin(struct shell *sh, char **argv) {
//     // Check for any arguments, EOF, or exit command
//     if (!argv[0]) return false;
//     if (strcmp(argv[0], "exit") == 0) {
//         sh_destroy(sh);
//         exit(0);
//     }

//     // Check for directory change command
//     if (strcmp(argv[0], "cd") == 0) {
//         if (change_dir(argv) != 0) {
//             perror("cd");
//         }
//         return true;
//     }

//     if (strcmp(argv[0], "history") == 0) {
//         HIST_ENTRY **history_entries = history_list();
//         if (history_entries) {
//             for (int i = 0; history_entries[i]; i++) {
//                 printf("%d %s\n", i + history_base, history_entries[i]->line);
//             }
//         }
//         return true;
//     }

//     pid_t pid = fork();
//     if (pid < 0) {
//         perror("fork");
//         return true;
//     }
//     if (pid == 0) { 
//         execvp(argv[0], argv);
//         perror("execvp");
//         exit(EXIT_FAILURE);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     return true;

    // pid_t pid = fork();
    // if (pid < 0) {
    //     perror("fork");
    //     return true;
    // }
    // if (pid == 0) {  // Child process
    //     pid_t child = getpid();
    //     setpgid(child, child);
    //     tcsetpgrp(STDIN_FILENO, child);

    //     // Restore default signal behavior in the child
    //     signal(SIGINT, SIG_DFL);
    //     signal(SIGQUIT, SIG_DFL);
    //     signal(SIGTSTP, SIG_DFL);
    //     signal(SIGTTIN, SIG_DFL);
    //     signal(SIGTTOU, SIG_DFL);

    //     execvp(argv[0], argv);
    //     perror("execvp");
    //     exit(EXIT_FAILURE);
    // } else {  // Parent process
    //     int status;
    //     waitpid(pid, &status, 0);
        
    //     // Restore shell as foreground process group
    //     tcsetpgrp(STDIN_FILENO, getpgrp());
    // }
// }

// void sh_init(struct shell *sh) {
//     sh->shell_terminal = STDIN_FILENO;
//     sh->shell_is_interactive = isatty(sh->shell_terminal);

//     if (sh->shell_is_interactive) {
//         while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
//             kill(-sh->shell_pgid, SIGTTIN);
//         }
//         signal(SIGINT, SIG_IGN);
//         signal(SIGQUIT, SIG_IGN);
//         signal(SIGTSTP, SIG_IGN);
//         signal(SIGTTIN, SIG_IGN);
//         signal(SIGTTOU, SIG_IGN);
//     }

//     sh->prompt = get_prompt("SHELL_PROMPT");
// }


// void sh_init(struct shell *sh) {
//     sh->shell_terminal = STDIN_FILENO;
//     sh->shell_is_interactive = isatty(sh->shell_terminal);
    
//     if (sh->shell_is_interactive) {
//         while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
//             kill(-sh->shell_pgid, SIGTTIN);
//         }
//         signal(SIGINT, SIG_IGN);
//         signal(SIGQUIT, SIG_IGN);
//         signal(SIGTSTP, SIG_IGN);
//         signal(SIGTTIN, SIG_IGN);
//         signal(SIGTTOU, SIG_IGN);
//     }

//     if (sh->prompt) {
//         free(sh->prompt);
//     }

//     sh->prompt = get_prompt("SHELL_PROMPT");
// }

// void sh_destroy(struct shell *sh) {
//     clear_history();
//     if (sh->prompt) {
//         free(sh->prompt);
//         sh->prompt = NULL;
//     }
// }

// void parse_args(int argc, char **argv) {
//     int opt;
//     while ((opt = getopt(argc, argv, "v")) != -1) {
//         if (opt == 'v') {
//             printf("Shell version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
//             exit(0);
//         }
//     }
// }


// #include "lab.h"
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <pwd.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// #include <signal.h>

// char *get_prompt(const char *env) {
//     char *prompt = getenv(env);
//     if (!prompt) {
//         prompt = "shell>";
//     }
//     return strdup(prompt);
// }

// int change_dir(char **dir) {
//     if (!dir || !dir[1]) {
//         const char *home = getenv("HOME");
//         if (!home) {
//             struct passwd *pw = getpwuid(getuid());
//             if (!pw) {
//                 perror("getpwuid");
//                 return -1;
//             }
//             home = pw->pw_dir;
//         }
//         return chdir(home) == 0 ? 0 : (perror("cd"), -1);
//     }
//     return chdir(dir[1]) == 0 ? 0 : (perror("cd"), -1);
// }

// char **cmd_parse(const char *line) {
//     if (!line || *line == '\0') return NULL;
    
//     size_t arg_max = sysconf(_SC_ARG_MAX) / 2;
//     char **args = malloc(arg_max * sizeof(char *));
//     if (!args) return NULL;

//     char *line_copy = strdup(line);
//     if (!line_copy) {
//         free(args);
//         return NULL;
//     }

//     char *token = strtok(line_copy, " ");
//     // int i = 0;
//     size_t i = 0;
//     while (token && i < arg_max - 1) {
//         args[i++] = strdup(token);
//         token = strtok(NULL, " ");
//         args[i] = strdup(token);
//         // if (!args[i]) {  // If strdup() fails, free everything
//         //     for (size_t j = 0; j < i; j++) {
//         //         free(args[j]);
//         //     }
//         //     free(args);
//         //     free(line_copy);
//         //     return NULL;
//         // }
//         // i++;
//         // token = strtok(NULL, " ");
//     }
//     args[i] = NULL;
//     free(line_copy);

//     if (i == 0) {
//         free(args);
//         return NULL;
//     }
//     return args;
// }

// void cmd_free(char **cmd) {
//     if (!cmd) return;
//     for (int i = 0; cmd[i]; i++) {
//         free(cmd[i]);
//     }
//     free(cmd);
// }

// char *trim_white(char *line) {
//     if (!line) return NULL;

//     while (isspace((unsigned char)*line)) line++;
//     // if (*line == 0) return strdup("");
//     if (*line == '\0') return line;


//     char *end = line + strlen(line) - 1;
//     while (end > line && isspace((unsigned char)*end)) end--;
//     *(end + 1) = '\0';

//     return strdup(line);
// }

// bool do_builtin(struct shell *sh, char **argv) {
//     if (!argv || !argv[0]) return false;
    
//     if (strcmp(argv[0], "exit") == 0) {
//         sh_destroy(sh);
//         exit(0);
//     }
//     if (strcmp(argv[0], "cd") == 0) {
//         return change_dir(argv) == 0;
//     }
//     if (strcmp(argv[0], "history") == 0) {
//         HIST_ENTRY **history_entries = history_list();
//         if (history_entries) {
//             for (int i = 0; history_entries[i]; i++) {
//                 printf("%d %s\n", i + history_base, history_entries[i]->line);
//             }
//         }
//         return true;
//     }
//     return false;
// }

// void sh_init(struct shell *sh) {
//     sh->shell_terminal = STDIN_FILENO;
//     sh->shell_is_interactive = isatty(sh->shell_terminal);
    
//     if (sh->shell_is_interactive) {
//         while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
//             kill(-sh->shell_pgid, SIGTTIN);
//         }
//         signal(SIGINT, SIG_IGN);
//         signal(SIGQUIT, SIG_IGN);
//         signal(SIGTSTP, SIG_IGN);
//         signal(SIGTTIN, SIG_IGN);
//         signal(SIGTTOU, SIG_IGN);
//     }

//     if (sh->prompt) {
//         free(sh->prompt);
//     }

//     sh->prompt = get_prompt("SHELL_PROMPT");
// }

// void sh_destroy(struct shell *sh) {
//     clear_history();
//     if (sh->prompt) {
//         free(sh->prompt);
//         sh->prompt = NULL;
//     }
// }

// void parse_args(int argc, char **argv) {
//     int opt;
//     while ((opt = getopt(argc, argv, "v")) != -1) {
//         if (opt == 'v') {
//             printf("Shell version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
//             exit(0);
//         }
//     }
// }



// #include "lab.h"
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <pwd.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// #include <signal.h>

// void parse_args(int argc, char **argv) {
//     int opt;
//     while ((opt = getopt(argc, argv, "v")) != -1) {
//         if (opt == 'v') {
//             printf("Shell version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
//             exit(0);
//         }
//     }
// }

// char *get_prompt(const char *env) {
//     char *prompt = getenv(env);
//     if (!prompt) {
//         prompt = "shell> ";
//     }
//     return strdup(prompt);

//     // char *prompt = getenv(env);
//     // return prompt ? strdup(prompt) : strdup("shell>");
// }

// int change_dir(char **dir) {
//     if (dir[1] == NULL) {
//         struct passwd *pw = getpwuid(getuid());
//         if (!pw) {
//             perror("getpwuid");
//             return -1;
//         }
//         return chdir(pw->pw_dir);
//     }
//     return chdir(dir[1]);
// }

// bool do_builtin(struct shell *sh, char **argv) {
//     if (!argv[0]) return false;
//     if (strcmp(argv[0], "exit") == 0) {
//         sh_destroy(sh);
//         exit(0);
//     }
//     if (strcmp(argv[0], "cd") == 0) {
//         if (change_dir(argv) != 0) {
//             perror("cd");
//         }
//         return true;
//     }
//     if (strcmp(argv[0], "history") == 0) {
//         HIST_ENTRY **history_entries = history_list();
//         if (history_entries) {
//             for (int i = 0; history_entries[i]; i++) {
//                 printf("%d %s\n", i + history_base, history_entries[i]->line);
//             }
//         }
//         return true;
//     }
//     return false;
// }

// char **cmd_parse(const char *line) {
//     if (!line) return NULL;
//     char **args = malloc((sysconf(_SC_ARG_MAX) / 2) * sizeof(char *));
//     if (!args) return NULL;

//     char *line_copy = strdup(line);
//     if (!line_copy) {
//         free(args);
//         return NULL;
//     }

//     char *token = strtok(line_copy, " ");
//     int i = 0;
//     while (token) {
//         args[i++] = strdup(token);
//         token = strtok(NULL, " ");
//     }
//     args[i] = NULL;

//     free(line_copy);
//     return args;
// }

// void cmd_free(char **cmd) {
//     if (!cmd) return;
//     for (int i = 0; cmd[i]; i++) {
//         free(cmd[i]); 
//     }
//     free(cmd); 
// }

// char *trim_white(char *line) {
//     if (!line) return NULL;
//     while (isspace(*line)) line++;
//     if (*line == 0) return line;
//     char *end = line + strlen(line) - 1;
//     while (end > line && isspace(*end)) end--;
//     *(end + 1) = '\0';
//     return line;
// }

// void sh_init(struct shell *sh) {
//     sh->shell_terminal = STDIN_FILENO;
//     sh->shell_is_interactive = isatty(sh->shell_terminal);
//     if (sh->shell_is_interactive) {
//         while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
//             kill(-sh->shell_pgid, SIGTTIN);
//         }
//         signal(SIGINT, SIG_IGN);
//         signal(SIGQUIT, SIG_IGN);
//         signal(SIGTSTP, SIG_IGN);
//         signal(SIGTTIN, SIG_IGN);
//         signal(SIGTTOU, SIG_IGN);
//     }
// }

// void sh_destroy(struct shell *sh) {
//     // if (sh->prompt) {
//     //     free(sh->prompt);
//     //     sh->prompt = NULL;
//     // }
//     UNUSED(sh);
//     clear_history();
//     // if (sh->prompt) {
//     //     free(sh->prompt);
//     //     sh->prompt = NULL;
//     // }
//     // free(sh);
//     // UNUSED(sh);


//     // clear_history();
//     // if (sh->prompt) {
//     //     free(sh->prompt);
//     //     sh->prompt = NULL;
//     // }
//     // clear_history();
// }



// // #include "lab.h"
// // #include <stdio.h>
// // #include <string.h>
// // #include <unistd.h>
// // #include <sys/types.h>
// // #include <sys/wait.h>
// // #include <pwd.h>
// // #include <readline/readline.h>
// // #include <readline/history.h>
// // #include <signal.h>

// // char *get_prompt(const char *env) {
// //     char *prompt = getenv(env);
// //     if (!prompt) {
// //         prompt = "shell>";
// //     }
// //     return strdup(prompt);
// // }

// // int change_dir(char **dir) {
// //     if (dir[1] == NULL) {
// //         struct passwd *pw = getpwuid(getuid());
// //         if (!pw) {
// //             perror("getpwuid");
// //             return -1;
// //         }
// //         return chdir(pw->pw_dir);
// //     }
// //     return chdir(dir[1]);
// // }

// // char **cmd_parse(const char *line) {
// //     if (!line) return NULL;
// //     char **args = malloc((sysconf(_SC_ARG_MAX) / 2) * sizeof(char *));
// //     if (!args) return NULL;

// //     char *line_copy = strdup(line);
// //     if (!line_copy) {
// //         free(args);
// //         return NULL;
// //     }

// //     char *token = strtok(line_copy, " ");
// //     int i = 0;
// //     while (token) {
// //         args[i++] = strdup(token);
// //         token = strtok(NULL, " ");
// //     }
// //     args[i] = NULL;

// //     free(line_copy);
// //     return args;
// // }

// // void cmd_free(char **cmd) {
// //     if (!cmd) return;
// //     for (int i = 0; cmd[i]; i++) {
// //         free(cmd[i]);
// //     }
// //     free(cmd);
// // }

// // char *trim_white(char *line) {
// //     if (!line) return NULL;
// //     while (isspace(*line)) line++;
// //     if (*line == 0) return line;
// //     char *end = line + strlen(line) - 1;
// //     while (end > line && isspace(*end)) end--;
// //     *(end + 1) = '\0';
// //     return line;
// // }

// // bool do_builtin(struct shell *sh, char **argv) {
// //     if (!argv[0]) return false;
// //     if (strcmp(argv[0], "exit") == 0) {
// //         sh_destroy(sh);
// //         exit(0);
// //     }
// //     if (strcmp(argv[0], "cd") == 0) {
// //         if (change_dir(argv) != 0) {
// //             perror("cd");
// //         }
// //         return true;
// //     }
// //     if (strcmp(argv[0], "history") == 0) {
// //         HIST_ENTRY **history_entries = history_list();
// //         if (history_entries) {
// //             for (int i = 0; history_entries[i]; i++) {
// //                 printf("%d %s\n", i + history_base, history_entries[i]->line);
// //             }
// //         }
// //         return true;
// //     }
// //     return false;
// // }

// // void sh_init(struct shell *sh) {
// //     sh->shell_terminal = STDIN_FILENO;
// //     sh->shell_is_interactive = isatty(sh->shell_terminal);
// //     if (sh->shell_is_interactive) {
// //         while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
// //             kill(-sh->shell_pgid, SIGTTIN);
// //         }
// //         signal(SIGINT, SIG_IGN);
// //         signal(SIGQUIT, SIG_IGN);
// //         signal(SIGTSTP, SIG_IGN);
// //         signal(SIGTTIN, SIG_IGN);
// //         signal(SIGTTOU, SIG_IGN);
// //     }
// // }

// // void sh_destroy(struct shell *sh) {
// //     UNUSED(sh);
// //     clear_history();
// // }

// // void parse_args(int argc, char **argv) {
// //     int opt;
// //     while ((opt = getopt(argc, argv, "v")) != -1) {
// //         if (opt == 'v') {
// //             printf("Shell version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
// //             exit(0);
// //         }
// //     }
// // }


#include "lab.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

#define DEFAULT_PROMPT "shell>"


// Version handling
void parse_args(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "v")) != -1) {
        if (opt == 'v') {
            printf("Shell Version %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
            exit(0);
        }
    }
}

// Get prompt from environment
char *get_prompt(const char *env) {
    // char *prompt = getenv(env);
    // return prompt ? strdup(prompt) : strdup(DEFAULT_PROMPT);

        char *prompt = getenv(env);
        if (!prompt) {
            prompt = "shell>";
        }
        return strdup(prompt);
}

// Change directory command
int change_dir(char **dir) {
    // if (!dir || !dir[0]) {
    //     const char *home = getenv("HOME");
    //     if (!home) {
    //         struct passwd *pw = getpwuid(getuid());
    //         home = pw ? pw->pw_dir : NULL;
    //     }
    //     if (!home) {
    //         fprintf(stderr, "cd: HOME not found\n");
    //         return -1;
    //     }
    //     return chdir(home);
    // }
    // if (chdir(dir[0]) != 0) {
    //     perror("cd");
    //     return -1;
    // }
    // return 0;


    if (!dir || !dir[0]) {
        const char *home = getenv("HOME");
        if (!home) {
            struct passwd *pw = getpwuid(getuid());
            home = pw ? strdup(pw->pw_dir) : NULL;  // strdup to match potential getenv() allocation
        }
        if (!home) {
            fprintf(stderr, "cd: HOME not found\n");
            return -1;
        }

        int result = chdir(home);
        
        if (!getenv("HOME")) free((char *)home);  // Free home if allocated via getpwuid

        return result;
    }

    if (chdir(dir[0]) != 0) {
        perror("cd");
        return -1;
    }

    return 0;
}

// Trim whitespace from input
char *trim_white(char *line) {
    if (!line) return NULL;
    while (*line == ' ' || *line == '\t') line++;
    char *end = line + strlen(line) - 1;
    while (end > line && (*end == ' ' || *end == '\t')) *end-- = '\0';
    return line;
}

// Parse command into arguments
char **cmd_parse(const char *line) {
    if (!line) return NULL;
    long arg_max = sysconf(_SC_ARG_MAX);
    char **args = malloc((arg_max + 1) * sizeof(char *));
    if (!args) return NULL;
    char *copy = strdup(line);
    char *token = strtok(copy, " ");
    int i = 0;
    while (token && i < arg_max) {
        args[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    free(copy);
    // free(token);
    return args;
}

// Free parsed command
void cmd_free(char **line) {
    if (!line) return;
    for (int i = 0; line[i]; i++) free(line[i]);
    free(line);

    // char *input = readline(prompt);
    // char **args = cmd_parse(input);
    // if (args) {
    //     if (!do_builtin(&sh, args)) {
    //         // Execute command logic here...
    //     }
    //     cmd_free(args);  // Prevent memory leak
    // }
    // free(input); 
}

// Handle built-in commands
// bool do_builtin(struct shell *sh, char **argv) {
//     if (!argv || !argv[0]) return false;
//     if (strcmp(argv[0], "exit") == 0) {
//         sh_destroy(sh);
//         exit(0);
//     } else if (strcmp(argv[0], "cd") == 0) {
//         return change_dir(&argv[1]) == 0;
//     } else if (strcmp(argv[0], "history") == 0) {
//         HIST_ENTRY **history_entries = history_list();
//         if (history_entries) {
//             for (int i = 0; history_entries[i]; i++) {
//                 printf("%d %s\n", i + history_base, history_entries[i]->line);
//             }
//         }
//         return true;
//     }
//     return false;
// }

bool do_builtin(struct shell *sh, char **argv) {
    if (!argv || !argv[0]) return false;

    bool handled = false;

    if (strcmp(argv[0], "exit") == 0) {
        sh_destroy(sh);
        cmd_free(argv);  // Free command arguments before exiting
        exit(0);
    } else if (strcmp(argv[0], "cd") == 0) {
        handled = (change_dir(&argv[1]) == 0);
    } else if (strcmp(argv[0], "history") == 0) {
        HIST_ENTRY **history_entries = history_list();
        if (history_entries) {
            for (int i = 0; history_entries[i]; i++) {
                printf("%d %s\n", i + history_base, history_entries[i]->line);
            }
        }
        handled = true;
    }

    cmd_free(argv);  // Free parsed arguments before returning
    return handled;
}

// Initialize shell
void sh_init(struct shell *sh) {
    sh->shell_terminal = STDIN_FILENO;
    sh->shell_is_interactive = isatty(sh->shell_terminal);
    if (sh->shell_is_interactive) {
        while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
            kill(-sh->shell_pgid, SIGTTIN);
        }
        sh->shell_pgid = getpid();
        setpgid(sh->shell_pgid, sh->shell_pgid);
        tcsetpgrp(sh->shell_terminal, sh->shell_pgid);
        tcgetattr(sh->shell_terminal, &sh->shell_tmodes);
    }
}

// Cleanup shell
void sh_destroy(struct shell *sh) {
    UNUSED(sh);
    // clear_history();
    // if (sh->prompt) free(sh->prompt);
    clear_history();
}

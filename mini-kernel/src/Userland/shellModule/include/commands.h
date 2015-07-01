#ifndef _COMMANDS_H
#define _COMMANDS_H

int exec_echo(int argc, char **argv);
int exec_help(int argc, char **argv);
int exec_set_rtc(int argc, char **argv);
int exec_print_rtc(int argc, char **argv);
int exec_test_ss(int argc, char **argv);
int exec_set_ss(int argc, char **argv);
int exec_heap_test(int argc, char **argv);

int args_shift(int n, int *argc, char ***argv);

typedef struct {
    char *name;
    int (*execute)(int argc, char **argv);
} command_t;

#endif
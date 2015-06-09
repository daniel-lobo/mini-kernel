#ifndef _SH_H
#define _SH_H

void sh_init(void);
void sh_show_prompt(void);
int sh_do_command(char buf[]);
void sh_parse_args(char buf[], int *argc, char *argv[]);
void display_prompt();

int execute(int argc, char **argv);
void sh_clean_buffer(char * buf);

#endif

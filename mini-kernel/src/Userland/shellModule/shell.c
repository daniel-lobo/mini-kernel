#include <stdint.h>
#include "./include/commands.h"
#include "./include/shell.h"
#include "../libcModule/include/stdio.h"
#include "../libcModule/include/ctype.h"
#include "../libcModule/include/string.h"
#include "../syscallModule/include/syscall.h"
#include "./include/color.h"

#define MAX_ARGS 256
#define MAX_ARGC 256

#define PROMPT "root@ArquiOS"

extern char bss;
extern char endOfBinary;

command_t commands[] = {
    { "echo", exec_echo },
    { "help", exec_help },
    { "srtc", exec_set_rtc },
    { "prtc", exec_print_rtc },
    { "sstest", exec_test_ss },
    { "sstime", exec_set_ss },
    { NULL, NULL }
};

int
main(void)
{

    memset(&bss, 0, &endOfBinary - &bss);
    sh_show_prompt();
	return 0;
}

void
sh_show_prompt()
{
    display_prompt();
    char buf[MAX_ARGS];

    while (1)
    {
        gets(buf, sizeof(buf));
        trim(buf);
        if (strlen(buf) > 0)
        {
            sc_set_format(LIGHT_GREY);
            sh_do_command(buf);            
            sh_clean_buffer(buf);
            display_prompt();
        }
    }
}

void
sh_clean_buffer(char * buf){
    memset(buf, 0, MAX_ARGS);

}

void
display_prompt()
{
    if (!sc_screensaver_status())
    {
      sc_set_format(RED);
    printf("%s>", PROMPT);
    sc_set_format(WHITE);       
    }

}

int
sh_do_command(char buf[])
{
    int argc = 0;
    char *argv[MAX_ARGC];
    sh_tokenize(buf, &argc, argv);

    return execute(argc, argv);
}

int
execute(int argc, char **argv)
{
    int i = 0;

    while (commands[i].name != NULL)
    {
        if (strcmp(argv[0], commands[i].name) == 0)
        {
            return commands[i].execute(argc, argv);
        }
        i++;
    }

    printf("%s: Not a valid command\n", argv[0]);
    return 1;
}

void
sh_tokenize(char buf[], int *argc, char *argv[])
{
    char ch;
    enum state { SPACE, TOKEN, QUOTED_START, QUOTED } state = SPACE;

    while ((ch = *buf) != '\0')
    {
        switch (state)
        {
            case SPACE:
                if (ch == '"')
                {
                    state = QUOTED_START;
                }
                else if (!isspace(ch))
                {
                    /* TODO: Print error: Max arguments limit reached */
                    if (*argc == MAX_ARGC)
                    {
                        return;
                    }
                    state = TOKEN;
                    argv[(*argc)++] = buf;
                }
                break;
            case TOKEN:
                if (isspace(ch))
                {
                    state = SPACE;
                    *buf = '\0';
                }
                break;
            case QUOTED_START:
                if (ch == '"')
                {
                    state = SPACE;
                }
                else
                {
                    /* TODO: Print error: Max arguments limit reached */
                    if (*argc == MAX_ARGC)
                    {
                        return;
                    }
                    argv[(*argc)++] = buf;
                    state = QUOTED;
                }
                break;
            case QUOTED:
                if (ch == '"')
                {
                    state = SPACE;
                    *buf = '\0';
                }
                break;
        }
        buf++;
    }
}


#include "../clibs/include/stdio.h"
#include "./include/commands.h"

int exec_echo(int argc, char **argv)
{
    args_shift(1, &argc, &argv);

    while (argc)
    {
        printf("%s", *argv++);
        if (--argc != 0)
            putchar(' ');
    }
    
    putchar('\n');

	return 0;
}

int
exec_help(int argc, char **argv)
{
	args_shift(1, &argc, &argv);

    printf("help         print this message\n");
    printf("echo         prints the arguments to stdout\n");    
    printf("prtc         print time\n");
    printf("srtc         set time\n");
    printf("sstest       display screensaver\n");
    printf("sstime       set screensaver timer\n");

    return 0;
}

int 
exec_set_rtc(int argc, char **argv)
{
	return 0;
}

int
exec_print_rtc(int argc, char **argv)
{
	return 0;
}

int
exec_test_ss(int argc, char **argv)
{
	return 0;
}

int
exec_set_ss(int argc, char **argv)
{
	return 0;
}

int
args_shift(int n, int *argc, char ***argv)
{
    if (*argc >= n)
    {
        *argc -= n;
        *argv += n;
    }
    return *argc - n;
}
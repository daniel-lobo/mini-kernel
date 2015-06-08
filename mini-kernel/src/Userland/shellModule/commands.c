#include <stdint.h>
#include "../libcModule/include/stdio.h"
#include "../libcModule/include/ctype.h"
#include "../libcModule/include/stdlib.h"
#include "./include/commands.h"
#include "../syscallModule/include/syscall.h"

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
    args_shift(1, &argc, &argv);

    if (argc != 3){
        printf("Incorrect clock format: srtc H M $S\n");
    }

    int hour = atoi(*argv++);
    int min = atoi(*argv++);
    int sec = atoi(*argv++);

    sc_rtc_set(hour * 10000 + min * 100 + sec);

    return 0;
}

int
exec_print_rtc(int argc, char **argv)
{
    int rtc = sc_rtc_get();

    int hour = rtc / 10000;
    int min = (rtc / 100) % 100;
    int sec = rtc % 100;

    printf("%d:%d:%d\n", hour, min, sec);

	return 0;
}

int
exec_test_ss(int argc, char **argv)
{
    sc_screensaver_test();
	return 0;
}

int
exec_set_ss(int argc, char **argv)
{
    args_shift(1, &argc, &argv);
    sc_set_screensaver_timer(atoi(*argv));

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
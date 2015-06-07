#include "./include/syscall.h"
#include "./include/rtc.h"
#include "./include/kasm.h"
#include "./include/video.h"
#include "./include/keyboard.h"
#include <stdint.h>

void write_handler(int fd, char * s, int len)
{
	video_write_char(s[0]);
}

uint32_t read_handler(int fd, char * s, int len)
{
	if (!(peek() - s[0]))
	{
		return 1;
	}
	return 0;
}

uint32_t rtc_handler()
{
	return rtc_time();
}

void rtc_set_handler(uint32_t t)
{
	rtc_set_time(t);
}

void ss_test_handler()
{
	video_set_screensaver();
}

void ss_set_handler(uint64_t t)
{
	set_ss_timer(t);
}

void format_set_handler(COLOR color)
{
	set_format(color, BLACK);
}

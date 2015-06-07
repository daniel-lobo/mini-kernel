#include "./include/syscall.h"
#include "./include/rtc.h"
#include "./include/kasm.h"
#include "./include/video.h"
#include <stdint.h>

void write_handler(int fd, char * s, int len)
{
	video_write_string(s);
}

void read_handler(int fd, char * s, int len)
{

}

void rtc_handler()
{

}

void rtc_set_handler(uint32_t t)
{

}

void ss_test_handler()
{

}

void ss_set_handler(uint64_t t)
{

}

void format_set_handler()
{

}

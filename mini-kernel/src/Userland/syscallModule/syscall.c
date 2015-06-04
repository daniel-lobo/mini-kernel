#include <stdint.h>
#include "syscall.h"

void
sc_write(int fd, char * s, int len)
{
	_syscall((uint64_t) SYSCALL_WRITE, (uint64_t)fd, (uint64_t)s, (uint64_t)len);
}

int
sc_read(int fd, char * s, int len)
{
	return _syscall((uint64_t) SYSCALL_READ, (uint64_t)fd, (uint64_t)s, (uint64_t)len);
}

uint32_t
sc_rtc_get()
{
	return _syscall((uint64_t) SYSCALL_RTC);
}

void
sc_rtc_set(uint32_t t)
{
	_syscall((uint64_t) SYSCALL_RTC_SET, (uint64_t)t);
}

void
sc_screensaver_test()
{
	_syscall((uint64_t) SYSCALL_SS);
}

void
sc_set_screensaver_timer(uint64_t t)
{
	_syscall((uint64_t) SYSCALL_SS_SET, t);
}
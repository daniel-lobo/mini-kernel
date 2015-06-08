#ifndef _SC_UL_
#define _SC_UL_

#define SYSCALL_WRITE 	0
#define SYSCALL_READ	1
#define SYSCALL_RTC		2
#define SYSCALL_RTC_SET	3
#define SYSCALL_SS		4
#define SYSCALL_SS_SET	5
#define SYSCALL_SET_COLOR 6
#define SYSCALL_IS_SS_ON 7
#define SYSCALL_ERROR  -1

uint64_t _syscall(uint64_t callid, ...);
void _hlt();

int sc_write(int fd, char* s, int len);
int sc_read(int fd, char* s, int len);
uint32_t sc_rtc_get();
void sc_rtc_set(uint32_t t);
void sc_screensaver_test();
void sc_set_screensaver_timer(uint64_t t);
void sc_set_format(int color);
int sc_screensaver_status();

#endif
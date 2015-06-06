#ifndef _SYSCALL_H
#define _SYSCALL_H

#define SYSCALL_WRITE 	0
#define SYSCALL_READ	1
#define SYSCALL_RTC		2
#define SYSCALL_RTC_SET	3
#define SYSCALL_SS		4
#define SYSCALL_SS_SET	5
#define SYSCALL_SET_COLOR	6
#define SYSCALL_ERROR  -1

int syscall_handler(int number,
					int param1,
					int param2,
					int param3,
					int param4,
					int param5);

#endif



#include "./include/syscall.h"
#include "./include/rtc.h"
#include "./include/kasm.h"
#include <stdint.h>

int
syscall_handler(int number,
					int param1,
					int param2,
					int param3,
					int param4,
					int param5)

{
	int retval;

	switch (number)
	{
	case SYSCALL_WRITE:
		/* TODO: print to scr
		retval = ;
		*/		
		break;
	case SYSCALL_READ:
		/* TODO: read from kb
		retval = ;
		 */
		break;
	case SYSCALL_RTC:
		retval = (uint32_t)rtc_time();
		break;
	case SYSCALL_RTC_SET:
		/* TODO: set rtc time
		retval = (int)rtc_set_time();*/
		break;
	case SYSCALL_SS:
		/* TODO: display ss
		retval = (int)rtc_time();*/
		break;
	case SYSCALL_SS_SET:
		/* TODO: set ss timer
		retval = (int)rtc_time();*/
		break;
	default:
        /* TODO: Print error explanation */
		retval = SYSCALL_ERROR;
		break;
	}

	return retval;
}

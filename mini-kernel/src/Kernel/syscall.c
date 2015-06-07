#include "./include/syscall.h"
#include "./include/rtc.h"
#include "./include/kasm.h"
#include "./include/video.h"
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
		    
		video_write_char((char) param1);
		retval = 0;	
		break;
	case SYSCALL_READ:
		/* TODO: read from kb
		retval = ;
		 */
		break;
	case SYSCALL_RTC:
		retval = rtc_time();
		break;
	case SYSCALL_RTC_SET:
		retval = rtc_set_time(param1);
		break;
	case SYSCALL_SS:
		/* TODO: display ss
		retval = (int)rtc_time();*/
		break;
	case SYSCALL_SS_SET:
		/* TODO: set ss timer
		retval = (int)rtc_time();*/
		break;
	case SYSCALL_SET_COLOR:
		set_format(COLOR_WHITE, COLOR_BLACK);
		retval = 0;
		break;
	default:
        /* TODO: Print error explanation */
		retval = SYSCALL_ERROR;
		break;
	}

	return retval;
}

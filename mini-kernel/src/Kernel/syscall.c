#include "./include/syscall.h"
#include "./include/rtc.h"
#include "./include/kasm.h"

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
		/* TODO: syscalls 
		retval = ;
		*/		
		break;
	case SYSCALL_READ:
		/* TODO: syscalls
		retval = ;
		 */
		break;
	case SYSCALL_TIME:
		retval = (int)rtc_time();
		break;
	default:
        /* TODO: Print error explanation */
		retval = SYSCALL_ERROR;
		break;
	}

	return retval;
}

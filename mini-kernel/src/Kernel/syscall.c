#include "./include/syscall.h"
#include "./include/rtc.h"
#include "./include/kasm.h"
#include "./include/video.h"
#include "./include/keyboard.h"
#include <stdint.h>

uint32_t write_handler(int fd, char * s, int len)
{
	unsigned int i = 0;    
    while( i < len){
		char key = ((char*)s)[i++];
		if(fd == STDIN_FILENO){
			sendToBuffer(key);
		} else if (fd == STDOUT_FILENO){
			video_write_char(key);
		}
	}
    return i;
}

uint32_t read_handler(int fd, char * s, int len)
{
	int readCharacters = 0;
    if(fd == STDIN_FILENO){
        int aux;
        while(readCharacters < len){
        	aux = pop();
        	if (bufferIsFull()){
        		clean_buffer();
        	}
		    if(aux != -1){
	            s[readCharacters++] = aux;
	        }
		}
    }

    return readCharacters;
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

int ss_status_handler()
{
	return is_ss_on();
}

#ifndef _RTC_
#define _RTC_
#include <stdint.h>

int read_cmos_register(unsigned int number);
int get_update_in_progress_flag(void);

uint64_t rtc_time();
int rtc_set_time(uint64_t time);
void rtc_fix();

#endif

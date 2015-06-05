#ifndef _RTC_
#define _RTC_

char read_cmos_register(unsigned int number);
int get_update_in_progress_flag(void);

int rtc_time(void);
int rtc_set_time(int time);

#endif

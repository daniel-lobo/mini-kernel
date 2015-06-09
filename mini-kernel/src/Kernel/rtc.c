#include "./include/rtc.h"
#include "./include/kasm.h"

#define CMOS_ADDRESS 0x70
#define CMOS_DATA    0x71

char
read_cmos_register(unsigned int number)
{
    _outb(number, CMOS_ADDRESS);
    return _inb(CMOS_DATA);
}

int
get_update_in_progress_flag(void)
{
    _outb(0x0A, CMOS_ADDRESS);
    return _inb(CMOS_DATA) & 0x80;
}

int
rtc_time(void)
{
    while (get_update_in_progress_flag());

    int second = (int) read_cmos_register(0x00);
    int minute = (int) read_cmos_register(0x02);
    int hour = (int) read_cmos_register(0x04);

    int registerB = read_cmos_register(0x0B);
 
    // Convert BCD to binary values if necessary
 
    if (!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
    }

    hour -= 3;

    if (hour < 0){
        hour += 24;
    }

    return second + minute * 100 + hour * 10000;
}

int
rtc_set_time(int time)
{
    int hour = time / 10000;
    int min = (time / 100) % 100;
    int sec = time % 100;
    
    _outb(0x00, CMOS_ADDRESS);
    _outb(sec, CMOS_DATA);
    _outb(0x02, CMOS_ADDRESS);
    _outb(min, CMOS_DATA);
    _outb(0x04, CMOS_ADDRESS);
    _outb(hour, CMOS_DATA);
    return 0;
}

void
rtc_startup_fix()
{
    int rtc = rtc_time();

    int hour = (rtc / 10000) - 3;
    int min = (rtc / 100) % 100;
    int sec = rtc % 100;

    if (hour < 0){
        hour += 24;
    }

    rtc_set_time(sec + min * 100 + hour * 10000);
}
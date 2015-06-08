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
    int sec = (int) read_cmos_register(0x00);
    int min = (int) read_cmos_register(0x02);
    int hour = (int) read_cmos_register(0x04);

    return sec + min * 100 + hour * 10000;
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
    int tim = rtc_time();
    rtc_set_time(tim - 3*10000);
}
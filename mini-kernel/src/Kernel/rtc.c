#include "./include/rtc.h"
#include "./include/kasm.h"


#define CMOS_ADDRESS 0x70
#define CMOS_DATA    0x71

int
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

uint64_t
rtc_time()
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

    return second + minute * 100 + hour * 10000;
}

int
rtc_set_time(uint64_t time)
{
    int hour = (int)  (time / 10000);
    int min = (int) (time / 100) % 100;
    int sec = (int) time % 100;

    int registerB = read_cmos_register(0x0B);

    if (!(registerB & 0x04)) {
        sec = ((((sec) / 10) * 16) | (sec % 10));
        min = ((((min) / 10) * 16) | (min % 10));
        hour = ((((hour) / 10) * 16) | (hour % 10));
    }

    _outb(0x00, CMOS_ADDRESS);
    _outb(sec, CMOS_DATA);
    _outb(0x02, CMOS_ADDRESS);
    _outb(min, CMOS_DATA);
    _outb(0x04, CMOS_ADDRESS);
    _outb(hour, CMOS_DATA);
    return 0;
}

void
rtc_fix()
{
    int time = rtc_time();

    int hour = (int)  (time / 10000) - 3;
    int minute = (int) (time / 100) % 100;
    int second = (int) time % 100;

    if (hour - 3 < 0) {
        hour += 24;
    }

    rtc_set_time(second + minute * 100 + hour * 10000);
}
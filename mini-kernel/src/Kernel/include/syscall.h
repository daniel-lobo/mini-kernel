#ifndef _SYSCALL_H
#define _SYSCALL_H
#include <stdint.h>

void pit_handler();
void keyboard_handler();
void write_handler();
void read_handler();
uint32_t rtc_handler();
void rtc_set_handler(uint32_t t);
void ss_test_handler();
void ss_set_handler();
void format_set_handler();

#endif



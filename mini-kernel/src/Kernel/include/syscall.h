#ifndef _SYSCALL_H
#define _SYSCALL_H
#include <stdint.h>

void pit_handler();
void keyboard_handler();
uint32_t write_handler();
uint32_t read_handler();
uint32_t rtc_handler();
void rtc_set_handler(uint32_t t);
void ss_test_handler();
void ss_set_handler();
void format_set_handler();
int ss_status_handler();

#endif



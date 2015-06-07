#include <stdint.h>
#include "./color.h"

#ifndef _VIDEO_
#define _VIDEO_

#define SCREEN_START 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

typedef struct SCREEN
{
	int x, y;
	uint8_t content[SCREEN_WIDTH * SCREEN_HEIGHT];
	uint8_t format[SCREEN_WIDTH * SCREEN_HEIGHT];
} SCREEN;

void video_init();
void video_clear();

void ss_clock();

void set_ss_timer(uint64_t t);

void video_write_string(char * c);
void video_write_char(char c);

void video_put_char(int pos, char c);
void video_nl();
void screen_mv_pos_backwards();
void screen_mv_pos_forwards();

void video_update_cursor();
void video_update_screen();
void video_scroll();

void set_format(COLOR fg, COLOR bg);
void clear_cur_screen(void);
int get_cur_position();

void video_init_screensaver();
void video_set_screensaver();
void video_set_terminal();
void video_save_screen();

#endif


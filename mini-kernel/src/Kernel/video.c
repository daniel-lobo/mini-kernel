#include "./include/video.h"

SCREEN screens[3];
SCREEN cur_screen;
char cur_format;

void
video_init()
{
	cur_screen = screens[0];
	cur_screen.x = 0;
	cur_screen.y = 0;
}

void
video_clear()
{
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < SCREEN_WIDTH; j++)
		{
			video_put_char(' ');
		}
	}
	cur_screen.x = 0;
	cur_screen.y = 0;
	video_update_cursor();
}

void
video_put_char(int x, int y, char content, char format)
{
	if (SCREEN_HEIGHT * cur_screen.y + cur_screen.x > SCREEN_WIDTH * SCREEN_HEIGHT)
	{
		video_scroll();
	}
	cur_screen.content[SCREEN_HEIGHT * cur_screen.y + cur_screen.x] = content;
	cur_screen.format[SCREEN_HEIGHT * cur_screen.y + cur_screen.x] = format;
}

void
video_update_cursor()
{
	unsigned short position = (cur_screen.x * SCREEN_WIDTH) + cur_screen.y;
 
    _outb(0x0F, 0x3D4);
    _outb((unsigned char)(position & 0xFF), 0x3D5);

    _outb(0x0E, 0x3D5);
    _outb((unsigned char)((position >> 8) & 0xFF), 0x3D5);
}

void
video_update_screen()
{
	int i, j = 0;
	for (i = 0; i < SCREEN_WIDTH; i++) 
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			SCREEN_START[cur_screen.y * SCREEN_WIDTH + cur_screen.x] = cur_screen.content[cur_screen.y * SCREEN_WIDTH + cur_screen.x]);
		}
	}
}

uint8_t
formatted_char(COLOR fg, COLOR bg)
{
	return (((0xF0 & bg) >> 4) << 4) | (0x0F & fg));
}

void
video_write_char(char content, char format)
{

}

void
video_write_string(char * content, char format)
{

}

void
video_nl(char * content, char format)
{

}

void
video_init_screensaver()
{
	
}

void
video_set_screensaver()
{

}

void
video_set_terminal()
{

}

void
video_backup_screen()
{

}
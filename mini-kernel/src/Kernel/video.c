#include "./include/video.h"


SCREEN screens[3];
SCREEN cur_screen;

char cur_format;

uint16_t * vga_hw = SCREEN_START;

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
	clear_cur_screen();
	video_update_cursor();
	video_update_screen();
}

void
clear_cur_screen(void)
{
	cur_screen.x = 0;
	cur_screen.y = 0;
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < SCREEN_WIDTH; j++)
		{
			cur_screen.content[cur_screen.y * SCREEN_WIDTH + cur_screen.x] = ' ';
			cur_screen.format[cur_screen.y * SCREEN_WIDTH + cur_screen.x] = 0x00;
		}
	}
}

void
video_write(char content)
{
	video_put_char(c, cur_format);
}

void
video_put_char(char content)
{
	if (SCREEN_HEIGHT * cur_screen.y + cur_screen.x != SCREEN_WIDTH * SCREEN_HEIGHT)
	{
		switch(content){
        case '\n':
            video_nl();
            break;
        case '\t':
            cur_screen.content[SCREEN_HEIGHT * cur_screen.y + cur_screen.x] = '      ';
			cur_screen.format[SCREEN_HEIGHT * cur_screen.y + cur_screen.x] = cur_format;
            break;
        case '\b':
            cur_screen.content[SCREEN_HEIGHT * cur_screen.y + cur_screen.x] = ' ';
			cur_screen.format[SCREEN_HEIGHT * cur_screen.y + cur_screen.x] = cur_format;
            break;
        default:
            cur_screen.content[SCREEN_HEIGHT * cur_screen.y + cur_screen.x] = content;
			cur_screen.format[SCREEN_HEIGHT * cur_screen.y + cur_screen.x] = cur_format;
            break;
    	}
		video_update_cursor();
		video_update_screen();
	}
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
	int i = 0, j = 0;
	for (i = 0; i < SCREEN_WIDTH; i++) 
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			vga_hw[(cur_screen.y * SCREEN_WIDTH + cur_screen.x) * 2] = cur_screen.content[cur_screen.y * SCREEN_WIDTH + cur_screen.x]);
			vga_hw[(cur_screen.y * SCREEN_WIDTH + cur_screen.x) * 2 + 1] = cur_screen.format[cur_screen.y * SCREEN_WIDTH + cur_screen.x]);
		}
	}
}

uint8_t
get_format(COLOR fg, COLOR bg)
{
	return (  ((0xF0 & bg) >> 4) << 4) | (0x0F & fg) );
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
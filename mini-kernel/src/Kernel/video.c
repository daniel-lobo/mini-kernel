#include "./include/video.h"
#include "./include/kasm.h"
#include "./include/naiveConsole.h"

SCREEN screens[3];
SCREEN cur_screen;

uint8_t cur_format = 0x07;

uint8_t * vga_hw = (uint8_t *) SCREEN_START;

void
video_init()
{
	video_clear();
	cur_screen = screens[0];
	cur_screen.x = 0;
	cur_screen.y = 0;
}

void
video_clear()
{
	clear_cur_screen();
	video_update_screen();
	set_format(COLOR_WHITE, COLOR_BLACK);
}

void
clear_cur_screen(void)
{
	cur_screen.x = 0;
	cur_screen.y = 0;
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			cur_screen.content[j * SCREEN_WIDTH + i] = ' ';
			cur_screen.format[j * SCREEN_WIDTH + i] = 0x00;
		}
	}
}

void
video_write_string(char * content)
{
	int i;
	for (i = 0; content[i] != 0; i++)
	{
		video_write_char(content[i]);
	}
	video_update_screen();
}

void
video_write_char(char content)
{	
	switch(content){
    case '\n':
        video_nl();
        break;
    case '\t':
        video_put_char(get_cur_position(), ' ');
		screen_mv_pos_forwards();
        break;
    case '\b':
    	video_put_char(get_cur_position() - 1, ' ');
		screen_mv_pos_backwards();
        break;
    default:
        video_put_char(get_cur_position(), content);
		screen_mv_pos_forwards();
        break;
    }
	video_update_screen();
}

int
get_cur_position()
{
	return SCREEN_WIDTH * cur_screen.y + cur_screen.x;
}

void
video_put_char(int pos, char content)
{
	if (pos >= 0 && pos < SCREEN_WIDTH * SCREEN_HEIGHT)
	{
		cur_screen.content[pos] = content;
		cur_screen.format[pos] = cur_format;
		video_update_screen();
	}
}

void
screen_mv_pos_backwards()
{
	if (cur_screen.x == 0 )
	{
		if (cur_screen.y != 0)
		{
			cur_screen.x = SCREEN_WIDTH;
			cur_screen.y = cur_screen.y - 1;
		}
	}
	else
	{
		cur_screen.x = cur_screen.x - 1;
	}
}

void
screen_mv_pos_forwards()
{
	if (cur_screen.x == SCREEN_WIDTH )
	{
		if (cur_screen.y != SCREEN_HEIGHT)
		{
			cur_screen.x = 0;
			cur_screen.y = cur_screen.y + 1;
		}
		else
		{
			video_scroll();
		}
	}
	else
	{
		cur_screen.x = cur_screen.x + 1;
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
	vga_hw[(SCREEN_HEIGHT * SCREEN_WIDTH -1) * 2 ] = 'x';
	int i, j;
	for (i = 0; i < SCREEN_WIDTH; i++) 
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			vga_hw[(j * SCREEN_WIDTH + i) * 2] = cur_screen.content[j * SCREEN_WIDTH + i];
			vga_hw[(j * SCREEN_WIDTH + i) * 2  + 1] = cur_screen.format[j * SCREEN_WIDTH + i];
		}
	}
}

void
set_format(COLOR fg, COLOR bg)
{
	cur_format = (bg << 4) | (fg & 0x0F);
}

void
video_nl()
{
	int dist_to_border = SCREEN_WIDTH - cur_screen.x;
	int i;
	for (i = 1; i < dist_to_border; i++){
		cur_screen.content[cur_screen.y * SCREEN_WIDTH + cur_screen.x + i] = ' ';
		cur_screen.format[cur_screen.y * SCREEN_WIDTH + cur_screen.x + i] = 0x00;
	}
	if (cur_screen.y < SCREEN_HEIGHT)
	{
		cur_screen.x = 0;
		cur_screen.y = cur_screen.y + 1;
	}
	else
	{
		video_scroll();
	}
}

void
video_scroll()
{
	cur_screen.x = 0;
	cur_screen.y = SCREEN_HEIGHT;
	int i;

	for (i = SCREEN_WIDTH; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		cur_screen.content[i - SCREEN_WIDTH] = cur_screen.content[i];
		cur_screen.format[i - SCREEN_WIDTH] = cur_screen.content[i];
	}
	for (i = SCREEN_WIDTH * SCREEN_HEIGHT - SCREEN_WIDTH; i < SCREEN_WIDTH * SCREEN_HEIGHT; i ++)
	{
		cur_screen.content[i] = ' ';
		cur_screen.format[i] = 0x00;
	}

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
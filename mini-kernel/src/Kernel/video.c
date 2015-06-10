#include "./include/video.h"
#include "./include/kasm.h"
#include "./include/color.h"

SCREEN screens[2];
SCREEN cur_screen;
int ss_on = 0;

uint8_t cur_format = 0x00;

uint8_t * vga_hw = (uint8_t *) SCREEN_START;

// 30 seg default
uint64_t scrensaver_timer = 540;
uint64_t aux = 540;

void
video_init()
{
	video_clear();
	cur_screen = screens[1];
	cur_screen.x = 0;
	cur_screen.y = 0;
}

void
video_clear()
{
	clear_cur_screen();
	video_update_screen();
	set_format(WHITE, BLACK);
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
video_write_char(unsigned char content)
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
		if (cur_screen.y > 0)
		{
			cur_screen.x = SCREEN_WIDTH - 1;
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
	if (cur_screen.x == SCREEN_WIDTH - 1)
	{
		if (cur_screen.y + 1 < SCREEN_HEIGHT)
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
	unsigned short position = (cur_screen.y * SCREEN_WIDTH) + cur_screen.x + 1;
 
    _outb(0x3D4, 0x0F);
    _outb((unsigned char)(position & 0xFF), 0x3D5);

    _outb(0x3D5, 0x0E);
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
	//video_update_cursor();
}

void
set_format(COLOR fg, COLOR bg)
{
	cur_format = (bg << 4) | (fg & 0x0F);
}

void
video_nl()
{
	if (cur_screen.y + 1 < SCREEN_HEIGHT)
	{
		cur_screen.x = 0;
		cur_screen.y = cur_screen.y + 1;
	}
	else
	{
		video_scroll();
	}
	int dist_to_border = SCREEN_WIDTH - cur_screen.x;
	int i;
	for (i = 1; i < dist_to_border; i++)
	{
		cur_screen.content[cur_screen.y * SCREEN_WIDTH + cur_screen.x + i] = ' ';
		cur_screen.format[cur_screen.y * SCREEN_WIDTH + cur_screen.x + i] = 0x00;
	}

}

void
video_scroll()
{
	
	int i;

	for (i = 0; i < SCREEN_WIDTH * (SCREEN_HEIGHT -1); i++)
	{
		cur_screen.content[i] = cur_screen.content[i + SCREEN_WIDTH];
		cur_screen.format[i] = cur_screen.format[i + SCREEN_WIDTH];
	}
	for (i = SCREEN_WIDTH * (SCREEN_HEIGHT -1); i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		cur_screen.content[i] = ' ';
		cur_screen.format[i] = 0x00;
	}
	cur_screen.x = 0;
	cur_screen.y = SCREEN_HEIGHT - 1;
}

void
video_init_screensaver()
{
	char * ss = "This is a screensaver. Press any key to leave.";
	int i, j = 16 + SCREEN_WIDTH * 13;

	for (i = j; ss[i - j] != '\0'; i++)
	{
		screens[0].content[i] = ss[i - j];
		screens[0].format[i] = 0x04;
	}
}

void
video_set_screensaver()
{
	video_save_screen();
	cur_screen = screens[0];
	video_update_screen();
	ss_on = 1;
}

void
video_set_terminal()
{
	if (is_ss_on())
	{
		cur_screen = screens[1];
		video_update_screen();
		ss_reset_clock();	
		ss_on = 0;
	}
}

void
video_refresh()
{
	ss_reset_clock();
	video_set_terminal();
}

void
video_save_screen()
{
	screens[1] = cur_screen;
}	

int
is_ss_on()
{
	if (ss_on)
	{
		return 1;
	}
	return 0;
}

void
ss_clock()
{
	scrensaver_timer--;
	if (scrensaver_timer < 0.0001)
	{
		video_set_screensaver();
	}
}

void
ss_reset_clock()
{
	scrensaver_timer = aux;
}

void
set_ss_timer(uint64_t t)
{
	scrensaver_timer = aux = t * 18;
}
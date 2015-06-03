#define VIDMEM_ADDRESS					0xB8000
#define CHAR_SIZE           		2
#define SCREEN_COLS         		80
#define SCREEN_ROWS							25
#define SCREEN_SIZE 						SCREEN_COLS*SCREEN_ROWS*CHAR_SIZE
#define LINE_SIZE	    					SCREEN_COLS*CHAR_SIZE

#define LAST_LINE	    					24*SCREEN_COLS*CHAR_SIZE
#define LAST_LINE_ADDRESS   		SCREEN_ADDRESS+LAST_LINE

// Colours:
#define WHITE_TXT     					0x07 // Black background, white letters
#define GREEN_SQUARE  					0xA7 // Green background, white letters
#define GREEN_TXT     					0x0A // Black background, green letters

// These might go in a separate file called stdio.h
#define STDIN  									2
#define STDERR 									3
#define TAB_LENGTH          		8


void clear_tty_screen();
char * itoa(int seconds);
void print_to_screen(char * string);
void print_to_scr(char * message, int background_color, int foreground_color);





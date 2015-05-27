// The text screen video memory for colour monitors resides at 0xB8000
// Check: http://wiki.osdev.org/Printing_To_Screen
#define VIDMEM_ADDRESS			0xB8000
#define MAIN_SCREEN_ROWS    15

// Colours:
#define WHITE_TXT     			0x07 // Black background, white letters
#define GREEN_SQUARE  			0xA7 // Green background, white letters
#define GREEN_TXT     			0x0A // Black background, green letters

// Screen properties:
#define MAIN_SCREEN_ROWS		25
#define MAIN_SCREEN_COLS 		80
#define CHAR_SIZE 					2
#define LINE_SIZE						80*2









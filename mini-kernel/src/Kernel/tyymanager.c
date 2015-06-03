#include "./include/ttymanager.h"

int writepos;
char * vidmem;
int maxpos;

void 
video_initialize(){
	writepos = 0;
	vidmem   = (char *)VIDMEM_ADDRESS;
	maxpos   = 0;
}

void 
writeToScreen(char key, int fd){
  int size;
  int * pos;

	vidmem = (char *)VIDMEM_ADDRESS;
	size   = SCREEN_SIZE;

  if(writepos == size){
  	scrollMainScreen();
	}

  insertKey(key, &writepos, vidmem);
  set_cursor(getrow(), getcol());
}

void 
reset_writepos(){
	writepos = 0;
}

void 
insertKey(char key, int * pos, char * vid){
  vid[(*pos)] = key;
  (*pos)++;

  vid[(*pos)] = WHITE_TXT;
  (*pos)++;
}

// paintLetter?


void 
scrollMainScreen(){
	vidmem   = (char *) VIDMEM_ADDRESS;
	int i    = LINE_SIZE;
	writepos = 0;

  while(i < SCREEN_SIZE){
    vidmem[writepos++] = vidmem[i++];
  }

  clearLine((char *) LAST_LINE_ADDRESS);
  writepos = LAST_LINE;
}

void 
clearLine(char * vidmem){
  int i = 0;

  while(i < LINE_SIZE){
		vidmem[i++] = ' ';
		vidmem[i++] = WHITE_TXT;
  }
}


void 
clear_tty_screen(){
  char * vidmem_address = (char *) VIDMEM_ADDRESS;
  unsigned int i = 0;

  while(i < SCREEN_SIZE){
      vidmem_address[i++] = ' ';
      vidmem_address[i++] = WHITE_TXT;
  };
}


void 
video_write(char c,int fd){
  switch(c){
    case '\n':
        video_enter(fd);
        break;
    case '\t':
        video_tab(fd);
        break;
    case '\b':
        video_backspace();
        break;
    default:
        writeToScreen(c,fd);
        break;
  }
}

void 
video_enter(int fd){
  while((writepos % 160) != 0){
      writepos += 2;
  }
  if(writepos == SCREEN_SIZE){
      scrollMainScreen();
  }
}

void video_tab(int fd){
	int i    = 0;
	int size = 0;
	int * pos;

	vidmem = (char *)VIDMEM_ADDRESS;
	size   = SCREEN_SIZE;
	pos    = &writepos;

  for(i = 0; i < TAB_LENGTH && i < size; i++){
      insertKey(' ',pos, vidmem);
  }
  set_cursor(getrow(),getcol());
}

void 
video_backspace(){
  vidmem = (char *)MAIN_SCREEN_ADDRESS;

  if(maxpos > 0){
    vidmem[--writepos] = WHITE_TXT;
    vidmem[--writepos] = ' ';
  }

  set_cursor(getrow(),getcol());
}

int getrow(){
  int response = ((int)(writepos/160)) + 10;
  
  if(response % 2 == 1){
      return response - 1 ;
  }

  return response;
}

int getcol(){
	return ((writepos/2) % 160);
}












// ========================================
// we were using the methods below for tp6
char response[128];

char * 
itoa(int seconds){
	response[0] = (char) (((seconds >> 4) & 0x0F) + '0') ;
	response[1] = (seconds & 0x0F) + '0';
	response[2] = 0;

	return response;
}

void 
print_to_screen(char * string){
   print_to_scr(string, 0, 10); //hand-picked colours
}

void 
print_to_scr(char * message, int background_color, int foreground_color){
  int i, j;
  int leng = strlen(message)*2;
  
  int attribute = background_color << 4; //
  char color = (char) attribute + foreground_color;

  for(i = 0, j = 0; i < leng; i++, j++){
		video[i]   = message[j];
		video[++i] = color;
  } 
}

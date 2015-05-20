#include "./include/video.h"


void k_clear_screen(){
    char * vidmem_address = (char *) VIDMEM_ADDRESS;
    unsigned int i=0;
    while(i < UPPER_SCREEN_SIZE-LINE_SIZE){
        vidmem_address[i++]=' ';
        vidmem_address[i++]=WHITE_TXT;
    };
    while(i<UPPER_SCREEN_SIZE){
        vidmem_address[i++]=' ';
        vidmem_address[i++]=LILAC_SQUARE;
    }
    while(i < MAIN_SCREEN_SIZE){
        vidmem_address[i++]=' ';
        vidmem_address[i++]=WHITE_TXT;
    };
}
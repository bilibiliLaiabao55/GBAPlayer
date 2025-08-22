#include <stdlib.h>
#include <stdio.h>
#include <gba.h>
#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"
bg_scroll bg_xy0;
u8 double_frame;
u8 keys;
u16 pos_this;
u16 last_pos;
u16 current_song;
void tick(){
	VBlankIntrWait();
	mmFrame();
}
int main(void){
	irqInit();
	irqSet(IRQ_VBLANK, mmVBlank);
	irqEnable(IRQ_VBLANK);
	consoleDemoInit();
	mmInitDefault((mm_addr)soundbank_bin, 18);
    bg_xy0.y = 16;
    last_pos = -1;
    current_song = 0;
	mmStart(current_song, MM_PLAY_LOOP);
    tick();
	iprintf("\x1b[2;0HPress UP and DOWN to switch");
	iprintf("\x1b[3;0Hsong");
    while(1){
		iprintf("\x1b[10;6Htick:%d Row:%d Pos:%d       \n", mmGetPositionTick(), mmGetPositionRow(), mmGetPosition());
        pos_this = mmGetPosition();
        scanKeys();
        keys = keysDownRepeat();
        if(keys & KEY_DOWN){
            current_song += 1;
            if(current_song == MSL_NSONGS)current_song = 0;
	        mmStart(current_song, MM_PLAY_LOOP);
        }else if(keys & KEY_UP){
            if(current_song == 0)current_song = MSL_NSONGS;
            current_song -= 1;
	        mmStart(current_song, MM_PLAY_LOOP);
        }
        if(mmGetPositionRow() % 8 == 0)
            bg_xy0.y = 0;
        else if(double_frame==1){
            if (bg_xy0.y <= 8)bg_xy0.y += (8-bg_xy0.y)/2;
        }
        REG_BG0VOFS = bg_xy0.y;
        double_frame = !double_frame;
        last_pos = mmGetPosition();
        tick();
    }
    return 0;
}
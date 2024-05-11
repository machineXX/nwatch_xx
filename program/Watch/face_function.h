#ifndef FACE_FUNCTION_H_
#define FACE_FUNCTION_H_
#include "common.h"
typedef struct
{
	byte x;
	byte y;
	const byte* bitmap;
	byte w;
	byte h;
	byte offsetY;
	byte val;
	byte maxVal;
	bool moving;
}tickerData_my_t;

typedef struct
{
	char name[20];
	byte num;
	display_f func;
}watchface_arr;

bool old_btn4(void);
void face_drawTickerNum(tickerData_my_t* data);
display_t face_ticker(u8 pos_x,u8 pos_y);
void moving2_set(byte *yPos,byte *yPos_secs,bool *moving,bool *moving2);
u8 get_watchface_total(void);
display_f get_watchface_fun(u8 num);
void watchface_save(display_f  fun);
display_f get_other_watchface(display_f  fun,u8 chice);

bool close_screen(void);

#endif

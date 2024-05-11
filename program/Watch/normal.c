/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define TIME_POS_X	1
#define TIME_POS_Y	20
#define TICKER_GAP	4

static bool down(void);
static bool up(void);
static display_t draw(void);
static void drawDate(void);
static display_t ticker(void);

void watchface_normal()
{
	display_setDrawFunc(draw);//
	buttons_setFuncs(up, menu_select, down);
	buttons_setFuncs_add4(old_btn4);

	animation_start(NULL, ANIM_MOVE_ON);
	watchface_save(watchface_normal); 
}

static bool down()
{
  // Set watchface
	display_f fun = get_other_watchface(watchface_normal, 1);
	display_set(fun);
	display_load();

	return true;
}

static bool up()
{
	// Set watchface
	display_f fun = get_other_watchface(watchface_normal, 0);
	display_set(fun);
	display_load();

	return true;
}


static display_t draw()
{
// #if COMPILE_ANIMATIONS
// 	static byte usbImagePos = FRAME_HEIGHT;
// 	static byte chargeImagePos = FRAME_HEIGHT;
// #endif

	// Draw date
	drawDate();  //更新年月日 放入显存

	// Draw time animated
	display_t busy ;
	
	busy= ticker(); //更新小时分钟秒 放入显存
	show_battery_mode_2();

	return busy;
}

static void drawDate()
{
	// Get day string
	char day[BUFFSIZE_STR_DAYS];
	strcpy(day, days[timeDate.date.day]);

	// Get month string
	char month[BUFFSIZE_STR_MONTHS];
	strcpy(month, months[timeDate.date.month]);

	// Draw date
	char buff[BUFFSIZE_DATE_FORMAT];
	sprintf_P(buff, PSTR(DATE_FORMAT), day, timeDate.date.date, month, timeDate.date.year);
	draw_string(buff,false,12,0);
}


static display_t ticker()
{
	static byte yPos;
	static byte yPos_secs;
	static bool moving = false;
	static bool moving2[5];

	tickerData_my_t data;
	moving2_set(&yPos,&yPos_secs,&moving,moving2);
	// Seconds
	data.x = 104;
	data.y = 28;
	data.bitmap = (const byte*)&small2Font;
	data.w = FONT_SMALL2_WIDTH;
	data.h = FONT_SMALL2_HEIGHT;
	data.offsetY = yPos_secs;
	data.val = div10(timeDate.time.secs);
	data.maxVal = 5;
	data.moving = moving2[4];
	face_drawTickerNum(&data);

	data.x = 116;
	data.val = mod10(timeDate.time.secs);
	data.maxVal = 9;
	data.moving = moving;
	face_drawTickerNum(&data);
	
	// Set new font data for hours and minutes
	data.y = TIME_POS_Y;
	data.w = MIDFONT_WIDTH;
	data.h = MIDFONT_HEIGHT;
	data.bitmap = (const byte*)&midFont;
	data.offsetY = yPos;

	// Minutes
	data.x = 60;
	data.val = div10(timeDate.time.mins);
	data.maxVal = 5;
	data.moving = moving2[2];
	face_drawTickerNum(&data);

	data.x = 83;
	data.val = mod10(timeDate.time.mins);
	data.maxVal = 9;
	data.moving = moving2[3];
	face_drawTickerNum(&data);

	// Hours
	data.x = 1;
	data.val = div10(timeDate.time.hour);
	data.maxVal = 5;
	data.moving = moving2[0];
	face_drawTickerNum(&data);

	data.x = 24;
	data.val = mod10(timeDate.time.hour);
	data.maxVal = 9;
	data.moving = moving2[1];
	face_drawTickerNum(&data);
	
	// Draw colon for half a second   画半秒的冒号
	if(timeDate.time.sec_10ms<50) 
		draw_bitmap(TIME_POS_X + 48, TIME_POS_Y, colon, FONT_COLON_WIDTH, FONT_COLON_HEIGHT, NOINVERT, 0);
	
	// Draw AM/PM character
	char tmp[2];
	tmp[0] = timeDate.time.ampm;
	tmp[1] = 0x00;
	draw_string(tmp, false, 104, 20);


	return (moving ? DISPLAY_BUSY : DISPLAY_DONE);
}


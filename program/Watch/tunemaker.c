/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
#include "delay.h"
#include "led.h"
#include "sys.h"
#include "usart.h"

static u8 way=1;

static bool down(void);
static bool up(void);
static bool select(void);
static display_t draw(void);

void tunemakerOpen()
{
	menu_close();
	cube_init();
	appConfig.showFPS = 0;   //禁用FPS
    pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_BUSY);
	display_setDrawFunc(draw);
	buttons_setFunc(BTN_1,	up);
	buttons_setFunc(BTN_2,	select);
	buttons_setFunc(BTN_3,	down);
}

static bool down()
{
	way=!way;
	return true;
}

static bool up()
{
	select();
	return true;
}

static bool select()
{
	free_OLED_GRAM();
	pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_NONE);	
	animation_start(display_load, ANIM_MOVE_OFF);

	return true;
}

static display_t draw()
{     
	show_cube(way);
	draw_string("Cube ", false, 64, 28);
	draw_string("Rotation", false, 64, 40);

	return DISPLAY_DONE;
}


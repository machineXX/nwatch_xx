#include "common.h"

#define TICKER_GAP	4

bool close_screen()
{
	static u8 OLED_SET = 1;

	appConfig.CTRL_LEDs = !appConfig.CTRL_LEDs;
	OLED_SET = !OLED_SET;
	OLED_Status_set(OLED_SET);
	user_state_change();

	return 0;
}

bool old_btn4()
{
	//close_screen();

	return 0;
}
void face_drawTickerNum(tickerData_my_t* data)
{
	byte arraySize = (data->w * data->h) / 8;
	byte yPos = data->offsetY;
	const byte* bitmap = &data->bitmap[data->val * arraySize];
	byte x = data->x;
	byte y = data->y;

	if(!data->moving || yPos == 0 || yPos == 255)
		draw_bitmap(x, y, bitmap, data->w, data->h, NOINVERT, 0);
	else
	{
		byte prev = data->val - 1;
		if(prev == 255)
			prev = data->maxVal;

		draw_bitmap(x, y, bitmap, data->w, data->h, NOINVERT, yPos - data->h - TICKER_GAP);
		draw_bitmap(x, y, &data->bitmap[prev * arraySize], data->w, data->h, NOINVERT, yPos);
	}	
}
display_t face_ticker(u8 pos_x,u8 pos_y)
{
	static byte yPos;
	static byte yPos_secs;
	static bool moving = false;
	static bool moving2[5];

	tickerData_my_t data;
	moving2_set(&yPos,&yPos_secs,&moving,moving2);

	// void moving2_set(u8 *yPos,u8 *yPos_secs,u8 *moving,u8 *moving2)
	data.w = FONT_SMALL2_WIDTH;
	data.h = FONT_SMALL2_HEIGHT;
	
	// Set new font data for hours and minutes
	data.y = pos_y;
	data.bitmap = (const byte*)&small2Font;
	data.offsetY = yPos;

	// Minutes
	data.x = pos_x+33;//71
	data.val = div10(timeDate.time.mins);
	data.maxVal = 5;
	data.moving = moving2[2];
	face_drawTickerNum(&data);

	data.x = pos_x+45;//83
	data.val = mod10(timeDate.time.mins);
	data.maxVal = 9;
	data.moving = moving2[3];
	face_drawTickerNum(&data);

	// Hours
	data.x = pos_x;//38//71
	data.val = div10(timeDate.time.hour);
	data.maxVal = 5;
	data.moving = moving2[0];
	face_drawTickerNum(&data);

	data.x = pos_x+12;//50
	data.val = mod10(timeDate.time.hour);
	data.maxVal = 9;
	data.moving = moving2[1];
	face_drawTickerNum(&data);
	
	// Draw colon for half a second   画半秒的冒号
	if(timeDate.time.sec_10ms<50) 
    {
        // draw_bitmap(63, TIME_POS_Y+4, colon, FONT_COLON_WIDTH, FONT_COLON_HEIGHT/2, NOINVERT, 0);
        // draw_bitmap(pos_x+25, pos_y-4, colon, FONT_COLON_WIDTH, FONT_COLON_HEIGHT, NOINVERT, 0);
        draw_string(":",false,pos_x+26,pos_y+4);
    }

	return (moving ? DISPLAY_BUSY : DISPLAY_DONE);
}
void moving2_set(byte *yPos,byte *yPos_secs,bool *moving,bool *moving2)
{
	//static bool moving = false;
	//static bool moving2[5];

#if COMPILE_ANIMATIONS
	static byte hour2;
	static byte mins;
	static byte secs;

	if(appConfig.animations)     
	{
		if(timeDate.time.secs != secs)
		{
			*yPos = 0;
			// printf(" 1 yPos=%d\r\n",*yPos);
			*yPos_secs = 0;
			*moving = true;

			moving2[0] = div10(timeDate.time.hour) != div10(hour2);
			moving2[1] = mod10(timeDate.time.hour) != mod10(hour2);
			moving2[2] = div10(timeDate.time.mins) != div10(mins);
			moving2[3] = mod10(timeDate.time.mins) != mod10(mins);
			moving2[4] = div10(timeDate.time.secs) != div10(secs);
		
			//memcpy(&timeDateLast, &timeDate, sizeof(timeDate_s));
			hour2 = timeDate.time.hour;
			mins = timeDate.time.mins;
			secs = timeDate.time.secs;
		}

		if(*moving)
		{
			if(*yPos <= 3)
				(*yPos)++;
			else if(*yPos <= 6)
				(*yPos) += 3;
			else if(*yPos <= 16)
				(*yPos) += 5;
			else if(*yPos <= 22)
				(*yPos) += 3;
			else if(*yPos <= 24 + TICKER_GAP)
				(*yPos)++;
			// printf(" 2 yPos=%d\r\n",*yPos);
			if(*yPos >= MIDFONT_HEIGHT + TICKER_GAP)
				*yPos = 255;
			// printf(" 3 yPos=%d\r\n",*yPos);
			if(*yPos_secs <= 1)
				(*yPos_secs)++;
			else if(*yPos_secs <= 13)
				(*yPos_secs) += 3;
			else if(*yPos_secs <= 16 + TICKER_GAP)
				(*yPos_secs)++;

			if(*yPos_secs >= FONT_SMALL2_HEIGHT + TICKER_GAP)
				*yPos_secs = 255;

			if(*yPos_secs > FONT_SMALL2_HEIGHT + TICKER_GAP && *yPos > MIDFONT_HEIGHT + TICKER_GAP)
			{
				*yPos = 0;
				*yPos_secs = 0;
				*moving = false;
				memset(moving2, false, sizeof(moving2));
				// printf(" 4 yPos=%d\r\n",*yPos);
			}
		}
	}
	else
#endif
	{
		*yPos = 0;
		*yPos_secs = 0;
		*moving = false;
		memset(moving2, false, sizeof(moving2));
		// printf(" 5 yPos=%d\r\n",*yPos);
	}
	//printf("yPos=%d,yPos_secs=%d,moving=%d \r\n",*yPos,*yPos_secs,*moving);
	// printf(" 6 yPos=%d\r\n",*yPos);
	//return yPos;
}

watchface_arr watchface_all[] =
{
	{"watchface2"		  , 0, watchface2		  },
	{"watchfaceOpen"	  ,	1, watchfaceOpen	  },
	{"my_watchface_normal", 2, my_watchface_normal},
	{"watchface_normal"   , 3, watchface_normal	  },
	//{"watchface_CPU"      , 4, watchface_CPU	  },
	{"watchface_ast"      , 4, watchface_ast	  },
};

static u8 watchface_num;

u8 get_watchface_total(void)
{
	watchface_num = sizeof(watchface_all)/sizeof(watchface_all[0]);
	return watchface_num;
}

display_f get_watchface_fun(u8 num)
{
	return watchface_all[num].func;
}

/*下次开机直接回到上次的桌面*/
void watchface_save(display_f  fun)
{
	u8 i =0;
	for(i=0;i<watchface_num;i++)
	{
		if(fun == watchface_all[i].func)
		{
			watchface_num_set(i);
			appconfig_save();
			return;
		}
	}
}

display_f get_other_watchface(display_f fun,u8 chice)
{
	u8 i =0;

	for(i = 0;i < watchface_num;i ++)
	{
		if(fun == watchface_all[i].func)
		{
			// printf("i = %d %s\r\n",i,watchface_all[i].name);
			if(chice)
			{
				if(i == watchface_num-1)
					return  watchface_all[0].func;
				else
					return  watchface_all[i+1].func;
			}
			else
			{
				if(i == 0)
					return  watchface_all[watchface_num-1].func;
				else
					return  watchface_all[i-1].func;
			}
		}
	}
	return watchface_all[0].func;
}

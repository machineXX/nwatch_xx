#include "common.h"

#define TIME_POS_X	64
#define TIME_POS_Y	1
#define TICKER_GAP	14
#define PIC_NUM_MAX 2

/*显示女孩图片的桌面*/

static bool down(void);
static bool up(void);
static display_t draw(void);
static void drawDate(void);
static display_t ticker(void);
// static void drawTickerNum(tickerData_t*);

#if COMPILE_Easter_Egg
	static bool set_pic = 0;
	bool set_draw_pic()
	{
		set_pic = !set_pic;
		return true;
	}
#endif

void my_watchface_normal()
{
	display_setDrawFunc(draw);//
	buttons_setFuncs(up, menu_select, down);
	buttons_setFuncs_add4(weather_forecast);
	animation_start(NULL, ANIM_MOVE_ON);
	watchface_save(my_watchface_normal);

#if COMPILE_Easter_Egg
	buttons_setFunc_long(BTN_4,set_draw_pic,2);
#endif
}

static bool down()
{
	display_f fun = get_other_watchface(my_watchface_normal, 1);
	display_set(fun);
	display_load();

#if COMPILE_Easter_Egg
	buttons_setFunc_long(BTN_4,close_screen,2);
#endif
	return true;
}

static bool up()
{
	display_f fun = get_other_watchface(my_watchface_normal, 0);
	display_set(fun);
	display_load();

#if COMPILE_Easter_Egg
	buttons_setFunc_long(BTN_4,close_screen,2);
#endif
	return true;
}

void draw_pic(void)
{
	draw_bitmap(0,0,(const byte *)bmp, 64, 64, NOINVERT, 0);
}
void draw_weather(void)
{
	int len;
	char temp[20];
	usb_ttl_weather_t weather;

	weather = get_weather(0);

#if COMPILE_Easter_Egg
	if(set_pic == 1)
		draw_pic();
	else
#endif
	{
		draw_bitmap(12,8,(const byte *)weather_bmp[weather.weather_type], 40, 40, NOINVERT, 0);
		sprintf(temp, "%d`C~%d`C", weather.temp_low, weather.temp_high);
		len = strlen(temp);
		draw_string(temp,false,9+(8-len)*3,52); /*  7-11  8*/ 
	}
}

static display_t draw()
{	   
	// draw_pic();
	draw_weather();
	drawDate();
	ticker();
	show_battery_mode_1();				
				
	return DISPLAY_DONE;
}

static void drawDate()
{
	// Get day string
	char day[BUFFSIZE_STR_DAYS];
	strcpy(day, days[timeDate.date.day]);

	// Get month string
	char month[BUFFSIZE_STR_MONTHS];
	strcpy(month, months[timeDate.date.month]);

	// Draw date 星期
	char buff[BUFFSIZE_DATE_FORMAT];
	sprintf_P(buff,"%s", day);
	draw_string(buff,false,TIME_POS_X+4,12);

	//日期
	sprintf_P(buff, "%02hhu",timeDate.date.date);
	draw_string(buff,false,TIME_POS_X+6,21);

	//sprintf_P(buff, PSTR(DATE_FORMAT), day, timeDate.date.date, month, timeDate.date.year);

	//month
	sprintf_P(buff, "%s",month);
	draw_string(buff,false,TIME_POS_X+4,29);

	// //year
	sprintf_P(buff, "20");
	draw_string(buff,false,TIME_POS_X,38);
	sprintf_P(buff, "%02hhu" ,timeDate.date.year);
	draw_string(buff,false,TIME_POS_X+12,38);
	
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
	data.x = TIME_POS_X;
	data.y = 46+TIME_POS_Y;
	data.bitmap = (const byte*)&small2Font;
	data.w = FONT_SMALL2_WIDTH;
	data.h = FONT_SMALL2_HEIGHT;
	data.offsetY = yPos_secs;
	data.val = div10(timeDate.time.secs);
	data.maxVal = 5;
	data.moving = moving2[4];
	face_drawTickerNum(&data);

	data.x = TIME_POS_X+12;
	data.val = mod10(timeDate.time.secs);
	data.maxVal = 9;
	data.moving = moving;
	face_drawTickerNum(&data);
	
	// Set new font data for hours and minutes
	data.y = 25+TICKER_GAP+TIME_POS_Y;
	data.w = MIDFONT_WIDTH;
	data.h = MIDFONT_HEIGHT;
	data.bitmap = (const byte*)&midFont;
	data.offsetY = yPos;

	// Minutes
	data.x = TIME_POS_X+24;
	data.val = div10(timeDate.time.mins);
	data.maxVal = 5;
	data.moving = moving2[2];
	face_drawTickerNum(&data);

	data.x = TIME_POS_X+46;
	data.val = mod10(timeDate.time.mins);
	data.maxVal = 9;
	data.moving = moving2[3];
	face_drawTickerNum(&data);

	// Hours
	data.y = TIME_POS_Y+12;
	data.x = TIME_POS_X+24;
	data.val = div10(timeDate.time.hour);
	data.maxVal = 5;
	data.moving = moving2[0];
	face_drawTickerNum(&data);

	data.x = TIME_POS_X+46;
	data.val = mod10(timeDate.time.hour);
	data.maxVal = 9;
	data.moving = moving2[1];
	face_drawTickerNum(&data);
	

	// Draw AM/PM character
	char tmp[2];
	tmp[0] = timeDate.time.ampm;
	tmp[1] = 0x00;
	draw_string(tmp, false, 104, 20);

//	char buff[12];
//	sprintf_P(buff, PSTR("%lu"), time_getTimestamp());
//	draw_string(buff, false, 30, 50);

	return (moving ? DISPLAY_BUSY : DISPLAY_DONE);
}

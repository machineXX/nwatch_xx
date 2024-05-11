#include "common.h"
// usb_ttl_weather_t weather;
static display_t draw(void);
static bool to_my_normal(void);
void show_weather_forecast(void);
static void weather_drawDate(u8 pos_x,int week_day,int date);

void weather_type_set(usb_ttl_weather_t *weather)
{
    if(strcmp(weather->weather,"sun") == 0)weather->weather_type = 0;
    else if(strcmp(weather->weather,"rai") == 0)weather->weather_type = 1; 
    else if(strcmp(weather->weather,"sno") == 0)weather->weather_type = 2; 
    else if(strcmp(weather->weather,"clo") == 0)weather->weather_type = 3; 
    else if(strcmp(weather->weather,"ove") == 0)weather->weather_type = 4; 
    else weather->weather_type = 5;
}

bool weather_forecast(void)
{
    display_setDrawFunc(draw);//
	buttons_setFuncs(to_my_normal, to_my_normal, to_my_normal);
	buttons_setFuncs_add4(to_my_normal);
	animation_start(NULL, ANIM_MOVE_ON);
	watchface_save(my_watchface_normal);

}

static bool to_my_normal()
{
    display_set(my_watchface_normal);
	display_load();

	return true;
}

static display_t draw()
{	   
	show_weather_forecast();					
	return DISPLAY_DONE;
}
void show_weather_forecast()
{
    usb_ttl_weather_t weather[WEATHER_NUM];
    u8 i;
    byte bmp_x = 0, bmp_y = 20;
    char temp[20];
    int temp_ave;
    int len;
    for(i = 0;i < WEATHER_NUM;i ++)
    {
        weather[i] = get_weather(i);

        draw_bitmap(bmp_x + 25*i, bmp_y,(const byte *)weather_bmp_24[weather[i].weather_type], 24, 24, NOINVERT, 0);
        weather_drawDate(4 + 25*i, timeDate.date.day+i, timeDate.date.date+i);

        temp_ave = (weather[i].temp_low + weather[i].temp_high)/2;
        sprintf(temp, "%d`C", temp_ave);
        len = strlen(temp);
        draw_string(temp,false, 6 + 25*i+(3-len)*3, bmp_y + 30);
    }
    
}

static void weather_drawDate(u8 pos_x,int week_day,int date)
{
	// Get day string
	char day[BUFFSIZE_STR_DAYS];

    if(week_day>6)week_day -= 7;

    // byte numDays = time_monthDayCount(timeDate.date.month, timeDate.date.year);
    // if (date == numDays + 1)
    // {
    //     date -= numDays;
    // }

	strcpy(day, days[week_day]);

	// Draw date 星期
	char buff[BUFFSIZE_DATE_FORMAT];
	sprintf_P(buff,"%s", day);
	draw_string(buff,false,pos_x,8);

	//日期
	// sprintf_P(buff, "%02hhu",date);
	// draw_string(buff,false,pos_x+16,8);	
}
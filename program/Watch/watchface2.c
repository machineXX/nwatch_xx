#include "common.h"
#include "delay.h"
#include "led.h"
#include "sys.h"
#include "usart.h"
/*
	显示主机状态的桌面
*/
static bool down(void);
static bool up(void);
static bool open_cpuface(void);
static display_t draw(void);
void show_alldata_use(u8 data_buf,u8 num,u8 x,u8 y);
void show_alldata_cmp(u8 *data_name,u8 *data_cmp,u8 x,u8 y);
void alldata_use_get(void);
void show(void);

static int data_buf[5];
void watchface2()
{
	display_setDrawFunc(draw);
	buttons_setFuncs(up, menu_select, down);
	buttons_setFuncs_add4(open_cpuface);
	animation_start(NULL, ANIM_MOVE_ON);
	watchface_save(watchface2);
}
static bool open_cpuface()
{
	// printf("open_cpuface 1\r\n");
	display_set(watchface_CPU);
	// printf("open_cpuface 2\r\n");
	display_load();
	return true;
}

static bool down()
{
	display_f fun = get_other_watchface(watchface2, 1);
	display_set(fun);

	display_load();
	return true;
}

static bool up()
{
	display_f fun = get_other_watchface(watchface2, 0);
	display_set(fun);

	display_load();
	return true;
}
static display_t draw()
{
	// Draw time animated	
    face_ticker(30,8);
    //  ticker(); 
	// usb_ttl_update();
	alldata_use_get();
	show();
	show_battery_mode_2();
	return DISPLAY_DONE;
}
void alldata_use_get()
{
    u8 i;
    for(i=0;i<5;i++)
    {
        data_buf[i]=put_cpu(i);
    }
}
void show()
{
//    u8 i,j;
//	char buf[2];
	
    show_alldata_cmp("CPU:","%",0,40);
    show_alldata_use(data_buf[0],0,24,32);

    show_alldata_cmp("MEM:","%",60,40);
    show_alldata_use(data_buf[1],0,84,32);

	// show_alldata_cmp("TMP:","C",60,28);
    // show_alldata_use(data_buf[2],3,84,20);

	// show_alldata_cmp("NET:","    KB/S",0,48);
	// //printf("%d\n",data_buf[3]);
    // show_alldata_use(data_buf[3]/100,0,24,40);
	// show_alldata_use(data_buf[3]%100,0,48,40);

	//show_alldata_cmp("TDP:","%",60,48);
    //show_alldata_use(data_buf[4],0,84,40);
}
void show_alldata_cmp(u8 *data_name,u8 *data_cmp,u8 x,u8 y)
{
	draw_string((char *)data_name,false,x,y);
	draw_string((char *)data_cmp,false,x+50,y); 
}
void show_alldata_use(u8 data_buf,u8 num,u8 x,u8 y)
{
	u8 data_x_pos1=x;
    u8 data_x_pos2=x+12;
	tickerData_my_t data;
	data.w = FONT_SMALL2_WIDTH;
	data.h = FONT_SMALL2_HEIGHT;
	
	// Set new font data for hours and minutes
	data.y = y;
	data.bitmap = (const byte*)&small2Font;
	data.offsetY = 0;

	// if(num==3)
	// {
	// 	draw_string(".",false,x+13,y+10); 
	// 	data_x_pos2=x+17;
	// }

	data.x = data_x_pos1;//38
	data.val = div10(data_buf);
	data.maxVal = 9;
	data.moving = 0;
	face_drawTickerNum(&data);

	data.x = data_x_pos2;//50
	data.val = mod10(data_buf);
	data.maxVal = 9;
	data.moving = 0;
	face_drawTickerNum(&data);
}

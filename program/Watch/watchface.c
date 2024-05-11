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

static const int faces1[6][4] = {{0,1,2,3},{1,5,6,2},{5,4,7,6},{4,0,3,7},{0,4,5,1},{3,2,6,7}};
static const int nvert1 = 8;
static const int nfaces1 = 6;
int Cnt1, Cnt1v;
uint8_t vert1[8][3];

/*__OLED_GRAM 不适合用malloc,会发生错误*/		   
u8 *__OLED_GRAM = NULL;	 //也是显存，但不是全图的显存这里是用来显示立方体的

static bool down(void);
static bool up(void);
static display_t draw(void);
static bool modeset(void);

static  u8 __SysTick(u8 way);
static  void _OLED_DrawPoint(u8 x,u8 y,u8 t);

static  void _OLED_DrawLine(u16 x11, u16 y11, u16 x21, u16 y21);



#define TIME_POS_X	1
#define TIME_POS_Y	1
#define TICKER_GAP	14

static u8 Dir_of_rot = 0;

void cube_init()
{
	if(__OLED_GRAM == NULL)
	{
		__OLED_GRAM = malloc(sizeof(u8)*FRAME_BUFFER_SIZE/2);
	}
	else
	{
		debug_printf("cube heap is not be free!\r\n");
	}
	memset(__OLED_GRAM, 0x00, FRAME_BUFFER_SIZE/2);
}

void watchfaceOpen()
{
	cube_init();
	appConfig.showFPS = 0;   //禁用FPS
	display_setDrawFunc(draw);
	buttons_setFuncs(up, menu_select, down);
	buttons_setFuncs_add4(modeset);
	animation_start(NULL, ANIM_MOVE_ON);
	watchface_save(watchfaceOpen);
}
static bool modeset()
{
	Dir_of_rot=!Dir_of_rot;
	return true;
}
void free_OLED_GRAM()
{
	if(__OLED_GRAM)
	{
		free(__OLED_GRAM);
		__OLED_GRAM = NULL;
	}
	else
	{
		debug_printf("cube heap have be free!\r\n");
	}
}
static bool down()
{
  // Set watchface
    display_f fun = get_other_watchface(watchfaceOpen,1);
	display_set(fun);

	free_OLED_GRAM();
	display_load();
	return true;
}

static bool up()
{
	// Set watchface
	display_f fun = get_other_watchface(watchfaceOpen,0);
	display_set(fun);

	free_OLED_GRAM();
	display_load();

	return true;
}

static display_t draw()
{
	
	// Infinite loop
		show_cube(Dir_of_rot);					
        watchface_drawDate();
		watchface_ticker();
		show_battery_mode_1();
				
				
	return DISPLAY_DONE;
}


static  u8 __SysTick(u8 way)
{
	static u8  iAng=1 ;
	if(way==0)
	{
		iAng++;
		if(iAng == 180) iAng = 0;
	}
	else
	{
		iAng--;
 		if(iAng == 0) iAng= 180;
	}
	return iAng;
	// printf("iAng=%d\r\n",iAng);
}

//画点 
//x:0~63
//y:0~63
//t:1 填充 0,清空				   
static  void _OLED_DrawPoint(u8 x,u8 y,u8 t)
{	
	//对应 byte __OLED_GRAM[512];	  // x= 64 ,y=8
	//变换一哈形式
	u8 pos,bx,temp=0;
	if(x>63||y>63)return;//超出范围了.
	pos=y/8;
	bx=y%8;
	temp=1<<(bx);
	 if(t) __OLED_GRAM[64*pos+x] |= temp;  // 做到的效果是竖着存放 竖着为y ，横着为x
	  else __OLED_GRAM[64*pos+x]&=~temp;		
}


//画线
//x11,y11:起点坐标
//x21,y21:终点坐标  
static  void _OLED_DrawLine(u16 x11, u16 y11, u16 x21, u16 y21)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x21-x11; //计算坐标增量 
	delta_y=y21-y11; 
	uRow=x11; 
	uCol=y11; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		_OLED_DrawPoint(uRow,uCol,1);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 



void watchface_drawDate()
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
	draw_string(buff,false,64,12);

	//日期
	sprintf_P(buff, "%02hhu",timeDate.date.date);
	draw_string(buff,false,66,21);

	//sprintf_P(buff, PSTR(DATE_FORMAT), day, timeDate.date.date, month, timeDate.date.year);

	//month
	sprintf_P(buff, "%s",month);
	draw_string(buff,false,64,29);

	//year
	sprintf_P(buff, "20");
	draw_string(buff,false,60,38);
	sprintf_P(buff, "%02hhu" ,timeDate.date.year);
	draw_string(buff,false,73,38);
	
}

display_t watchface_ticker()
{
	static byte yPos;
	static byte yPos_secs;
	static bool moving = false;
	static bool moving2[5];

	tickerData_my_t data;
	moving2_set(&yPos,&yPos_secs,&moving,moving2);
	// printf("yPos=%d,yPos_secs=%d,moving=%d \r\n",yPos,yPos_secs,moving);
	// Seconds
	data.x = 62;
	data.y = 48;
	data.bitmap = (const byte*)&small2Font;
	data.w = FONT_SMALL2_WIDTH;
	data.h = FONT_SMALL2_HEIGHT;
	data.offsetY = yPos_secs;
	data.val = div10(timeDate.time.secs);
	data.maxVal = 5;
	data.moving = moving2[4];
	face_drawTickerNum(&data);

	data.x = 75;
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
	data.x = 88;
	data.val = div10(timeDate.time.mins);
	data.maxVal = 5;
	data.moving = moving2[2];
	face_drawTickerNum(&data);

	data.x = 110;
	data.val = mod10(timeDate.time.mins);
	data.maxVal = 9;
	data.moving = moving2[3];
	face_drawTickerNum(&data);

	// Hours
	data.y = TIME_POS_Y+12;
	data.x = 88;
	data.val = div10(timeDate.time.hour);
	data.maxVal = 5;
	data.moving = moving2[0];
	face_drawTickerNum(&data);

	data.x = 110;
	data.val = mod10(timeDate.time.hour);
	data.maxVal = 9;
	data.moving = moving2[1];
	face_drawTickerNum(&data);
	

	// Draw AM/PM character
	char tmp[2];
	tmp[0] = timeDate.time.ampm;
	tmp[1] = 0x00;
	draw_string(tmp, false, 104, 20);


	return (moving ? DISPLAY_BUSY : DISPLAY_DONE);
}
void show_cube(u8 way)
{
	uint8_t x11=0, x21=0, y11=0, y21=0; //,statu=0;
	static u8  iAng1 = 1;
		iAng1=__SysTick(way);
				
		// Get vertices for the current angle
			//获取当前角度的顶点
		for(Cnt1v = 0; Cnt1v < nvert1; ++Cnt1v){
				vert1[Cnt1v][0] = vertices[iAng1*nvert1 + Cnt1v][0];
				vert1[Cnt1v][1] = vertices[iAng1*nvert1 + Cnt1v][1];
				vert1[Cnt1v][2] = vertices[iAng1*nvert1 + Cnt1v][2];
				}
		// Draw wireframe for each face of the cube
			//为立方体的每个面绘制线框
						//一个面有四条边，画六个面就是 4x6 =24次就可以完整显示
		for(Cnt1 = 0;Cnt1 < nfaces1; ++Cnt1){

			x11 = vert1[faces1[Cnt1][0]][0]-34;
			y11 = vert1[faces1[Cnt1][0]][1]-1;
			x21 = vert1[faces1[Cnt1][1]][0]-34;
			y21 = vert1[faces1[Cnt1][1]][1]-1;
			_OLED_DrawLine(x11, y11, x21, y21);
				
			x11 = vert1[faces1[Cnt1][1]][0]-34;
			y11 = vert1[faces1[Cnt1][1]][1]-1;
			x21 = vert1[faces1[Cnt1][2]][0]-34;
			y21 = vert1[faces1[Cnt1][2]][1]-1;
			_OLED_DrawLine(x11, y11, x21, y21);

			x11 = vert1[faces1[Cnt1][2]][0]-34;
			y11 = vert1[faces1[Cnt1][2]][1]-1;
			x21 = vert1[faces1[Cnt1][3]][0]-34;
			y21 = vert1[faces1[Cnt1][3]][1]-1;
				_OLED_DrawLine(x11, y11, x21, y21);


			x11 = vert1[faces1[Cnt1][3]][0]-34;
			y11 = vert1[faces1[Cnt1][3]][1]-1;
			x21 = vert1[faces1[Cnt1][0]][0]-34;
			y21 = vert1[faces1[Cnt1][0]][1]-1;
				_OLED_DrawLine(x11, y11, x21, y21);

		}

		//	// Draw platform
	    draw_bitmap(0, 0, (const byte *)__OLED_GRAM, 64, 64, NOINVERT, 0);
		memset(__OLED_GRAM, 0x00, FRAME_BUFFER_SIZE/2);
}


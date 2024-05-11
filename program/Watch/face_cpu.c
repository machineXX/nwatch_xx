#include "common.h"

#define TIME_POS_Y		0
#define DATA_max_num	1
u8 *cpu_use;
static char which_data=0;

static bool down(void);
static bool up(void);
static bool open_cpuface(void);
static display_t draw(void);
//static void drawDate(void);
void  ticker(void);
//static void drawTickerNum(tickerData_my_t*);

void cpu_use_init(void);
void cpu_use_get(void);
void show_cpu_use(void);
static  void _OLED_DrawPoint(u8 x,u8 y,u8 t);

void watchface_CPU()
{
	//usb_ttl_init();
    cpu_use_init();
	display_setDrawFunc(draw);
	// printf("open_cpuface 11\r\n");
	buttons_setFuncs(up, menu_select, down);
	buttons_setFuncs_add4(open_cpuface);
	animation_start(NULL, ANIM_MOVE_ON);
	//watchface_save(watchface_CPU);	
}

static bool open_cpuface()
{
	free(cpu_use);
	display_set(watchface2);
	display_load();
	return true;
}

static bool down()
{
	memset(cpu_use,0,FRAME_WIDTH);
	if(which_data==0)
		which_data=DATA_max_num;
	else
		which_data--;
	return true;
}

static bool up()
{
	memset(cpu_use,0,FRAME_WIDTH);
	if(which_data==DATA_max_num)
		which_data=0;
	else
		which_data++;
	return true;
}

static display_t draw()
{
	// Draw time animated	
	ticker(); //更新小时分钟秒 放入显存

	// usb_ttl_update();
	cpu_use_get();
	show_cpu_use();
	return DISPLAY_DONE;
}

void ticker()
{
//	char buf[4];
	u8 data_x_pos1=24,data_x_pos2=36;
	tickerData_my_t data;
	face_ticker(71,0);
	data.w = FONT_SMALL2_WIDTH;
	data.h = FONT_SMALL2_HEIGHT;
	
	// Set new font data for hours and minutes
	data.y = TIME_POS_Y;
	data.bitmap = (const byte*)&small2Font;
	data.offsetY = 0;

	if(which_data==3)
	{
		draw_string(".",false,39,10); 
		data_x_pos2=43;
	}

	data.x = data_x_pos1;
	data.val = div10(cpu_use[FRAME_WIDTH-1]);
	data.maxVal = 9;
	data.moving = 0;
	face_drawTickerNum(&data);

	data.x = data_x_pos2;//50
	data.val = mod10(cpu_use[FRAME_WIDTH-1]);
	data.maxVal = 9;
	data.moving = 0;
	face_drawTickerNum(&data);

}


int get_cpu_use(void)
{
    return put_cpu(which_data);
}
void cpu_use_init(void)
{
	// printf("cpu_use 1\r\n");
	cpu_use = malloc(FRAME_WIDTH);
	//printf("cpu_use = 0x%x\r\n",cpu_use);
    memset(cpu_use,0,FRAME_WIDTH);
}
void cpu_use_get(void)
{
	static millis8_t lastDraw; 
	millis8_t now = millis();

	if((millis8_t)(now - lastDraw) < 200) //500ms刷新一次
	{
		return;
	}
	// printf("fpsMs=%d\r\n",fpsMs);
	lastDraw = now;

    memcpy(cpu_use,cpu_use+1,FRAME_WIDTH-1);
    cpu_use[FRAME_WIDTH-1]=get_cpu_use();

	// printf("buf=%d\r\n", cpu_use[FRAME_WIDTH-1]);
}
void show_cpu_use()
{
    u8 i;
//	char buf[2];
	char DATA_NAME[5][4]={"CPU:","MEM:","TMP:","NET:","TDP:"},DATA_NAME_put[5]={"     "};
	char DATA_Company[5][5]={"%  ","%  ","'C "," MB","%  "},DATA_Company_put[3]={"   "};

	strncpy(DATA_NAME_put,DATA_NAME[which_data],4);
	strncpy(DATA_Company_put,DATA_Company[which_data],3);
	DATA_NAME_put[4]='\0';
	draw_string(DATA_NAME_put,false,0,8);
	draw_string(DATA_Company_put,false,52,8); 

    for(i=0;i<FRAME_WIDTH;i++)
    {
        _OLED_DrawPoint(i,64-cpu_use[i]/2,1);
    }
	// sprintf_P(buf,"%d",cpu_use[FRAME_WIDTH-1]);
	// draw_string(buf,false,23,8);


}

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
static  void _OLED_DrawPoint(u8 x,u8 y,u8 t)
{	
	//对应 byte __OLED_GRAM[512];	  // x= 64 ,y=8
	//变换一哈形式
//	u8 i=0;
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	while(y<64)
	{
		pos=y/8;
		bx=y%8;
		temp=1<<(bx);
		if(t) oledBuffer[128*pos+x] |= temp;  // 做到的效果是竖着存放 竖着为y ，横着为x
	  	else oledBuffer[128*pos+x]&=~temp;

		y++;		
	}
}

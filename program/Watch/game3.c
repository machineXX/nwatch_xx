/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2014 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
#include "led.h"

#if COMPILE_GAME3

#define TUBE_COUNT 4

#define BLACK	0
#define WHITE	1

typedef struct 
{
	byte x;
	byte y;
}tube_s;

static bool btnExit(void);
static bool btnBounce(void);
static bool btnPause(void);
static display_t draw(void);
void _DrawPoint(u8 x,u8 y,u8 t);
static void judgeOverlap(void);//判断是否重叠并且绘制人物 
//static const uint eepHighscore EEMEM;

int lives = 4;
byte _GRAM[64]={0};	   //64x1bit  宽8 高度64
// byte *_GRAM;	   //64x1bit  宽8 高度64
// Storage Vars
static uint highscore;
static uint score;
static bool bounce;
static bool newHighscore;
static bool overlap;
static bool gameState = false;

static const byte tube_top[] PROGMEM ={
	0xFF,0xD5,0xAB,0xD5,0x83,0x81,0x83,0x81,0x83,0x81,0x83,0x81,0x83,0x81,0x83,0x81,0xD5,0xFF,
};

static const byte tube[] PROGMEM ={
	0xFF,0x55,0xAA,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xAA,0xFF,
};

/*thingImg is bird*/
static u8 thingImg[] PROGMEM ={
	0x78,0xCC,0x84,0x8E,0x73,0x01,0xDF,0xA1,0xAD,0xAD,0xA3,0xBC,0xC0,0x00,0x00,0x00,
	0x00,0x00,0x03,0x02,0x02,0x02,0x02,0x03,0x02,0x02,0x02,0x01,0x00,0x00,0x00,0x00,
};

static tube_s tubes[TUBE_COUNT];
static image_s img_bird = {16, 30, thingImg, 16, 16, NOINVERT, 0};

void draw_bitmap_set(image_s *img)
{
	u8 t,temp;
	u8 x,y=0;
    draw_bitmap(img->x, img->y, img->bitmap, img->width, img->height, img->invert, img->offsetY);
	//提取出 14-21 列所有的数据，按照这个只需要保存一列图标的位置就行保持宽度为8 高度为64
	//那么位置方块就知道了  8x8
	//    x---y
	//   14   img->y
	//   21   y+8  

	/* 这段代码有bug，运行退出到桌面后，按确认键会死机*/
	/*
	for(x=0;x<8;x++)       //读取8个人物的位图，在oled的buff里面进行绘制，描点
	{	
		temp = img->bitmap[x];
		for(t=0;t<8;t++)
		{
			if(temp&0x01) _DrawPoint(x,img->y+y,1);
				temp=temp>>1;
			y++;
		}			
		y=0;
	}  
	*/
		//目前为止得到了一个8x64的buff缓冲区，用于和后面的OLEDBuff进行对比，求出是否重叠
}

void draw_bitmap_s2(image_s *img)
{
  draw_bitmap(img->x, img->y, img->bitmap, img->width, img->height, img->invert, img->offsetY);
}

void game3_start()
{
    pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_BUSY);
	menu_close();
	highscore = appConfig.game3_max_score;
	// _GRAM = (byte *)malloc(64*sizeof(byte));	
	memset(_GRAM,0,64*sizeof(byte));

	srand(millis());

	display_setDrawFunc(draw);
	buttons_setFuncs(btnPause, btnBounce, btnExit);
	
	byte a = 0;
	
	LOOP(TUBE_COUNT, i)
	{
		tubes[i].x = a;
		a += 42;
	}
	
	//显示信息初始化
	score=0;
	newHighscore = false;
    overlap=false;
	gameState = false;
	lives = 4;
}

static bool btnExit()
{
	// free(_GRAM);
	if(lives == 255)
	{
		//game3_start();
	}	
	else
	 {
		pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_NONE);	
		animation_start(display_load, ANIM_MOVE_OFF);
    }
	return true;
}

static bool btnBounce()
{
	bounce = true;
	return true;
}

static bool btnPause()
{
	gameState=!gameState;
	return true;
}


static display_t draw()     //还差一部是无法判断人物和管道是否重叠
{
	static float y;  
	static float yVel; //速度
	// static byte tubeOffset;
	float G_acc_up = 0.16,G_acc_dowm = 0.06;

	if(gameState)
	{
		if(bounce)
		{
			yVel = -2;
			//printf("y = %d,yVel = %f\r\n",y,yVel);
			bounce = false;
		}
		else if((byte)y < FRAME_HEIGHT - img_bird.height)
		{
			if(yVel < 0)
				yVel += G_acc_up;
			else
				yVel += G_acc_dowm;
		}		
		else
		{
			yVel = 0;
			y = FRAME_HEIGHT - img_bird.height;
		}		
	}
	y += yVel;
	img_bird.y = (int)y;// = {16, (int)y, thingImg, 7, 8, NOINVERT, 0};

	/*以下为管道运动*/
	image_s img = {16, 0, thingImg, 7, 8, NOINVERT, 0};

	//printf("y = %d\r\n",tubes[0].x);
	char buff[6];
	if(lives > 0)
	{
		img.width = 16;
		img.height = 8;
		LOOP(TUBE_COUNT, i)
		{
			img.bitmap = tube;
			img.x = tubes[i].x;
			
			LOOP(2, h)      //画上管道
			{
				img.y = (tubes[i].y + (8*h)) - 16;
				draw_bitmap_s2(&img);
			}
			// draw_string("]QWER", false, img.x, tubes[i].y+8);
			// draw_string("ASDF", false, img.x, tubes[i].y+40);

			LOOP(2, h)      //画下管道
			{
				img.y = tubes[i].y + (8*h) + 48;
				draw_bitmap_s2(&img);
			}


			if(gameState)
			{
				img.x --;
				tubes[i].x--;
			}

			img.width = 18;
			img.bitmap = tube_top;     //管道顶部方框
			img.y = tubes[i].y ; //上管道 tubes[i].y + (8*2) - 16
			draw_bitmap_s2(&img);
			img.y = tubes[i].y + 40; //下管道 (tubes[i].y + (8*2) + 48 + 16) - 16 - (8*3)
			draw_bitmap_s2(&img);
		
			// if(gameState)
			// 	tubes[i].x--;
			if(tubes[i].x > 200 && tubes[i].x < 240)//必须是这个范围，不然管道间隔不均匀
			{
				tubes[i].y = rand() % 16;
				tubes[i].x = FRAME_WIDTH + 16;
			}			
		}
	
		// tubeOffset++;
		// if(tubeOffset > 25)
		// 	tubeOffset = 0;
		
		static u8 last_secs = 0,last_lives = 4;
		if(timeDate.time.secs != last_secs)
		{
			score++;
			last_secs = timeDate.time.secs;
			last_lives = lives;
		}

		if(score>5 && last_lives == lives)
		{
			judgeOverlap();     //判断重叠
		}
		draw_bitmap_s2(&img_bird);    //绘画小鸟 

		// Draw score
		sprintf_P(buff, PSTR("%u"), score);
		draw_string(buff, false, FRAME_WIDTH - (7*5), 1);

		// Draw lives
		LOOP(lives, i)
			draw_bitmap(32 + (8*i), 1, livesImg, 7, 8, NOINVERT, 0);
	}
	else
	{
		// Draw end game stuff
		if(score > highscore)
		{
			highscore = score;
			newHighscore = true;
			appConfig.game3_max_score = highscore;
			appconfig_save();
		}
		else
			newHighscore = false;


		draw_string_P(PSTR(STR_GAMEOVER), false, 20, 0);
		draw_string_P(PSTR(STR_SCORE), false, 20, 16);
		draw_string_P(PSTR(STR_HIGHSCORE), false, 20, 32);

		if(newHighscore)
			draw_string_P(PSTR(STR_NEWHIGHSCORE), false, 20, 48);

		sprintf_P(buff, PSTR("%u"), score);
		draw_string(buff, false, 96, 16);

		sprintf_P(buff, PSTR("%u"), highscore);
		draw_string(buff, false, 96, 32);
	}
	
	
	
	return DISPLAY_BUSY;
}	



//画点 
//x:0~63
//y:0~63
//t:1 填充 0,清空				   
void _DrawPoint(u8 x,u8 y,u8 t)
{	
	//对应 byte OLED_GRAM[512];	  // x= 64 ,y=8
	//变换一哈形式
	u8 pos,bx,temp=0;
//	if(x>7||y>63)return;//超出范围了.
	pos=y/8;
	bx=y%8;
	temp=1<<(bx);
    if(t) _GRAM[8*pos+x] |= temp;  // 做到的效果是竖着存放 竖着为y ，横着为x
	 else _GRAM[8*pos+x]&=~temp;	

}



//判断是否重叠并且绘制人物 
static void judgeOverlap(void)
{
	int t,i;
	
	for(i = 0; i < TUBE_COUNT; i++)
	{
		if(img_bird.x+8 >= tubes[i].x)
		{
			if(img_bird.y > tubes[i].y + 6 && img_bird.y < tubes[i].y + 34) // 8 32
			{
				continue;
				//printf("o = %d\r\n",overlap);
			}
			else
			{
				overlap = true;
				//printf("o = %d\r\n",overlap);
			}
		}
	}				

	if(overlap)
	{
		overlap=false;
		lives--;
		/*
				//printf("lives = %d\r\n",lives);
				if(lives < 0)
						{
							// Check for new highscore
							if(score > highscore)
							{
								highscore = score;
								newHighscore = true;

							}
							else
								newHighscore = false;

							led_flash(LED_RED, 250, 255);
							// buzzer_buzz(250, TONE_2KHZ, VOL_UI, PRIO_UI, NULL);
						}	
						else
						{
							led_flash(LED_GREEN, 30, 255);
							// buzzer_buzz(100, TONE_2KHZ, VOL_UI, PRIO_UI, NULL);
						}
						*/
	}
	//score++;
}



#endif

#include "led.h"
#include "millis.h"
#include "common.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  


// LED control

typedef struct {
  byte flashLen;			// How long to light up for
  millis8_t startTime;	//
} led_s;

static led_s ledRed;//右边的led
static led_s ledGreen;//左边的led
		    
//LED IO初始化
void led_init()
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA,PD端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				 //LED0-->PA.8 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	 GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);						 //PA.8 输出高
}

static void flash(led_s* led, byte len,byte pin)
{
  led->flashLen = len;
  led->startTime = my_millis_get_10ms();
}

void led_flash(led_t led, byte len, byte brightness)
{
   if (appConfig.CTRL_LEDs)        //读出来是0 但是还是有问题估计与聚合体有关
	{
    switch (led)
    {
      case LED_RED:
			flash(&ledRed, len, LED_RED_GPIO_NUM);
        break;
      case LED_GREEN:
			flash(&ledGreen, len, LED_GREED_GPIO_NUM);
        break;
      default:
        break;
    }
  }
}

// Is an LED on?
BOOL led_flashing()
{
  return ledRed.flashLen || ledGreen.flashLen;
}

void _update(led_s* led,  byte pin)        //(volatile unsigned long  *)
{
  
  if ((millis8_t)(my_millis_get_10ms() - led->startTime) >= led->flashLen||((millis8_t)led->startTime-my_millis_get_10ms()) == led->flashLen)
//	  if (led->flashLen)
  {
		PAout(pin)=0;
		led->flashLen =0;
  }
	else
		PAout(pin)=1;
}

void led_update()
{
  _update(&ledRed,LED_RED_GPIO_NUM);          //LED0 PBout(12)
  _update(&ledGreen, LED_GREED_GPIO_NUM);    //LED1 PBout(13)
}

// Turn off LEDs
void led_stop()
{
  led_flash(LED_GREEN, 0, 0);
  led_flash(LED_RED, 0, 0);
  led_update();
}




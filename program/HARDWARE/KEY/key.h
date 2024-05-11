#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


// #define External_interrupt_Disable()   { EXTI->IMR &= ~(EXTI_Line0) ;  EXTI->IMR &= ~(EXTI_Line7) ;EXTI->IMR &= ~(EXTI_Line15) ; EXTI->IMR &= ~(EXTI_Line1) ;}
// #define External_interrupt_Enable()     {EXTI->IMR |= EXTI_Line0 ;EXTI->IMR |= EXTI_Line15 ;EXTI->IMR |= EXTI_Line7 ;EXTI->IMR |= EXTI_Line1 ;}
#define External_interrupt_Disable()   NULL
#define External_interrupt_Enable()    NULL


#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键 上
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//读取按键  下
#define KEY2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)//读取按键 确认
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键3(WK_UP) 复位

 

void KEY_Init(void);//IO初始化
#endif

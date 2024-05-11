#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
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
// static void EXTIX_Init(void);								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟
   //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_15|GPIO_Pin_7|GPIO_Pin_0;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //PA0设置成输入，默认下拉	  
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA4,3,2,0

	//EXTIX_Init();

}


//外部中断0服务程序
// void EXTI0_IRQHandler(void)
// {
//   printf("%d,%d,%d,%d\r\n",KEY1,KEY2,KEY0,KEY3);
	
// 	 EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
// }	
// //外部中断2服务程序
// void EXTI7_IRQHandler(void)
// {
//   printf("%d,%d,%d,%d\r\n",KEY1,KEY2,KEY0,KEY3);
// 	EXTI_ClearITPendingBit(EXTI_Line7);//清除LINE2上的中断标志位 
// }
// //外部中断3服务程序
// void EXTI15_IRQHandler(void)
// {
//   printf("%d,%d,%d,%d\r\n",KEY1,KEY2,KEY0,KEY3);
// 	EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE3上的中断标志位  
// }
// //外部中断1服务程序
// void EXTI1_IRQHandler(void)
// {
//   printf("%d,%d,%d,%d\r\n",KEY1,KEY2,KEY0,KEY3);
// 	EXTI_ClearITPendingBit(EXTI_Line1);//清除LINE4上的中断标志位  
// }



// //外部中断初始化程序
// //初始化PE2~4,PA0为中断输入.
// static  void EXTIX_Init(void)
// {
// 	NVIC_InitTypeDef   NVIC_InitStructure;
// 	EXTI_InitTypeDef   EXTI_InitStructure;
	
 
// 	RCC_APB2PeriphClockCmd(GPIO_PortSourceGPIOA, ENABLE);//使能GPIOA时钟
	
	
	
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);//PA7 连接到中断线2
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);//PA15 连接到中断线3
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//PA1 连接到中断线4
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//PA0 连接到中断线0
	
//   /* 配置EXTI_Line0 */
//   EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
//   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
//   EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
//   EXTI_Init(&EXTI_InitStructure);//配置
	
// 	/* 配置EXTI_Line2,3,4 */
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line7 | EXTI_Line15 | EXTI_Line1;
//   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//   EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//   EXTI_Init(&EXTI_InitStructure);//配置
 
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
//   NVIC_Init(&NVIC_InitStructure);//配置
	
// //	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
// //  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3
// //  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
// //  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
// //  NVIC_Init(&NVIC_InitStructure);//配置
// //	
// //	
// //	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断3
// //  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
// //  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
// //  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
// //  NVIC_Init(&NVIC_InitStructure);//配置
	
// //	
// //	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断4
// //  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
// //  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
// //  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
// //  NVIC_Init(&NVIC_InitStructure);//配置
	
	
	
// //	先关闭外部中断
// 	   External_interrupt_Disable();//关闭外部中断

// }

#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	


//-----------------OLED端口定义---------------- 

//#define OLED_SCL_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SCL
//#define OLED_SCL_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

//#define OLED_SDA_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SDA
//#define OLED_SDA_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

//#define OLED_RES_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_12)//RES
//#define OLED_RES_Set() GPIO_SetBits(GPIOA,GPIO_Pin_12)

//#define OLED_DC_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_11)//DC
//#define OLED_DC_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_11)
// 		     
//#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)//CS
//#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_15)

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_9)//CLK
#define OLED_SCL_Set() GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_8)//DIN
#define OLED_SDA_Set() GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_7)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_6)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_6)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_5)




#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_Status_set(u8 set);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);

//void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
//void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
//void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
//void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
//void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
//void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
//void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);


void oled_flush(void);

   

#endif


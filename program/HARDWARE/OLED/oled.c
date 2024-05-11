#include "delay.h"
#include "common.h"



// byte oledBuffer[FRAME_BUFFER_SIZE];	     //模拟oled的缓冲区
byte *oledBuffer;	     //模拟oled的缓冲区

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}  
//反显函数
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
		}
		 OLED_WR_Byte(0xAF,OLED_CMD); //开启显示 

}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}

void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();
	  OLED_CS_Clr();		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCL_Clr();
		if(dat&0x80)
		   OLED_SDA_Set();
		else 
		   OLED_SDA_Clr();
		OLED_SCL_Set();
		dat<<=1;   
	}				 		  
	OLED_DC_Set();   	  
}

//开启OLED显示 
void OLED_DisPlay_On(void)
{

	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
	 //初始化的时候，设置需要改变的状态	
	OLED_ColorTurn(appConfig.invert );
	
	
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}

void OLED_Status_set(u8 set)
{
	if(set)
		OLED_DisPlay_On();
	else
		OLED_DisPlay_Off();
}
//更新显存到OLED	
void OLED_Refresh(void)
{
	oled_flush();
}
//清屏函数
void OLED_Clear(void)
{
	memset(oledBuffer, 0x00, FRAME_BUFFER_SIZE);

	OLED_Refresh();//更新显示
}



////在指定位置显示一个字符,包括部分字符
////x:0~127
////y:0~63
////size1:选择字体 6x8/6x12/8x16/12x24
////mode:0,反色显示;1,正常显示
//void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
//{
//	u8 i,m,temp,size2,chr1;
//	u8 x0=x,y0=y;
//	if(size1==8)size2=6;
//	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
//	chr1=chr-' ';  //计算偏移后的值
//	for(i=0;i<size2;i++)
//	{
//		if(size1==8)
//			  {temp=asc2_0806[chr1][i];} //调用0806字体
//		else if(size1==12)
//        {temp=asc2_1206[chr1][i];} //调用1206字体
//		else if(size1==16)
//        {temp=asc2_1608[chr1][i];} //调用1608字体
//		else if(size1==24)
//        {temp=asc2_2412[chr1][i];} //调用2412字体
//		else return;
//		for(m=0;m<8;m++)
//		{
//			if(temp&0x01)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp>>=1;
//			y++;
//		}
//		x++;
//		if((size1!=8)&&((x-x0)==size1/2))
//		{x=x0;y0=y0+8;}
//		y=y0;
//  }
//}


////显示字符串
////x,y:起点坐标  
////size1:字体大小 
////*chr:字符串起始地址 
////mode:0,反色显示;1,正常显示
//void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
//{
//	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
//	{
//		OLED_ShowChar(x,y,*chr,size1,mode);
//		if(size1==8)x+=6;
//		else x+=size1/2;
//		chr++;
//  }
//}

////m^n
//u32 OLED_Pow(u8 m,u8 n)
//{
//	u32 result=1;
//	while(n--)
//	{
//	  result*=m;
//	}
//	return result;
//}

////显示数字
////x,y :起点坐标
////num :要显示的数字
////len :数字的位数
////size:字体大小
////mode:0,反色显示;1,正常显示
//void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
//{
//	u8 t,temp,m=0;
//	if(size1==8)m=2;
//	for(t=0;t<len;t++)
//	{
//		temp=(num/OLED_Pow(10,len-t-1))%10;
//			if(temp==0)
//			{
//				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
//      }
//			else 
//			{
//			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
//			}
//  }
//}

////显示汉字
////x,y:起点坐标
////num:汉字对应的序号
////mode:0,反色显示;1,正常显示
//void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
//{
//	u8 m,temp;
//	u8 x0=x,y0=y;
//	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //得到字体一个字符对应点阵集所占的字节数
//	for(i=0;i<size3;i++)
//	{
//		if(size1==16)
//				{temp=Hzk1[num][i];}//调用16*16字体
//		else if(size1==24)
//				{temp=Hzk2[num][i];}//调用24*24字体
//		else if(size1==32)       
//				{temp=Hzk3[num][i];}//调用32*32字体
//		else if(size1==64)
//				{temp=Hzk4[num][i];}//调用64*64字体
//		else return;
//		for(m=0;m<8;m++)
//		{
//			if(temp&0x01)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp>>=1;
//			y++;
//		}
//		x++;
//		if((x-x0)==size1)
//		{x=x0;y0=y0+8;}
//		y=y0;
//	}
//}

////num 显示汉字的个数
////space 每一遍显示的间隔
////mode:0,反色显示;1,正常显示
//void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
//{
//	u8 i,n,t=0,m=0,r;
//	while(1)
//	{
//		if(m==0)
//		{
//	    OLED_ShowChinese(128,24,t,16,mode); //写入一个汉字保存在OLED_GRAM[][]数组中
//			t++;
//		}
//		if(t==num)
//			{
//				for(r=0;r<16*space;r++)      //显示间隔
//				 {
//					for(i=1;i<144;i++)
//						{
//							for(n=0;n<8;n++)
//							{
//								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
//							}
//						}
//           OLED_Refresh();
//				 }
//        t=0;
//      }
//		m++;
//		if(m==16){m=0;}
//		for(i=1;i<144;i++)   //实现左移
//		{
//			for(n=0;n<8;n++)
//			{
//				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
//			}
//		}
//		OLED_Refresh();
//	}
//}

////x,y：起点坐标
////sizex,sizey,图片长宽
////BMP[]：要写入的图片数组
////mode:0,反色显示;1,正常显示
//void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
//{
//	u16 j=0;
//	u8 i,n,temp,m;
//	u8 x0=x,y0=y;
//	sizey=sizey/8+((sizey%8)?1:0);
//	for(n=0;n<sizey;n++)
//	{
//		 for(i=0;i<sizex;i++)
//		 {
//				temp=BMP[j];
//				j++;
//				for(m=0;m<8;m++)
//				{
//					if(temp&0x01)OLED_DrawPoint(x,y,mode);
//					else OLED_DrawPoint(x,y,!mode);
//					temp>>=1;
//					y++;
//				}
//				x++;
//				if((x-x0)==sizex)
//				{
//					x=x0;
//					y0=y0+8;
//				}
//				y=y0;
//     }
//	 }
//}
//OLED的初始化
void OLED_Init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	 //使能A端口时钟
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOA
// 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12);
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //??A????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??50MHz
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //???GPIOD3,6
 	GPIO_SetBits(GPIOD,GPIO_Pin_2);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //??A????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5;	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //???GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5);	

	OLED_RES_Clr();
	delay_ms(200);
	OLED_RES_Set();

	oledBuffer=malloc(sizeof(u8)*FRAME_BUFFER_SIZE);	     //模拟oled的缓冲区
	memset(oledBuffer, 0x00, FRAME_BUFFER_SIZE);

	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}


void oled_flush(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
		OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
		OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
		for(n=0;n<128;n++)
			OLED_WR_Byte(oledBuffer[128*i+n],OLED_DATA);
  	}

	memset(oledBuffer, 0x00, FRAME_BUFFER_SIZE);
}


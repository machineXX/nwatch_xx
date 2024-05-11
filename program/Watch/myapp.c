#include "common.h"
#include "usart.h"	
#if COMPILE_MYAPP

static uint strobe;

static bool btnExit(void);
static bool btnFlashRate(void);
static display_t draw(void);
void bluetooth_data_read(void);
void BLUETOOTH_BUF_draw(void);
void BLUETOOTH_BUF_pic_draw(void);

u8 *BLUETOOTH_RX_BUF; 
static u8 BUF_len_cur=1;
void myapp_open()
{
    pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_BUSY);
    menu_close(); //先关闭目录和清屏
    strobe = 0;
    display_setDrawFunc(draw);

    buttons_setFuncs(btnExit, btnFlashRate, btnExit);
}

static bool btnExit()
{
	pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_NONE);		
  OLED_ColorTurn(appConfig.invert);
  display_load();
  return true;
}

static bool btnFlashRate()
{
  if (strobe < 500)
    strobe += 50 * ((strobe / 50) + 1);
  else
    strobe = 0;
  return true;
}

static display_t draw()
{
  //bluetooth_data_read();
  draw_string("message", false, 40, 0); //最大一行定义22 有2个像素点空隙
  draw_string("Undeveloped module", false, 0, 12); 
  //BLUETOOTH_BUF_draw();
  //BLUETOOTH_BUF_pic_draw();
  return DISPLAY_DONE;
}
void bluetooth_data_read()
{
    u16 len,t;
//    u8 true_t=0;
    if(USART_RX_STA&0x8000)
    {	
//       memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);			
        len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
        free(BLUETOOTH_RX_BUF);
        BLUETOOTH_RX_BUF = (u8 *)malloc(len*sizeof(u8));
        //printf("\r\n message len=%d:\r\n",len);
        for(t=0;t<len;t++)
        {
          if(USART_RX_BUF[t]<32||USART_RX_BUF[t]>126)USART_RX_BUF[t]=32;
			      BLUETOOTH_RX_BUF[t]=USART_RX_BUF[t];					
        }
				BLUETOOTH_RX_BUF[t]='\0';
        draw_string("ok",false,20,0); //最大一行定义22 有2个像素点空隙
        printf("\r\n%s\r\n",BLUETOOTH_RX_BUF);//插入换行
        BUF_len_cur=len/21+1;
       // printf("\r\n\r\n");//插入换行
        USART_RX_STA=0;
    }
}
void BLUETOOTH_BUF_draw()
{ 
    u8 y=0;
  
    for(y=1;y<=BUF_len_cur;y++)
    {
        draw_string((char *)BLUETOOTH_RX_BUF+(y-1)*21,false,0,y*8); 
    }
}

void BLUETOOTH_BUF_pic_draw()
{
  //printf("\r\n buf=%d \r\n",BLUETOOTH_RX_BUF);//插入换行
  draw_bitmap(0,0,(byte *)BLUETOOTH_RX_BUF, 64, 64, NOINVERT, 0);
}

#endif

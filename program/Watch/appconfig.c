/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/ 
 */
#include "delay.h"
#include "common.h"



#define LENTH(Buffer) sizeof(Buffer)	 		  	//数组长度	

#define EEPROM_CHECK_NUM 0x1234
 
#define BKPCheck_SAVE_ADDR   BKP_DR1 	//设置BKP第一个地址保持第一次读写的标志位

appconfig_s appConfig;     //appconfig_s的长度为8
void Save_Flash_APPCONF_Data(void);
unsigned char Read_Flash_APPCONF_Data(void);
// static u8 watchface_num;
u8 flash_init(void)
{
	u8 datatemp[16];
	//static const uint8_t mapdata_time[7] __attribute__((at(STM32_FLASH_TIME_BASE)))={0x80,0xC0,0xC0,0xC0,0xC0,0x80,0xAA};	
	//static const uint8_t mapdata_config[8] __attribute__((at(STM32_FLASH_APPCONF_BASE)))={0x80,0xC0,0xC0,0xC0,0xC0,0x80,0xAA,0xAA};
	STMFLASH_Read(STM32_FLASH_TIME_BASE,(u16*)datatemp,8);
	// for(i=0;i<len_time;i++)
	// {
	// 	printf("flash_init:[%d]=%d\r\n",i,datatemp[i]);
	// }
	if(datatemp[6]==STM32_FLASH_CHECK_DATA)
		return 0;
	return 1;
}

void appconfig_init()
{
	//flash_init();
	display_f watchface_fun;
	appConfig = *(appconfig_s *) malloc(sizeof(appconfig_s));
	memset(&appConfig, 0x00, LENTH(appconfig_s));

	if(!Read_Flash_APPCONF_Data())
	{	
		appconfig_reset(); //flash读取失败，重新设定参数
	}
#if Flash_Save_TIME
	time_init();
#endif

	if(appConfig.sleepTimeout > 12)
		appConfig.sleepTimeout = 0;
	printf(" 2 display180=%d\r\n",appConfig.display180);
	
	get_watchface_total();
	watchface_fun = get_watchface_fun(appConfig.watchface_select);
	display_set(watchface_fun);
}

void appconfig_save()
{			   
	//printf("appconfig_save begin\n");
	Save_Flash_APPCONF_Data();	
	//printf("appconfig_save over\n");
}

void Save_Flash_APPCONF_Data(void)
{
	u8 TEXT_Buffer[20]; //必须一倍大 有bug

	TEXT_Buffer[0] = appConfig.sleepTimeout;
	TEXT_Buffer[1] = appConfig.invert;
#if COMPILE_ANIMATIONS
	TEXT_Buffer[2] = appConfig.animations;
#endif
	TEXT_Buffer[3] = appConfig.display180;
	TEXT_Buffer[4] = appConfig.CTRL_LEDs;
	TEXT_Buffer[5] = appConfig.showFPS;
	TEXT_Buffer[6] = appConfig.timeMode;
	TEXT_Buffer[7] = appConfig.volumes;
	TEXT_Buffer[8] = STM32_FLASH_CHECK_DATA;
	TEXT_Buffer[9] = appConfig.watchface_select;
	TEXT_Buffer[10] = appConfig.game2_max_score;
	TEXT_Buffer[11] = appConfig.game3_max_score;

	STMFLASH_Write(STM32_FLASH_APPCONF_BASE,(u16*)TEXT_Buffer,12);
}


unsigned char Read_Flash_APPCONF_Data(void)
{
	u8 datatemp[20];
	STMFLASH_Read(STM32_FLASH_APPCONF_BASE,(u16*)datatemp,12);	
	//printf("sizeof=%d\r\n",sizeof(datatemp[]));
	if(datatemp[8]==STM32_FLASH_CHECK_DATA)
	{
		appConfig.sleepTimeout  =datatemp[0];
		appConfig.invert 		=datatemp[1];
		appConfig.animations  	=datatemp[2];
		appConfig.display180  	=datatemp[3];
		appConfig.CTRL_LEDs  	=datatemp[4];
		appConfig.showFPS		=datatemp[5];
		appConfig.timeMode  	=(timemode_t)datatemp[6];
		//appConfig.timeMode  	=datatemp[6];
		appConfig.volumes		=datatemp[7];
		appConfig.watchface_select		=datatemp[9];
		appConfig.game2_max_score = datatemp[10];
		appConfig.game3_max_score = datatemp[11];
		printf("Read_Flash_APPCONF_Data ok!!!\r\n");

		// printf("sleepTimeout=%d invert=%d animations=%d\r\n",datatemp[0],datatemp[1],datatemp[2]);
		// printf("display180=%d CTRL_LEDs=%d showFPS=%d\r\n",datatemp[3],datatemp[4],datatemp[5]);
		// printf("timeMode=%d volumes=%d ,watchface_select =%d \r\n",datatemp[6],datatemp[7],datatemp[9]);
		printf("appConfig.game3_max_score = %d\r\n",appConfig.game3_max_score);
		return 1;
	}
	printf("Read_Flash_APPCONF_Data error\r\n");
	return 0;
}
void appconfig_reset()
{
	appConfig.sleepTimeout = 1;
	appConfig.invert = false; //反色
	#if COMPILE_ANIMATIONS
		appConfig.animations = true; //动画
	#endif
	appConfig.display180 = false;//屏幕旋转
  	appConfig.CTRL_LEDs = true;  //灯
	appConfig.showFPS = true;
	appConfig.timeMode = TIMEMODE_24HR;
	appConfig.volumes = 255;  //音量
	appConfig.watchface_select = 1;
	
	appconfig_save();
	printf("appconfig reset and save\r\n");
	alarm_reset();
}

void Save_Flash_TIME_Data(void)
{
	static millis8_t lastDraw; // Time of last draw
	//	static byte fpsMs; // Milliseconds to next draw

	// Limit frame rate
	millis8_t now =timeDate.time.secs;
	if(now == lastDraw) //1s刷新一次
	{
		return;
	}
	lastDraw = now;

	u8 TEXT_Buffer[20]; //必须一倍大 有bug

	TEXT_Buffer[0]=timeDate.time.secs;
	TEXT_Buffer[1]=timeDate.time.mins;
	TEXT_Buffer[2]=timeDate.time.hour;
	TEXT_Buffer[3]=timeDate.date.date;
	TEXT_Buffer[4]=timeDate.date.day;
	TEXT_Buffer[5]=timeDate.date.month;
	TEXT_Buffer[6]=timeDate.date.year;
	TEXT_Buffer[7]=STM32_FLASH_CHECK_DATA;

	STMFLASH_Write(STM32_FLASH_TIME_BASE,(u16*)TEXT_Buffer,10);	
	// printf("Save_Flash_TIME_Data,s=%d\r\n",TEXT_Buffer[0]);
}
unsigned char Read_Flash_Time_Data(void)
{
	u8 datatemp[16];
	STMFLASH_Read(STM32_FLASH_TIME_BASE,(u16*)datatemp,8);	
	if(datatemp[7]==STM32_FLASH_CHECK_DATA&&(datatemp[0]<=60||datatemp[1]<=60||datatemp[2]<=24||datatemp[3]<=31||datatemp[4]<=7||datatemp[5]<=12))
	{
		timeDate.time.secs  =datatemp[0];
		timeDate.time.mins 	=datatemp[1];
		timeDate.time.hour  =datatemp[2];
		timeDate.date.date  =datatemp[3];
		timeDate.date.day   =(day_t)datatemp[4];
		//timeDate.date.day   =datatemp[4];
		timeDate.date.month	=(month_t)datatemp[5];
		//timeDate.date.month	=datatemp[5];
		timeDate.date.year  =datatemp[6];
		// printf("Read_Flash_Time_Data is ok\r\n");
		
			printf("Read_Flash_Time_Data,s=%d\r\n",datatemp[2]);
		return 1;
	}
	printf("Read_Flash_Time_Data is error\r\n");
	return 0;
}
void watchface_num_set(u8  choice)
{
	appConfig.watchface_select = choice;
}

/*
unsigned char Read_Flash_Time_Data_test(void)
{
	u8 datatemp[16];
	STMFLASH_Read(STM32_FLASH_TIME_BASE,(u16*)datatemp,8);	
	if(datatemp[7]==STM32_FLASH_CHECK_DATA)
	{
		// timeDate.time.secs  =datatemp[0];
		// timeDate.time.mins 	=datatemp[1];
		// timeDate.time.hour  =datatemp[2];
		// timeDate.date.date  =datatemp[3];
		// timeDate.date.day   =datatemp[4];
		// timeDate.date.month	=datatemp[5];
		// timeDate.date.year  =datatemp[6];
		// printf("Read_Flash_Time_Data is ok\r\n");
		printf("Read_Flash_Time_Data,s:%d,m:%d,h:%d\r\n",datatemp[0],datatemp[1],datatemp[2]);
		return 1;
	}
	printf("Read_Flash_Time_Data is error\r\n");
	return 0;
}
*/

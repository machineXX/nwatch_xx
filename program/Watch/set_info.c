
#include "common.h"
#include "my_normal.h"
#define OPTION_COUNT		6

static prev_menu_s prevMenuData;

static void mSelect(void);
static void itemLoader(byte);
// static void updateTemperature(void);
// static void updateVoltage(void);
// static void updateFPS(void);
// static void setShowFPS(void);
// static void batteryUpdate(void);

void mInfoOpen()
{
	debug_printf("mInfoOpen\r\n");
	setMenuInfo(OPTION_COUNT, MENU_TYPE_STR, PSTR(STR_INFORMATION));
	setMenuFuncs(MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader);
  
	setPrevMenuOpen(&prevMenuData, mInfoOpen);
	
	beginAnimation2(NULL);
}

static void mSelect()
{
	mMainOpen();
}

static void itemLoader(byte num)
{
	//draw_pic();

	switch(num)
	{
		case 0:
			//auther_info();
			return;
	}
    setMenuOption_P(4, PSTR("Athor       " USER_NAME), NULL, NULL);
	// addBackOption();
}


// static void auther_info()
// {
// 	setMenuOption_P(0, PSTR("User       " USER_NAME), NULL, NULL);
// 	setMenuOption(0, buff, NULL, draw_pic);
// }

//static void updateTemperature()
//{
////	rtc_temperature_s temperature;
////	rtc_tempGet(&temperature);

////	byte temp = 0;
////	if(temperature.frac & 0b10000000)
////		temp += 5;
////	if(temperature.frac & 0b01000000)
////		temp += 2;
//	//temperature.frac = div10(temp);

//	char buff[24];
//	sprintf_P(buff, PSTR(STR_TEMPERATURE), 23,1);

//	setMenuOption(0, buff, NULL, NULL);
//}

//static void updateVoltage()
//{
//	char buff[24];
//	sprintf_P(buff, PSTR(STR_BATTERY), battery_voltage());    //显示当前电池mah
//	setMenuOption(1, buff, NULL, batteryUpdate);
//}

//static void updateFPS()
//{
//	char buff[20];
//	char c =  appConfig.showFPS ? CHAR_YES : CHAR_NO;
//	sprintf_P(buff, PSTR(STR_SHOWFPS), c);
//	setMenuOption(2, buff, NULL, setShowFPS);
//}

//static void setShowFPS()
//{
//	appConfig.showFPS = !appConfig.showFPS;
//}

//static void batteryUpdate()
//{
//	battery_updateNow();
//}

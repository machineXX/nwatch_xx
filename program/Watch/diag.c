/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define OPTION_COUNT		5

static prev_menu_s prevMenuData;

static void mSelect(void);
static void itemLoader(byte);
static void updateTemperature(void);
static void updateVoltage(void);
static void updateFPS(void);
static void setShowFPS(void);
static void batteryUpdate(void);
static void auther_info(void);
static void setShowAuther(void);

static u8 auther_data = 0;
void mDiagOpen()
{
	rtc_tempUpdate();
	battery_update();

	setMenuInfo(OPTION_COUNT, MENU_TYPE_STR, PSTR(STR_DIAGNOSTICSMENU));
	setMenuFuncs(MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader);
  
	setPrevMenuOpen(&prevMenuData, mDiagOpen);
	
	beginAnimation2(NULL);
}

static void mSelect()
{
	bool isExiting = exitSelected();
	if(isExiting)
		appconfig_save();
	setPrevMenuExit(&prevMenuData);
	doAction(exitSelected());
	
}
#define auther_in "Hello user!!!"

static void itemLoader(byte num)
{
	if(auther_data == 0)
	{
		switch(num)
		{
			case 0:
				updateTemperature();
				return;
			case 1:
				updateVoltage();
				return;
			case 2:
				updateFPS();
				return;
			case 4:
				auther_info();
				return;
		}

		setMenuOption_P(3, PSTR("FW    " FW_VERSION), NULL, NULL);
		setMenuOption_P(4, PSTR("Auther     " USER_NAME), NULL, NULL);
		addBackOption();
	}
	else
	{
		setMenuOption(0, " ", NULL, setShowAuther);
		setMenuOption(1, " ", NULL, setShowAuther);
		setMenuOption(2, " ", NULL, setShowAuther);
		setMenuOption(3, " ", NULL, setShowAuther);
		setMenuOption(4, " ", NULL, setShowAuther);
		setMenuOption(5, " ", NULL, setShowAuther);
		//draw_pic();
		setMenuOption_P(0, "Hello user!!!", NULL, NULL);
		setMenuOption_P(1, "This product was mo", NULL, NULL);
		setMenuOption_P(2, "ified from GitHub. ", NULL, NULL);
		setMenuOption_P(3, "   NWATCH-WATCHX   ", NULL, NULL);
		// setMenuOption_P(4, "My QQ: 1104807217  ", NULL, NULL);
		setMenuOption_P(4, "Hangzhou, China, a ", NULL, NULL);
		setMenuOption_P(5, "master who love Si.", NULL, NULL);
	}
}

static void updateTemperature()
{
	char buff[24];
	sprintf_P(buff, PSTR(STR_TEMPERATURE), 23,1);

	setMenuOption(0, buff, NULL, NULL);
}

static void updateVoltage()
{
	char buff[24];
	sprintf_P(buff, PSTR(STR_BATTERY), battery_voltage());    //显示当前电池mah
	setMenuOption(1, buff, NULL, batteryUpdate);
}

static void updateFPS()
{
	char buff[20];
	char c =  appConfig.showFPS ? CHAR_YES : CHAR_NO;
	sprintf_P(buff, PSTR(STR_SHOWFPS), c);
	setMenuOption(2, buff, NULL, setShowFPS);
}

static void setShowFPS()
{
	appConfig.showFPS = !appConfig.showFPS;
}

static void batteryUpdate()
{
	battery_updateNow();
}

static void setShowAuther()
{
	auther_data = !auther_data;
}

static void auther_info()
{	
	setMenuOption(4, PSTR("Auther     " USER_NAME), NULL, setShowAuther);
}


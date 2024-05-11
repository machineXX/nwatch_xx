/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// Battery
// Battery is connected to a P-MOSFET then to a voltage divider. This is so we don't have a load of current being wasted through the voltage divider all the time.
// Polling based, interrupts not needed here since a conversion only takes ~500us and is only done every-so-often

#include "common.h"

#define MAX_VOLTAGE  4.21
#define ADC_MAX      4096   //这里需要修改
#define MAX_ADCVAL	((uint16_t)(ADC_MAX)/2/16)
#define battIconEmpty_val 2500
#define battIconLow_val   2600
#define battIconHigh_val  2700
#define battIconFull_val  3000
/*
#if MAX_ADCVAL > ADC_MAX
//	#undef MAX_ADCVAL
//	#define MAX_ADCVAL ADCMAX
	#warning "MAX_ADCVAL > ADCMAX"
#endif
*/

// Resolution = MAX_VOLTAGE / MAX_ADCVAL
// Resolution = 4.88mV

static uint voltage;
static byte lastSecs;
static byte changeCount;

void battery_init()
{
	// MOSFET pin
  Adc_Init();		  		//ADC初始化
}

// Set next update to happen in a x seconds
void battery_setUpdate(byte secs)
{
	changeCount = secs;
	lastSecs = timeDate.time.secs;
}

void battery_update()
{
	// See if seconds has changed
	if(lastSecs == timeDate.time.secs)
		return;
	lastSecs = timeDate.time.secs;

	if(changeCount)
	{
		changeCount--;
		return;
	}

	// Next update in 5 seconds
	battery_setUpdate(1);

	battery_updateNow();
}

// Update voltage
void battery_updateNow()
{
	// Get ADC value
	//u16 adc = Get_Adc_Average(6,5);

	// Convert ADC value to voltage
	
	voltage = Get_Adc_Average(6,5);
}

// Get voltage
uint battery_voltage()
{
	return voltage;
}
void show_battery_mode_1(void)
{
	static byte usbImagePos=2,usbmode=0;
	static byte chargeImagePos = 2;

	usbmode = usb_ttl_connected();
#if Battery_Enable
	usbmode = 0;
	uint battLevel =    battery_voltage();       
	const byte* battIcon;

	// printf("battLevel=%d\r\n",battLevel);
	if(battLevel < battIconEmpty_val)
		battIcon = battIconEmpty;
	else if(battLevel < battIconLow_val)
		battIcon = battIconLow;
	else if(battLevel < battIconHigh_val)
		battIcon = battIconHigh;
	else if(battLevel < battIconFull_val)
		battIcon = battIconFull;
	else
	{
		battIcon = battIconFull;
		usbmode=1;
	}
	draw_bitmap(112, 2, battIcon, 16, 8, NOINVERT, 0);
#else
	draw_bitmap(112, 2, battIconFull, 16, 8, NOINVERT, 0);
#endif

	#if COMPILE_ANIMATIONS
	if(usbmode)
	{
		draw_bitmap(87, usbImagePos, usbIcon, 16, 8, NOINVERT, 0);
		// draw_bitmap(x, usbImagePos, usbIcon, 16, 8, NOINVERT, 0);
	}
#else
	if(UDADDR != 0)
	{
		draw_bitmap(x, FRAME_HEIGHT - 9, usbIcon, 16, 8, NOINVERT, 0);
		x += 20;
	}
#endif
	
	// Draw charging icon
#if COMPILE_ANIMATIONS
	if(usbmode)
	{
		if(timeDate.time.sec_10ms<50) 
		{
			draw_bitmap(104, chargeImagePos, chargeIcon, 8, 8, NOINVERT, 0);
		}
	}	
#else
	if(CHARGING())
	{
		draw_bitmap(x, FRAME_HEIGHT - 9, chargeIcon, 8, 8, NOINVERT, 0);
		x += 12;
	}
#endif
}
static bool animateIcon(bool active, byte* pos)
{
	byte y = *pos;
	if(active || (!active && y < FRAME_HEIGHT))
	{
		if(active && y > FRAME_HEIGHT - 8)
			y -= 1;
		else if(!active && y < FRAME_HEIGHT)
			y += 1;
		*pos = y;
		return true;
	}
	return false;
}
void show_battery_mode_2(void)
{
	static byte usbImagePos=FRAME_HEIGHT,usbmode = 0;
	static byte chargeImagePos = FRAME_HEIGHT;

	usbmode = usb_ttl_connected();
	// Draw battery icon
#if Battery_Enable
	const byte* battIcon;
	usbmode = 0;
	uint battLevel =    battery_voltage();       
	if(battLevel < battIconEmpty_val)
		battIcon = battIconEmpty;
	else if(battLevel < battIconLow_val)
		battIcon = battIconLow;
	else if(battLevel < battIconHigh_val)
		battIcon = battIconHigh;
	else if(battLevel < battIconFull_val)
		battIcon = battIconFull;
	else 
	{
		battIcon = battIconFull;
		usbmode = 1;
	}

	draw_bitmap(0, FRAME_HEIGHT - 8, battIcon, 16, 8, NOINVERT, 0);
#else
	draw_bitmap(0, FRAME_HEIGHT - 8, battIconFull, 16, 8, NOINVERT, 0);

#endif
	byte x = 20;           

#if COMPILE_ANIMATIONS
	if(animateIcon(1, &usbImagePos)&&usbmode)
//		if(animateIcon(1, &usbImagePos))
	{
		draw_bitmap(x, usbImagePos, usbIcon, 16, 8, NOINVERT, 0); //画usb
		x += 20;
	}
#else
	if(UDADDR != 0)
	{
		draw_bitmap(x, FRAME_HEIGHT - 9, usbIcon, 16, 8, NOINVERT, 0);
		x += 20;
	}
#endif
	
	// Draw charging icon
#if COMPILE_ANIMATIONS
	if(animateIcon(1, &chargeImagePos)&&usbmode)
	{
		if(timeDate.time.sec_10ms<50) 
			draw_bitmap(x, chargeImagePos, chargeIcon, 8, 8, NOINVERT, 0);
		// x += 12;
	}	
#else
	if(CHARGING())
	{
		draw_bitmap(x, FRAME_HEIGHT - 9, chargeIcon, 8, 8, NOINVERT, 0);
		x += 12;
	}
#endif

// #if COMPILE_STOPWATCH
// 	// Stopwatch icon
// 	if(stopwatch_active())
// 	{
// 		draw_bitmap(x, FRAME_HEIGHT - 8, stopwatch, 8, 8, NOINVERT, 0);
// 		x += 12;
// 	}
// #endif	
}

